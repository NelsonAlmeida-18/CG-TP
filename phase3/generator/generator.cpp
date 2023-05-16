#include <stdio.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <math.h>
#include <vector>

using namespace std;

std::stringstream buffer;
vector<float> normals;

class point{
    public:
        float x;
        float y;
        float z;

        point(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

        void translate(float Tx, float Ty, float Tz){
            x+=Tx;
            y+=Ty;
            z+=Tz;
        }

        point operator- (const point& other) const {
            return point(x - other.x, y - other.y, z - other.z);
        }

        void setPoint(float xx, float yy, float zz){
            x=xx;
            y=yy;
            z=zz;
        }

        point cross(const point &p) const {
            return point(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
        }

        string pointCoords(){
            return "v " + to_string(x) + " " + to_string(y) + " " + to_string(z);
        }

        void normalize(){
            float size = sqrt(pow(x,2)+pow(y,2)+pow(z,2));
            x=x/size;
            y=y/size;
            z=z/size;
        }
};




void addNormals(){
    for(int i=0; i<normals.size(); i+=3){
        buffer << "vn "<< normals[i] << " " << normals[i+1]<< " " <<normals[i+2] << "\n";
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

            for (int k=0;k<3;k++){
                normals.push_back(0);
                normals.push_back(1);
                normals.push_back(0);
            }


            //lower triangle
            buffer << point4.pointCoords() << '\n';
            buffer << point1.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';


            for (int k=0;k<3;k++){
                normals.push_back(0);
                normals.push_back(1);
                normals.push_back(0);
            }


            //downward face
            //upper triangle
            buffer << point1.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';
            buffer << point2.pointCoords() << '\n';


            for (int k=0;k<3;k++){
                normals.push_back(0);
                normals.push_back(-1);
                normals.push_back(0);
            }

            //lower triangle
            buffer << point1.pointCoords() << '\n';
            buffer << point4.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';
            

            for (int k=0;k<3;k++){
                normals.push_back(0);
                normals.push_back(-1);
                normals.push_back(0);
            }
        }    
    }
    addNormals();
}



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

           

            for (int k=0;k<3;k++){
                normals.push_back(0);
                normals.push_back(-1);
                normals.push_back(0);

            }
            //lower triangle
            buffer << point4.pointCoords() << '\n';
            buffer << point1.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';

            for (int k=0;k<3;k++){
                normals.push_back(0);
                normals.push_back(-1);
                normals.push_back(0);
            }

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
            
            for (int k=0;k<3;k++){
                normals.push_back(0);
                normals.push_back(1);
                normals.push_back(0);
            }

            //lower triangle
            buffer << point5.pointCoords() << '\n';
            buffer << point8.pointCoords() << '\n';
            buffer << point7.pointCoords() << '\n';

            for (int k=0;k<3;k++){
                normals.push_back(0);
                normals.push_back(1);
                normals.push_back(0);
            }

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

            for (int k=0;k<3;k++){
                normals.push_back(-1);
                normals.push_back(0);
                normals.push_back(0);
            }
            //lower triangle
            buffer << point4.pointCoords() << '\n';
            buffer << point1.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';
            
            for (int k=0;k<3;k++){
                normals.push_back(-1);
                normals.push_back(0);
                normals.push_back(0);
            }

            // Right Face
            point5.setPoint(initX, initY - (j + 1) * edgeIncrement, initZ - i * edgeIncrement);
            point6.setPoint(initX, initY - j * edgeIncrement, initZ - i * edgeIncrement);
            point7.setPoint(initX, initY - j * edgeIncrement, initZ - (i + 1) * edgeIncrement);
            point8.setPoint(initX, initY - (j + 1) * edgeIncrement, initZ - (i + 1) * edgeIncrement);

            // Write points to buffer                      
            //upper triangle
            buffer << point5.pointCoords() << '\n';
            buffer << point6.pointCoords() << '\n';
            buffer << point7.pointCoords() << '\n';

            for (int k=0;k<3;k++){
                normals.push_back(1);
                normals.push_back(0);
                normals.push_back(0);
            }

            //lower triangle
            buffer << point5.pointCoords() << '\n';
            buffer << point8.pointCoords() << '\n';
            buffer << point7.pointCoords() << '\n';

            for (int k=0;k<3;k++){
                normals.push_back(1);
                normals.push_back(0);
                normals.push_back(0);
            }
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


            for (int k=0;k<3;k++){
                normals.push_back(0);
                normals.push_back(0);
                normals.push_back(1);
            }

            //lower triangle
            buffer << point1.pointCoords() << '\n';
            buffer << point4.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';


            for (int k=0;k<3;k++){
                normals.push_back(0);
                normals.push_back(0);
                normals.push_back(1);
            }

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


            for (int k=0;k<3;k++){
                normals.push_back(0);
                normals.push_back(0);
                normals.push_back(-1);
            }

            //lower triangle
            buffer << point8.pointCoords() << '\n';
            buffer << point5.pointCoords() << '\n';
            buffer << point7.pointCoords() << '\n';


            for (int k=0;k<3;k++){
                normals.push_back(0);
                normals.push_back(0);
                normals.push_back(-1);
            }
       }
    }

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
        
        buffer << point1.pointCoords() << '\n';
        buffer << point2.pointCoords() << '\n';
        buffer << point3.pointCoords() << '\n';

        //ponto central
        normals.push_back(0);
        normals.push_back(-1);
        normals.push_back(0);

        //p2
        normals.push_back(0);
        normals.push_back(sin(angleStep*i));
        normals.push_back(cos(angleStep*i));

        //p3    
        normals.push_back(0);
        normals.push_back(sin(angleStep*(i+1)));
        normals.push_back(cos(angleStep*(i+1)));

        // add upper triangle
        point4.setPoint(0, height, 0);
        point5.setPoint(cos(angleStep*(i+0.5))*radius, 0, sin(angleStep*(i+0.5))*radius);

        buffer << point5.pointCoords() << '\n';
        buffer << point3.pointCoords() << '\n';
        buffer << point2.pointCoords() << '\n';

        //p5
        normals.push_back(0);
        normals.push_back(1);
        normals.push_back(0);

        //p3
        extra = point3;
        extra.normalize();
        normals.push_back(extra.x);
        normals.push_back(extra.y);
        normals.push_back(extra.z);

        //p2
        extra = point2;
        extra.normalize();
        normals.push_back(extra.x);
        normals.push_back(extra.y);
        normals.push_back(extra.z);
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
            buffer << point1.pointCoords() << '\n';
            buffer << point2.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';

            // Calculate the normal for the first triangle of the quad
            extra.setPoint(cos(angle) * (1 - currHeight / height),nextHeight-currHeight,sin(angle) * (1 - currHeight / height));
            extra.normalize();
            normals.push_back(extra.x);
            normals.push_back(extra.y);
            normals.push_back(extra.z);

            extra.setPoint(cos(nextAngle) * (1 - currHeight / height),nextHeight-currHeight,sin(nextAngle) * (1 - currHeight / height));
            extra.normalize();
            normals.push_back(extra.x);
            normals.push_back(extra.y);
            normals.push_back(extra.z);

            extra.setPoint(cos(angle) * (1 - nextHeight / height),nextHeight-currHeight,sin(nextAngle) * (1 - nextHeight / height));
            extra.normalize();
            normals.push_back(extra.x);
            normals.push_back(extra.y);
            normals.push_back(extra.z);

            // Add the second triangle of the quad
            buffer << point2.pointCoords() << '\n';
            buffer << point4.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';

            // Calculate the normal for the second triangle of the quad

            extra.setPoint(point2.x/radius,nextHeight-currHeight,point2.z/radius);
            extra.normalize();
            normals.push_back(extra.x);
            normals.push_back(extra.y);
            normals.push_back(extra.z);

            extra.setPoint(point4.x/radius,nextHeight-currHeight,point4.z/radius);
            extra.normalize();
            normals.push_back(extra.x);
            normals.push_back(extra.y);
            normals.push_back(extra.z);

            extra.setPoint(point3.x/radius,nextHeight-currHeight,point3.z/radius);
            extra.normalize();
            normals.push_back(extra.x);
            normals.push_back(extra.y);
            normals.push_back(extra.z);

        }
    }
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


    for (int i=0;i<slices;i++){
        for(int j=0;j<stacks;j++){
            point1.setPoint(radius*cos(i*angleBase)*cos((j+1)*angleFace), radius*sin((j+1)*angleFace), radius*sin(i*angleBase)*cos((j+1)*angleFace));
            point2.setPoint(radius*cos(i*angleBase)*cos(j*angleFace), radius*sin(j*angleFace), radius*sin(i*angleBase)*cos(j*angleFace));
            point3.setPoint(radius*cos((i+1)*angleBase)*cos(j*angleFace), radius*sin(j*angleFace), radius*sin((i+1)*angleBase)*cos(j*angleFace));
            point4.setPoint(radius*cos((i+1)*angleBase)*cos((j+1)*angleFace), radius*sin((j+1)*angleFace), radius*sin((i+1)*angleBase)*cos((j+1)*angleFace));
        
            
            buffer << point2.pointCoords() << '\n';
            buffer << point1.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';

            buffer << point1.pointCoords() << '\n';
            buffer << point4.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';

            normals.push_back(cos(i*angleBase)*cos((j+1)*angleFace));
            normals.push_back(sin((j+1)*angleFace));
            normals.push_back(sin(i*angleBase)*cos((j+1)*angleFace));
            
            normals.push_back(cos(i*angleBase)*cos(j*angleFace));
            normals.push_back(sin(j*angleFace));
            normals.push_back(sin(i*angleBase)*cos(j*angleFace));

            normals.push_back(cos((i+1)*angleBase)*cos(j*angleFace));
            normals.push_back(sin(j*angleFace));
            normals.push_back(sin((i+1)*angleBase)*cos(j*angleFace));

            normals.push_back(cos(i*angleBase)*cos((j+1)*angleFace));
            normals.push_back(sin((j+1)*angleFace));
            normals.push_back(sin(i*angleBase)*cos((j+1)*angleFace));
            
            normals.push_back(cos((i+1)*angleBase)*cos((j+1)*angleFace));
            normals.push_back(sin((j+1)*angleFace));
            normals.push_back(sin((i+1)*angleBase)*cos((j+1)*angleFace));

            normals.push_back(cos((i+1)*angleBase)*cos(j*angleFace));
            normals.push_back(sin(j*angleFace));
            normals.push_back(sin((i+1)*angleBase)*cos(j*angleFace));
        }
    }

    for (int i=0;i<slices;i++){
        for(int j=0;j<stacks;j++){
            point1.setPoint(radius*cos(i*angleBase)*cos((j+1)*angleFace), -radius*sin((j+1)*angleFace), radius*sin(i*angleBase)*cos((j+1)*angleFace));
            point2.setPoint(radius*cos(i*angleBase)*cos(j*angleFace), -radius*sin(j*angleFace), radius*sin(i*angleBase)*cos(j*angleFace));
            point3.setPoint(radius*cos((i+1)*angleBase)*cos(j*angleFace), -radius*sin(j*angleFace), radius*sin((i+1)*angleBase)*cos(j*angleFace));
            point4.setPoint(radius*cos((i+1)*angleBase)*cos((j+1)*angleFace), -radius*sin((j+1)*angleFace), radius*sin((i+1)*angleBase)*cos((j+1)*angleFace));
        
            
            buffer << point2.pointCoords() << '\n';
            buffer << point1.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';

            buffer << point1.pointCoords() << '\n';
            buffer << point4.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';

            normals.push_back(cos(i*angleBase)*cos((j+1)*angleFace));
            normals.push_back(-sin((j+1)*angleFace));
            normals.push_back(sin(i*angleBase)*cos((j+1)*angleFace));
            
            normals.push_back(cos(i*angleBase)*cos(j*angleFace));
            normals.push_back(-sin(j*angleFace));
            normals.push_back(sin(i*angleBase)*cos(j*angleFace));

            normals.push_back(cos((i+1)*angleBase)*cos(j*angleFace));
            normals.push_back(-sin(j*angleFace));
            normals.push_back(sin((i+1)*angleBase)*cos(j*angleFace));

            normals.push_back(cos(i*angleBase)*cos((j+1)*angleFace));
            normals.push_back(-sin((j+1)*angleFace));
            normals.push_back(sin(i*angleBase)*cos((j+1)*angleFace));
            
            normals.push_back(cos((i+1)*angleBase)*cos((j+1)*angleFace));
            normals.push_back(-sin((j+1)*angleFace));
            normals.push_back(sin((i+1)*angleBase)*cos((j+1)*angleFace));

            normals.push_back(cos((i+1)*angleBase)*cos(j*angleFace));
            normals.push_back(-sin(j*angleFace));
            normals.push_back(sin((i+1)*angleBase)*cos(j*angleFace));
        }
    }

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

     for (int i=0;i<slices;i++){
        for(int j=0;j<stacks;j++){
            point1.setPoint((outer_r+inner_r*cos(j*angleFace))*cos(i*angleBase), (outer_r+inner_r*cos(j*angleFace))*sin(i*angleBase), ratio*inner_r*sin(j*angleFace));
            
            point2.setPoint((outer_r+inner_r*cos((j+1)*angleFace))*cos(i*angleBase), (outer_r+inner_r*cos((j+1)*angleFace))*sin(i*angleBase), ratio*inner_r*sin((j+1)*angleFace));
            
            point3.setPoint((outer_r+inner_r*cos((j+1)*angleFace))*cos((i+1)*angleBase), (outer_r+inner_r*cos((j+1)*angleFace))*sin((i+1)*angleBase), ratio*inner_r*sin((j+1)*angleFace));
            
            buffer << point1.pointCoords() << '\n';
            buffer << point2.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';

            normals.push_back(cos(j*angleFace)*cos(i*angleBase));
            normals.push_back(cos(j*angleFace)*sin(i*angleBase));
            normals.push_back(sin(j*angleFace));

            normals.push_back(cos((j+1)*angleFace)*cos(i*angleBase));
            normals.push_back(cos((j+1)*angleFace)*sin(i*angleBase));
            normals.push_back(sin((j+1)*angleFace));

            normals.push_back(cos((j+1)*angleFace)*cos((i+1)*angleBase));
            normals.push_back(cos((j+1)*angleFace)*sin((i+1)*angleBase));
            normals.push_back(sin((j+1)*angleFace));
            
            point1.setPoint((outer_r+inner_r*cos(j*angleFace))*cos(i*angleBase), (outer_r+inner_r*cos(j*angleFace))*sin(i*angleBase), ratio*inner_r*sin(j*angleFace));
            
            point2.setPoint((outer_r+inner_r*cos((j+1)*angleFace))*cos((i+1)*angleBase), (outer_r+inner_r*cos((j+1)*angleFace))*sin((i+1)*angleBase), ratio*inner_r*sin((j+1)*angleFace));
            
            point3.setPoint((outer_r+inner_r*cos(j*angleFace))*cos((i+1)*angleBase), (outer_r+inner_r*cos(j*angleFace))*sin((i+1)*angleBase), ratio*inner_r*sin(j*angleFace));
            
            buffer << point1.pointCoords() << '\n';
            buffer << point2.pointCoords() << '\n';
            buffer << point3.pointCoords() << '\n';

            normals.push_back(cos(j*angleFace)*cos(i*angleBase));
            normals.push_back(cos(j*angleFace)*sin(i*angleBase));
            normals.push_back(sin(j*angleFace));

            normals.push_back(cos((j+1)*angleFace)*cos((i+1)*angleBase));
            normals.push_back(cos((j+1)*angleFace)*sin((i+1)*angleBase));
            normals.push_back(sin((j+1)*angleFace));

            normals.push_back(cos((j*angleFace))*cos((i+1)*angleBase));
            normals.push_back(cos(j*angleFace)*sin((i+1)*angleBase));
            normals.push_back(sin(j*angleFace));

        }
    }
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
