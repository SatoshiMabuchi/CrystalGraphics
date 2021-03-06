#include "gtest/gtest.h"

#include "../Math/Box.h"
#include "../Math/Sphere.h"

#include "../Physics/ParticleBuilder.h"

using namespace Crystal::Math;
using namespace Crystal::Physics;

using T = float;

TEST(ParticleBuilderTest, TestConstruct)
{
	ParticleBuilder<T> builder;
	EXPECT_EQ(0, builder.getNextId());
	EXPECT_EQ(1.0f, builder.getDivideLength());
}

TEST(ParticleBuilderTest, TestBuildBox)
{
	ParticleBuilder<T> builder;
	const Box<T> b(Vector3d<T>(0.0f, 0.0f, 0.0f), Vector3d<T>(10.0f, 1.0f, 1.0f));
	const ParticleSPtrVector& actual = builder.create(b.getInnerOffset(0.5f));
	EXPECT_EQ(10, actual.size());
}

TEST(ParticleBuilderTest, TestCreateSphere)
{
	ParticleBuilder<T> builder;
	const Sphere<T> s(Vector3d<T>(0.0, 0.0, 0.0), 1.0);
	const ParticleSPtrVector& particles = builder.create(s);
	EXPECT_EQ(7, particles.size());
}
