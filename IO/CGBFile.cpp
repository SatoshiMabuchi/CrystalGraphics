#include "CGBFile.h"

#include "VolumeFile.h"

using namespace Crystal::Math;
using namespace Crystal::IO;
using namespace tinyxml2;

const std::string resStr = "resolution";
const std::string originStr = "origin";

static tinyxml2::XMLElement* create(XMLDocument& xml, const std::string& str, const Vector3d<float>& v)
{
	XMLElement* e = xml.NewElement(str.c_str());

	e->SetAttribute("x", v.getX());
	e->SetAttribute("y", v.getY());
	e->SetAttribute("z", v.getZ());

	return e;
}

Vector3d<float> XMLHelper::parse(tinyxml2::XMLElement& elem)
{
	const auto x = elem.FloatAttribute("x");
	const auto y = elem.FloatAttribute("y");
	const auto z = elem.FloatAttribute("z");
	return Vector3d<float>(x, y, z);
}

template<typename GeomType, typename ValueType>
bool CGBFile<GeomType, ValueType>::save(const std::string& directoryname, const std::string& fn, const Volume3d<GeomType, ValueType>& volume)
{	
	const std::string& filename = directoryname + "/" + fn;
	XMLDocument xml;
	XMLDeclaration* decl = xml.NewDeclaration();
	xml.InsertEndChild(decl);
	XMLNode* root = xml.NewElement("root");
	xml.InsertEndChild(root);

	{
		XMLElement* res = xml.NewElement(resStr.c_str());

		res->SetAttribute("x", volume.getResolutions()[0]);
		res->SetAttribute("y", volume.getResolutions()[1]);
		res->SetAttribute("z", volume.getResolutions()[2]);

		root->InsertEndChild(res);
	}

	root->InsertEndChild( create(xml, originStr, volume.getStart()) );
	root->InsertEndChild( create(xml, "length", volume.getSpace().getLengths()));

	imageFileNames.clear();
	{
		XMLElement* e = xml.NewElement("volume");

		e->SetAttribute("type", "unsigned char");
		e->SetAttribute("format", "png");

		VolumeFile vFile(directoryname);
		for (size_t i = 0; i < volume.getResolutions()[2]; ++i) {
			const auto iFile = vFile.toImageFile( "image", i, ImageFile::Type::PNG);
			XMLElement* elem = xml.NewElement("image");
			const auto& str = iFile.getFileNameIncludingPath();
			elem->SetAttribute("path", str.c_str());
			e->InsertEndChild(elem);
			imageFileNames.push_back(str);
		}

		root->InsertEndChild(e);
	}

	//for (size_t i = 0; i < )
	
	xml.SaveFile(filename.c_str());
	return true;
}


Volume3d<float, float>::Attribute CGBFile<float, float>::load(const std::string& filename)
{
	tinyxml2::XMLDocument xml;
	xml.LoadFile(filename.c_str());

	XMLElement* root = xml.FirstChildElement("root");

	Volume3d<float, float>::Attribute attr;

	{
		XMLElement* res = root->FirstChildElement(resStr.c_str());
		attr.resx = res->IntAttribute("x");
		attr.resy = res->IntAttribute("y");
		attr.resz = res->IntAttribute("z");
	}

	XMLElement* res = root->FirstChildElement("origin");
	Vector3d<float> origin = XMLHelper::parse(*res);

	XMLElement* lengthElem = root->FirstChildElement("length");
	Vector3d<float> length = XMLHelper::parse(*lengthElem);

	Space3d<float> space(origin, length);
	attr.space = space;

	XMLElement* volumeElem = root->FirstChildElement("volume");
	XMLElement* imageElem = volumeElem->FirstChildElement("image");

	imageFileNames.clear();
	while (imageElem != nullptr) {
		imageFileNames.push_back(imageElem->Attribute("path"));
		imageElem = imageElem->NextSiblingElement("image");
	}


	return attr;
}



template bool CGBFile<float, float>::save(const std::string&, const std::string& filename, const Volume3d<float, float>& volume);

template Volume3d<float, float>::Attribute CGBFile<float, float>::load(const std::string& str);
