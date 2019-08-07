#include <iostream>
#include <stdio.h>
#include <string.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <pulse/simple.h>
#include "main.h"
#define SAMPLE_RATE 192000
//#define SAMPLE_RATE 44100
#define FRAME_RATE 30
#define BUFFER_SIZE SAMPLE_RATE/FRAME_RATE
#define DOUBLE_BUFFER 2*BUFFER_SIZE
//#define QUAD_BUFFER 4*BUFFER_SIZE

int main(int argc, char* args[])
{
    ss.format = PA_SAMPLE_U8;
    ss.channels = 1;
    ss.rate = SAMPLE_RATE;
    s = pa_simple_new(NULL,
                    "Peak",
                    PA_STREAM_RECORD,
                    "alsa_output.pci-0000_00_1b.0.analog-stereo.monitor",
                    //"alsa_output.pci-0000_00_03.0.hdmi-stereo.monitor",
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

    glutMainLoop(); 
    return 0;
}

void mainLoop(int val){
    render();
    glutTimerFunc(1000/60, mainLoop, val);
}

void render(){
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