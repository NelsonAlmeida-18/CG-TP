#include <stdio.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <math.h>
#include <vector>
#include "point.h"

using namespace std;

std::stringstream buffer;
vector<float> normals;
vector<point>points;


void tokenize(std::string const &str, const char* delim, std::vector<float> &out){ 

    char *token = strtok(const_cast<char*>(str.c_str()), delim); 
    while (token != nullptr){ 
        out.push_back(atof(token)); 
        token = strtok(nullptr, delim); 
    }
}


point stringToPoint(string str){
    point p;
    std::vector<float> out;
    tokenize(str, " ", out);
    p.setPoint(out[1], out[2], out[3]);
    return p;
}


void addPoints(){
    for(int i=0; i<points.size(); i++){
        buffer << points[i].pointCoords();
    }
}

void addNormals(){
    for(int i=0; i<points.size(); i++){
        buffer << points[i].normalCoords();
    }
}

point cross(point a, point b){
    return point(a.y-b.z*a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x);
}

std::vector<int> findPoint(point a){
    std::vector<int> indices;
    for(int i=0; i<points.size();i++){
        point extra=points[i];
        if(extra.x==a.x && extra.y==a.y && extra.z==a.z){
            indices.push_back(i);
        }
    }
    return indices;
}

void normalizeAllVertices() {
    for (int i = 0; i < points.size(); i++) {
        float sum = sqrt(pow(points[i].xN, 2) + pow(points[i].yN, 2) + pow(points[i].zN, 2));
        if(sum!=0){
            points[i].xN /= sum;
            points[i].yN /= sum;
            points[i].zN /= sum;
        }

    }
}

void executeNormalAlgorithm(point p, point extra){
    std::vector<int> indices = findPoint(p);
    for(int i=0;i<indices.size();i++){
        points[indices[i]].add2Normal(extra);
    }
}

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
    point extra;

    for (int x=0;x<divisions;x++){
        for (int z=0;z<divisions;z++){
            point1.setPoint(initX+x*edgeIncrement,0,initZ-(z+1)*edgeIncrement);
            point2.setPoint(initX+x*edgeIncrement,0,initZ-z*edgeIncrement);
            point3.setPoint(initX+(x+1)*edgeIncrement,0,initZ-z*edgeIncrement);

            point4.setPoint(initX+(x+1)*edgeIncrement,0,initZ-(z+1)*edgeIncrement);
            
            //write points to buffer
            //upper face

            //upper triangle
            points.push_back(point1);
            points.push_back(point2);
            points.push_back(point3);

            extra=cross(point2-point1, point3-point1);
        
            executeNormalAlgorithm(point1, extra);
            executeNormalAlgorithm(point2, extra);
            executeNormalAlgorithm(point3, extra);

            //lower triangle
            points.push_back(point4);
            points.push_back(point1);
            points.push_back(point3);

            extra=cross(point4-point1, point3-point1);
            executeNormalAlgorithm(point4, extra);
            executeNormalAlgorithm(point1, extra);
            executeNormalAlgorithm(point3, extra);

            //downward face
            //upper triangle
            points.push_back(point1);
            points.push_back(point3);
            points.push_back(point2);

            extra=cross(point1-point3, point2-point3);
            executeNormalAlgorithm(point1, extra);
            executeNormalAlgorithm(point3, extra);
            executeNormalAlgorithm(point2, extra);
            //lower triangle
            
            points.push_back(point1);
            points.push_back(point4);
            points.push_back(point3);

            extra=cross(point1-point4, point3-point4);
            executeNormalAlgorithm(point1, extra);
            executeNormalAlgorithm(point4, extra);
            executeNormalAlgorithm(point3, extra);
        }    
    }
    normalizeAllVertices();
    addPoints();
    addNormals();
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

    point extra;

    std::vector<int> indices;

    for (int j = 0; j < divisions; j++) {
        for (int i = 0; i < divisions; i++) {
            // Bottom Face
            point1.setPoint(initX - j * edgeIncrement, -initY, initZ - (i + 1) * edgeIncrement);
            point2.setPoint(initX - j * edgeIncrement, -initY, initZ - i * edgeIncrement);
            point3.setPoint(initX - (j + 1) * edgeIncrement, -initY, initZ - i * edgeIncrement);
            point4.setPoint(initX - (j + 1) * edgeIncrement, -initY, initZ - (i + 1) * edgeIncrement);

            //upper triangle

            points.push_back(point1);
            points.push_back(point2);
            points.push_back(point3);

            extra=cross(point1-point2, point3-point2);
            executeNormalAlgorithm(point1, extra);
            executeNormalAlgorithm(point2, extra);
            executeNormalAlgorithm(point3, extra);

            //lower triangle
            points.push_back(point4);
            points.push_back(point1);
            points.push_back(point3);

            extra=cross(point4-point1, point3-point1);
            executeNormalAlgorithm(point4, extra);
            executeNormalAlgorithm(point1, extra);
            executeNormalAlgorithm(point3, extra);


            // Top Face
            point5.setPoint(initX - j * edgeIncrement, initY, initZ - (i + 1) * edgeIncrement);
            point6.setPoint(initX - j * edgeIncrement, initY, initZ - i * edgeIncrement);
            point7.setPoint(initX - (j + 1) * edgeIncrement, initY, initZ - i * edgeIncrement);
            point8.setPoint(initX - (j + 1) * edgeIncrement, initY, initZ - (i + 1) * edgeIncrement);

            // Write point to buffer
            //upper triangle
            points.push_back(point6);
            points.push_back(point5);
            points.push_back(point7);

            extra=cross(point6-point5, point7-point5);
            executeNormalAlgorithm(point6, extra);
            executeNormalAlgorithm(point5, extra);
            executeNormalAlgorithm(point7, extra);

            //lower triangle
            points.push_back(point5);
            points.push_back(point8);
            points.push_back(point7);

            extra=cross(point5-point8, point7-point8);
            executeNormalAlgorithm(point5, extra);
            executeNormalAlgorithm(point8, extra);
            executeNormalAlgorithm(point7, extra);
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
            points.push_back(point1);
            points.push_back(point2);
            points.push_back(point3);

            extra=cross(point1-point2, point3-point2);
            executeNormalAlgorithm(point1, extra);
            executeNormalAlgorithm(point2, extra);
            executeNormalAlgorithm(point3, extra);

            //lower triangle
            points.push_back(point4);
            points.push_back(point1);
            points.push_back(point3);

            extra=cross(point4-point1, point3-point1);
            executeNormalAlgorithm(point4, extra);
            executeNormalAlgorithm(point1, extra);
            executeNormalAlgorithm(point3, extra);

            // Right Face
            point5.setPoint(initX, initY - (j + 1) * edgeIncrement, initZ - i * edgeIncrement);
            point6.setPoint(initX, initY - j * edgeIncrement, initZ - i * edgeIncrement);
            point7.setPoint(initX, initY - j * edgeIncrement, initZ - (i + 1) * edgeIncrement);
            point8.setPoint(initX, initY - (j + 1) * edgeIncrement, initZ - (i + 1) * edgeIncrement);

            // Write points to buffer                      
            //upper triangle

            points.push_back(point5);
            points.push_back(point6);
            points.push_back(point7);

            extra=cross(point5-point6, point7-point6);
            executeNormalAlgorithm(point5, extra);
            executeNormalAlgorithm(point6, extra);
            executeNormalAlgorithm(point7, extra);
            //lower triangle
            points.push_back(point5);
            points.push_back(point8);
            points.push_back(point7);

            extra=cross(point5-point8, point7-point8);
            executeNormalAlgorithm(point5, extra);
            executeNormalAlgorithm(point8, extra);
            executeNormalAlgorithm(point7, extra);

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

            points.push_back(point2);
            points.push_back(point1);
            points.push_back(point3);

            extra=cross(point2-point1, point3-point1);
            executeNormalAlgorithm(point2, extra);
            executeNormalAlgorithm(point1, extra);
            executeNormalAlgorithm(point3, extra);

            //lower triangle
            points.push_back(point1);
            points.push_back(point4);
            points.push_back(point3);

            extra=cross(point1-point4, point3-point4);
            executeNormalAlgorithm(point1, extra);
            executeNormalAlgorithm(point4, extra);
            executeNormalAlgorithm(point3, extra);

            // Back Face
            point5.setPoint(initX - (j + 1) * edgeIncrement, initY - (i + 1) * edgeIncrement, -initZ);
            point6.setPoint(initX - (j + 1) * edgeIncrement, initY - i * edgeIncrement, -initZ);
            point7.setPoint(initX - j * edgeIncrement, initY - i * edgeIncrement, -initZ);
            point8.setPoint(initX - j * edgeIncrement, initY - (i + 1) * edgeIncrement, -initZ);

            // Write points to buffer
            //upper triangle
            points.push_back(point5);
            points.push_back(point6);
            points.push_back(point7);

            extra=cross(point5-point6, point7-point6);
            executeNormalAlgorithm(point5, extra);
            executeNormalAlgorithm(point6, extra);
            executeNormalAlgorithm(point7, extra);

            //lower triangle
            points.push_back(point8);
            points.push_back(point5);
            points.push_back(point7);

            extra=cross(point8-point5, point7-point5);
            executeNormalAlgorithm(point8, extra);
            executeNormalAlgorithm(point5, extra);
            executeNormalAlgorithm(point7, extra);

       }
    }
    normalizeAllVertices();
    addPoints();
    addNormals();
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
    point point5;
    point extra;

    for(int i=0; i<slices; i++){
        point2.setPoint(cos(angleStep*i)*radius, 0, sin(angleStep*i)*radius);
        point3.setPoint(cos(angleStep*(i+1))*radius, 0, sin(angleStep*(i+1))*radius);
        

        points.push_back(point1);
        points.push_back(point2);
        points.push_back(point3);

        extra=cross(point1-point2, point3-point2);
        executeNormalAlgorithm(point1, extra);
        executeNormalAlgorithm(point2, extra);
        executeNormalAlgorithm(point3, extra);


        // add upper triangle
        point4.setPoint(0, height, 0);
        point5.setPoint(cos(angleStep*(i+0.5))*radius, 0, sin(angleStep*(i+0.5))*radius);

        points.push_back(point5);
        points.push_back(point3);
        points.push_back(point2);

        extra=cross(point5-point3, point2-point3);
        executeNormalAlgorithm(point5, extra);
        executeNormalAlgorithm(point3, extra);
        executeNormalAlgorithm(point2, extra);
    }
    
    for (int i = 0; i < slices; i++) {
        float angle = i * angleStep;
        float nextAngle = (i + 1) * angleStep;

        for (int j = 0; j < stacks; j++) {
            float currHeight = j * heightStep;
            float nextHeight = (j + 1) * heightStep;

            // Calculate the points for the four vertices of each quad
            point1.setPoint(radius * cos(angle) * (1 - currHeight / height), currHeight, radius * sin(angle) * (1 - currHeight / height));
            point2.setPoint(radius * cos(nextAngle) * (1 - currHeight / height), currHeight, radius * sin(nextAngle) * (1 - currHeight / height));
            point3.setPoint(radius * cos(angle) * (1 - nextHeight / height), nextHeight, radius * sin(angle) * (1 - nextHeight / height));
            point4.setPoint(radius * cos(nextAngle) * (1 - nextHeight / height), nextHeight , radius * sin(nextAngle) * (1 - nextHeight / height));

            // Add the first triangle of the quad
            points.push_back(point1);
            points.push_back(point2);
            points.push_back(point3);

            extra=cross(point1-point2, point3-point2);
            executeNormalAlgorithm(point1, extra);
            executeNormalAlgorithm(point2, extra);
            executeNormalAlgorithm(point3, extra);

            // Add the second triangle of the quad
            // Calculate the normal for the second triangle of the quad
            points.push_back(point2);
            points.push_back(point4);
            points.push_back(point3);

            extra=cross(point2-point4, point3-point4);
            executeNormalAlgorithm(point2, extra);
            executeNormalAlgorithm(point4, extra);
            executeNormalAlgorithm(point3, extra);

        }
    }
    normalizeAllVertices();
    addPoints();
    addNormals();
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
    point extra;

    for (int i=0;i<slices;i++){
        for(int j=0;j<stacks;j++){
            point1.setPoint(radius*cos(i*angleBase)*cos((j+1)*angleFace), radius*sin((j+1)*angleFace), radius*sin(i*angleBase)*cos((j+1)*angleFace));
            point2.setPoint(radius*cos(i*angleBase)*cos(j*angleFace), radius*sin(j*angleFace), radius*sin(i*angleBase)*cos(j*angleFace));
            point3.setPoint(radius*cos((i+1)*angleBase)*cos(j*angleFace), radius*sin(j*angleFace), radius*sin((i+1)*angleBase)*cos(j*angleFace));
            point4.setPoint(radius*cos((i+1)*angleBase)*cos((j+1)*angleFace), radius*sin((j+1)*angleFace), radius*sin((i+1)*angleBase)*cos((j+1)*angleFace));

            points.push_back(point2);
            points.push_back(point1);
            points.push_back(point3);

            extra=cross(point2-point1, point3-point1);
            executeNormalAlgorithm(point2, extra);
            executeNormalAlgorithm(point1, extra);
            executeNormalAlgorithm(point3, extra);

            points.push_back(point1);
            points.push_back(point4);
            points.push_back(point3);

            extra=cross(point1-point4, point3-point4);
            executeNormalAlgorithm(point1, extra);
            executeNormalAlgorithm(point4, extra);
            executeNormalAlgorithm(point3, extra);
        }
    }

    for (int i=0;i<slices;i++){
        for(int j=0;j<stacks;j++){
            point1.setPoint(radius*cos(i*angleBase)*cos((j+1)*angleFace), -radius*sin((j+1)*angleFace), radius*sin(i*angleBase)*cos((j+1)*angleFace));
            point2.setPoint(radius*cos(i*angleBase)*cos(j*angleFace), -radius*sin(j*angleFace), radius*sin(i*angleBase)*cos(j*angleFace));
            point3.setPoint(radius*cos((i+1)*angleBase)*cos(j*angleFace), -radius*sin(j*angleFace), radius*sin((i+1)*angleBase)*cos(j*angleFace));
            point4.setPoint(radius*cos((i+1)*angleBase)*cos((j+1)*angleFace), -radius*sin((j+1)*angleFace), radius*sin((i+1)*angleBase)*cos((j+1)*angleFace));

            points.push_back(point2);
            points.push_back(point1);
            points.push_back(point3);

            extra=cross(point2-point1, point3-point1);
            executeNormalAlgorithm(point2, extra);
            executeNormalAlgorithm(point1, extra);
            executeNormalAlgorithm(point3, extra);

            points.push_back(point1);
            points.push_back(point4);
            points.push_back(point3);

            extra=cross(point1-point4, point3-point4);
            executeNormalAlgorithm(point1, extra);
            executeNormalAlgorithm(point4, extra);
            executeNormalAlgorithm(point3, extra);
        }
    }
    normalizeAllVertices();
    addPoints();
    addNormals();
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
    point extra;

     for (int i=0;i<slices;i++){
        for(int j=0;j<stacks;j++){
            point1.setPoint((outer_r+inner_r*cos(j*angleFace))*cos(i*angleBase), (outer_r+inner_r*cos(j*angleFace))*sin(i*angleBase), ratio*inner_r*sin(j*angleFace));
            
            point2.setPoint((outer_r+inner_r*cos((j+1)*angleFace))*cos(i*angleBase), (outer_r+inner_r*cos((j+1)*angleFace))*sin(i*angleBase), ratio*inner_r*sin((j+1)*angleFace));
            
            point3.setPoint((outer_r+inner_r*cos((j+1)*angleFace))*cos((i+1)*angleBase), (outer_r+inner_r*cos((j+1)*angleFace))*sin((i+1)*angleBase), ratio*inner_r*sin((j+1)*angleFace));
            
            buffer << point1.pointCoords() << '\n';
            buffer << point2.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';

            points.push_back(point1);
            points.push_back(point2);
            points.push_back(point3);

            extra=cross(point1-point2, point3-point2);
            executeNormalAlgorithm(point1, extra);
            executeNormalAlgorithm(point2, extra);
            executeNormalAlgorithm(point3, extra);
            
            point1.setPoint((outer_r+inner_r*cos(j*angleFace))*cos(i*angleBase), (outer_r+inner_r*cos(j*angleFace))*sin(i*angleBase), ratio*inner_r*sin(j*angleFace));
            
            point2.setPoint((outer_r+inner_r*cos((j+1)*angleFace))*cos((i+1)*angleBase), (outer_r+inner_r*cos((j+1)*angleFace))*sin((i+1)*angleBase), ratio*inner_r*sin((j+1)*angleFace));
            
            point3.setPoint((outer_r+inner_r*cos(j*angleFace))*cos((i+1)*angleBase), (outer_r+inner_r*cos(j*angleFace))*sin((i+1)*angleBase), ratio*inner_r*sin(j*angleFace));
            
            points.push_back(point1);
            points.push_back(point2);
            points.push_back(point3);

            extra=cross(point1-point2, point3-point2);
            executeNormalAlgorithm(point1, extra);
            executeNormalAlgorithm(point2, extra);
            executeNormalAlgorithm(point3, extra);

        }
    }
    normalizeAllVertices();
    addPoints();
    addNormals();
}

void tokenize(std::string const &str, const char* delim, std::vector<int> &out){ 

    char *token = strtok(const_cast<char*>(str.c_str()), delim); 
    while (token != nullptr){ 
        out.push_back(atof(token)); 
        token = strtok(nullptr, delim); 
    }
}


void multMatrixMatrix(float m1[4][4], float m2[4][4], float res[4][4]){

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            res[i][j] = 0;
            for(int k = 0; k < 4; k++)
                res[i][j] += m1[i][k] * m2[k][j];
        }
    }
}


void multMatrixVector(float *m, float *v, float *res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j*4+k];
		}
	}
}


float calculateB(float u, float v, float m[4][4]){

    float V[4] = {powf(v, 3), powf(v, 2), v, 1};
    float U[4] = {powf(u, 3), powf(u, 2), u, 1};

    float temp[4];
    multMatrixVector((float *) m, U, temp);
    
    float result = 0;
    for(int i = 0; i < 4; i++){
        result += temp[i]*V[i];
    }

    return result;
}


void generatePatch(const char *file, float tesselation_level){
    std::string filename(file);
    std::ifstream patchfile(filename);
    std::vector<std::vector<int> > patches;
    int num_of_patches;
    int num_of_points;
    std::vector<point> control_points;

    point extra;

    float M[4][4] = {{-1.0f, 3.0f, -3.0f, 1.0f},
                     {3.0f, -6.0f, 3.0f, 0.0f},
                     {-3.0f, 3.0f, 0.0f, 0.0f},
                     {1.0f, 0.0f, 0.0f, 0.0f}};

    if(patchfile.is_open()){
        printf("Generating .3d file for patch\n");
        int total = tesselation_level*tesselation_level;

        std::string str;
        getline(patchfile, str);
        num_of_patches = std::atoi(str.c_str());
        const char* delim = ", ";
        total *= num_of_patches*6;
        buffer << total << "\n";

        while(num_of_patches > 0){
            getline(patchfile, str);
            std::vector<int> temp;
            tokenize(str, delim, temp);
            patches.push_back(temp);
            num_of_patches--;
        }

        getline(patchfile, str);
        num_of_points = std::atoi(str.c_str());
        
        while(num_of_points > 0){
            getline(patchfile, str);
            std::vector<float> temp;

            char *token = strtok(const_cast<char*>(str.c_str()), delim);
            while(token != nullptr){
                temp.push_back(atof(token));
                token = strtok(nullptr, delim);
            }

            point p;
            p.setPoint(temp[0], temp[1], temp[2]);
            control_points.push_back(p);
            num_of_points--;
        }

        float mx[4][4], my[4][4], mz[4][4];

        for(std::vector<int> indexes : patches){
            float temp[4][4];

            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    mx[i][j] = control_points[indexes[i*4+j]].x;
                    my[i][j] = control_points[indexes[i*4+j]].y;
                    mz[i][j] = control_points[indexes[i*4+j]].z;
                }
            }

            //M * x/y/z * M^T
            multMatrixMatrix(M, mx, temp);
            multMatrixMatrix(temp, M, mx);

            multMatrixMatrix(M, my, temp);
            multMatrixMatrix(temp, M, my);

            multMatrixMatrix(M, mz, temp);
            multMatrixMatrix(temp, M, mz);

            point p1, p2, p3, p4;
            float tesselation = 1/tesselation_level;
            for(float i = 0; i < 1; i += tesselation){
                for(float j = 0; j < 1; j += tesselation){
                    p1.setPoint(calculateB(i, j, mx), calculateB(i, j, my), calculateB(i, j, mz));
                    
                    p2.setPoint(calculateB(i+tesselation, j, mx), calculateB(i+tesselation, j, my), calculateB(i+tesselation, j, mz));

                    p3.setPoint(calculateB(i, j+tesselation, mx), calculateB(i, j+tesselation, my), calculateB(i, j+tesselation, mz));

                    extra=cross(p1-p2, p3-p2);
                    executeNormalAlgorithm(p1, extra);
                    executeNormalAlgorithm(p2, extra);
                    executeNormalAlgorithm(p3, extra);
                }
            }
        }
    }
    normalizeAllVertices();
    addPoints();
    addNormals();
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

        if(strcmp(argv[1], "patch")==0){
            generatePatch(argv[2], std::atof(argv[3]));
            filepath = argv[4];
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
