#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "tinyxml2.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include "classes.h"
#include "parser.h"
#include "drawing.h"
#include <array>
#include <cstring>

GLuint *buffers;

Scene scene;


void changeSize(int w, int h) {
    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if(h == 0)
        h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(scene.camera.projection.fov, ratio, scene.camera.projection.near, scene.camera.projection.far);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

float pitchAngle=0;
float yawAngle=0;
int initialMouseX=-1;
int initialMouseY=-1;
float cameraLookingX, cameraLookingY;
// 100% screen width = rotação de 360 graus

void spinRoutine(int x, int y){
    float windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    float windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    
    float operationX =(x-initialMouseX)*360/windowWidth;
    float operationY =(y-initialMouseY)*360/windowHeight;
    yawAngle+=operationX;
    pitchAngle+=operationY;
    glutPostRedisplay();
    initialMouseX=x;
    initialMouseY=y;
}

void youSpinMyHead(int button, int state, int x, int y){

    
    if (button==GLUT_LEFT_BUTTON){
        initialMouseX=x;
        initialMouseY=y;
        glutMotionFunc(spinRoutine);
    }
    
    if (cameraLookingX==-1){
        cameraLookingX=x;
        cameraLookingY=y;
    }
}



void renderScene(){
    //clear buffers
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    //set the camera
    glLoadIdentity();
    gluLookAt(scene.camera.position.x, scene.camera.position.y, scene.camera.position.z,
              scene.camera.lookAt.x, scene.camera.lookAt.y, scene.camera.lookAt.z,
              scene.camera.up.x, scene.camera.up.y, scene.camera.up.z);

    //draw objects
    glPolygonMode(GL_FRONT,GL_LINE);
    
    drawAxis();

    glRotatef(yawAngle, 0, 1, 0);
    glRotatef(pitchAngle,1,0,0);

    std::vector<int> numVertices = drawModels(scene, buffers);
    for(int i=0; i<scene.models.size(); i++){
        glBindBuffer(GL_ARRAY_BUFFER,buffers[i]);
        glVertexPointer(3,GL_FLOAT, 0,0);
        glColor3f(0.4,0.8,0.7);
        glDrawArrays(GL_TRIANGLES, 0, numVertices[i]/3);
    }

    glutPostRedisplay();

    //end of frame
    glutSwapBuffers();
}


char path[50];

int main(int argc, char **argv){

    strcpy(path, "../../xml/");

    if(argc > 1){
        strcat(path, argv[1]);
        readXML(path,scene);
        //generate buffers
        buffers = (GLuint*)malloc(sizeof(GLuint)*scene.models.size());
    }else{
        return 1;
    }

    //init GLUT and window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Project");
    glEnableClientState(GL_VERTEX_ARRAY); // para aceitar os arrays de vértices para a otimização

    //Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutMouseFunc(youSpinMyHead);

    //OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //GLUT main cycle
    glutMainLoop();

    return 0;
}
