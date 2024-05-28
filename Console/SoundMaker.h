#pragma once

#include "AudioRecorder.h"
#include "SafeVector.h"
#include "MainWindowControls.h"

#pragma comment(lib, "winmm.lib")

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <chrono>
using namespace std;

#include <Windows.h>

#ifndef FTYPE
#define FTYPE double
#endif

#ifndef STYPE
#define STYPE short
#endif

#define MAX_SAMPLE(type) (type)pow(2, (sizeof(type) * 8) - 1) - 1

class SoundMaker
{
	//vector<STYPE> internal_audio_buffer;

public:
	thread m_thread;

	SoundMaker(unsigned int nSampleRate = 44100, unsigned int nChannels = 1, unsigned int nBlocks = 8, unsigned int nBlockSamples = 512)
	{
		Create(nSampleRate, nChannels, nBlocks, nBlockSamples);
	}

	~SoundMaker()
	{
		Destroy();
	}

	SoundMaker()
	{
	}

	unsigned int m_nSampleRate;
	unsigned int m_nChannels;
	unsigned int m_nBlockCount;
	unsigned int m_nBlockSamples;
	unsigned int m_nBlockCurrent;

	AudioRecorder recorder;

	std::chrono::high_resolution_clock::time_point start_time =
		std::chrono::high_resolution_clock::now();

	#define VecType short
	SafeVector audio_buffer = SafeVector();

	bool Create(unsigned int nSampleRate = 44100, unsigned int nChannels = 1, unsigned int nBlocks = 8, unsigned int nBlockSamples = 512)
	{
		m_bReady = false;
		m_nSampleRate = nSampleRate;
		m_nChannels = nChannels;
		m_nBlockCount = nBlocks;
		m_nBlockSamples = nBlockSamples;
		m_nBlockFree = m_nBlockCount;
		m_nBlockCurrent = 0;
		m_pBlockMemory = nullptr;
		m_pWaveHeaders = nullptr;

		m_userFunction = nullptr;

		recorder = AudioRecorder(16, -1, nSampleRate, nChannels);
		//recorder.open_file("./output.wav");

		// Validate device
		vector<wstring> devices = Enumerate();
		auto d = devices.begin();
		if (d != devices.end())
		{
			// Device is available
			int nDeviceID = distance(devices.begin(), d);
			WAVEFORMATEX waveFormat;
			waveFormat.wFormatTag = WAVE_FORMAT_PCM;
			waveFormat.nSamplesPerSec = m_nSampleRate;
			waveFormat.wBitsPerSample = sizeof(STYPE) * 8;
			waveFormat.nChannels = m_nChannels;
			waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
			waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
			waveFormat.cbSize = 0;

			// Open Device if valid
			if (waveOutOpen(&m_hwDevice, nDeviceID, &waveFormat, (DWORD_PTR)waveOutProcWrap, (DWORD_PTR)this, CALLBACK_FUNCTION) != S_OK)
				return Destroy();
		}

		// Allocate Wave|Block Memory
		m_pBlockMemory = new STYPE[m_nBlockCount * m_nBlockSamples];
		if (m_pBlockMemory == nullptr)
			return Destroy();
		ZeroMemory(m_pBlockMemory, sizeof(STYPE) * m_nBlockCount * m_nBlockSamples);

		m_pWaveHeaders = new WAVEHDR[m_nBlockCount];
		if (m_pWaveHeaders == nullptr)
			return Destroy();
		ZeroMemory(m_pWaveHeaders, sizeof(WAVEHDR) * m_nBlockCount);

		// Link headers to block memory
		for (unsigned int n = 0; n < m_nBlockCount; n++)
		{
			m_pWaveHeaders[n].dwBufferLength = m_nBlockSamples * sizeof(STYPE);
			m_pWaveHeaders[n].lpData = (LPSTR)(m_pBlockMemory + (n * m_nBlockSamples));
		}

		m_bReady = true;

		m_thread = thread(&SoundMaker::MainThread, this);

		// Start the ball rolling
		unique_lock<mutex> lm(m_muxBlockNotZero);
		m_cvBlockNotZero.notify_one();

		return true;
	}

	bool Destroy()
	{
		return false;
	}

	void Stop()
	{
		m_bReady = false;
		m_thread.join();
	}

	FTYPE GetTime()
	{
		auto duration = std::chrono::duration_cast<std::chrono::duration<double>>
			(std::chrono::high_resolution_clock::now() - start_time);

		return duration.count();
	}



public:
	static vector<wstring> Enumerate()
	{
		int nDeviceCount = waveOutGetNumDevs();
		vector<wstring> sDevices;
		WAVEOUTCAPS woc;
		for (int n = 0; n < nDeviceCount; n++)
			if (waveOutGetDevCaps(n, &woc, sizeof(WAVEOUTCAPS)) == S_OK)
				sDevices.push_back(woc.szPname);
		return sDevices;
	}

	void SetUserFunction(STYPE* (*func)(STYPE*, int, double, int, int))
	{
		m_userFunction = func;
	}

	


private:
	STYPE* (*m_userFunction)(STYPE*, int, double, int, int);

	

	STYPE* m_pBlockMemory;
	WAVEHDR* m_pWaveHeaders;
	HWAVEOUT m_hwDevice;

	atomic<bool> m_bReady;
	atomic<unsigned int> m_nBlockFree;
	condition_variable m_cvBlockNotZero;
	mutex m_muxBlockNotZero;

	atomic<FTYPE> m_dGlobalTime;

	// Handler for soundcard request for more data
	void waveOutProc(HWAVEOUT hWaveOut, UINT uMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
	{
		if (uMsg != WOM_DONE) return;

		m_nBlockFree++;
		unique_lock<mutex> lm(m_muxBlockNotZero);
		m_cvBlockNotZero.notify_one();
	}

	// Static wrapper for sound card handler
	static void CALLBACK waveOutProcWrap(HWAVEOUT hWaveOut, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
	{
		((SoundMaker*)dwInstance)->waveOutProc(hWaveOut, uMsg, dwParam1, dwParam2);
	}

	// Main thread. This loop responds to requests from the soundcard to fill 'blocks'
	// with audio data. If no requests are available it goes dormant until the sound
	// card is ready for more data. The block is fille by the "user" in some manner
	// and then issued to the soundcard.
	void MainThread()
	{
		m_dGlobalTime = 0.0;
		FTYPE dTimeStep = 1.0 / (FTYPE)m_nSampleRate;

		// Goofy hack to get maximum integer for a type at run-time
		//STYPE _nMaxSample = ;
		FTYPE dMaxSample = (FTYPE)MAX_SAMPLE(STYPE);
		//STYPE nPreviousSample = 0;

		while (m_bReady)
		{
			// Wait for block to become available
			if (m_nBlockFree == 0)
			{
				unique_lock<mutex> lm(m_muxBlockNotZero);
				while (m_nBlockFree == 0) // sometimes, Windows signals incorrectly
					m_cvBlockNotZero.wait(lm);
			}

			// Block is here, so use it
			m_nBlockFree--;

			// Prepare block for processing
			if (m_pWaveHeaders[m_nBlockCurrent].dwFlags & WHDR_PREPARED)
				waveOutUnprepareHeader(m_hwDevice, &m_pWaveHeaders[m_nBlockCurrent], sizeof(WAVEHDR));

			//STYPE nNewSample = 0;
			int nCurrentBlock = m_nBlockCurrent * m_nBlockSamples;


			for (int n = 0; n < m_nBlockSamples; n += m_nChannels)
				for (int c = 0; c < m_nChannels; c++)
					m_pBlockMemory[nCurrentBlock + n + c] = 0;

			m_userFunction(m_pBlockMemory, nCurrentBlock,
				dTimeStep, m_nBlockSamples, m_nChannels);

			audio_buffer.clear();

			for (int n = 0; n < m_nBlockSamples; n += m_nChannels)
			{
				for (int c = 0; c < m_nChannels; c++)
				{
					STYPE nSample = m_pBlockMemory[nCurrentBlock + n + c];

					audio_buffer.push_back(nSample);

					if (recorder.isRecording) {
						recorder.write_sample(nSample);
					}

					

					m_pBlockMemory[nCurrentBlock + n + c] = nSample * vol_radial_slider.get_value();

					// thread safe buffer that can be rendered on the ui
				}
			}


			// Send block to sound device
			waveOutPrepareHeader(m_hwDevice, &m_pWaveHeaders[m_nBlockCurrent], sizeof(WAVEHDR));
			waveOutWrite(m_hwDevice, &m_pWaveHeaders[m_nBlockCurrent], sizeof(WAVEHDR));
			m_nBlockCurrent++;
			m_nBlockCurrent %= m_nBlockCount;
		}
	}
};

inline SoundMaker* soundMaker;