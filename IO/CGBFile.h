#ifndef __CRYSTAL_IO_CGB_FILE_H__
#define __CRYSTAL_IO_CGB_FILE_H__

#include "../ThirdParty/tinyxml2-master/tinyxml2.h"

#include "../Math/Volume.h"

#include <string>
#include <memory>
#include <vector>

namespace Crystal {
	namespace IO {

class XMLHelper {
	static tinyxml2::XMLElement* create(tinyxml2::XMLDocument& xml,const std::string& str, const Math::Vector3d<float>& v);
};

class ImageFile {
public:
	enum class Type {
		BMP,
		PNG,
		JPEG,
	};

	ImageFile(const std::string& folderPath, const std::string& filename, const Type type):
		folderPath(folderPath),
		filename(filename),
		type(type)
	{}

	std::string getFileNameIncludingPath() {
		return folderPath + "\\" + filename + getExtension(type);
	}


private:
	Type type;
	std::string folderPath;
	std::string filename;

	std::string getExtension(const Type& type) {
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

};

class CGBFile final{
public:
	bool save(const std::string& filename, const Math::Volume3d<float>& volume);

	std::shared_ptr<tinyxml2::XMLDocument> buildXML(const Math::Volume3d<float>& volume);

	std::vector< std::string > getImageFileNames(const std::string& folderpath, const std::string& baseFileName, const Math::Volume3d<float>& volume);

	std::string toImageFileName(const std::string& folderpath, const std::string& baseFileName, const int index);

	bool load(const std::string& filename, Math::Volume3d<float>& volume);

	bool parse(tinyxml2::XMLDocument& xml, Math::Volume3d<float>& volume);

	//std::vector< std::string > getImageFileNames() const { return imageFileNames; }

private:
	//std::vector< std::string > imageFileNames;

};
	}
}
#endif