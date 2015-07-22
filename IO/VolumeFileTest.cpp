#include "gtest/gtest.h"
#include "VolumeFile.h"

using namespace Crystal::IO;

TEST(VolumeFileTest, Test)
{
	const VolumeFile file( "directory/" );
	const ImageFile& imageFile = file.toImageFile("image", 0, ImageFile::Type::PNG);
	//EXPECT_EQ( imageFile.getFileNameIncludingPath());
	const std::string& actual = imageFile.getFileNameIncludingPath();
	const std::string expected = "directory/image0000.png";
	EXPECT_EQ(expected, actual);
}