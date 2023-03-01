#include <stdio.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

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

class triangle{
    public:
        point p1;
        point p2;
        point p3;

        void moveTriangle(float x,float y, float z){
            p1.translate(x,y,z);
            p2.translate(x,y,z);
            p3.translate(x,y,z);
        }

        void setP1(float x, float y, float z){
            p1.x=x;
            p1.y=y;
            p1.z=z;
        }

        void setP2(float x, float y, float z){
            p2.x=x;
            p2.y=y;
            p2.z=z;
        }
        void setP3(float x, float y, float z){
            p3.x=x;
            p3.y=y;
            p3.z=z;
        }
};


int generatePlane(double length, int divisions, char* filename){
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

    return 0;
}


int generateBox(char *length, char* divisions, char* filename){

    return 0;
}

int generateSphere(char* radius, char* slices, char* stacks, char* filename) {

    return 0;
}

int generateCone(char *radius, char *height, char *slices, char *stacks, char *filename) {


    return 0;
}



int main(int argc, char **argv){
    ofstream file;
    
    try{
       file.open(argv[4]);

        if(!file){
            std::cout << ("\nUnable to open file\n\0");
            return 1;
        }

        if (strcmp(argv[1], "plane")==0){
            generatePlane(std::stod(argv[2]), std::atoi(argv[3]), argv[4]);
        }
        
        if (strcmp(argv[1], "box")==0){
            generateBox(argv[2],argv[3], argv[4]);
        
        }

        if (strcmp(argv[1], "sphere")==0){
            generateSphere(argv[2], argv[3], argv[4], argv[5]);
        }

        if (strcmp(argv[1], "cone")==0){
            generateCone(argv[2], argv[3], argv[4], argv[5], argv[6]);
        }

    }

    //melhorar este catch pois pode ser por argumentos a mais
    catch(...){
        std::cout << "\0Not enough arguments\n\0";
        return 1;
    }

    file << buffer.str();
    file.close();

    std::cout << "\nDone\n\0";

    return 0;
}
