#include "AudioRecorder.h"
#include <corecrt_malloc.h>
#include <winnt.h>
#include "shobjidl_core.h"

AudioRecorder::AudioRecorder(
	double record_beats,
	double bpm,
	uint sample_rate,
	ushort channels)
{
	this->record_beats = record_beats;
	this->music_bpm = bpm;
	this->sample_rate = sample_rate;
	this->channels = channels;
}

double AudioRecorder::beats_recorded()
{
	return samples_to_beats(samples_recorded);
}

int AudioRecorder::write_wave_header()
{
	if (!wav.is_open())
		return -1;


	unsigned int subChunk2Size = sample_count() * channels * bit_depth() / 8;
	unsigned short byteRate = sample_rate * channels * bit_depth() / 8;
	unsigned short blockAlign = channels * bit_depth() / 8;

	wav_header header;
	header.ChunkSize = sizeof(header) + subChunk2Size - 8;
	header.Subchunk1Size = 16;
	header.AudioFormat = 1; // PCM
	header.NumOfChan = channels;
	header.SamplesPerSec = sample_rate;
	header.bytesPerSec = byteRate;
	header.blockAlign = blockAlign;
	header.bitsPerSample = bit_depth();
	header.Subchunk2Size = subChunk2Size;

	wav.write((char*)&header, sizeof(header));
	debug_printf("wrote wave header\n");

	return sizeof(header);
}

void AudioRecorder::write_sample(STYPE sample) {
	wav.write((char*)&sample, sizeof(STYPE));
	samples_recorded += 1;


	double beats = samples_to_beats(samples_recorded);
	if (beats >= record_beats) {
		this->isRecording = false;
		wav.flush();
		wav.close();

		debug_printf("record complete\n");
	}
}

bool AudioRecorder::open_file(const char* file)
{
	wav.open(file, std::ios::binary);
	if (wav.is_open()) {
		write_wave_header();
		this->isRecording = true;
		this->samples_recorded = 0;
		return 1;
	}
	return 0;
}
