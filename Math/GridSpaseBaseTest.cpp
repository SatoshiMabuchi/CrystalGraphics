#include "gtest/gtest.h"

#include "../Math/GridSpaceBase.h"

using namespace Crystal::Math;


TEST(GridSpaceBaseTest, TestGetResolutions)
{
	EXPECT_EQ( 1, GridSpaceBase<float>(Space3d<float>(), Index3d{ 1, 2, 3 } ).getResolutions()[0] );
	EXPECT_EQ( 2, GridSpaceBase<float>(Space3d<float>(), Index3d{ 1, 2, 3 } ).getResolutions()[1] );
}

TEST(GridSpaceBaseTest, TestGetUnitLengths)
{
	EXPECT_EQ( Vector3d<float>(0.5,0.5,0.5), GridSpaceBase<float>(Space3d<float>::Unit(), Index3d{ 2, 2, 2 }).getUnitLengths() );
	EXPECT_EQ( Vector3d<float>(0.5, 0.25, 0.125), GridSpaceBase<float>(Space3d<float>::Unit(), Index3d{ 2, 4, 8 }).getUnitLengths());
}

TEST(GridSpaceBaseTest, TestGetUnitVolume)
{
	EXPECT_EQ( 0.125, GridSpaceBase<float>(Space3d<float>::Unit(), Index3d{ 2, 2, 2 }).getUnitVolume());
}

TEST(GridSpaceBaseTest, TestGetNormalized)
{
	using T = float;
	EXPECT_EQ( Vector3d<T>(0.25, 0.25, 0.25), GridSpaceBase<T>(Space3d<float>::Unit(), Index3d{ 2, 2, 2 }).getNormalized(0, 0, 0));
	EXPECT_EQ( Vector3d<T>(0.75, 0.75, 0.75), GridSpaceBase<T>(Space3d<float>::Unit(), Index3d{ 2, 2, 2 }).getNormalized(1, 1, 1));
}

TEST(GridSpaceBaseTest, TestToIndex)
{
	using T = float;
	const auto s = Space3d<T>(Vector3d<T>(0, 0, 0), Vector3d<T>(10, 10, 10));
	GridSpaceBase<T> bs(s, Index3d{ 2, 2, 2 });
	{
		const std::array<unsigned int, 3> expected = { 0, 0, 0 };
		EXPECT_EQ( expected, bs.toIndex(Vector3d<T>(2, 2, 2)));
	}

	{
		const std::array<unsigned int, 3> expected = { 1, 1, 1 };
		EXPECT_EQ(expected, bs.toIndex(Vector3d<T>(8, 8, 8)));
	}

	//	bs.toIndex(Vector3d<T>(8, 8, 8));
}

TEST(GridSpaceBaseTest, TestScale)
{
	using T = float;
	GridSpaceBase<T> original( Space3d<T>::Unit(), Index3d{ 1, 1, 1 });
	original.scale(Vector3d<T>(2, 4, 8));
	EXPECT_EQ( Vector3d<T>(2,4,8), original.getSpace().getLengths());
}