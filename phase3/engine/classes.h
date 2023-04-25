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
    float mass;
};


struct Point{
    float x;
    float y; 
    float z;

    Point(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
};


class Transform{
    
    public:
        virtual void execute() = 0;
        virtual float getX() = 0;
        virtual float getY() = 0;
        virtual float getZ() = 0;
        virtual float getAngle() = 0;
        virtual Point getPoint(int i) = 0;
        virtual bool getAlign() = 0;
        virtual float getTime() = 0;
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
            glTranslatef(this->x, this->y, this->z);
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

        float getAngle(){return 0;}
        Point getPoint(int i){return Point(0,0,0);}
        bool getAlign(){return false;}
        float getTime(){return 0;}
};


void buildRotMatrix(float *x, float *y, float *z, float *m);
void cross(float *a, float *b, float *res);
void normalize(float *a);
float length(float *v);
void multMatrixVector(float *m, float *v, float *res);
void getCatmullRomPoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *deriv);
void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv, std::vector<Point> points);
void renderCatmullRomCurve(std::vector<Point> points);

class TranslateCurve : public Transform{
    std::vector<Point> points;
    float time, t, time_checkpoint;
    bool align;
    float y_prev[3];

    public:
        TranslateCurve(float time, bool align, std::vector<Point> points){
            this->t = 0;
            this->time = time*1000;
            this->align = align;
            this->points = points;
            this->y_prev[0] = 0;
            this->y_prev[1] = 1;
            this->y_prev[2] = 0;
            this->time_checkpoint = 0;
        }

        bool getAlign(){
            return this->align;
        }

        float getTime(){
            return this->time;
        }

        Point getPoint(int i){
            return this->points[i];
        }

        float getX(){return 0;}
        float getY(){return 0;}
        float getZ(){return 0;}
        float getAngle(){return 0;}

        void execute();
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
            glRotatef(this->angle, this->x, this->y, this->z);
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

        Point getPoint(int i){return Point(0,0,0);}
        float getTime(){return 0;}
        bool getAlign(){return false;}
};


class RotateTime : public Transform{
    float time;
    float x;
    float y;
    float z;

    public:
        RotateTime(float time, float x, float y, float z){
            this->time = time;
            this->x = x;
            this->y = y;
            this->z = z;
        }

        void execute(){
            glRotatef((glutGet(GLUT_ELAPSED_TIME)*360/this->time), this->x, this->y, this->z);
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

        float getTime(){
            return this->time;
        }

        float getAngle(){return 0;}
        bool getAlign(){return false;}
        Point getPoint(int i){return Point(0,0,0);}
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
            glScalef(this->x, this->y, this->z);
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

        float getAngle(){return 0;}
        float getTime(){return 0;}
        Point getPoint(int i){return Point(0,0,0);}
        bool getAlign(){return false;}
};


struct DrawModel{
    Model model;
    std::vector<Transform*> transformations;
};


struct Scene{
    float width;
    float height;
    Camera camera;
    std::vector<Light> lights;
    std::vector<DrawModel> drawModels;
};
