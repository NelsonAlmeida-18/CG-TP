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
#include <array>
#include <cstring>


Scene scene;


void readGroupXML(tinyxml2::XMLElement *group){
    using namespace tinyxml2;

    while(group){    

        //TRANSFORM
        XMLElement *transformXML = group->FirstChildElement("transform");
        XMLElement *elem = transformXML->FirstChildElement();
        while(elem){
            std::string name = std::string(elem->Name());

            if(name == "translate"){
                float x, y, z;
                x = atof(elem->Attribute("x"));
                y = atof(elem->Attribute("y"));
                z = atof(elem->Attribute("z"));

                scene.transformations.push_back(new Translate(x, y, z));

            }else if(name == "rotate"){
                float angle, x, y, z;
                x = atof(elem->Attribute("x"));
                y = atof(elem->Attribute("y"));
                z = atof(elem->Attribute("z"));
                angle = atof(elem->Attribute("angle"));

                scene.transformations.push_back(new Rotate(angle, x, y, z));

            }else if(name == "scale"){
                float x, y, z;
                x = atof(elem->Attribute("x"));
                y = atof(elem->Attribute("y"));
                z = atof(elem->Attribute("z"));

                scene.transformations.push_back(new Scale(x, y, z));
            }

            elem = elem->NextSiblingElement();
        }

        //MODELS
        XMLElement *modelsXML = group->FirstChildElement("models");

        if (modelsXML){
            XMLElement *modelXML = modelsXML->FirstChildElement("model");
            while(modelXML){
                Model model;
                model.model_file = modelXML->Attribute("file");
                XMLElement *texture = modelXML->FirstChildElement("texture");
                model.texture_file = texture->Attribute("file");

                XMLElement *color = modelXML->FirstChildElement("color");
                if(color){
                    model.diffuse.r = atof(color->FirstChildElement("diffuse")->Attribute("R"));
                    model.diffuse.g = atof(color->FirstChildElement("diffuse")->Attribute("G"));
                    model.diffuse.b = atof(color->FirstChildElement("diffuse")->Attribute("B"));
                    model.ambient.r = atof(color->FirstChildElement("ambient")->Attribute("R"));
                    model.ambient.g = atof(color->FirstChildElement("ambient")->Attribute("G"));
                    model.ambient.b = atof(color->FirstChildElement("ambient")->Attribute("B"));
                    model.specular.r = atof(color->FirstChildElement("specular")->Attribute("R"));
                    model.specular.g = atof(color->FirstChildElement("specular")->Attribute("G"));
                    model.specular.b = atof(color->FirstChildElement("specular")->Attribute("B"));
                    model.emissive.r = atof(color->FirstChildElement("emissive")->Attribute("R"));
                    model.emissive.g = atof(color->FirstChildElement("emissive")->Attribute("G"));
                    model.emissive.b = atof(color->FirstChildElement("emissive")->Attribute("B"));
                    model.shininess = atof(color->FirstChildElement("shininess")->Attribute("value"));
                }

                scene.models.push_back(model);

                modelXML = modelXML->NextSiblingElement("model");
            }
        }
        
        readGroupXML(group->FirstChildElement("group"));
        group = group->NextSiblingElement("group");
    }
}


void readXML(char* filename){
    using namespace tinyxml2;

    XMLDocument doc;
    doc.LoadFile(filename);

    //WINDOW XML READING
    XMLElement *window = doc.FirstChildElement("world")->FirstChildElement("window");
    scene.width = atof(window->Attribute("width"));
    scene.height = atof(window->Attribute("height"));

    //CAMERA XML READING
    XMLElement *camera = doc.FirstChildElement("world")->FirstChildElement("camera");
    
    XMLElement *position = camera->FirstChildElement("position");
    Position pos;
    pos.x = atof(position->Attribute("x"));
    pos.y = atof(position->Attribute("y"));
    pos.z = atof(position->Attribute("z"));

    XMLElement *lookat = camera->FirstChildElement("lookAt");
    LookAt look;
    look.x = atof(lookat->Attribute("x"));
    look.y = atof(lookat->Attribute("y"));
    look.z = atof(lookat->Attribute("z"));

    XMLElement *upXML = camera->FirstChildElement("up");
    Up up;
    up.x = atof(upXML->Attribute("x"));
    up.y = atof(upXML->Attribute("y"));
    up.z = atof(upXML->Attribute("z"));

    Projection projection;
    XMLElement *projectionXML = camera->FirstChildElement("projection");
    if (projectionXML){
        projection.fov = atof(projectionXML->Attribute("fov"));
        projection.near = atof(projectionXML->Attribute("near"));
        projection.far = atof(projectionXML->Attribute("far"));
    }

    Camera cameraData;
    cameraData.position = pos;
    cameraData.lookAt = look;
    cameraData.up = up;
    cameraData.projection = projection;

    scene.camera = cameraData;

    //LIGHTS
    XMLElement *lightsXML = doc.FirstChildElement("world")->FirstChildElement("lights");
    XMLElement *lightXML = lightsXML->FirstChildElement("light");

    while(lightXML){
        Light light;
        light.type = lightXML->Attribute("type");

        if (lightXML->Attribute("posX")){
            light.posX = atof(lightXML->Attribute("posX"));
        }
        if (lightXML->Attribute("posY")){
            light.posY = atof(lightXML->Attribute("posY"));
        }
        if (lightXML->Attribute("posZ")){
            light.posZ = atof(lightXML->Attribute("posZ"));
        }
        if (lightXML->Attribute("dirX")){
            light.dirX = atof(lightXML->Attribute("dirX"));
        }
        if (lightXML->Attribute("dirY")){
            light.dirY = atof(lightXML->Attribute("dirY"));
        }
        if (lightXML->Attribute("dirZ")){
            light.dirZ = atof(lightXML->Attribute("dirZ"));
        }
        if (lightXML->Attribute("cutoff")){
            light.cutoff = atof(lightXML->Attribute("cutoff"));
        }

        scene.lights.push_back(light);
        lightXML = lightXML->NextSiblingElement("light");
    }

    //GROUP XML READING
    XMLElement *group = doc.FirstChildElement("world")->FirstChildElement("group");
    readGroupXML(group);
}


void tokenize(std::string const &str, const char* delim, std::vector<float> &out){ 

    char *token = strtok(const_cast<char*>(str.c_str()), delim); 
    while (token != nullptr){ 
        out.push_back(atof(token)); 
        token = strtok(nullptr, delim); 
    }
} 


void drawFigure(std::string filename){
    std::string str;
    std::ifstream file3d(filename);

    const char* delim = " ";
    if (file3d.is_open()){
        glBegin(GL_TRIANGLES);
        while(getline(file3d, str)){
            std::cout << str;
            
            std::vector<float> out;
            tokenize(str, delim, out);
            
            glVertex3f(out[0], out[1], out[2]);
            
            str = "";
        }
        glEnd();
        file3d.close();
    }
    else{
        std::cout << "Could not open file: " << filename << "\n\0";
    }
}


void drawModels(){

    int size = scene.models.size();
    for(int i = 0; i < size; i++){
        glBegin(GL_TRIANGLES);
        drawFigure(scene.models[i].model_file);
        glEnd();
    }
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
    drawModels();

    //end of frame
    glutSwapBuffers();
}


char path[50];

int main(int argc, char **argv){

    strcpy(path, "../../xml/");

    if(argc > 1){
        strcat(path, argv[1]);
        readXML(path);
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


    //OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //GLUT main cycle
    glutMainLoop();

    return 0;
}