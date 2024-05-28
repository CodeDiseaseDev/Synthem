#pragma once
#include <fstream>
#include "utils.h"

#ifndef STYPE
#define STYPE short
#endif

#define byte unsigned char
#define uint unsigned int
#define ushort unsigned short

typedef struct WAV_HEADER
{
  /* RIFF Chunk Descriptor */
  byte         RIFF[4] = { 'R','I','F','F' };        // RIFF Header Magic header
  uint        ChunkSize;      // RIFF Chunk Size
  byte         WAVE[4] = { 'W','A','V','E' };        // WAVE Header
  /* "fmt" sub-chunk */
  byte         fmt[4] = { 'f','m','t',' ' };         // FMT header
  uint        Subchunk1Size;  // Size of the fmt chunk
  ushort        AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
  ushort        NumOfChan;      // Number of channels 1=Mono 2=Sterio
  uint        SamplesPerSec;  // Sampling Frequency in Hz
  uint        bytesPerSec;    // bytes per second
  ushort        blockAlign;     // 2=16-bit mono, 4=16-bit stereo
  ushort        bitsPerSample;  // Number of bits per sample
  /* "data" sub-chunk */
  byte         Subchunk2ID[4] = { 'd','a','t','a' }; // "data"  string
  uint        Subchunk2Size;  // Sampled data length
} wav_header;


class AudioRecorder
{
public:
  bool isRecording = false;

  AudioRecorder(
    double record_beats = 5,
    double bpm = 125,
    uint sample_rate = 44100,
    ushort channels = 1 // mono
  );

  double record_beats = 5;
  double music_bpm = 125;

  double beats_recorded();

private:

  uint sample_rate = 44100;
  ushort channels = 1;


  

  std::ofstream wav;

  int samples_recorded = 0;

  // convert beats -> seconds
  const double record_seconds() { return (record_beats / music_bpm) * 60.0; }
  const int samples_per_beat() { return (60.0 / music_bpm) * sample_rate; }
  const double samples_to_beats(int samples) { return (double)samples / samples_per_beat(); }

  const short bit_depth() { return sample_size * 8; }
  int sample_count() { return record_seconds() * sample_rate * channels; };
  short sample_size = sizeof(STYPE);

  int write_wave_header();

public:
  void write_sample(STYPE sample);

  bool open_file(const char* file);
};

