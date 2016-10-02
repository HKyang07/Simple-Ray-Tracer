#ifndef _RAY_H
#define _RAY_H

#include <iostream>
#include "vectors.h"

using namespace std;
class Ray {

public:
  // CONSTRUCTOR & DESTRUCTOR
  Ray () {}
  Ray (const Vec3f &orig, const Vec3f &dir) {
    origin = orig; 
    direction = dir;// Do not normalize this direction vector!!!the Transform can change the magnitude to obtain correct t value
  }
  Ray (const Ray& r) {*this=r;}

  // ACCESSORS
  const Vec3f& getOrigin() const { return origin; }
  const Vec3f& getDirection() const { return direction; }
  Vec3f pointAtParameter(float t) const {
    return origin+direction*t; }

private:
  Vec3f origin;
  Vec3f direction;
};

inline ostream &operator<<(ostream &os, const Ray &r) {
  os << "Ray <o:" <<r.getOrigin()<<", d:"<<r.getDirection()<<">";
  return os;
}

#endif
