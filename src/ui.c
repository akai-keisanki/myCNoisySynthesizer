#include "ui.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

#include <math.h>

#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#include <portaudio.h>

#include "audio_gen.h"
#include "audio_defs.def"

#define PA_SAMPLE_TYPE paFloat32
#define FRAMES_PER_BUFFER (size_t)(0x16)
#define INPUT_CHANNELS 0

struct play_ctx {
  size_t bass_oct;
  uint8_t freq_width;
  bool toggle_mode;
  bool notes[24];
  double pys[24][2];
};

pthread_mutex_t ctx_mutex = PTHREAD_MUTEX_INITIALIZER;

double generate_sound(struct play_ctx *ctx) {
  double n;
  double x;

  n = gen_white_noise();
  x = -n;

  for (size_t i = 0; i < 24; ++i)
    if (ctx->notes[i])
      x -= extract_freq(x, ctx->pys[i], 440.0 * pow(2.0, (double)ctx->bass_oct - 4.0 + (double)i / 12.0), (double)ctx->freq_width / (double)0x80);

  return (n + x) / 2.0;
}

signed int audio_callback(
    const void *inputBuffer,
    void *outputBuffer,
    size_t framesPerBuffer,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData
  ) {
  struct play_ctx *ctx = userData;
  const float *input = inputBuffer;
  float *output = outputBuffer;

  pthread_mutex_lock(&ctx_mutex);
  for (size_t i = 0; i < framesPerBuffer; ++i)
    output[i] = generate_sound(ctx);
  pthread_mutex_unlock(&ctx_mutex);

  return paContinue;
}

void set_terminal_raw_mode(bool state) {
  static struct termios old_termios;
  struct termios raw_termios;
  static int old_flags;

  if (state) {
    old_flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    tcgetattr(STDIN_FILENO, &old_termios);
    raw_termios = old_termios;
    raw_termios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_termios);
    fcntl(STDIN_FILENO, F_SETFL, old_flags | O_NONBLOCK);
  } else {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &old_termios);
    fcntl(STDIN_FILENO, F_SETFL, old_flags);
  }
}

bool find_char(char *str, char c, size_t sz) {
  for (size_t i = 0; i < sz; ++i)
    if (str[i] == c)
      return true;
  return false;
}

void ui_loop(struct play_ctx *ctx) {
  bool running;
  char input[30] = {};
  ssize_t in_sz = 0;

  set_terminal_raw_mode(true);

  for (size_t i = 0; i < 24; ++i)
    ctx->notes[i] = false;

  running = true;

  while (running) {
    printf("\nfreq_width = %lf\nbass_oct = %zu\ntoggle_mode = %d\n", ctx->freq_width / (double)0x80, ctx->bass_oct, ctx->toggle_mode);
    puts("press ; to exit, q-[ and a-] for notes, z and x to change bass_oct, , and . to change freq_width, m to toggle toggle_mode or c to clean the notes");
    fflush(stdout);

    in_sz = read(STDIN_FILENO, input, 30);
  
    pthread_mutex_lock(&ctx_mutex);
    if (!ctx->toggle_mode)
      memset(ctx->notes, 0, 24);
    for (ssize_t i = 0; i < in_sz; ++i) {
      switch (input[i]) {
      case 'q':
        ctx->notes[0] = !ctx->notes[0] | !ctx->toggle_mode;
        break;
      case 'w':
        ctx->notes[1] = !ctx->notes[1] | !ctx->toggle_mode;
        break;
      case 'e':
        ctx->notes[2] = !ctx->notes[2] | !ctx->toggle_mode;
        break;
      case 'r':
        ctx->notes[3] = !ctx->notes[3] | !ctx->toggle_mode;
        break;
      case 't':
        ctx->notes[4] = !ctx->notes[4] | !ctx->toggle_mode;
        break;
      case 'y':
        ctx->notes[5] = !ctx->notes[5] | !ctx->toggle_mode;
        break;
      case 'u':
        ctx->notes[6] = !ctx->notes[6] | !ctx->toggle_mode;
        break;
      case 'i':
        ctx->notes[7] = !ctx->notes[7] | !ctx->toggle_mode;
        break;
      case 'o':
        ctx->notes[8] = !ctx->notes[8] | !ctx->toggle_mode;
        break;
      case 'p':
        break;
        ctx->notes[9] = !ctx->notes[9] | !ctx->toggle_mode;
        break;
      case '`':
        ctx->notes[10] = !ctx->notes[10] | !ctx->toggle_mode;
        break;
      case '[':
        ctx->notes[11] = !ctx->notes[11] | !ctx->toggle_mode;
        break;
      case 'a':
        ctx->notes[12] = !ctx->notes[12] | !ctx->toggle_mode;
        break;
      case 's':
        ctx->notes[13] = !ctx->notes[13] | !ctx->toggle_mode;
        break;
      case 'd':
        ctx->notes[14] = !ctx->notes[14] | !ctx->toggle_mode;
        break;
      case 'f':
        ctx->notes[15] = !ctx->notes[15] | !ctx->toggle_mode;
        break;
      case 'g':
        ctx->notes[16] = !ctx->notes[16] | !ctx->toggle_mode;
        break;
      case 'h':
        ctx->notes[17] = !ctx->notes[17] | !ctx->toggle_mode;
        break;
      case 'j':
        ctx->notes[18] = !ctx->notes[18] | !ctx->toggle_mode;
        break;
      case 'k':
        ctx->notes[19] = !ctx->notes[19] | !ctx->toggle_mode;
        break;
      case 'l':
        ctx->notes[20] = !ctx->notes[20] | !ctx->toggle_mode;
        break;
      case 0xC3:
        ctx->notes[21] = !ctx->notes[21] | !ctx->toggle_mode;
        break;
      case '~':
        ctx->notes[22] = !ctx->notes[22] | !ctx->toggle_mode;
        break;
      case ']':
        ctx->notes[23] = !ctx->notes[23] | !ctx->toggle_mode;
        break;
      case 'z':
        if (ctx->bass_oct > 0)
          ctx->bass_oct -= 1;
        break;
      case 'x':
        if (ctx->bass_oct < 12)
          ctx->bass_oct += 1;
        break;
      case ',':
        if (ctx->freq_width > 0x00)
          ctx->freq_width -= 0x01;
        break;
      case '.':
        if (ctx->freq_width < 0x80)
          ctx->freq_width += 0x01;
        break;
      case ';':
        running = false;
        break;
      case 'm':
        ctx->toggle_mode = !ctx->toggle_mode;
        break;
      case 'c':
        memset(ctx->notes, 0, 24);
        break;
      }
    }
    pthread_mutex_unlock(&ctx_mutex);

    usleep(50000);

    fputs("input: ", stdout);
    for (ssize_t i = 0; i < in_sz; ++i)
      putchar(input[i]);
    puts("");
  }

  set_terminal_raw_mode(false);
}

signed int run_ui(void) {
  PaStream *stream;
  struct play_ctx ctx = {
      .bass_oct = 3,
      .notes = {},
      .toggle_mode = true,
      .pys = {},
      .freq_width = 0x80
    };

  Pa_Initialize();

  Pa_OpenDefaultStream(
      &stream,
      INPUT_CHANNELS,
      OUTPUT_CHANNELS,
      PA_SAMPLE_TYPE,
      SAMPLE_RATE,
      FRAMES_PER_BUFFER,
      audio_callback,
      &ctx
    );

  Pa_StartStream(stream);
  
  ui_loop(&ctx);

  Pa_StopStream(stream);
  Pa_CloseStream(stream);
  Pa_Terminate();

  return EXIT_SUCCESS;
}
