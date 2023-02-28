#include <string>


struct Light{
    std::string type;
    int posX;
    int posY;
    int posZ;
    int dirX;
    int dirY;
    int dirZ;
    int cutoff;
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
    std::string texture;
};
