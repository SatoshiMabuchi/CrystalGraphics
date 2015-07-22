#ifndef __CRYSTAL_IO_CGB_FILE_H__
#define __CRYSTAL_IO_CGB_FILE_H__

#include "TinyXML.h"

#include "../Math/Volume.h"

#include "ImageFile.h"

#include <string>
#include <memory>
#include <vector>

namespace Crystal {
	namespace IO {

class XMLHelper {
public:
	static tinyxml2::XMLElement* create(tinyxml2::XMLDocument& xml,const std::string& str, const Math::Vector3d<float>& v);

	static Math::Vector3d<float> parse(tinyxml2::XMLElement& elem);
};


class CGBFile final{
public:
	bool save(const std::string& filename, const Math::Volume3d<float>& volume);

	std::shared_ptr<tinyxml2::XMLDocument> buildXML(const Math::Volume3d<float>& volume);

	Math::Volume3d<float>::Attribute load(const std::string& filename);

	Math::Volume3d<float>::Attribute parse(tinyxml2::XMLDocument& xml);


private:
	std::vector< std::string > imageFileNames;

};
	}
}
#endif