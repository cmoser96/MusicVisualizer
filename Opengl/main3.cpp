#include <iostream>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <pulse/simple.h>
#include <fftw3.h>
#include "main3.h"

int main(int argc, char* args[])
{
    lower = 0;
    upper = 800;
    ss.format = PA_SAMPLE_U8;
    ss.channels = 1;
    ss.rate = SAMPLE_RATE;
    s = pa_simple_new(NULL,
                    "Peak",
                    PA_STREAM_RECORD,
                    "alsa_output.pci-0000_00_1b.0.analog-stereo.monitor",
                    "Recording",
                    &ss,
                    NULL,
                    NULL,
                    NULL
                    );

    glutInit(&argc, args);
    glutInitContextVersion(3,0);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(1800,950);
    glutCreateWindow("Visualizer");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glutTimerFunc(1000/60, mainLoop, 0);

    in = (double *) fftw_malloc(sizeof(double)*N);
    out = (double (*)[2])fftw_malloc(sizeof(fftw_complex)*nc);

    glutMainLoop();
    fftw_free(in);
    fftw_free(out);
    return 0;
}

void mainLoop(int val){
    render();
    glutTimerFunc(1000/1800, mainLoop, val);
}

void render(){
    glClear(GL_COLOR_BUFFER_BIT);

    pa_simple_read(s, buf, BUFFER_SIZE, NULL);
    for (int i = 0; i < N; i++){
        in[i] = abs(buf[i] - 128);
    }

    p = fftw_plan_dft_r2c_1d(N, in, out, FFTW_MEASURE);
    fftw_execute(p);

    for(int i = lower; i < upper; i ++){
        glBegin (GL_LINES);
        glColor3f(1.0f, 0.6f, 0.0f);
        glLineWidth(3.f);
        glVertex2f(-1.f + 2*float(i)/(upper+lower), -1.f);
        glVertex2f(-1.f + 2*float(i)/(upper+lower), (float)abs(out[i][0])/5000-1);
        glEnd();
    }
    
    fftw_destroy_plan(p);

    glutSwapBuffers();
}

void bucket(){
}
