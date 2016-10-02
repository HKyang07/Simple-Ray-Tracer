#ifndef _GROUP_H_
#define _GROUP_H_

#include "object3d.h"

using namespace std;

class Group:public Object3D
{
private:
	int objnum;
	Object3D **list;  //备注，研究一下**怎么申请空间。

public:
	Group(int num):objnum(num)
	{
		list = new Object3D*[objnum];
	}
	~Group(){}

	void addObject(int index, Object3D *obj)
	{
		assert(index<objnum);
		list[index]=obj;
	}

	void paint()
	{
		Object3D *curr1;
		//bool result = false;
		for (int i = 0; i < objnum; i++)
		{
			curr1 = list[i];
			//cout << curr->mp->exponent;
			//cout << curr->mp->specularColor;
			curr1->paint();
		}
		//return 0;
	}

	bool intersect(const Ray &r, Hit &h, float tmin)
	{
		Object3D *curr;
		bool result = false;
		for (int i = 0; i < objnum; i++)
		{
			curr = list[i];
			if (curr->intersect(r, h, tmin))
			{
				result = true;
			}
		}
		return result;
	}
};

#endif