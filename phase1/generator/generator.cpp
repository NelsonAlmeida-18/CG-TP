#include <stdio.h>
#include <cstring>
#include <iostream>

int generatePlane(double length, double divisions, char* filename){
    
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
    if (strcmp(argv[1], "plane")==0){
        generatePlane(std::stod(argv[2]), std::stod(argv[3]), argv[4]);
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
    
    return 0;
}