#include "FilterEffect.h"
#include "AudioEffect.h"

void FilterEffect::CalculateCoeffs(AudioEffectType type)
{
  debug_printf("%.2f, %.2f\n", cutoff, q);

  w0 = 2 * Pi * cutoff / Fs;
  alpha = sin(w0) / (2 * q);
  float cos_w0 = cos(w0);

  switch (type)
  {
  case HiPass:
    a0 = 1 + alpha;
    a1 = -2 * cos_w0;
    a2 = 1 - alpha;
    b0 = (1 + cos_w0) / 2;
    b1 = -(1 + cos_w0);
    b2 = (1 + cos_w0) / 2;
    break;

  case LoPass:
    a0 = 1 + alpha;
    a1 = -2 * cos_w0;
    a2 = 1 - alpha;
    b0 = (1 - cos_w0) / 2;
    b1 = 1 - cos_w0;
    b2 = (1 - cos_w0) / 2;
    break;
  }

  b0 /= a0;
  b1 /= a0;
  b2 /= a0;
  a1 /= a0;
  a2 /= a0;
}

FilterEffect::FilterEffect(int sampleRate, int channels)
{
  Fs = sampleRate;

  CalculateCoeffs(_type);

  channels++;

  prevInputs = new float* [channels];
  prevOutputs = new float* [channels];
  for (int i = 0; i < channels; ++i) {
    prevInputs[i] = new float[2] {0.0f, 0.0f};
    prevOutputs[i] = new float[2] {0.0f, 0.0f};
  }
}

void FilterEffect::process(STYPE* buf, int currentBlock, double dTimeStep, int m_nBlockSamples, int m_nChannels)
{
  for (int i = 0; i < m_nBlockSamples; i++)
  {
    for (int c = 0; c < m_nChannels; c++)
    {
      STYPE* current = &buf[(currentBlock + i) + c];

      float input = *current;
      float output = b0 * input + b1 * prevInputs[0][c] + b2 * prevInputs[1][c]
        - a1 * prevOutputs[0][c] - a2 * prevOutputs[1][c];

      // Shift the samples for the next iteration
      prevInputs[1][c] = prevInputs[0][c];
      prevInputs[0][c] = input;
      prevOutputs[1][c] = prevOutputs[0][c];
      prevOutputs[0][c] = output;

      // Update the buffer
      *current = output;
    }
  }
}

void FilterEffect::SetFilterType(AudioEffectType type)
{
  this->_type = type;
  CalculateCoeffs(_type);
}

void FilterEffect::SetCutoff(float cutoff, float q)
{
  this->cutoff = cutoff;
  this->q = q;
  CalculateCoeffs(_type);
}
