#ifndef __CRYSTAL_GRAPHICS_BRUSH_H__
#define __CRYSTAL_GRAPHICS_BRUSH_H__

#include "../Math/Vector.h"
#include "../Math/Volume.h"

namespace Crystal {
	namespace Graphics {

template<typename GeomType, typename ValueType>
class Brush {
public:
	Brush() = default;

	explicit Brush(const Math::Vector3d<GeomType>& center) :
		center(center),
		size(1,1,1)
	{}

	Brush(const Math::Vector3d<GeomType>& center, const Math::Vector3d<GeomType>& size) :
		center(center),
		size(size)
	{}

	~Brush() = default;

	Math::Vector3d<GeomType> getCenter() const { return center; }

	Math::Vector3d<GeomType> getSize() const { return size; }

	void move(const Math::Vector3d <GeomType> &v) {
		this->center += v;
	}

	void scale(const Math::Vector3d <GeomType> &s) {
		this->size.scale(x);
	}

	void addSize(const Math::Vector3d<GeomType>& s) {
		this->size += s;
	}

	Math::Vector3d<GeomType> getMinPosition() const { return center - size*0.5; }

	Math::Vector3d<GeomType> getMaxPosition() const { return center + size*0.5; }

	//Space3d<T> getSpace() const { return Space3d<T>( ) }

	virtual void add(Math::Volume3d<GeomType, ValueType>& grid) const = 0;


	Math::Space3d<GeomType> getSpace() const {
		const auto& start = getMinPosition();
		return Math::Space3d<GeomType>(start, size);
	}


private:
	Math::Vector3d<GeomType> center;
	Math::Vector3d<GeomType> size;
};

template<typename GeomType, typename ValueType>
class BlendBrush final : public Brush<GeomType, ValueType>
{
public:
	BlendBrush() :
		Brush( Math::Vector3d<GeomType>(0, 0, 0) ),
		density(1)
	{}

	explicit BlendBrush(const Math::Vector3d<GeomType>& pos) :
		Brush(pos, Math::Vector3d<GeomType>(1, 1, 1)),
		density(1)
	{}

	BlendBrush(const Math::Vector3d<GeomType>& pos, const Math::Vector3d<GeomType>& size) :
		Brush(pos,size),
		density(1)
	{}

	~BlendBrush() = default;

	GeomType getDensity() const { return density; }

	virtual void add(Math::Volume3d<GeomType,ValueType>& grid) const override {
		const GeomType radius = getSize().getX() * GeomType(0.5);
		for (size_t x = 0; x < grid.getResolutions()[0]; ++x) {
			for (size_t y = 0; y < grid.getResolutions()[1]; ++y) {
				for (size_t z = 0; z < grid.getResolutions()[2]; ++z) {
					const auto& pos = grid.toCenterPosition(x, y, z);
					if ( getCenter().getDistanceSquared(pos) < radius * radius) {
						const auto v = getValue(pos);
						grid.add(x, y, z, v);
					}
					/*
					metaball.getSpace();
					const auto& pos = toCenterPosition(x, y, z);
					*/
				}
			}
		}
	}

	ValueType getValue(const Math::Vector3d<GeomType>& pos) const
	{
		const auto dist = pos.getDistance(getCenter());
		const auto v = 1 - dist / getSize().getX();//radius;
		return v * density;
	}

private:
	ValueType density;
};

template<typename GeomType, typename ValueType>
class FillBrush final : public Brush < GeomType, ValueType >
{
public:
	FillBrush() :
		Brush(Math::Vector3d<GeomType>(0, 0, 0)),
		fillValue(0)
	{}

	explicit FillBrush(const GeomType fillValue) :
		Brush(Math::Vector3d<GeomType>(0, 0, 0)),
		fillValue(fillValue)
	{}

	FillBrush(const Math::Vector3d<GeomType>& pos, const GeomType fillValue) :
		Brush(pos, Math::Vector3d<GeomType>(1, 1, 1), fillValue)
	{}

	FillBrush(const Math::Vector3d<GeomType>& pos, const Math::Vector3d<GeomType>& size, const GeomType fillValue) :
		Brush(pos, size, fillValue)
	{}

	GeomType getFillValue() const { return fillValue; }

	virtual void add(Math::Volume3d<GeomType,ValueType>& grid) const override {
		const GeomType radius = getSize().getX() * 0.5;
		for (size_t x = 0; x < grid.getResolutions()[0]; ++x) {
			for (size_t y = 0; y < grid.getResolutions()[1]; ++y) {
				for (size_t z = 0; z < grid.getResolutions()[2]; ++z) {
					const auto& pos = grid.toCenterPosition(x, y, z);
					if (getCenter().getDistanceSquared(pos) < radius * radius) {
						grid.setValue(x, y, z, fillValue);
					}
					/*
					metaball.getSpace();
					const auto& pos = toCenterPosition(x, y, z);
					*/
				}
			}
		}
	}

private:
	GeomType fillValue;
};

template<typename GeomType, typename ValueType>
using BrushSPtr = std::shared_ptr < Brush<GeomType, ValueType> > ;

template<typename GeomType, typename ValueType>
using BrushSPtrVector = std::vector < BrushSPtr<GeomType, ValueType> > ;

	}
}

#endif