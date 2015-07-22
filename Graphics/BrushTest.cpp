#include "gtest/gtest.h"

#include "../Graphics/Brush.h"

using namespace Crystal::Math;
using namespace Crystal::Graphics;

template<class T>
class BrushTest : public testing::Test {
};

using TestTypes = ::testing::Types <
	std::tuple< float, float >,
	std::tuple< float, unsigned char >
>;

TYPED_TEST_CASE(BrushTest, TestTypes);

TYPED_TEST(BrushTest, TestConstruct)
{
	using GeomType = std::tuple_element<0, TypeParam>::type;
	using ValueType = std::tuple_element<1, TypeParam>::type;

	BlendBrush<GeomType, ValueType> brush;
	EXPECT_EQ( Vector3d<GeomType>(1,1,1), brush.getSize() );
}

TYPED_TEST(BrushTest, TestMove)
{
	using GeomType = std::tuple_element<0, TypeParam>::type;
	using ValueType = std::tuple_element<1, TypeParam>::type;

	BlendBrush<GeomType, ValueType> brush( Vector3d<GeomType>(1,2,3));
	brush.move(Vector3d<GeomType>(3, 2, 1));
	EXPECT_EQ(Vector3d<GeomType>(4, 4, 4), brush.getCenter());
}