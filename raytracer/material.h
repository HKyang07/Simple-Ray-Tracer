#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.h" 
#include "ray.h"
#include "hit.h"

using namespace std;

class Material {
public:

  // CONSTRUCTORS & DESTRUCTOR
  Material(){}
  Material(const Vec3f &d_color) { diffuseColor = d_color; }
  virtual ~Material() {}

  // ACCESSORS
  virtual Vec3f getDiffuseColor() const { return diffuseColor; }
  virtual Vec3f getSpecularColor() const{ return specularColor; }
  virtual Vec3f Shade (const Ray &ray, const Hit &hit, const Vec3f &dirToLight,const Vec3f &lightColor,bool back) const = 0;
  virtual void glSetMaterial(void) const = 0;

  // REPRESENTATION
  Vec3f diffuseColor, specularColor,reflectiveColor,transparentColor;
  float exponent,i_refraction;
};

class PhongMaterial : public Material
{
public:
	PhongMaterial(){}
	PhongMaterial(const Vec3f &d_Color, const Vec3f &s_Color, float exp, const Vec3f &refle_Color, const Vec3f &trans_Color, float index){
		specularColor = s_Color;
		diffuseColor = d_Color;
		exponent = exp;
		reflectiveColor = refle_Color;
		transparentColor = trans_Color;
		i_refraction = index;
		//cout << "phongmaterialinit!!\n";
	}
	~PhongMaterial(){}

	virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor,bool back) const;
	void glSetMaterial() const;

	Vec3f getSpecularColor() const
	{
		return specularColor;
	}
	Vec3f getDiffuseColor() const { return diffuseColor; }
};


#endif
