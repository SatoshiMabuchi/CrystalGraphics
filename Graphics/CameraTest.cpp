#include "gtest/gtest.h"
 
#include "../Graphics/Camera.h"

using namespace Crystal::Math;
using namespace Crystal::Graphics;

using T = float;

TEST( CameraTest, TestGetConstruct )
{
	Camera<T> c;
	EXPECT_EQ(  1, c.getNear() );
	EXPECT_EQ( 10, c.getFar()  );
	EXPECT_EQ( Vector3d<T>(0, 0, 0), c.getPos() );
}

TEST( CameraTest, TestMove )
{
	Camera<T> c;
	c.move(Vector3d<T>(1.0f, 0.0f, 0.0f));

	EXPECT_EQ(Vector3d<T>(1.0f, 0.0f, 0.0f), c.getPos() );
}

TEST(CameraTest, TestGetRotationMatrix)
{
	{
		Camera<T> c;
		const auto& expected = Matrix3d<T>::Identity();
		const auto& actual = c.getRotationMatrix();
		EXPECT_EQ(expected, actual);
	}

	{
		Camera<T> c;
		c.setAngle(Vector3d<T>(Tolerance<T>::getHalfPI(), 0, 0));
		const auto& actual = c.getRotationMatrix();
	}
}


TEST( CameraTest, TestGetPerspectiveMatrix )
{
	const Camera<float> c;
	const Matrix4d<float>& m = c.getPerspectiveMatrix();

	EXPECT_EQ( 2, m.getX00() );
	EXPECT_EQ( 0, m.getX01() );
	EXPECT_EQ( 0, m.getX02() );
	EXPECT_EQ( 0, m.getX03() );
	EXPECT_EQ( 0, m.getX10() );
	EXPECT_EQ( 2, m.getX11() );
	EXPECT_EQ( 0, m.getX12() );
	EXPECT_EQ( 0, m.getX13() );
}

TEST( CameraTest, TestGetOrthogonalMatrix )
{
	const Camera<float> c;
	const Matrix4d<float>& m = c.getOrthogonalMatrix();

	{
		EXPECT_EQ(2, m.getX00());
		EXPECT_EQ(2, m.getX11());
	}

}