#ifndef MAIN_H
#define MAIN_H
#include <pulse/simple.h>

float r = .5f;
float avg;
pa_simple *s;
pa_sample_spec ss;
void mainLoop(int val);
void render();

#endif // MAIN_H