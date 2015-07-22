#include "gtest/gtest.h"

#include "CGBFile.h"
#include <vector>
#include <string>
#include <fstream>


using namespace tinyxml2;
using namespace Crystal::Math;
using namespace Crystal::IO;

TEST(ImageFileTest, Test)
{
	ImageFile file("folder/", "file", ImageFile::Type::BMP);
	const std::string expected = "folder/file.bmp";
	const auto& actual = file.getFileNameIncludingPath();
	EXPECT_EQ(expected, actual);
}


TEST(CGBFileTest, TestSave)
{
	CGBFile<float, float> file;
	Space3d<float> space;
	Grid3d<float> grid(2,2,2);
	Volume3d<float, float> v(space, grid);
	file.save("../IO/","CGBTestSaveFile.cgb", v);
	EXPECT_EQ( 2, file.getImageFileNames().size() );
}

TEST(CGBFileTest, TestLoad)
{
	CGBFile<float, float> file;
	file.load("../IO/CGBTestFile.cgb");
	EXPECT_EQ(2, file.getImageFileNames().size());

}