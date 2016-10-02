#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "object3d.h"
#include <cmath>
using namespace std;

class Sphere:public Object3D
{
public:
	Sphere(Vec3f center, float radius, Material *smp, float a1, float a2, bool g) 
		:center(center), radius(radius), tst(a1), pst(a2), ginter(g)
	{
		this->mp = smp;
	}
	~Sphere(){}

	void paint()
	{
		float ip, it;
		Vec3f tv,p1,p2,p3,p4,e1,e2,e3,ei;
		mp->glSetMaterial();
		glBegin(GL_QUADS);
		for (ip =-90; ip<90; ip +=(180/pst))
			for (it = 0; it <360; it +=(360/tst)) {
			p1 = computexyz(ip,it);
			p2 = computexyz(ip, it + (360 / tst));
			p3 = computexyz(ip + (180 / pst), it + (360 / tst));
			p4 = computexyz(ip + (180 / pst), it);
			if (!ginter)
			{
				e1 = p2 - p1;
				e2 = p4 - p1;
				Vec3f::Cross3(e3,e1,e2);
				e3.Normalize();
				glNormal3f(e3.x(), e3.y(), e3.z());
			}

			tv = p1;
			if (ginter)
			{
				ei = tv - center;
				ei.Normalize();
				glNormal3f(ei.x(),ei.y(),ei.z());
			}
			glVertex3f(tv.x(), tv.y(), tv.z());

			tv = p2;
			if (ginter)
			{
				ei = tv - center;
				ei.Normalize();
				glNormal3f(ei.x(), ei.y(), ei.z());
			}
			glVertex3f(tv.x(), tv.y(), tv.z());
			
			tv = p3;
			if (ginter)
			{
				ei = tv - center;
				ei.Normalize();
				glNormal3f(ei.x(), ei.y(), ei.z());
			}
			glVertex3f(tv.x(), tv.y(), tv.z());
	
			tv = p4;
			if (ginter)
			{
				ei = tv - center;
				ei.Normalize();
				glNormal3f(ei.x(), ei.y(), ei.z());
			}
			glVertex3f(tv.x(), tv.y(), tv.z());
		}
		glEnd();
	}

	Vec3f computexyz(float phi,float theta)
	{
		float x, y, z,xz;
		y = (-1)*radius*sin(phi/180*3.1415f);
		xz = radius*cos(phi/180*3.1415f);
		x = xz*cos(theta / 180 * 3.1415f);
		z = xz*sin(theta / 180 * 3.1415f);
		Vec3f answer(x,y,z);
		answer += center;
		return answer;
	}

	virtual bool intersect(const Ray &r, Hit &h, float tmin)
	{
		Vec3f r0 = r.getOrigin(), rd = r.getDirection();
		float a = pow(rd.Length(), 2),
			  b = 2 * rd.Dot3(r0 - center), 
			  c = r0.Dot3(r0) + center.Dot3(center) - 2 * r0.Dot3(center) - pow(radius, 2);  //求解直线和球体相交的一元二次方程，注意center不一定是原点
		float t;
		if (pow(b,2)< 4*a*c)
		{
			return false;
		}
		else
		{
			t = (-b-sqrt(pow(b,2)-4*a*c))/(2*a);
			if (t<=tmin)
				t = (-b+sqrt(pow(b, 2) - 4*a*c))/(2*a);
			//cout << center << " " << mp->getDiffuseColor() << " " << t << " " << h.getT()<<" "<<r0<< endl;
			if (t > tmin)
			{
				if (t < h.getT())
				{
					Vec3f nor = (r0 + rd*t) - center;
					nor.Normalize();
					h.set(t, mp, nor, r);
				}
				return true;
			}
			return false;
		}
	}
private:
	Vec3f center;
	float radius,tst,pst;
	bool ginter;
};
#endif