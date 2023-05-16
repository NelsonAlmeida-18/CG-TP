#include <cstring>

using namespace std;

class point{
    public:
        float x;
        float y;
        float z;
        float xN=0;
        float yN=0;
        float zN=0;

        point(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

        void translate(float Tx, float Ty, float Tz){
            x+=Tx;
            y+=Ty;
            z+=Tz;
        }

        point operator- (const point& other) const {
            return point(x - other.x, y - other.y, z - other.z);
        }

        point operator/ (const point& other) const {
            return point(x / other.x, y / other.y, z / other.z);
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
            return "v " + to_string(x) + " " + to_string(y) + " " + to_string(z) + "\n";
        }

        string normalCoords(){
            return "vn " + to_string(xN) + " " + to_string(yN) +" " + to_string(zN) + "\n";
        }

        void normalize(){
            float size = sqrt(pow(x,2)+pow(y,2)+pow(z,2));
            xN=xN/size;
            yN=yN/size;
            zN=zN/size;
        }

        point divideNormal(float size){
            return point(xN/size, yN/size, zN/size);
        }

        void add2Normal(point a){
            xN+=a.x;
            yN+=a.y;
            zN+=a.z;
        }
};