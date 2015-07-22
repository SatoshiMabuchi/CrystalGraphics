#ifndef __CRYSTAL_GRAPHICS_DISPLAY_LIST_H__
#define __CRYSTAL_GRAPHICS_DISPLAY_LIST_H__

#include "../Math/Volume.h"
#include "../Math/Line.h"
#include "../Graphics/Surface.h"
#include "../Graphics/Brush.h"

namespace Crystal {
	namespace Graphics {

template<typename GeomType, typename ValueType>
class DisplayList final
{
public:
	DisplayList() = default;

	~DisplayList() = default;

	void add(const DisplayList& rhs) {
		brushes.insert(brushes.end(), rhs.brushes.begin(), rhs.brushes.end());
		lines.insert(lines.end(), rhs.lines.begin(), rhs.lines.end());
		volumes.insert(volumes.end(), rhs.volumes.begin(), rhs.volumes.end());
		surfaces.insert(surfaces.end(), rhs.surfaces.begin(), rhs.surfaces.end());
	}

	void add(const Graphics::BrushSPtr<GeomType, ValueType>& b) {
		brushes.push_back(b);
	}

	void add(const Math::Line3d<GeomType>& l) {
		lines.push_back(l);
	}

	void add(const Math::Volume3dSPtr<GeomType, ValueType>& v) {
		volumes.push_back(v);
	}

	void add(const Graphics::SurfaceSPtr<GeomType>& s) {
		surfaces.push_back(s);
	}

	void clear(){
		brushes.clear();
		lines.clear();
		volumes.clear();
		surfaces.clear();
	}

	BrushSPtrVector<GeomType, ValueType> getBrushes() const { return brushes; }

	Math::Line3dVector<GeomType> getLines() const { return lines; }

	Math::Volume3dSPtrList<GeomType, ValueType> getVolumes() const { return volumes; }

	Graphics::SurfaceSPtrList<GeomType> getSurfaces() const { return surfaces; }

private:
	BrushSPtrVector<GeomType, ValueType> brushes;
	Math::Line3dVector<GeomType> lines;
	Math::Volume3dSPtrList<GeomType, ValueType> volumes;
	Graphics::SurfaceSPtrList<GeomType> surfaces;
};
	}
}

#endif