#ifndef __CRYSTAL_MATH_GRID_SPACE_BASE_H__
#define __CRYSTAL_MATH_GRID_SPACE_BASE_H__

#include "Space.h"

namespace Crystal{
	namespace Math{

using Index3d = std::array < unsigned int, 3 > ;

template<typename T>
class GridSpaceBase
{
public:
	GridSpaceBase() = default;

	virtual ~GridSpaceBase() = default;

	explicit GridSpaceBase(const Space3d<T>& space) :
		space(space),
		sizes({1,1,1})
	{
	}

	GridSpaceBase(const Space3d<T>& space, const Index3d& sizes) :
		space(space),
		sizes(sizes)
	{
	}

public:
	Space3d<T> getSpace() const { return space; }

	void scale(const Vector3d<T>& v) {
		space.scale(v);
	}

	Vector3d<T> getLengths() const { return space.getLengths(); }

	T getMinLength() const { return std::min < T >( { getLengths().getX(), getLengths().getY(), getLengths().getZ() } ); }

	T getMaxLength() const { return std::max < T >( { getLengths().getX(), getLengths().getY(), getLengths().getZ() } ); }

	Vector3d<T> getStart() const { return space.getStart(); }

	Vector3d<T> getCenter() const { return space.getCenter(); }

	Vector3d<T> getEnd() const { return space.getEnd(); }

	std::array < unsigned int, 3 > getResolutions() const { return sizes; }

	Vector3d<T> getUnitLengths() const {
		const auto x = space.getLengths().getX() / getResolutions()[0];
		const auto y = space.getLengths().getY() / getResolutions()[1];
		const auto z = space.getLengths().getZ() / getResolutions()[2];
		return Vector3d<T>(x, y, z);
	}

	T getUnitVolume() const {
		const auto unitLength = getUnitLengths();
		return unitLength.getX() * unitLength.getY() * unitLength.getZ();
	}

	Vector3d<T> getNormalized(const size_t ix, const size_t iy, const size_t iz) const {
		const auto x = ix / static_cast<T>(getResolutions()[0]) * space.getLengths().getX();
		const auto y = iy / static_cast<T>(getResolutions()[1]) * space.getLengths().getY();
		const auto z = iz / static_cast<T>(getResolutions()[2]) * space.getLengths().getZ();
		return Vector3d<T>(x, y, z) + getUnitLengths() * T(0.5);
	}

	std::array< unsigned int, 3 > toIndex(const Vector3d<T>& p) const {
		const auto unitLength = getUnitLengths();
		const auto ix = static_cast<unsigned int>((p.getX() - space.getStart().getX()) / unitLength.getX());
		const auto iy = static_cast<unsigned int>((p.getY() - space.getStart().getY()) / unitLength.getY());
		const auto iz = static_cast<unsigned int>((p.getZ() - space.getStart().getZ()) / unitLength.getZ());
		return clamp({ ix, iy, iz });
	}


	Vector3d<T> toCenterPosition(const size_t x, const size_t y, const size_t z) const {
		const auto unitLengths = getUnitLengths();
		const auto xx = x * unitLengths.getX() + unitLengths.getX() * T(0.5) + getStart().getX();
		const auto yy = y * unitLengths.getY() + unitLengths.getY() * T(0.5) + getStart().getY();
		const auto zz = z * unitLengths.getZ() + unitLengths.getZ() * T(0.5) + getStart().getZ();
		return Vector3d<T>(xx, yy, zz);
	}

	Vector3dVector<T> toCenterPositions() const {
		Vector3dVector<T> positions;
		for (size_t x = 0; x < sizes[0]; ++x) {
			for (size_t y = 0; y < sizes[1]; ++y) {
				for (size_t z = 0; z < sizes[2]; ++z) {
					positions.push_back( toCenterPosition(x, y, z) );
				}
			}
		}
		return positions;
	}

	Space3d<T> getOverlappedSpace(const Space3d<T>& rhs) const {
		return space.getOverlapped(rhs);
	}

	GridSpaceBase& move(const Vector3d<T>& vector) {
		space.move(vector);
		return (*this);
	}

private:
	Space3d<T> space;
	Index3d sizes;

	std::array< unsigned int, 3 > clamp(const Index3d& i) const {
		const auto ix = std::min<unsigned int>(getResolutions()[0], i[0]);
		const auto iy = std::min<unsigned int>(getResolutions()[1], i[1]);
		const auto iz = std::min<unsigned int>(getResolutions()[2], i[2]);
		return{ ix, iy, iz };
	}
};


	}
}

#endif