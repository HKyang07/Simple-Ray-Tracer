#ifndef _PLANE_H_
#define _PLANE_H_

#include "object3d.h"
#include <cmath>
using namespace std;

class Plane :public Object3D
{
public:
	Plane(Vec3f normal, float offset, Material *smp) :normal(normal), offset(offset)
	{
		this->mp = smp;
	}
	~Plane(){}

	void paint()
	{
		Vec3f d1, d2,v(1,0,0);
		normal.Normalize();
		if (normal == v) v = Vec3f(0, 1, 0);
		Vec3f::Cross3(d1,v,normal);
		d1.Normalize();
		Vec3f::Cross3(d2,normal,d1);
		d2.Normalize();
		d1 *= 10000;
		d2 *= 10000;
		Vec3f e1 = d1 + d2, e2 = d1*(-1) + d2, e3 = d1*(-1) - d2, e4 = d1 - d2;
		e1 += offset*normal;
		e2 += offset*normal;
		e3 += offset*normal;
		e4 += offset*normal;
		mp->glSetMaterial();
		glBegin(GL_QUADS);
		glNormal3f(normal.x(), normal.y(), normal.z());
		glVertex3f(e1.x(), e1.y(), e1.z());
		glVertex3f(e2.x(), e2.y(), e2.z());
		glVertex3f(e3.x(), e3.y(), e3.z());
		glVertex3f(e4.x(), e4.y(), e4.z());
		glEnd();
	}

	virtual bool intersect(const Ray &r, Hit &h, float tmin)
	{
		Vec3f r0 = r.getOrigin(), rd = r.getDirection();
		float t;
		if (normal.Dot3(rd) == 0)
			return false;
		else
		{
			t = (offset-normal.Dot3(r0)) / (normal.Dot3(rd));
			if (t <= tmin) return false;
			if (t < h.getT())
			{
				normal.Normalize();
				h.set(t,mp,normal,r);
			}
			else return false;
		}
		return true;
	}
private:
	Vec3f normal;
	float offset;
};
#endif