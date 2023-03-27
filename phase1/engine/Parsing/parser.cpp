#include "tinyxml2.h"
#include <string>
#include <iostream>
#include "classes.h"

void readGroupXML(tinyxml2::XMLElement *group, Scene scene){
    using namespace tinyxml2;

    while(group){    

        //TRANSFORM
        XMLElement *transformXML = group->FirstChildElement("transform");
        if(transformXML){
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
        }

        //MODELS
        XMLElement *modelsXML = group->FirstChildElement("models");

        if (modelsXML){
            XMLElement *modelXML = modelsXML->FirstChildElement("model");
            while(modelXML){
                Model model;
                model.model_file = modelXML->Attribute("file");
                XMLElement *texture = modelXML->FirstChildElement("texture");
                if(texture){
                    model.texture_file = texture->Attribute("file");
                }

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
        
        readGroupXML(group->FirstChildElement("group"), scene);
        group = group->NextSiblingElement("group");
    }
}

void readXML(char* filename, Scene scene){
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
    if(lightsXML){
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
    }

    //GROUP XML READING
    XMLElement *group = doc.FirstChildElement("world")->FirstChildElement("group");
    readGroupXML(group, scene);
}
