#include <iostream>
#include <string.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fftw3.h>

#include "main.h"
#include "pulse_interface.h"

int main(int argc, char* args[])
{
    pulse_interface::init_context();
    pulse_interface::get_sinks();
    std::string sink_name = pulse_interface::get_active_sink_name()+".monitor";

    ss.format = PA_SAMPLE_U8;
    ss.channels = 1;
    ss.rate = SAMPLE_RATE;
    s = pa_simple_new(NULL,
                    "Peak",
                    PA_STREAM_RECORD,
                    sink_name .c_str(),
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
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

    glMatrixMode(GL_PROJECTION | GL_MODELVIEW);
    glLoadIdentity();

    in = fftw_alloc_real(N);
    out = fftw_alloc_complex(nc);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glutTimerFunc(1000/60, mainLoop, 0);

    glutMainLoop();
    fftw_free(in);
    fftw_free(out);
    pa_simple_free(s);
    pulse_interface::deinit_context();

    return 0;
}

void mainLoop(int val){
    renderWaveform();
    glutTimerFunc(1000/1800, mainLoop, val);
}

void renderBouncy(){
    glClear(GL_COLOR_BUFFER_BIT);

    uint8_t buf[BUFFER_SIZE];
    pa_simple_read(s, buf, sizeof(buf), NULL);
    float avg = 0;
    for(int i = 0; i < sizeof(buf); i ++){
        avg = avg+abs(int(buf[i] - 128));
    }
    avg = avg/(sizeof(buf)*140);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.6f, 0.0f);
    glVertex2f(-avg-.5, -avg-.5);
    glVertex2f(avg-.5, -avg-.5);
    glVertex2f(avg-.5, avg-.5);
    glVertex2f(-avg-.5, avg-.5);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.6f, 0.0f);
    glVertex2f(-avg-.5, -avg+.5);
    glVertex2f(avg-.5, -avg+.5);
    glVertex2f(avg-.5, avg+.5);
    glVertex2f(-avg-.5, avg+.5);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.6f, 0.0f);
    glVertex2f(-avg+.5, -avg-.5);
    glVertex2f(avg+.5, -avg-.5);
    glVertex2f(avg+.5, avg-.5);
    glVertex2f(-avg+.5, avg-.5);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.6f, 0.0f);
    glVertex2f(-avg+.5, -avg+.5);
    glVertex2f(avg+.5, -avg+.5);
    glVertex2f(avg+.5, avg+.5);
    glVertex2f(-avg+.5, avg+.5);
    glEnd();

    glutSwapBuffers();
}

void renderWaveform(){
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin (GL_LINES);
    glColor3f(1.0f, 0.6f, 0.0f);
    glLineWidth(2.f);
    static uint8_t buf[DOUBLE_BUFFER];
    memcpy(buf, (buf+BUFFER_SIZE), BUFFER_SIZE);
    pa_simple_read(s, (buf+BUFFER_SIZE), BUFFER_SIZE, NULL);
    for(int i = 0; i < DOUBLE_BUFFER; i ++){
        glVertex2f(-1.f + 2*float(i)/sizeof(buf), float(int(buf[i] - 128))/364);
    }
    glEnd();

    glutSwapBuffers();
}


void renderFFT(){
    glClear(GL_COLOR_BUFFER_BIT);

    pa_simple_read(s, buf, BUFFER_SIZE, NULL);
    for (int i = 0; i < N; i++){
        in[i] = abs(buf[i] - 128);
    }

    p = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE);
    fftw_execute(p);

    for(int i = LOWER; i < UPPER; i ++){
        glBegin (GL_LINES);
        glColor3f(1.0f, 0.6f, 0.0f);
        glLineWidth(3.f);
        glVertex2f(-1.f + 2*float(i)/(UPPER+LOWER), -1.f);
        glVertex2f(-1.f + 2*float(i)/(UPPER+LOWER), (float)abs(out[i][0])/10000-1);
        glEnd();
    }

    fftw_destroy_plan(p);

    glutSwapBuffers();
}

void bucket(){
}