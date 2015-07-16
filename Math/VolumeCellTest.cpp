#include "gtest/gtest.h"

#include "VolumeCell.h"

using namespace Crystal::Math;

template<class T>
class VolumeCell3dTest : public testing::Test {
};

//using GeomTypes = ::testing::Types < float > ;

//using ValueTypes = ::testing::Types < unsigned char > ;
using TestTypes = ::testing::Types <
	std::tuple< float, float >,
	std::tuple< float, unsigned char >
>;


TYPED_TEST_CASE(VolumeCell3dTest, TestTypes);

TYPED_TEST(VolumeCell3dTest, TestGetStart)
{
	using GeomType = std::tuple_element<0, TypeParam>::type;
	using ValueType = std::tuple_element<1, TypeParam>::type;
	const Space3d<GeomType> space(Vector3d<GeomType>(0, 0, 0), Vector3d<GeomType>(10, 20, 30));
	const std::array < ValueType, 8 > values = { 0, 1, 2, 3, 4, 5, 6, 7 };
	VolumeCell3d<GeomType, ValueType> cell(space, values);
}