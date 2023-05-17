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
#include <cmath>


GLuint *vertexBuffer, *normalBuffer;

Scene scene;
std::vector<DrawModel> drawModelVector;
std::vector<int> numVertices;

int timebase;
float frames, time_passed, fps;
char fps_buffer[100];
float cameraSpeed = 0.1;
float cameraAlpha = M_PI;
float cameraBeta = 0;
bool renderCurve = true;



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


void processNormalKeys(unsigned char key, int x, int y){
    float alpha, beta;

	if(key == 99 || key == 67){
		renderCurve = !renderCurve;
	}else if(key == 119 || key == 87){ //w
        alpha = cameraAlpha;
        beta = cameraBeta;
    }else if(key == 97 || key == 65){ //a
        alpha = cameraAlpha + M_PI_2;
        beta = cameraBeta;
    }else if(key == 100 || key == 68){ //d
        alpha = cameraAlpha - M_PI_2;
        beta = cameraBeta;
    }else if(key == 115 || key == 83){ //s
        alpha = cameraAlpha + M_PI;
        beta = cameraBeta;
    }

    float xx = sin(alpha) * cos(beta);
    float yy = sin(beta);
    float zz = cos(alpha) * cos(beta);

    scene.camera.position.x += xx*0.1;
    scene.camera.position.y += yy*0.1;
    scene.camera.position.z += zz*0.1;

    glutPostRedisplay();
}


void processSpecialKeys(int key, int xx, int yy){
    if(key == GLUT_KEY_UP){
        scene.camera.lookAt.y += 1;
    }else if(key == GLUT_KEY_DOWN){
        scene.camera.lookAt.y -= 1;
    }else if(key == GLUT_KEY_LEFT){
        scene.camera.lookAt.x -= 1;
    }else if(key == GLUT_KEY_RIGHT){
        scene.camera.lookAt.x += 1;
    }

    glutPostRedisplay();
}


void tokenize(std::string const &str, const char* delim, std::vector<float> &out){ 

    char *token = strtok(const_cast<char*>(str.c_str()), delim); 
    while (token != nullptr){ 
        out.push_back(atof(token)); 
        token = strtok(nullptr, delim); 
    }
}


int drawFigure(Model model, int buffer){
    std::string str;
    std::string filename = "../../3d/" + model.model_file;
    std::ifstream file3d(filename);

    const char* delim = " ";
    if (file3d.is_open()){
               getline(file3d,str);

        std::vector<float> vectorBuffer;
        std::vector<float> normals;
        int flag=0;
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
            if (std::strcmp(line[0],"vn")==0){
                normals.push_back(atof(line[1]));
                normals.push_back(atof(line[2]));
                normals.push_back(atof(line[3]));
                pos+=3;
                flag=1;
            }
            else if (std::strcmp(line[0],"v")!=0 && std::strcmp(line[0],"vn")!=0 && flag==1) {
                break;
            }

           str = "";
        }

        file3d.close();

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[buffer]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vectorBuffer.size()*3,vectorBuffer.data(),GL_STATIC_DRAW);
        
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer[buffer]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*normals.size()*3, normals.data(), GL_STATIC_DRAW);
    
        return (int)vectorBuffer.size();
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
        numVertices.push_back(drawFigure(scene.drawModels[i].model, i));
    }
    
    return numVertices;
}


void drawAxis(){

    glBegin(GL_LINES);

    float windowWidth = glutGet(GLUT_WINDOW_WIDTH)/2;
    float windowHeight = glutGet(GLUT_WINDOW_HEIGHT)/2;

    glColor3f(1,0,0); //x axis - red
    glVertex3f(windowWidth, 0, 0);
    glVertex3f(-windowWidth, 0, 0);

    glColor3f(0, 1, 0); // y axis - green
    glVertex3f(0, windowHeight, 0);
    glVertex3f(0, -windowHeight, 0);

    glColor3f(0, 0, 1); //z axis - blue
    glVertex3f(0, 0, windowWidth);
    glVertex3f(0, 0, -windowWidth);

    glEnd();
}


void renderScene(){
    float fps;
    int time;
    char s[64];
    
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt(scene.camera.position.x, scene.camera.position.y, scene.camera.position.z,
              scene.camera.lookAt.x, scene.camera.lookAt.y, scene.camera.lookAt.z,
              scene.camera.up.x, scene.camera.up.y, scene.camera.up.z);

    //draw objects

    drawAxis();

    numVertices = drawModels();

    /*glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 128);*/

    for(Light *l : scene.lights){
        l->execute();
    }

    for(int i=0; i<scene.drawModels.size(); i++){
        glPushMatrix();

        scene.drawModels[i].model.diffuse->execute();
        scene.drawModels[i].model.specular->execute();
        scene.drawModels[i].model.ambient->execute();
        scene.drawModels[i].model.emissive->execute();
        scene.drawModels[i].model.shininess->execute();
                
        for(Transform *t : scene.drawModels[i].transformations){
            t->execute();
        }

        glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer[i]);
        glVertexPointer(3,GL_FLOAT, 0,0);
        
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer[i]);
        glNormalPointer(GL_FLOAT,0,0);

        glDrawArrays(GL_TRIANGLES,0, numVertices[i]);


        glPopMatrix();
    }

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


char path[50];

int main(int argc, char **argv){

    strcpy(path, "../../xml/");
    if(argc > 1){
        strcat(path, argv[1]);
        readXML(path, scene);
        //generate buffers
        vertexBuffer = (GLuint*)malloc(sizeof(GLuint)*scene.drawModels.size());
        normalBuffer = (GLuint*)malloc(sizeof(GLuint)*scene.drawModels.size());
    }else{
        return 1;
    }
    
    //init GLUT and window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Project");
    glEnable(GL_LIGHTING);

    //Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutSpecialFunc(processSpecialKeys);
    glutKeyboardFunc(processNormalKeys);

    //OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_RESCALE_NORMAL);
    //glCullFace(GL_BACK);

    #ifndef __APPLE__
        glewInit();
    #endif

    glEnableClientState(GL_VERTEX_ARRAY); // para aceitar os arrays de vértices para a otimização
    glEnableClientState(GL_NORMAL_ARRAY);
    glGenBuffers(scene.drawModels.size(), normalBuffer);
    glGenBuffers(scene.drawModels.size(), vertexBuffer);

    time_passed = glutGet(GLUT_ELAPSED_TIME);

    //GLUT main cycle
    glutMainLoop();

    return 0;
}
