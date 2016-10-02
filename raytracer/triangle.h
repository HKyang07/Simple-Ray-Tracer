#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "object3d.h"
#include <cmath>
using namespace std;

class Triangle :public Object3D
{
public:
	Triangle(Vec3f v0, Vec3f v1, Vec3f v2, Material *smp) :a(v0), b(v1), c(v2)
	{
		this->mp = smp;
		e1 = v1 - v0;
		e2 = v2 - v0;
		Vec3f::Cross3(e3, e1, e2);
		//area = e3.Length();
	}
	~Triangle(){}

	void paint()
	{
		e3.Normalize();
		mp->glSetMaterial();
		glBegin(GL_TRIANGLES);
		glNormal3f(e3.x(), e3.y(), e3.z());
		glVertex3f(a.x(), a.y(), a.z());
		glVertex3f(b.x(), b.y(), b.z());
		glVertex3f(c.x(), c.y(), c.z());
		glEnd();
	}

	virtual bool intersect(const Ray &r, Hit &h, float tmin)
	{
		Vec3f r0 = r.getOrigin(), rd = r.getDirection();
		float alpha, beta, t,
			a1[] = { a.x() - r0.x(), a.x() - c.x(), rd.x(), 0, a.y() - r0.y(), a.y() - c.y(), rd.y(), 0, a.z() - r0.z(), a.z() - c.z(), rd.z(), 0, 0, 0, 0, 1 },
			b1[] = { a.x() - b.x(), a.x() - r0.x(), rd.x(), 0, a.y() - b.y(), a.y() - r0.y(), rd.y(), 0, a.z() - b.z(), a.z() - r0.z(), rd.z(), 0, 0, 0, 0, 1 },
			c1[] = { a.x() - b.x(), a.x() - c.x(), a.x() - r0.x(), 0, a.y() - b.y(), a.y() - c.y(), a.y() - r0.y(), 0, a.z() - b.z(), a.z() - c.z(), a.z() - r0.z(), 0, 0, 0, 0, 1 },
			d[] = { a.x() - b.x(), a.x() - c.x(), rd.x(), 0, a.y() - b.y(), a.y() - c.y(), rd.y(), 0, a.z() - b.z(), a.z() - c.z(), rd.z(), 0, 0, 0, 0, 1 };
		Matrix m1(a1), m2(b1), m3(c1), m4(d);
		alpha = m1.Detv() / m4.Detv();
		beta = m2.Detv() / m4.Detv();
		t = m3.Detv() / m4.Detv();
		//cout<<area;
		//cout << alpha + beta << " " << alpha << " " << beta << " " << t << endl;
		if (alpha + beta > 1 || alpha < 0 || beta < 0 || t <= tmin) return false;
		//if (alpha + beta >= 1 || alpha <= 0 || beta <= 0) return false;
		//if (t<0)cout << m3.Detv() << " ";
		if (t < h.getT())
		{
			e3.Normalize();
			h.set(t, mp, e3, r);
		}
		else return false;
		return true;
	}
private:
	Vec3f a, b, c, e1, e2, e3;
	float area;
};
#endif