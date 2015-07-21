#ifndef __CRYSTAL_MATH_MARCHING_CUBE_H__
#define __CRYSTAL_MATH_MARCHING_CUBE_H__


#include "Triangle.h"
#include "Vector.h"
#include "Space.h"
#include "../Util/UnCopyable.h"
#include "MarchingCubeTable.h"
#include <bitset>
#include <vector>
#include <array>
#include "Volume.h"

namespace Crystal {
	namespace Math {

template<typename GeomType, typename ValueType>
class MarchingCube final : UnCopyable
{
public:
	MarchingCube() {
		table.buildEdgeTable();
		table.buildTriangleTable();
	}

	~MarchingCube() = default;

	TriangleVector<GeomType> march(const Volume3d<GeomType, ValueType>& ss, const ValueType isolevel) const
	{
		TriangleVector<GeomType> triangles;
		const auto& cells = ss.toBoundaryCells(isolevel);
		for (const auto& c : cells) {
			const auto& ts = build(c, isolevel);
			triangles.insert(triangles.end(), ts.begin(), ts.end());
		}
		return std::move(triangles);
	}

private:
	MarchingCubeTable table;

	TriangleVector<GeomType> build(const VolumeCell3d<GeomType, ValueType>& cell, const ValueType isolevel) const
	{
		TriangleVector<GeomType> triangles;
		const int cubeindex = getCubeIndex(cell.getValues(), isolevel);
		const auto& vertices = getPositions(cubeindex, cell, isolevel);
		return std::move(build(cubeindex, vertices));
	}

	TriangleVector<GeomType> build(const int cubeindex, const std::array<Vector3d<GeomType>, 12>& vertices) const {
		TriangleVector<GeomType> triangles;
		const auto& triTable = table.getTriangleTable();
		for (int i = 0; triTable[cubeindex][i] != -1; i += 3) {
			const auto& v1 = vertices[triTable[cubeindex][i]];
			const auto& v2 = vertices[triTable[cubeindex][i + 1]];
			const auto& v3 = vertices[triTable[cubeindex][i + 2]];
			const Triangle<GeomType> t(v1, v2, v3);
			triangles.emplace_back(t);
		}
		return std::move(triangles);
	}


	int getCubeIndex(const std::array< ValueType, 8 >& val, const ValueType isolevel) const {
		std::bitset<8> bit;
		if (val[0] < isolevel) { bit.set(0); }
		if (val[1] < isolevel) { bit.set(1); }
		if (val[2] < isolevel) { bit.set(2); }
		if (val[3] < isolevel) { bit.set(3); }
		if (val[4] < isolevel) { bit.set(4); }
		if (val[5] < isolevel) { bit.set(5); }
		if (val[6] < isolevel) { bit.set(6); }
		if (val[7] < isolevel) { bit.set(7); }
		return static_cast<int>( bit.to_ulong() );
	}



	std::array< Vector3d<GeomType>, 12 > getPositions(const int cubeindex, const VolumeCell3d<GeomType,ValueType>& cell, const ValueType isolevel) const {
		std::array< Vector3d<GeomType>, 12 > vertices;
		//const PositionValue
		const auto& pvs = cell.toPositionValues();
		const auto& edgeTable = table.getEdgeTable();
		if (edgeTable[cubeindex][0]) {
			vertices[0] = pvs[0].getInterpolatedPosition(isolevel, pvs[1]); // interpolate(isolevel, p[0], p[1], val[0], val[1]);
		}
		if (edgeTable[cubeindex][1]) {
			vertices[1] = pvs[1].getInterpolatedPosition(isolevel, pvs[2]);
		}
		if (edgeTable[cubeindex][2]) {
			vertices[2] = pvs[2].getInterpolatedPosition(isolevel, pvs[3]);
		}
		if (edgeTable[cubeindex][3]) {
			vertices[3] = pvs[3].getInterpolatedPosition(isolevel, pvs[0]);
		}
		if (edgeTable[cubeindex][4]) {
			vertices[4] = pvs[4].getInterpolatedPosition(isolevel, pvs[5]);
		}
		if (edgeTable[cubeindex][5]) {
			vertices[5] = pvs[5].getInterpolatedPosition(isolevel, pvs[6]);
		}
		if (edgeTable[cubeindex][6]) {
			vertices[6] = pvs[6].getInterpolatedPosition(isolevel, pvs[7]);
		}
		if (edgeTable[cubeindex][7]) {
			vertices[7] = pvs[7].getInterpolatedPosition(isolevel, pvs[4]);
		}
		if (edgeTable[cubeindex][8]) {
			vertices[8] = pvs[0].getInterpolatedPosition(isolevel, pvs[4]);
		}
		if (edgeTable[cubeindex][9]) {
			vertices[9] = pvs[1].getInterpolatedPosition(isolevel, pvs[5]);
		}
		if (edgeTable[cubeindex][10]) {
			vertices[10] = pvs[2].getInterpolatedPosition(isolevel, pvs[6]);
		}
		if (edgeTable[cubeindex][11]) {
			vertices[11] = pvs[3].getInterpolatedPosition(isolevel, pvs[7]);
		}
		return vertices;
	}

};
	}
}

#endif