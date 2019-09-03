#ifndef MAIN_H
#define MAIN_H
#include <pulse/pulseaudio.h>
#include <pulse/simple.h>

#define BUCKETS 10
#define SAMPLE_RATE 192000
#define FRAME_RATE 29
#define BUFFER_SIZE SAMPLE_RATE/FRAME_RATE
#define DOUBLE_BUFFER 2*BUFFER_SIZE

//Opengl
const float r = .5f;
float avg;

//Pulse
pa_simple *s;
pa_sample_spec ss;
pa_context *c;
pa_operation *op;

//FFT
int N = BUFFER_SIZE;
int nc = (N/2)+1;
double *in;
fftw_complex *out;
fftw_plan p;
uint8_t buf[BUFFER_SIZE];
int lower;
int upper;

void mainLoop(int val);
void renderBouncy();
void renderWaveform();
void renderFFT();
void bucket();


#endif // MAIN_H