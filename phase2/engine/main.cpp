#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "Parsing/tinyxml2.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include "classes.h"
#include "Parsing/parser.h"
#include <array>
#include <cstring>
#include <unistd.h>

GLuint *buffers;

Scene scene;
std::vector<DrawModel> drawModelVector;
std::vector<int> numVertices;

int timebase;
float frames, time_passed, fps;
char fps_buffer[100];


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
    
    /*if (cameraLookingX==-1){
        cameraLookingX=x;
        cameraLookingY=y;
    }*/
}


void tokenize(std::string const &str, const char* delim, std::vector<float> &out){ 

    char *token = strtok(const_cast<char*>(str.c_str()), delim); 
    while (token != nullptr){ 
        out.push_back(atof(token)); 
        token = strtok(nullptr, delim); 
    }
}

int drawObj(std::string filename, int buffer){
    std::string str;
    std::ifstream file3d(filename);

    const char* delim = " ";
    if (file3d.is_open()){
               getline(file3d,str);
        int numVertices = 0;

        std::vector<float> vectorBuffer;
        
        int pos=0;
        while(getline(file3d, str)){
            
            std::vector<char*> line;
            char *token = strtok(const_cast<char*>(str.c_str()), delim);
            while (token != nullptr){
                line.push_back(token);
                token = strtok(nullptr, delim);
            }

            if (std::strcmp(line[0],"v")==0){
                vectorBuffer.push_back(atof(line[1]));
                vectorBuffer.push_back(atof(line[2]));
                vectorBuffer.push_back(atof(line[3]));
                pos+=3;
            }
            else{
                break;
            }

            str = "";
        }

        file3d.close();
        
        glBindBuffer(GL_ARRAY_BUFFER, buffers[buffer]);
        glBufferData(GL_ARRAY_BUFFER, vectorBuffer.size()*sizeof(float),vectorBuffer.data(),GL_STATIC_DRAW);
        return vectorBuffer.size();
    }
    else{
        std::cout << "Could not open file: " << filename << "\n\0";
        exit(1);
    }
    return 0;
}

int drawFigure(std::string filename, int buffer){
    std::string str;
    filename = "../../3d/" + filename;
    std::ifstream file3d(filename);

    const char* delim = " ";
    if (file3d.is_open()){
               getline(file3d,str);
        int numVertices = std::atoi(str.c_str());
       
        std::vector<float> vectorBuffer;

        int pos=0;
        while(getline(file3d, str)){

            std::vector<float> out;
            tokenize(str, delim, out);
            for (int i=0; i<3;i++,pos++){
                vectorBuffer.push_back(out[i]);
            }
            
            str = "";
        }

        file3d.close();
        
        glBindBuffer(GL_ARRAY_BUFFER, buffers[buffer]);
        glBufferData(GL_ARRAY_BUFFER, pos*sizeof(float),vectorBuffer.data(),GL_STATIC_DRAW);
        
        return pos;
    }
    else{
        std::cout << "Could not open file: " << filename << "\n\0";
        exit(1);
    }
    return 0;
}


std::vector<int> drawModels(){
    int size = scene.drawModels.size();
    std::vector<int> numVertices;
    
    for(int i = 0; i < size; i++){
        std::string filename = scene.drawModels[i].model.model_file;
        if (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".obj") {
            numVertices.push_back(drawObj(filename, i));
        }
        else{
            numVertices.push_back(drawFigure(scene.drawModels[i].model.model_file, i));
        }
        
    }
    
    return numVertices;
}


void drawAxis(){

    glBegin(GL_LINES);

    float windowWidth = glutGet(GLUT_WINDOW_WIDTH)/2;
    float windowHeight = glutGet(GLUT_WINDOW_HEIGHT)/2;

    glColor3f(1,0,0);
    glVertex3f(windowWidth, 0, 0);
    glVertex3f(-windowWidth, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, windowHeight, 0);
    glVertex3f(0, -windowHeight, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, windowWidth);
    glVertex3f(0, 0, -windowWidth);

    glEnd();
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

    numVertices = drawModels();

    for(int i=0; i<scene.drawModels.size(); i++){
        glPushMatrix();
        
        for(Transform *t : scene.drawModels[i].transformations){
            t->execute();
        }

        glBindBuffer(GL_ARRAY_BUFFER,buffers[i]);
        glVertexPointer(3,GL_FLOAT, 0,0);
        glColor3f(0.4,0.8,0.7);
        glDrawArrays(GL_TRIANGLES, 0, numVertices[i]/3);

        glPopMatrix();
    }

    glutPostRedisplay();

    frames++;
    time_passed = glutGet(GLUT_ELAPSED_TIME);
    if (time_passed - timebase > 1000) {
        fps = frames*1000.0/(time_passed-timebase);
        timebase = time_passed;
        frames = 0;
    }

    sprintf(fps_buffer, "FPS: %d", (int) fps);

    glutSetWindowTitle(fps_buffer);

    //end of frame
    glutSwapBuffers();
}


void genVbos(){

}


char path[50];

int main(int argc, char **argv){

    strcpy(path, "../../xml/");
    if(argc > 1){
        strcat(path, argv[1]);
        readXML(path, scene);
        //generate buffers
        buffers = (GLuint*)malloc(sizeof(GLuint)*scene.drawModels.size());
    }else{
        return 1;
    }
    
    //init GLUT and window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Project");

    //Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutMouseFunc(youSpinMyHead);

    //OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    #ifdef __APPLE__
        glewInit();
    #endif

    glEnableClientState(GL_VERTEX_ARRAY); // para aceitar os arrays de vértices para a otimização
    glGenBuffers(scene.drawModels.size(), buffers);

    time_passed = glutGet(GLUT_ELAPSED_TIME);

    //GLUT main cycle
    glutMainLoop();

    return 0;
}
