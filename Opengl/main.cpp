#include <iostream>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <pulse/simple.h>
#include "main.h"
#define SAMPLE_RATE 192000
#define FRAME_RATE 30
#define BUFFER_SIZE SAMPLE_RATE/FRAME_RATE

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
    glutTimerFunc(1000/1800, mainLoop, val);
}

void render(){
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