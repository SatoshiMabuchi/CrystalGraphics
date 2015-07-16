#ifndef __CRYSTAL_MATH_VOLUME_CELL_H__
#define __CRYSTAL_MATH_VOLUME_CELL_H__

#include "Space.h"
#include "PositionValue.h"
#include <array>

namespace Crystal {
	namespace Math {

template< typename GeomType, typename ValueType >
class VolumeCell3d
{
public:
	VolumeCell3d(const Space3d<GeomType>& space, const std::array< ValueType, 8>& values) :
		space(space),
		values(values)
	{}

	Space3d<GeomType> getSpace() const { return space; }

	std::array< ValueType, 8 > getValues() const { return values; }

	std::array< PositionValue<GeomType, ValueType>, 8 > toPositionValues() const {
		std::array< PositionValue<GeomType, ValueType>, 8 > pvs;
		const auto& positions = space.toArray();
		for (size_t i = 0; i < 8; ++i) {
			pvs[i] = PositionValue<GeomType, ValueType>(positions[i], values[i]);
		}
		return pvs;
	}

private:
	Space3d<GeomType> space;
	std::array< ValueType, 8> values;
};

	}
}

#endif