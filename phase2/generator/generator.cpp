#include <stdio.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <math.h>

using namespace std;

std::stringstream buffer;

class point{
    public:
        float x;
        float y;
        float z;

        void translate(float Tx, float Ty, float Tz){
            x+=Tx;
            y+=Ty;
            z+=Tz;
        }

        void setPoint(float xx, float yy, float zz){
            x=xx;
            y=yy;
            z=zz;
        }

        string pointCoords(){
            return to_string(x) + " " + to_string(y) + " " + to_string(z);
        }
};

void generatePlane(float length, int divisions){
    printf("Generating Plane");

    float edgeIncrement = length/divisions;
    float initX = -length/2;
    float initZ = length/2;
    //number of triangles = divisions*divisions* trianglesPerSquare(2)* numberOfFaces(2)
    int numOfTriangs = (divisions*divisions*2)*2;
    int numOfPoints = numOfTriangs*3;
    //write number of points 
    buffer << numOfPoints << '\n';

    point point1;
    point point2;
    point point3;
    point point4;

    for (int x=0;x<divisions;x++){
        for (int z=0;z<divisions;z++){
            point1.setPoint(initX+x*edgeIncrement,0,initZ-(z+1)*edgeIncrement);
            point2.setPoint(initX+x*edgeIncrement,0,initZ-z*edgeIncrement);
            point3.setPoint(initX+(x+1)*edgeIncrement,0,initZ-z*edgeIncrement);

            point4.setPoint(initX+(x+1)*edgeIncrement,0,initZ-(z+1)*edgeIncrement);
            
            //write points to buffer
            //upper face

            //upper triangle
            buffer << point1.pointCoords() << '\n';
            buffer << point2.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';
            //lower triangle
            buffer << point4.pointCoords() << '\n';
            buffer << point1.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';

            //downward face
            //upper triangle
            buffer << point1.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';
            buffer << point2.pointCoords() << '\n';
            //lower triangle
            buffer << point1.pointCoords() << '\n';
            buffer << point4.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';

        }    
    }
}

void generateBox(float length, int divisions) {
    printf("Generating Box");
    float edgeIncrement = length / divisions;
    float initX = length / 2;
    float initY = length / 2;
    float initZ = length / 2;
    int numOfTriangs = 6 * divisions * divisions * 2;
    int numOfPoints = numOfTriangs * 3;
    
    buffer << numOfPoints << '\n';

    

    point point1;
    point point2;
    point point3;
    point point4;

    point point5;
    point point6;
    point point7;
    point point8;

    for (int j = 0; j < divisions; j++) {
        for (int i = 0; i < divisions; i++) {
            // Bottom Face
            point1.setPoint(initX - j * edgeIncrement, -initY, initZ - (i + 1) * edgeIncrement);
            point2.setPoint(initX - j * edgeIncrement, -initY, initZ - i * edgeIncrement);
            point3.setPoint(initX - (j + 1) * edgeIncrement, -initY, initZ - i * edgeIncrement);
            point4.setPoint(initX - (j + 1) * edgeIncrement, -initY, initZ - (i + 1) * edgeIncrement);

            
            //upper triangle
            buffer << point1.pointCoords() << '\n';
            buffer << point2.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';
            //lower triangle
            buffer << point4.pointCoords() << '\n';
            buffer << point1.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';

            // Top Face
            point5.setPoint(initX - j * edgeIncrement, initY, initZ - (i + 1) * edgeIncrement);
            point6.setPoint(initX - j * edgeIncrement, initY, initZ - i * edgeIncrement);
            point7.setPoint(initX - (j + 1) * edgeIncrement, initY, initZ - i * edgeIncrement);
            point8.setPoint(initX - (j + 1) * edgeIncrement, initY, initZ - (i + 1) * edgeIncrement);

            // Write point to buffer
            //upper triangle
            buffer << point6.pointCoords() << '\n';
            buffer << point5.pointCoords() << '\n';
            buffer << point7.pointCoords() << '\n';
            //lower triangle
            buffer << point5.pointCoords() << '\n';
            buffer << point8.pointCoords() << '\n';
            buffer << point7.pointCoords() << '\n';


        }
    }

    for (int j = 0; j < divisions; j++) {
        for (int i = 0; i < divisions; i++) {
            // Left Face
            point1.setPoint(-initX, initY - (j + 1) * edgeIncrement, initZ - i * edgeIncrement);
            point2.setPoint(-initX, initY - j * edgeIncrement, initZ - i * edgeIncrement);
            point3.setPoint(-initX, initY - j * edgeIncrement, initZ - (i + 1) * edgeIncrement);
            point4.setPoint(-initX, initY - (j + 1) * edgeIncrement, initZ - (i + 1) * edgeIncrement);

               //upper triangle
            buffer << point1.pointCoords() << '\n';
            buffer << point2.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';
            //lower triangle
            buffer << point4.pointCoords() << '\n';
            buffer << point1.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';


                     // Right Face
            point5.setPoint(initX, initY - (j + 1) * edgeIncrement, initZ - i * edgeIncrement);
            point6.setPoint(initX, initY - j * edgeIncrement, initZ - i * edgeIncrement);
            point7.setPoint(initX, initY - j * edgeIncrement, initZ - (i + 1) * edgeIncrement);
            point8.setPoint(initX, initY - (j + 1) * edgeIncrement, initZ - (i + 1) * edgeIncrement);

            // Write points to buffer                        // Write point to buffer
            //upper triangle
            buffer << point5.pointCoords() << '\n';
            buffer << point6.pointCoords() << '\n';
            buffer << point7.pointCoords() << '\n';
            //lower triangle
            buffer << point5.pointCoords() << '\n';
            buffer << point8.pointCoords() << '\n';
            buffer << point7.pointCoords() << '\n';
        }
    }

    for (int j = 0; j < divisions; j++) {
        for (int i = 0; i < divisions; i++) {
            // Front Face
            point1.setPoint(initX - (j + 1) * edgeIncrement, initY - (i + 1) * edgeIncrement, initZ);
            point2.setPoint(initX - (j + 1) * edgeIncrement, initY - i * edgeIncrement, initZ);
            point3.setPoint(initX - j * edgeIncrement, initY - i * edgeIncrement, initZ);
            point4.setPoint(initX - j * edgeIncrement, initY - (i + 1) * edgeIncrement, initZ);

            // Write points to buffer

            buffer << point2.pointCoords() << '\n';
            buffer << point1.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';
            //lower triangle
            buffer << point1.pointCoords() << '\n';
            buffer << point4.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';



            // Back Face
            point5.setPoint(initX - (j + 1) * edgeIncrement, initY - (i + 1) * edgeIncrement, -initZ);
            point6.setPoint(initX - (j + 1) * edgeIncrement, initY - i * edgeIncrement, -initZ);
            point7.setPoint(initX - j * edgeIncrement, initY - i * edgeIncrement, -initZ);
            point8.setPoint(initX - j * edgeIncrement, initY - (i + 1) * edgeIncrement, -initZ);

            // Write points to buffer
            //upper triangle
            buffer << point5.pointCoords() << '\n';
            buffer << point6.pointCoords() << '\n';
            buffer << point7.pointCoords() << '\n';
            //lower triangle
            buffer << point8.pointCoords() << '\n';
            buffer << point5.pointCoords() << '\n';
            buffer << point7.pointCoords() << '\n';
       }
    }
}


void generateCone(float radius, float height, int slices, int stacks) {
    printf("Generating Cone");
    float angleStep = (2*M_PI)/slices;
    float heightStep = height / stacks;
    
    //numOfPoints = (numOfBaseTriangs+2*stacks*slices)*3
    int numOfPoints = (slices+2*stacks*slices)*3;
    buffer << numOfPoints << '\n';

    point point1;
    //point1 = origem
    point1.setPoint(0,0,0);
    point point2;
    point point3;
    point point4;

    for(int i=0; i<slices; i++){
        point2.setPoint(cos(angleStep*i)*radius, 0, sin(angleStep*i)*radius);
        point3.setPoint(cos(angleStep*(i+1))*radius, 0, sin(angleStep*(i+1))*radius);
        
            buffer << point1.pointCoords() << '\n';
            buffer << point2.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';
    }
    
    for (int i = 0; i < slices; i++) {
        float angle = i * angleStep;
        float nextAngle = (i + 1) * angleStep;
        for (int j = 0; j < stacks; j++) {
            float currHeight = j * heightStep;
            float nextHeight = (j + 1) * heightStep;

            point1.setPoint(radius * cos(angle) * (1 - currHeight / height), currHeight, radius * sin(angle) * (1 - currHeight / height));
            point2.setPoint(radius * cos(nextAngle) * (1 - currHeight / height), currHeight, radius * sin(nextAngle) * (1 - currHeight / height));
            point3.setPoint(radius * cos(angle) * (1 - nextHeight / height), nextHeight, radius * sin(angle) * (1 - nextHeight / height));

            point4.setPoint(radius * cos(angle) * (1 - nextHeight / height), nextHeight, radius * sin(angle) * (1 - nextHeight / height));

            buffer << point2.pointCoords() << '\n';
            buffer << point1.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';

            buffer << point1.pointCoords() << '\n';
            buffer << point4.pointCoords() << '\n';
            buffer << point2.pointCoords() << '\n';
        }
    }
}

void generateSphere(float radius, int slices, int stacks){
    printf("Generating Sphere");
    float angleFace = M_PI/(2*stacks);
    float angleBase = 2*M_PI/slices;

    int numOfPoints = (slices+2*stacks*slices)*3;

    buffer << numOfPoints << '\n';
    
    point point1;
    point point2;
    point point3;
    point point4;


    for (int i=0;i<slices;i++){
        for(int j=0;j<stacks;j++){
            point1.setPoint(radius*cos(i*angleBase)*cos((j+1)*angleFace), radius*sin((j+1)*angleFace), radius*sin(i*angleBase)*cos((j+1)*angleFace));
            
            point2.setPoint(radius*cos(i*angleBase)*cos(j*angleFace), radius*sin(j*angleFace), radius*sin(i*angleBase)*cos(j*angleFace));
            
            point3.setPoint(radius*cos((i+1)*angleBase)*cos(j*angleFace), radius*sin(j*angleFace), radius*sin((i+1)*angleBase)*cos(j*angleFace));
            
            buffer << point2.pointCoords() << '\n';
            buffer << point1.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';
        }
    }
    
    for (int i=0;i<slices;i++){
        for(int j=0;j<stacks;j++){
            point1.setPoint(radius*cos(i*angleBase)*cos((j+1)*angleFace), -radius*sin((j+1)*angleFace), radius*sin(i*angleBase)*cos((j+1)*angleFace));
            
            point2.setPoint(radius*cos(i*angleBase)*cos(j*angleFace), -radius*sin(j*angleFace), radius*sin(i*angleBase)*cos(j*angleFace));
            
            point3.setPoint(radius*cos((i+1)*angleBase)*cos(j*angleFace), -radius*sin(j*angleFace), radius*sin((i+1)*angleBase)*cos(j*angleFace));
            
            buffer << point1.pointCoords() << '\n';
            buffer << point2.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';
        }
    }
}


void generateTorus(float outer_r, float inner_r, float ratio, int slices, int stacks){
    printf("Generating Torus");
    float angleFace = (2 * M_PI) / stacks;
    float angleBase = (2 * M_PI) / slices;
    int numOfPoints = (slices+2*stacks*slices)*3;

    buffer << numOfPoints << '\n';

    point point1;
    point point2;
    point point3;
    point point4;

     for (int i=0;i<slices;i++){
        for(int j=0;j<stacks;j++){
            point1.setPoint((outer_r+inner_r*cos(j*angleFace))*cos(i*angleBase), (outer_r+inner_r*cos(j*angleFace))*sin(i*angleBase), ratio*inner_r*sin(j*angleFace));
            
            point2.setPoint((outer_r+inner_r*cos((j+1)*angleFace))*cos(i*angleBase), (outer_r+inner_r*cos((j+1)*angleFace))*sin(i*angleBase), ratio*inner_r*sin((j+1)*angleFace));
            
            point3.setPoint((outer_r+inner_r*cos((j+1)*angleFace))*cos((i+1)*angleBase), (outer_r+inner_r*cos((j+1)*angleFace))*sin((i+1)*angleBase), ratio*inner_r*sin((j+1)*angleFace));
            
            buffer << point1.pointCoords() << '\n';
            buffer << point2.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';
            
            point1.setPoint((outer_r+inner_r*cos(j*angleFace))*cos(i*angleBase), (outer_r+inner_r*cos(j*angleFace))*sin(i*angleBase), ratio*inner_r*sin(j*angleFace));
            
            point2.setPoint((outer_r+inner_r*cos((j+1)*angleFace))*cos((i+1)*angleBase), (outer_r+inner_r*cos((j+1)*angleFace))*sin((i+1)*angleBase), ratio*inner_r*sin((j+1)*angleFace));
            
            point3.setPoint((outer_r+inner_r*cos(j*angleFace))*cos((i+1)*angleBase), (outer_r+inner_r*cos(j*angleFace))*sin((i+1)*angleBase), ratio*inner_r*sin(j*angleFace));
            
            buffer << point1.pointCoords() << '\n';
            buffer << point2.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';
        }
    }
}


int main(int argc, char **argv){
    ofstream file;
    char* filepath;

    try{
        if (strcmp(argv[1], "plane")==0){
            generatePlane(std::stof(argv[2]), std::atoi(argv[3]));
            filepath=argv[4];
        }
        
        if (strcmp(argv[1], "box")==0){
            generateBox(std::stof(argv[2]), std::atoi(argv[3]));
            filepath=argv[4];
        
        }

        if (strcmp(argv[1], "sphere")==0){
            generateSphere(std::stof(argv[2]), std::atoi(argv[3]),std::atoi(argv[4]));
            filepath=argv[5];
        }

        if (strcmp(argv[1], "cone")==0){
            generateCone(std::stof(argv[2]), std::stof(argv[3]),std::atoi(argv[4]),std::atoi(argv[5]));
            filepath=argv[6];
        }
        
        if(strcmp(argv[1], "torus")==0){
            generateTorus(std::stof(argv[2]), std::stof(argv[3]),std::atoi(argv[4]),std::atoi(argv[5]), std::atoi(argv[6]));
            filepath=argv[7];
        }
    }
    catch(...){
        std::cout << "\0Not enough arguments\n\0";
        return 1;
    }

    try{
        file.open(filepath);
    }
    catch(...){
        std::cout << "\0Cannot open file\n\0";
        return 1;
    }

    file << buffer.str();
    file.close();

    std::cout << "\nDone\n\0";
    return 0;
}
