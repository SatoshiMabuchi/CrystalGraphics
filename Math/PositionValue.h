#ifndef __CRYSTAL_MATH_POSITION_VALUE_H__
#define __CRYSTAL_MATH_POSITION_VALUE_H__

#include "Vector.h"

namespace Crystal {
	namespace Math {

template<typename GeomType, typename ValueType>
class PositionValue
{
public:
	PositionValue() :
		pos(Vector3d<GeomType>(0,0,0)),
		value(0)
	{}


	PositionValue(const Vector3d<GeomType>& p, const ValueType& v) :
		pos(p),
		value(v)
	{}

	Vector3d<GeomType> getInterpolatedPosition(const ValueType v, const PositionValue& rhs) const
	{
		const GeomType scale = (v - this->value) / static_cast<GeomType>(rhs.value - this->value);
		return this->pos + scale * (rhs.pos - this->pos);
	}

	bool equals(const PositionValue& rhs) const {
		return
			pos == rhs.pos &&
			value == rhs.value;
	}

	bool operator==(const PositionValue& rhs) const {
		return equals(rhs);
	}

	bool operator!=(const PositionValue& rhs) const {
		return !equals(rhs);
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