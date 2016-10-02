#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vectors.h"
#include "ray.h"
#include <cmath>
#include <cfloat>
#include <GL/glut.h>
using namespace std;

class Camera
{
public:
	Camera(){};
	~Camera(){};
	virtual Ray generateRay(Vec2f point) = 0;  //generate the ray
	virtual float getTMin() const = 0;
	virtual void glInit(int w, int h) = 0;
	virtual void glPlaceCamera(void) = 0;
	virtual void dollyCamera(float dist) = 0;
	virtual void truckCamera(float dx, float dy) = 0;
	virtual void rotateCamera(float rx, float ry) = 0;
};

class OrthographicCamera :public Camera
{
public:
	Vec3f center, direction, up, horizontal;
	float size;

	OrthographicCamera(){};
	OrthographicCamera(Vec3f center1,Vec3f direction1,Vec3f up1, float size){
		center = center1;
		direction = direction1;
		direction.Normalize();
		up = up1;
		Vec3f::Cross3(horizontal,up,direction);  //ͨ���������ˮƽ����λ�������������������direction�������޸ĺ��up����
		horizontal.Negate();  //opengl������ϵ����ѧ���õ�������ϵ��һ��Ľ�ģ��Ϸ���������ϵ��
		horizontal.Normalize();
		Vec3f::Cross3(up,horizontal,direction);
		up.Normalize();
		this->size = size;
	};
	~OrthographicCamera(){};
	
	Ray generateRay(Vec2f point)
	{
		return Ray(center+size*horizontal*(point.x()-0.5f)+size*up*(point.y()-0.5f), direction); //���������ǣ�0��0������1��1���ı������꣬����Ļ�ߴ紫��ȥ
	}
	float getTMin() const
	{
		return 0.0001f;  //��Сֵ
	}

	virtual void glInit(int w, int h);
	virtual void glPlaceCamera(void);
	virtual void dollyCamera(float dist);
	virtual void truckCamera(float dx, float dy);
	virtual void rotateCamera(float rx, float ry);

};

class PerspectiveCamera :public Camera
{
public:
	Vec3f center, direction, up, horizontal;
	float angle;

	PerspectiveCamera(){};
	PerspectiveCamera(Vec3f center1, Vec3f direction1, Vec3f up1, float angle){
		center = center1;
		direction = direction1;
		direction.Normalize();
		up = up1;
		Vec3f::Cross3(horizontal, up, direction);  //ͨ���������ˮƽ����λ�������������������direction�������޸ĺ��up����
		horizontal.Negate();  //opengl������ϵ����ѧ���õ�������ϵ��һ��Ľ�ģ��Ϸ���������ϵ��
		horizontal.Normalize();
		Vec3f::Cross3(up, horizontal, direction);
		up.Normalize();
		this->angle = angle;
	};
	~PerspectiveCamera(){};

	Ray generateRay(Vec2f point)
	{
		Vec3f screenpoint = center + horizontal*(point.x() - 0.5f) + up*(point.y() - 0.5f)+direction*sqrt(0.25f / pow(sin(angle / 2), 2) - 0.5f);

		Vec3f dir1 = screenpoint - center;
		dir1.Normalize();
		return Ray(center , dir1); //���������ǣ�0��0������1��1���ı������꣬����Ļ�ߴ紫��ȥ
	}
	float getTMin() const
	{
		return 0.0001f;  //��Сֵ
	}

	void glInit(int w, int h);
	void glPlaceCamera(void);
	void dollyCamera(float dist);
	void truckCamera(float dx, float dy);
	void rotateCamera(float rx, float ry);

};


#endif