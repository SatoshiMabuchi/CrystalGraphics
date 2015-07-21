#ifndef __CRYSTAL_IO_IMAGE_FILE_H__
#define __CRYSTAL_IO_IMAGE_FILE_H__

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
		return folderPath + "\\" + filename + getExtension(type);
	}


private:
	Type type;
	std::string folderPath;
	std::string filename;

	std::string getExtension(const Type& type) const {
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


	}
}
#endif