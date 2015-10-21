#ifndef __CRYSTAL_MATH_CONE_H__
#define __CRYSTAL_MATH_CONE_H__

#include "Vector.h"

#include "Box.h"

#include <vector>
#include <cassert>

namespace Crystal {
	namespace Math {

template<typename T>
class Cone final
{
public:
	Cone() :
		center( Vector3d<T>::Zero() ),
		radius( 1.0f ),
		height( 1.0f )
	{}

	Cone( const Vector3d<T>& center, float radius ) :
	center( center ),
	radius( radius )
	{
	}

	Vector3d<T> getCenter() const { return center; }

	void setRadius(const float r) { this->radius = r; }

	float getRadius() const { return radius; }

	void setHeight(const float h) { this->height = h; }

	float getHeight() const { return height; }

	bool isValid() const {
		return ( height > 0.0f ) && ( radius > 0.0f ) ;
	}

	float getVolume() const {
		return radius * radius * Tolerance<T>::getPI() * height / 3.0f;
	}

private:
	Vector3d<T> center;
	float radius;
	float height;
};

	}
}

#endif