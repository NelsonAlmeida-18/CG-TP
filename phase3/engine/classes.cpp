#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include "classes.h"
#include <math.h>
#include <string.h>


bool renderCurve = true;

void processNormalKeys(unsigned char key, int x, int y){
	if(key == 99 || key == 67){
		renderCurve = !renderCurve;
	}
}


void buildRotMatrix(float *x, float *y, float *z, float *m) {

	m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
	m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
	m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}


void cross(float *a, float *b, float *res) {

	res[0] = a[1]*b[2] - a[2]*b[1];
	res[1] = a[2]*b[0] - a[0]*b[2];
	res[2] = a[0]*b[1] - a[1]*b[0];
}


void normalize(float *a) {

	float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0]/l;
	a[1] = a[1]/l;
	a[2] = a[2]/l;
}


float length(float *v) {

	float res = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	return res;

}

void multMatrixVector(float *m, float *v, float *res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j*4+k];
		}
	}

}


void getCatmullRomPoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *deriv) {

	// catmull-rom matrix
	float m[4][4] = {	{-0.5f,  1.5f, -1.5f,  0.5f},
						{ 1.0f, -2.5f,  2.0f, -0.5f},
						{-0.5f,  0.0f,  0.5f,  0.0f},
						{ 0.0f,  1.0f,  0.0f,  0.0f}};

	// Compute A = M * P
	for(int i = 0; i < 3; i++){
		float point[4] = {p0[i], p1[i], p2[i], p3[i]};
		float a[4];

		multMatrixVector((float *) m, point, a);
		
		// Compute pos = T * A
		pos[i] = 0;
		float T[4] = {powf(t, 3.0), powf(t, 2.0), t, 1.0};
		for(int j = 0; j < 4; j++){
			pos[i] += T[j]*a[j];
		}
		
		// compute deriv = T' * A
		deriv[i] = 0;
		float t_deriv[4] = {3*powf(t, 2.0), 2*t, 1, 0};
		for(int j = 0; j < 4; j++){
			deriv[i] += t_deriv[j]*a[j];
		}
	}
}


void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv, std::vector<Point> points) {
	int num_points = points.size();
	float p[num_points][3];
	float t = gt * num_points; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

	for(int i = 0; i < num_points; i++){
		p[i][0] = points[i].x;
		p[i][1] = points[i].y;
		p[i][2] = points[i].z;
	}

	// indices store the points
	int indices[4]; 
	indices[0] = (index + num_points-1)%num_points;	
	indices[1] = (indices[0]+1)%num_points;
	indices[2] = (indices[1]+1)%num_points; 
	indices[3] = (indices[2]+1)%num_points;

	getCatmullRomPoint(t, p[indices[0]], p[indices[1]], p[indices[2]], p[indices[3]], pos, deriv);
}


void renderCatmullRomCurve(std::vector<Point> points) {

// draw curve using line segments with GL_LINE_LOOP
	float point[3];
	float point_deriv[3];

	glBegin(GL_LINE_LOOP);

	for(float gt = 0; gt < 1; gt += 0.01){
		getGlobalCatmullRomPoint(gt, point, point_deriv, points);
		glVertex3f(point[0], point[1], point[2]);
	}
	glEnd();
}


void TranslateCurve::execute(){
	float pos[3], pos_deriv[3];

	if(renderCurve){
		renderCatmullRomCurve(this->points);
	}
	
	getGlobalCatmullRomPoint(this->t, pos, pos_deriv, this->points);
	
	glTranslatef(pos[0], pos[1], pos[2]);

	if(this->align){
		float x[3] = {pos_deriv[0], pos_deriv[1], pos_deriv[2]};
		float y[3], z[3];

		normalize(x);
		cross(x, this->y_prev, z);
		normalize(z);
		cross(z, x, y);

		for(int i = 0; i < 3; i++){
			this->y_prev[i] = y[i];
		}

		float m[16];
		buildRotMatrix(x, y, z, m);
		glMultMatrixf(m);
	}

	float elapsed_time = glutGet(GLUT_ELAPSED_TIME);
	float interval = elapsed_time - this->time_checkpoint;

	this->time_checkpoint = elapsed_time;
	this->t += interval/(this->time);
	
}