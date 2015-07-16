#ifndef __CRYSTAL_MATH_VOLUME_CELL_H__
#define __CRYSTAL_MATH_VOLUME_CELL_H__

#include "Space.h"
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

private:
	Space3d<GeomType> space;
	std::array< ValueType, 8> values;
};

	}
}

#endif