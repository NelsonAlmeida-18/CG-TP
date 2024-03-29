#include <IL/il.h>
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


GLuint *vertexBuffer, *normalBuffer, *textureBuffer;

Scene scene;
std::vector<DrawModel> drawModelVector;
std::vector<int> numVertices;

int timebase;
float frames, time_passed, fps;
char fps_buffer[100];
float cameraSpeed = 0.1;
bool renderCurve = true;
float lastMouseX, lastMouseY;
float cameraAngleX = 0;
float cameraAngleY = 0;
float cameraDistance = 5;
bool renderAxis = true;


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


void normalizeVector(float v[3]){
    float norm = sqrt((v[0]*v[0]) + (v[1]*v[1]) + (v[2]*v[2]));
    
    for(int i = 0; i < 3; i++){
        v[i] = v[i]/norm;
    }
}


void cameraMotion(unsigned char key){
    float k[3] = {scene.camera.lookAt.x - scene.camera.position.x,
                  scene.camera.lookAt.y - scene.camera.position.y,
                  scene.camera.lookAt.z - scene.camera.position.z};
    normalizeVector(k);

    float up[3] = {scene.camera.up.x,
                   scene.camera.up.y,
                   scene.camera.up.z};

    normalizeVector(up);

    float i[3] = {up[1] * k[2] - up[2]*k[1],
                  up[2] * k[0] - up[0]*k[2],
                  up[0] * k[1] - up[1]*k[0]};
    normalizeVector(i);

    if(key == 119 || key == 87){ //w
        scene.camera.position.x += k[0] * cameraSpeed;
        scene.camera.position.y += k[1] * cameraSpeed;
        scene.camera.position.z += k[2] * cameraSpeed;
        scene.camera.lookAt.x += k[0] * cameraSpeed;
        scene.camera.lookAt.y += k[1] * cameraSpeed;
        scene.camera.lookAt.z += k[2] * cameraSpeed;
    }else if(key == 100 || key == 68){ //a
        scene.camera.position.x -= i[0] * cameraSpeed;
        scene.camera.position.y -= i[1] * cameraSpeed;
        scene.camera.position.z -= i[2] * cameraSpeed;
        scene.camera.lookAt.x -= i[0] * cameraSpeed;
        scene.camera.lookAt.y -= i[1] * cameraSpeed;
        scene.camera.lookAt.z -= i[2] * cameraSpeed;
    }else if(key == 97 || key == 65){ //d
        scene.camera.position.x += i[0] * cameraSpeed;
        scene.camera.position.y += i[1] * cameraSpeed;
        scene.camera.position.z += i[2] * cameraSpeed;
        scene.camera.lookAt.x += i[0] * cameraSpeed;
        scene.camera.lookAt.y += i[1] * cameraSpeed;
        scene.camera.lookAt.z += i[2] * cameraSpeed;
    }else if(key == 115 || key == 83){ //s
        scene.camera.position.x -= k[0] * cameraSpeed;
        scene.camera.position.y -= k[1] * cameraSpeed;
        scene.camera.position.z -= k[2] * cameraSpeed;
        scene.camera.lookAt.x -= k[0] * cameraSpeed;
        scene.camera.lookAt.y -= k[1] * cameraSpeed;
        scene.camera.lookAt.z -= k[2] * cameraSpeed;
    }
}


void processNormalKeys(unsigned char key, int x, int y){
    float k[3] = {scene.camera.lookAt.x - scene.camera.position.x,
                  scene.camera.lookAt.y - scene.camera.position.y,
                  scene.camera.lookAt.z - scene.camera.position.z};
    normalizeVector(k);

    float up[3] = {scene.camera.up.x,
                   scene.camera.up.y,
                   scene.camera.up.z};

    normalizeVector(up);

    float i[3] = {up[1] * k[2] - up[2]*k[1],
                  up[2] * k[0] - up[0]*k[2],
                  up[0] * k[1] - up[1]*k[0]};
    normalizeVector(i);

	if(key == 99 || key == 67){
		renderCurve = !renderCurve;
    }else if(key == 69 || key == 101){
        renderAxis = !renderAxis;
	}else{
        cameraMotion(key);
    }

    glutPostRedisplay();
}


void mouseMotion(int x, int y)
{
    int deltaX = x - lastMouseX;
    int deltaY = y - lastMouseY;

    // Adjust camera rotation based on mouse movement
    cameraAngleY += deltaX * 0.1f;
    cameraAngleX -= deltaY * 0.1f;

    // Clamp camera rotation angles to avoid over-rotation
    if (cameraAngleX > 89.0f)
        cameraAngleX = 89.0f;
    else if (cameraAngleX < -89.0f)
        cameraAngleX = -89.0f;

    // Update the eye position based on cameraDistance, cameraAngleX, and cameraAngleY
    scene.camera.lookAt.x = cameraDistance * sin(cameraAngleY * M_PI / 180.0f) * cos(cameraAngleX * M_PI / 180.0f);
    scene.camera.lookAt.y = cameraDistance * sin(cameraAngleX * M_PI / 180.0f);
    scene.camera.lookAt.z = cameraDistance * cos(cameraAngleY * M_PI / 180.0f) * cos(cameraAngleX * M_PI / 180.0f);

    lastMouseX = x;
    lastMouseY = y;

    glutPostRedisplay();
}


void mouseButton(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            lastMouseX = x;
            lastMouseY = y;
            glutMotionFunc(mouseMotion);
        }
        else if (state == GLUT_UP)
        {
            glutMotionFunc(NULL);
        }
    }
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


GLuint loadTexture(std::string filename){
    GLuint texID;
    unsigned char *texData;
    unsigned int t, tw, th;
    std::string file_path = "../../textures/" + filename;

    ilInit();

    //coloca a origem da textura no canto inferior esquerdo do primeiro quadrante
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

    //Coloca a textura na memória
    ilGenImages(1, &t);
    ilBindImage(t);
    ilLoadImage((ILstring)file_path.c_str());
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);

    //Assegura que está em RGBA
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    texData = ilGetData();

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texID;
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
               //getline(file3d,str);

        std::vector<float> vectorBuffer;
        std::vector<float> normals;
        std::vector<float> textures;
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
            if(std::strcmp(line[0], "vt") == 0){
                textures.push_back(atof(line[1]));
                textures.push_back(atof(line[2]));
            }
            else if (std::strcmp(line[0],"v")!=0 && std::strcmp(line[0],"vn")!=0 && flag==1) {
                break;
            }

           str = "";
        }

        file3d.close();

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[buffer]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(vectorBuffer.size()-1),vectorBuffer.data(),GL_STATIC_DRAW);
        
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer[buffer]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(normals.size()-1), normals.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, textureBuffer[buffer]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(vectorBuffer.size()-1), textures.data(), GL_STATIC_DRAW);
    
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
    if(renderAxis){
        glDisable(GL_LIGHTING);
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
        glEnable(GL_LIGHTING);
    }
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

    //float amb[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

    for(Light *l : scene.lights){
        l->execute();
    }

    for(int i=0; i<scene.drawModels.size(); i++){
        glPushMatrix();
                
        for(Transform *t : scene.drawModels[i].transformations){
            t->execute();
        }

        if(scene.drawModels[i].model.colorFlag){
            scene.drawModels[i].model.diffuse->execute();
            scene.drawModels[i].model.specular->execute();
            scene.drawModels[i].model.ambient->execute();
            scene.drawModels[i].model.emissive->execute();
            scene.drawModels[i].model.shininess->execute();
        }

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[i]);
        glVertexPointer(3,GL_FLOAT, 0,0);
        
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer[i]);
        glNormalPointer(GL_FLOAT,0,0);

        if(scene.drawModels[i].model.texture_file.compare("") != 0){
            GLuint texID = loadTexture(scene.drawModels[i].model.texture_file);
            if(texID != -1){
                glBindTexture(GL_TEXTURE_2D, texID);
                glBindBuffer(GL_ARRAY_BUFFER, textureBuffer[i]);
                glTexCoordPointer(2, GL_FLOAT, 0, 0);
            }
        }

        glDrawArrays(GL_TRIANGLES,0, numVertices[i]);

        glBindTexture(GL_TEXTURE_2D, 0);
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


void initLights(){
    float dark[4] = {0.3, 0.3, 0.3, 1.0};
    float white[4] = {1.0, 1.0, 1.0, 1.0};

    for(Light *l : scene.lights){
        glEnable(l->getIndex());
        glLightfv(l->getIndex(), GL_AMBIENT, dark);
        glLightfv(l->getIndex(), GL_DIFFUSE, white);
        glLightfv(l->getIndex(), GL_SPECULAR, white);
    }
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
        textureBuffer = (GLuint*)malloc(sizeof(GLuint)*scene.drawModels.size());
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
    initLights();

    //Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutSpecialFunc(processSpecialKeys);
    glutKeyboardFunc(processNormalKeys);
    glutMouseFunc(mouseButton);

    //OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_RESCALE_NORMAL);
    glEnable(GL_TEXTURE_2D);
    //glCullFace(GL_BACK);

    #ifndef __APPLE__
        glewInit();
    #endif

    glEnableClientState(GL_VERTEX_ARRAY); // para aceitar os arrays de vértices para a otimização
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glGenBuffers(scene.drawModels.size(), normalBuffer);
    glGenBuffers(scene.drawModels.size(), vertexBuffer);
    glGenBuffers(scene.drawModels.size(), textureBuffer);

    time_passed = glutGet(GLUT_ELAPSED_TIME);

    //GLUT main cycle
    glutMainLoop();

    return 0;
}
