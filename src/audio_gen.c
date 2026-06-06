#include "audio_gen.h"

#include <stdlib.h>

#include <math.h>

#include "audio_defs.def"

#define WN_RES (unsigned long)(0xFFF)

double gen_white_noise(void) {
  return 2.0 * (rand() % WN_RES / (double)WN_RES) - 1.0;
}

double freq_to_cut(double freq) {
  return 1.0 - exp(-2.0 * M_PI * freq / SAMPLE_RATE);
}

double extract_freq(double x, double pys[2], double freq, double width) {
  double w;
  double y;

  w = 2.0 * M_PI * freq / SAMPLE_RATE;
  y = x + 2.0 * width * cos(w) * pys[0] - width * width * pys[1];

  pys[1] = pys[0];
  pys[0] = y;

  return y;
}
