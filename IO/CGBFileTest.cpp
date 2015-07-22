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
	ImageFile file("folder", "file", ImageFile::Type::BMP);
	const std::string expected = "folder\\file.bmp";
	const auto& actual = file.getFileNameIncludingPath();
	EXPECT_EQ(expected, actual);
}


TEST(CGBFileTest, TestBuild)
{
	CGBFile<float, unsigned char> file;
	Space3d<float> space;
	Grid3d<unsigned char> grid(10,20,30);
	Volume3d<float, unsigned char> v(space, grid);
	std::shared_ptr< XMLDocument > doc = file.buildXML(v);
	doc->Print();
}

TEST(CGBFileTest, TestParse)
{
	XMLDocument doc;
	doc.LoadFile("./CGBTestFile.cgb");
	CGBFile<float, unsigned char> file;
	file.parse(doc);
}