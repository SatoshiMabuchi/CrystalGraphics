#ifndef __CRYSTAL_MATH_BOX_H__
#define __CRYSTAL_MATH_BOX_H__

#include "Vector.h"

#include <vector>
#include <string>
#include <algorithm>

namespace Crystal{
	namespace Math{

template<typename T>
class Box final
{
public:
	
	Box() : Box(Vector3d<T>(0, 0, 0), Vector3d<T>(1, 1, 1))
	{
	}

	Box(const Vector3d<T>& pointX, const Vector3d<T>& pointY)
	{
		const auto x = std::min<T>(pointX.getX(), pointY.getX());
		const auto y = std::min<T>(pointX.getY(), pointY.getY());
		const auto z = std::min<T>(pointX.getZ(), pointY.getZ());
		this->start = Vector3d<T>(x, y, z);
		const auto endX = std::max<T>(pointX.getX(), pointY.getX());
		const auto endY = std::max<T>(pointX.getY(), pointY.getY());
		const auto endZ = std::max<T>(pointX.getZ(), pointY.getZ());
		this->end = Vector3d<T>(endX, endY, endZ);
		assert(isValid());
	}

	~Box() = default;

	static Box Unit() {
		return Box();
	}

	Box getBoundingBox() const { return *this; }

	void add(const Vector3d<T>& v) {
		const auto x = std::min<T>( getMinX(), v.getX());
		const auto y = std::min<T>( getMinY(), v.getY());
		const auto z = std::min<T>( getMinZ(), v.getZ());
		start = Vector3d <T>(x, y, z);

		const auto endX = std::max<T>(end.getX(), v.getX());
		const auto endY = std::max<T>(end.getY(), v.getY());
		const auto endZ = std::max<T>(end.getZ(), v.getZ());
		end = Vector3d<T>(endX, endY, endZ);
	}

	void add(const Box& b) {
		const auto x = std::min<T>( getMinX(), b.getMinX());
		const auto y = std::min<T>( getMinY(), b.getMinY());
		const auto z = std::min<T>( getMinZ(), b.getMinZ());


		const auto ex = std::max<T>(end.getX(), b.getMaxX());
		const auto ey = std::max<T>(end.getY(), b.getMaxY());
		const auto ez = std::max<T>(end.getZ(), b.getMaxZ());
		end = Vector3d<T>(ex, ey, ez);
	}
	
	T getVolume() const {
		return (end.getX() - getMinX()) * (end.getY() - getMinY()) * (end.getZ() - getMinZ());
	}
	
	Vector3d<T> getMax() const { return Vector3d<T>(end.getX(), end.getY(), end.getZ()); }
	
	Vector3d<T> getMin() const { return Vector3d<T>(getMinX(), getMinY(), getMinZ()); }

	Vector3d<T> getStart() const { return getMin(); }

	Vector3d<T> getEnd() const { return getMax(); }

	Vector3d<T> getCenter() const {
		return Vector3d<T>(
			(getMinX() + end.getX()) / T{ 2 },
			(getMinY() + end.getY()) / T{ 2 },
			(getMinZ() + end.getZ()) / T{ 2 }
			);
	}


	bool isInterior(const Vector3d<T>& point) const {
		const bool xIsInterior = (getMinX() < point.getX() && point.getX() < end.getX());
		const bool yIsInterior = (getMinY() < point.getY() && point.getY() < end.getY());
		const bool zIsInterior = (getMinZ() < point.getZ() && point.getZ() < end.getZ());
		return xIsInterior && yIsInterior && zIsInterior;
	}
	
	bool isExterior(const Vector3d<T>& point) const {
		return !isInterior(point);
	}
	
	void outerOffset(const T offsetLength) {
		const auto x = getMinX() - offsetLength;
		const auto y = getMinY() - offsetLength;
		const auto z = getMinZ() - offsetLength;
		start = Vector3d<T>(x, y, z);
		end += Vector3d<T>(offsetLength, offsetLength, offsetLength);
		assert(isValid());
	}
	
	Box getOuterOffset(const T offsetLength) const {
		Box box = *this;
		box.outerOffset(offsetLength);
		return box;
	}

	void innerOffset(const T offsetLength) {
		outerOffset(-offsetLength);
		assert(isValid());
	}

	Box getInnerOffset(const T offsetLength) const {
		return getOuterOffset(-offsetLength);
	}

	Vector3dVector<T> toPoints(const T divideLength) const {
		Vector3dVector<T> points;
		for (T x = getMinX(); x <= end.getX(); x += divideLength) {
			for (T y = getMinY(); y <= end.getY(); y += divideLength) {
				for (T z = getMinZ(); z <= end.getZ(); z += divideLength) {
					points.push_back(Vector3d(x, y, z));
				}
			}
		}
		return points;
	}

	T getMaxX() const { return end.getX(); }

	T getMinX() const { return start.getX(); }

	T getMaxY() const { return end.getY(); }

	T getMinY() const { return start.getY(); }

	T getMaxZ() const { return end.getZ(); }

	T getMinZ() const { return start.getZ(); }

	Vector3d<T> getLength() const {
		return Vector3d<T>(end.getX() - getMinX(), end.getY() - getMinY(), end.getZ() - getMinZ());
	}

	bool isValid() const {
		return
			(getMinX() <= end.getX()) && (getMinY() <= end.getY()) && (getMinZ() <= end.getZ());
	}

	bool isShirinked() const{
		return
			(getMinX() == end.getX()) && (getMinY() == end.getY()) && (getMinZ() == end.getZ());
	}

	bool equals(const Box& rhs) const {
		return
			start == rhs.getStart() &&
			Tolerance<T>::isEqualLoosely(end.getX(), rhs.end.getX()) &&
			Tolerance<T>::isEqualLoosely(end.getY(), rhs.end.getY()) &&
			Tolerance<T>::isEqualLoosely(end.getZ(), rhs.end.getZ());
	}

	bool operator==( const Box& rhs ) const { return equals( rhs ); }

	bool operator!=( const Box& rhs ) const { return !equals( rhs ); }

	bool hasIntersection(const Box& rhs) const {
		const auto distx = std::fabs(getCenter().getX() - rhs.getCenter().getX());
		const auto lx = getLength().getX() / T{ 2 } +rhs.getLength().getX() / T{ 2 };

		const auto disty = std::fabs(getCenter().getY() - rhs.getCenter().getY());
		const auto ly = getLength().getY() / T{ 2 } +rhs.getLength().getY() / T{ 2 };

		const auto distz = std::fabs(getCenter().getZ() - rhs.getCenter().getZ());
		const auto lz = getLength().getZ() / T{ 2 } +rhs.getLength().getZ() / T{ 2 };

		return (distx < lx && disty < ly && distz < lz);
	}


	Box getOverlapped(const Box& rhs) const {
		assert(hasIntersection(rhs));
		const auto minx = std::max<T>(this->getStart().getX(), rhs.getStart().getX());
		const auto miny = std::max<T>(this->getStart().getY(), rhs.getStart().getY());
		const auto minz = std::max<T>(this->getStart().getZ(), rhs.getStart().getZ());

		const auto maxx = std::min<T>(this->getEnd().getX(), rhs.getEnd().getX());
		const auto maxy = std::min<T>(this->getEnd().getY(), rhs.getEnd().getY());
		const auto maxz = std::min<T>(this->getEnd().getZ(), rhs.getEnd().getZ());

		const Vector3d<T> min_(minx, miny, minz);
		const Vector3d<T> max_(maxx, maxy, maxz);
		return Box(min_, max_);
	}


private:
	Vector3d<T> start;
	Vector3d<T> end;
};

	}
}

#endif