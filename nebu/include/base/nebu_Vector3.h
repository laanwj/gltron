#ifndef NEBU_Vector3_H
#define NEBU_Vector3_H

#ifdef DEBUG
#include "iostream.h"
#endif

#include "math.h"

class Vector3 {
 public:
  Vector3() { };
  Vector3(float a, float b, float c) {
    x = a; y = b; z = c;
  };
  Vector3(const float *v) {
    x = v[0]; y = v[1]; z = v[2];
  };
  float x, y, z;

  const Vector3 operator+ (const Vector3& v) const {
    return Vector3(x + v.x, y + v.y, z + v.z);
  };

  const Vector3 operator- (const Vector3& v) const {
    return Vector3(x - v.x, y - v.y, z - v.z);
  };

  float operator* (const Vector3& v) const {
    return x * v.x + y * v.y + z * v.z;
  };

  const Vector3 operator* (float f) const {
    return Vector3(x * f, y * f, z * f);
  }

  void operator*= (float f) {
    x *= f; y *= f; z *= f;
  }

  void operator/= (float f) {
    x /= f; y /= f; z /= f;
  }

  float Length() const {
    return (float) sqrt(*this * *this);
  }

  float Length2() {
    return *this * *this;
  }

  Vector3& Normalize() {
    float length = Length();
    if(length != 0)
      *this /= length;
    return *this;
  }

  Vector3 Cross(const Vector3& v) const {
    return Vector3(y * v.z - z * v.y,
		   z * v.x - x * v.z, 
		   x * v.y - y * v.z);
  }

  #ifdef DEBUG
  friend ostream& 
    operator<<(ostream& os, const Vector3& v);

  friend Vector3
    operator*(float f, const Vector3& v);
  #endif
};

inline Vector3 operator*(float f, const Vector3& v) {
  return v * f;
}

#ifdef DEBUG
inline ostream& operator<<(ostream& os, const Vector3& v) {
  os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  return os;
}
#endif
#endif
