#ifndef __CRYSTAL_IO_IMAGE_FILE_H__
#define __CRYSTAL_IO_IMAGE_FILE_H__

#include <string>
#include "TinyXML.h"

namespace Crystal {
	namespace IO {

class ImageFile {
public:
	enum class Type {
		BMP,
		PNG,
		JPEG,
	};

	ImageFile(const std::string& folderPath, const std::string& filename, const Type type) :
		folderPath(folderPath),
		filename(filename),
		type(type)
	{}

	std::string getFileNameIncludingPath() const {
		return folderPath + "/" + filename + getExtension(type);
	}


private:
	Type type;
	std::string folderPath;
	std::string filename;

	std::string getExtension(const Type& type) const;

};


	}
}
#endif