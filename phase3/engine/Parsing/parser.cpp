#include "tinyxml2.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include "../classes.h"


void readSubgroupsXML(tinyxml2::XMLElement *subgroupXML, std::vector<DrawModel> &sceneDrawModels, std::vector<Transform*> transf){
    using namespace tinyxml2;

    int n = 0;
    while(subgroupXML){
        //TRANSFORM
        XMLElement *transformXML = subgroupXML->FirstChildElement("transform");
        if(transformXML){
            XMLElement *elem = transformXML->FirstChildElement();
            while(elem){
                std::string name = std::string(elem->Name());

                if(name == "translate"){
                    if(elem->Attribute("time")){
                        bool align = false;
                        if(strcmp(elem->Attribute("align"), "true") == 0){
                            align = true;
                        }

                        float time = atof(elem->Attribute("time"));
                        std::vector<Point> points;
                        for(XMLElement *point = elem->FirstChildElement("point"); point; point = point->NextSiblingElement("point")){
                            float x = atof(point->Attribute("x"));
                            float y = atof(point->Attribute("y"));
                            float z = atof(point->Attribute("z"));

                            points.push_back(Point(x, y, z));
                        }

                        transf.push_back(new TranslateCurve(time, align, points));

                    }else{
                        float x, y, z;
                        x = atof(elem->Attribute("x"));
                        y = atof(elem->Attribute("y"));
                        z = atof(elem->Attribute("z"));

                        transf.push_back(new Translate(x, y, z));
                    }
                    n++;

                }else if(name == "rotate"){
                    if(elem->Attribute("time")){
                        float time, x, y, z;
                        x = atof(elem->Attribute("x"));
                        y = atof(elem->Attribute("y"));
                        z = atof(elem->Attribute("z"));
                        time = atof(elem->Attribute("time"));

                        transf.push_back(new RotateTime(time, x, y, z));
                    }else{
                        float angle, x, y, z;
                        x = atof(elem->Attribute("x"));
                        y = atof(elem->Attribute("y"));
                        z = atof(elem->Attribute("z"));
                        angle = atof(elem->Attribute("angle"));

                        transf.push_back(new Rotate(angle, x, y, z));
                    }
                    n++;

                }else if(name == "scale"){
                    float x, y, z;
                    x = atof(elem->Attribute("x"));
                    y = atof(elem->Attribute("y"));
                    z = atof(elem->Attribute("z"));

                    transf.push_back(new Scale(x, y, z));
                    n++;
                }

                elem = elem->NextSiblingElement();
            }
        }

        //MODELS
        XMLElement *modelsXML = subgroupXML->FirstChildElement("models");

        if (modelsXML){
            XMLElement *modelXML = modelsXML->FirstChildElement("model");
            while(modelXML){
                DrawModel drawModel;
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

                drawModel.model = model;
                drawModel.transformations = transf;
                sceneDrawModels.push_back(drawModel);
                modelXML = modelXML->NextSiblingElement("model");
            }
        }
        
        if(subgroupXML->FirstChildElement("group")){
            readSubgroupsXML(subgroupXML->FirstChildElement("group"), sceneDrawModels, transf);
        }else{
            while(n > 0){
                transf.pop_back();
                n--;
            }
        }
        
        subgroupXML = subgroupXML->NextSiblingElement("group");
    }
}


void readGroupXML(tinyxml2::XMLElement *groupXML, std::vector<DrawModel> &sceneDrawModels){
    using namespace tinyxml2;

    //TRANSFORM
    XMLElement *transformXML = groupXML->FirstChildElement("transform");
    std::vector<Transform*> transf;
    if(transformXML){
        XMLElement *elem = transformXML->FirstChildElement();
        while(elem){
            std::string name = std::string(elem->Name());

            if(name == "translate"){

                if(elem->Attribute("time")){
                    bool align = false;
                    if(strcmp(elem->Attribute("align"), "true") == 0){
                        align = true;
                    }

                    float time = atof(elem->Attribute("time"));
                    std::vector<Point> points;
                    for(XMLElement *point = elem->FirstChildElement("point"); point; point = point->NextSiblingElement("point")){
                        float x = atof(point->Attribute("x"));
                        float y = atof(point->Attribute("y"));
                        float z = atof(point->Attribute("z"));

                        points.push_back(Point(x, y, z));
                    }

                    transf.push_back(new TranslateCurve(time, align, points));

                }else{
                    float x, y, z;
                    x = atof(elem->Attribute("x"));
                    y = atof(elem->Attribute("y"));
                    z = atof(elem->Attribute("z"));

                    transf.push_back(new Translate(x, y, z));
                }

            }else if(name == "rotate"){
                if(elem->Attribute("time")){
                    float time, x, y, z;
                    x = atof(elem->Attribute("x"));
                    y = atof(elem->Attribute("y"));
                    z = atof(elem->Attribute("z"));
                    time = atof(elem->Attribute("time"));

                    transf.push_back(new RotateTime(time, x, y, z));
                }else{
                    float angle, x, y, z;
                    x = atof(elem->Attribute("x"));
                    y = atof(elem->Attribute("y"));
                    z = atof(elem->Attribute("z"));
                    angle = atof(elem->Attribute("angle"));

                    transf.push_back(new Rotate(angle, x, y, z));
                }

            }else if(name == "scale"){
                float x, y, z;
                x = atof(elem->Attribute("x"));
                y = atof(elem->Attribute("y"));
                z = atof(elem->Attribute("z"));

                transf.push_back(new Scale(x, y, z));
            }

            elem = elem->NextSiblingElement();
        }
    }

    //MODELS
    XMLElement *modelsXML = groupXML->FirstChildElement("models");

    if (modelsXML){
        XMLElement *modelXML = modelsXML->FirstChildElement("model");
        while(modelXML){
            DrawModel drawModel;
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

            drawModel.transformations = transf;
            drawModel.model = model;
            sceneDrawModels.push_back(drawModel);

            modelXML = modelXML->NextSiblingElement("model");
        }
    }
    
    if(groupXML->FirstChildElement("group")){
        XMLElement *subgroup = groupXML->FirstChildElement("group");
        readSubgroupsXML(subgroup, sceneDrawModels, transf);
    }
}


void readXML(char* filename, Scene &mainScene){
    using namespace tinyxml2;

    Scene scene;

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
    scene.camera.position.x = atof(position->Attribute("x"));
    scene.camera.position.y = atof(position->Attribute("y"));
    scene.camera.position.z = atof(position->Attribute("z"));

    XMLElement *lookat = camera->FirstChildElement("lookAt");
    LookAt look;
    scene.camera.lookAt.x = atof(lookat->Attribute("x"));
    scene.camera.lookAt.y = atof(lookat->Attribute("y"));
    scene.camera.lookAt.z = atof(lookat->Attribute("z"));

    XMLElement *upXML = camera->FirstChildElement("up");
    Up up;
    scene.camera.up.x = atof(upXML->Attribute("x"));
    scene.camera.up.y = atof(upXML->Attribute("y"));
    scene.camera.up.z = atof(upXML->Attribute("z"));

    Projection projection;
    XMLElement *projectionXML = camera->FirstChildElement("projection");
    if (projectionXML){
        scene.camera.projection.fov = atof(projectionXML->Attribute("fov"));
        scene.camera.projection.near = atof(projectionXML->Attribute("near"));
        scene.camera.projection.far = atof(projectionXML->Attribute("far"));
    }

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
    mainScene = scene;
    readGroupXML(group, mainScene.drawModels);
}
