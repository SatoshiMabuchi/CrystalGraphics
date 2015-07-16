#ifndef __CRYSTAL_MATH_POSITION_VALUE_H__
#define __CRYSTAL_MATH_POSITION_VALUE_H__

#include "Vector.h"

namespace Crystal {
	namespace Math {

template<typename GeomType, typename ValueType>
class PositionValue
{
public:
	PositionValue(const Vector3d<GeomType>& p, const ValueType& v) :
		pos(p),
		value(v)
	{}

	Vector3d<GeomType> getInterpolatedPosition(const ValueType v, PositionValue& rhs) const
	{
		const GeomType scale = (v - this->value) / static_cast<GeomType>(rhs.value - this->value);
		return this->pos + scale * (rhs.pos - this->pos);
	}

	/*
	Math::Vector3d<GeomType> getPosition() const { return pos; }

	ValueType getValue() const { return value; }
	*/

private:
	Math::Vector3d<GeomType> pos;
	ValueType value;
};

	}
}

#endif