#include "CGBFile.h"

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



bool CGBFile::save(const std::string& filename, const Volume3d<float>& volume)
{
	auto xml = buildXML(volume);
	xml->SaveFile(filename.c_str());
	return true;//saveImages(directory, filename);
}

std::shared_ptr<XMLDocument> CGBFile::buildXML(const Volume3d<float>& volume)
{
	std::shared_ptr<XMLDocument> xml = std::make_shared< XMLDocument >();
	XMLDeclaration* decl = xml->NewDeclaration();
	xml->InsertEndChild(decl);
	XMLNode* root = xml->NewElement("root");
	xml->InsertEndChild(root);

	{
		XMLElement* res = xml->NewElement(resStr.c_str());

		res->SetAttribute("x", volume.getResolutions()[0]);
		res->SetAttribute("y", volume.getResolutions()[1]);
		res->SetAttribute("z", volume.getResolutions()[2]);

		root->InsertEndChild(res);
	}

	root->InsertEndChild( create(*xml, originStr, volume.getStart()) );
	root->InsertEndChild(create(*xml, "length", volume.getSpace().getLengths()));

	{
		XMLElement* e = xml->NewElement("volume");

		ImageFile image("Folder","FileName", ImageFile::Type::PNG);
		XMLElement* elem = xml->NewElement("Path");
		const auto& str =  image.getFileNameIncludingPath();
		elem->SetAttribute("Path", str.c_str());
		e->InsertEndChild(elem);

		root->InsertEndChild(e);
	}

	//for (size_t i = 0; i < )
	
	return xml;

}

std::vector< std::string > CGBFile::getImageFileNames(const std::string& folderpath, const std::string& baseFileName, const Volume3d<float>& volume)
{
	std::vector< std::string > filenames;
	for (size_t i = 0; i < volume.getResolutions()[2]; ++i) {
		filenames.push_back(toImageFileName(folderpath, baseFileName, i));
	}
	return filenames;
}

std::string CGBFile::toImageFileName(const std::string& folderpath, const std::string& baseFileName, const int index)
{
	return folderpath + baseFileName + std::to_string(index);
}

bool CGBFile::load(const std::string& filename, Volume3d<float>& volume)
{
	std::shared_ptr<XMLDocument> xml = std::make_shared< XMLDocument >();
	xml->LoadFile(filename.c_str());
	return parse(*xml, volume);
}

bool CGBFile::parse(tinyxml2::XMLDocument& xml, Volume3d<float>& volume)
{
	XMLElement* root = xml.FirstChildElement("root");

	{
		XMLElement* res = root->FirstChildElement(resStr.c_str());
		Math::Index3d resolution;
		resolution[0] = res->IntAttribute("x");
		resolution[1] = res->IntAttribute("y");
		resolution[2] = res->IntAttribute("z");
	}


	root->FirstChildElement(originStr.c_str());
	root->FirstChildElement("length");
	return false;
}
