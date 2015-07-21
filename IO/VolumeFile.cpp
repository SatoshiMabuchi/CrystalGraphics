#include "VolumeFile.h"

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace Crystal::IO;

ImageFile VolumeFile::toImageFile(const std::string& baseImageFileName, const unsigned int index, const ImageFile::Type type) const
{
	std::ostringstream sout;
	sout << std::setfill('0') << std::setw(4) << index;
	const std::string fn = baseImageFileName + sout.str();
	ImageFile image(directory, fn, type);
	return image;
}
