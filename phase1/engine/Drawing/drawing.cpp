
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "string"
#include <iostream>
#include <fstream>
#include <vector>
#include "classes.h"

void tokenize(std::string const &str, const char* delim, std::vector<float> &out){ 

    char *token = strtok(const_cast<char*>(str.c_str()), delim); 
    while (token != nullptr){ 
        out.push_back(atof(token)); 
        token = strtok(nullptr, delim); 
    }
} 


int drawFigure(std::string filename, GLuint *buffers, int buffer){
    std::string str;
    std::ifstream file3d(filename);

    const char* delim = " ";
    if (file3d.is_open()){
               getline(file3d,str);
        int numVertices = std::atoi(str.c_str());
        
        float* vertexBuffer = (float*)malloc(numVertices*sizeof(float));
        
        int pos=0;
        while(getline(file3d, str)){
            std::cout << str;
            
            std::vector<float> out;
            tokenize(str, delim, out);
            for (int i=0; i<3;i++,pos++){
                vertexBuffer[pos]=out[i];
            }
            
            str = "";
        }

        file3d.close();
        
        glGenBuffers(1, buffers);
        glBindBuffer(GL_ARRAY_BUFFER, buffers[buffer]);
        glBufferData(GL_ARRAY_BUFFER, pos*sizeof(float),vertexBuffer,GL_STATIC_DRAW);
        return pos;
    }
    else{
        std::cout << "Could not open file: " << filename << "\n\0";
    }
    return 0;
}


std::vector<int> drawModels(Scene scene, GLuint *buffers){
    int size = scene.models.size();
    std::vector<int> numVertices;
    for(int i = 0; i < size; i++){
        numVertices.push_back(drawFigure(scene.models[i].model_file, buffers, i));
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
