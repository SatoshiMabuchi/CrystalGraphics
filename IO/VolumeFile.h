#ifndef __CRYSTAL_IO_VOLUME_FILE_H__
#define __CRYSTAL_IO_VOLUME_FILE_H__

#include <string>
#include <vector>
#include "ImageFile.h"
#include "../Math/Volume.h"

namespace Crystal {
	namespace IO {

class VolumeFile final
{
public:
	VolumeFile() = default;

	VolumeFile(const std::string& directory) :
		directory(directory)
	{
	}

	~VolumeFile() = default;

	//std::vector<ImageFile> toImageFiles(const std::string& baseImageFileName) const { return directory + "\\" + baseImageFileName; }

	ImageFile toImageFile(const std::string& baseImageFileName, const unsigned int index, const ImageFile::Type type) const;

private:
	std::string directory;
};

	}
}

#endif