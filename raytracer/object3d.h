#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "vectors.h"
#include "material.h"
#include "matrix.h"
#include "hit.h"
#include "ray.h"
#include <GL/glut.h>
using namespace std;

class Object3D
{
public:
	Object3D(){};
	~Object3D(){};
	
	Material *mp;
	virtual bool intersect(const Ray &r, Hit &h, float tmin)=0;  //记录交点的纯虚函数

	virtual void paint(void) = 0;
};

#endif