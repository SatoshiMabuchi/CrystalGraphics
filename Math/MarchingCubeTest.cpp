#include "gtest/gtest.h"

#include "../Math/MarchingCube.h"

using namespace Crystal::Math;

template<class T>
class MarchingCubeTest : public testing::Test {
};

using TestTypes = ::testing::Types <
	std::tuple< float, float >//,
//	std::tuple< float, unsigned char >
>;


TYPED_TEST_CASE(MarchingCubeTest, TestTypes);


TYPED_TEST(MarchingCubeTest, TestInterpolate)
{
	using GeomType = std::tuple_element<0, TypeParam>::type;
	using ValueType = std::tuple_element<1, TypeParam>::type;

	PositionValue<GeomType, ValueType> pv1(Vector3d<GeomType>(0, 0, 0), 0);
	PositionValue<GeomType, ValueType> pv2(Vector3d<GeomType>(2, 4, 6), 10);
	const auto& actual = pv1.getInterpolatedPosition(5, pv2);
}

TYPED_TEST(MarchingCubeTest, TestMarchScalarSpace)
{
	using GeomType = std::tuple_element<0, TypeParam>::type;
	using ValueType = std::tuple_element<1, TypeParam>::type;


	MarchingCube<GeomType, ValueType> mc;

	Space3d<GeomType> s(Vector3d<GeomType>(0, 0, 0), Vector3d<GeomType>(10, 10, 10));
	const Grid3d<ValueType> grid(2,2,2);
	Volume3d<GeomType, ValueType> ss(s, grid);

	mc.march(ss, 1);
}