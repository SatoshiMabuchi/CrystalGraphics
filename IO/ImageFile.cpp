#include "ImageFile.h"

#include <string>
#include <cassert>

using namespace Crystal::IO;
using namespace tinyxml2;

std::string ImageFile::getExtension(const Type& type) const
{
	if (type == Type::BMP) {
		return ".bmp";
	}
	else if (type == Type::PNG) {
		return ".png";
	}
	else if (type == Type::JPEG) {
		return ".jpeg";
	}
	else {
		assert(false);
		return "";
	}
}

