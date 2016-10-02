#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "object3d.h"
#include <cmath>
using namespace std;

class Transform :public Object3D
{
public:
	Transform(Matrix tm, Object3D *obj)
	{
		this->tm = tm;
		this->obj = obj;
		this->tm.Inverse(tmi);
		tmi.Transpose(tmit);
	}
	~Transform(){}

	void paint()
	{
		glPushMatrix();
		GLfloat *glMatrix = tm.glGet();
		glMultMatrixf(glMatrix);
		delete[] glMatrix;
		obj->paint();
		glPopMatrix();
	}

	virtual bool intersect(const Ray &r, Hit &h, float tmin)
	{
		Vec3f r0 = r.getOrigin(), rd = r.getDirection();
		//cout << r0 << " ";
		tmi.Transform(r0);
		//cout << r0 << endl;
		tmi.TransformDirection(rd);
		//cout << r0 << endl;
		Ray r1(r0, rd);
		if (obj->intersect(r1, h, tmin))
		{
			Vec3f n = h.getNormal();
			tmit.TransformDirection(n);
			n.Normalize();
			h.set(h.getT(), h.getMaterial(), n, r);
		}
		else return false;
		return true;
	}
private:
	Matrix tm, tmi, tmit;
	Object3D *obj;
};
#endif