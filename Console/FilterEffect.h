#pragma once
enum AudioEffectType;

class FilterEffect
{
private:
  float Fs;              /* sample rate in samples per second */
  float Pi = 3.141592;   /* the value of Pi */

  /* These floating point values implement the specific filter type */
  //float f0;                /* cut-off (or center) frequency in Hz */
  //float Q;                 /* filter Q */
  float w0;
  float alpha;
  float a0;
  float a1;
  float a2;
  float b0;
  float b1;
  float b2;

  float** prevInputs;
  float** prevOutputs;

  float q;
  float cutoff;

  int channels;

public:

  void CalculateCoeffs(AudioEffectType type);

  FilterEffect(int sampleRate, int channels);

  AudioEffectType _type;

  void process(
    short* buf, int currentBlock,
    double dTimeStep, int m_nBlockSamples,
    int m_nChannels
  );

  void SetFilterType(AudioEffectType type);
  void SetCutoff(float cutoff, float q);
};

