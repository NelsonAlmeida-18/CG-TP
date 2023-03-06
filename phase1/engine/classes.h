#ifndef ENGINE_CLASSES_H
#define ENGINE_CLASSES_H
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <string>
#include <vector>


struct Light{
    std::string type;
    float posX;
    float posY;
    float posZ;
    float dirX;
    float dirY;
    float dirZ;
    float cutoff;
};


struct Position{
    float x;
    float y;
    float z;
};


struct LookAt{
    float x;
    float y;
    float z;
};


struct Up{
    float x;
    float y;
    float z;
};


struct Projection{
    float fov = 60.0;
    float near = 1.0;
    float far = 1000.0;
};


struct Camera{
    Position position;
    LookAt lookAt;
    Up up;
    Projection projection;
};


struct Diffuse{
    float r = 200.0;
    float g = 200.0;
    float b = 200.0;
};


struct Ambient{
    float r = 50.0;
    float g = 50.0;
    float b = 50.0;
};


struct Specular{
    float r = 0.0;
    float g = 0.0;
    float b = 0.0;
};


struct Emissive{
    float r = 0.0;
    float g = 0.0;
    float b = 0.0;
};


struct Model{
    Diffuse diffuse;
    Ambient ambient;
    Specular specular;
    Emissive emissive;
    float shininess = 0.0;
    std::string texture_file;
    std::string model_file;
};


class Transform{
    
    public:
        virtual void execute() = 0;
        virtual float getX() = 0;
        virtual float getY() = 0;
        virtual float getZ() = 0;
        virtual float getAngle() = 0;
};


class Translate : public Transform{
    float x;
    float y;
    float z;

    public:
        Translate(float x, float y, float z){
            this->x = x;
            this->y = y;
            this->z = z;
        }

        void execute(){
            //glTranslatef(x, y, z);
        }

        float getX(){
            return this->x;
        }

        float getY(){
            return this->y;
        }

        float getZ(){
            return this->z;
        }

        float getAngle(){
            return NULL;
        }
};


class Rotate : public Transform{
    float angle;
    float x;
    float y;
    float z;

    public:
        Rotate(float angle, float x, float y, float z){
            this->angle = angle;
            this->x = x;
            this->y = y;
            this->z = z;
        }

        void execute(){
            //glRotatef(angle, x, y, z);
        }

        float getX(){
            return this->x;
        }

        float getY(){
            return this->y;
        }

        float getZ(){
            return this->z;
        }

        float getAngle(){
            return this->angle;
        }
};


class Scale : public Transform{
    float x;
    float y;
    float z;

    public:
        Scale(float x, float y, float z){
            this->x = x;
            this->y = y;
            this->z = z;
        }

        void execute(){
            //glScalef(x, y, z);
        }

        float getX(){
            return this->x;
        }

        float getY(){
            return this->y;
        }

        float getZ(){
            return this->z;
        }

        float getAngle(){
            return NULL;
        }
};


struct Scene{
    float width;
    float height;
    Camera camera;
    std::vector<Light> lights;
    std::vector<Transform*> transformations;
    std::vector<Model> models;
};