#include "ui.h"

#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <time.h>

signed int main(void) {
  signed int code;

  setlocale(LC_ALL, ".UTF8");
  srand(time(0));

  code = run_ui();

  if (code)
    puts("myCNoisySynth UI error!");

  return code;
}
