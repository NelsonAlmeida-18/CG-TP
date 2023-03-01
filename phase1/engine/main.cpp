#include "tinyxml2.h"
#include <string>
#include <iostream>
#include "structs.h"
#include <vector>


//std::string xml_path;

float width, height = 0;
Camera cameraData;
std::vector<Model> models;


int readXML(std::string file){
    using namespace tinyxml2;

    XMLDocument doc;
    doc.LoadFile(file.data());

    XMLElement *window = doc.FirstChildElement("world")->FirstChildElement("window");
    width = atof(window->Attribute("width"));
    height = atof(window->Attribute("height"));

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

    cameraData.position = pos;
    cameraData.lookAt = look;
    cameraData.up = up;
    cameraData.projection = projection;

    XMLElement *lights = doc.FirstChildElement("world")->FirstChildElement("lights");
    XMLElement *light = lights->FirstChildElement("light");

    while(light){
        Light lightObj;
        lightObj.type = light->Attribute("type");

        if (light->Attribute("posX")){
            lightObj.posX = atof(light->Attribute("posX"));
        }
        if (light->Attribute("posY")){
            lightObj.posY = atof(light->Attribute("posY"));
        }
        if (light->Attribute("posZ")){
            lightObj.posZ = atof(light->Attribute("posZ"));
        }
        if (light->Attribute("dirX")){
            lightObj.dirX = atof(light->Attribute("dirX"));
        }
        if (light->Attribute("dirY")){
            lightObj.dirY = atof(light->Attribute("dirY"));
        }
        if (light->Attribute("dirZ")){
            lightObj.dirZ = atof(light->Attribute("dirZ"));
        }
        if (light->Attribute("cutoff")){
            lightObj.cutoff = atof(light->Attribute("cutoff"));
        }

        light = light->NextSiblingElement("light");
    }

    XMLElement *modelsXML = doc.FirstChildElement("world")->FirstChildElement("group")->FirstChildElement("models");
    if (modelsXML){
        XMLElement *modelXML = modelsXML->FirstChildElement("model");
        while(modelXML){
            Model model;
            XMLElement *texture = modelXML->FirstChildElement("texture");
            model.texture = texture->Attribute("file");

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

            models.push_back(model);

            modelXML = modelXML->NextSiblingElement("model");
        }
    }

    /*while(group){



        group = group->NextSiblingElement("group");
    }*/

    return 0;
}


std::string xml_path;

int main(int argc, char **argv){

    //xml_path = "../xml/";
    xml_path = "";

    if(argc > 1){
        readXML(xml_path + argv[1]);
    }

    printf("%f\n", models[0].diffuse.r);

    return 0;
}