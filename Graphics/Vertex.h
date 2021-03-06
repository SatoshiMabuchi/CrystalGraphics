#ifndef __CRYSTAL_GRAPHICS_VERTEX_H__
#define __CRYSTAL_GRAPHICS_VERTEX_H__

#include "../Math/Vector.h"
#include "ColorRGBA.h"
#include <vector>
#include <list>
#include <memory>
#include <tuple>

namespace Crystal {
	namespace Graphics {

template<typename T>
//using Vertex = std::tuple < Math::Vector3d<T>, Math::Vector3d<T>, Math::Vector3d<T> >;

class Vertex final {
public:
	Vertex() = default;

	explicit Vertex(const Math::Vector3d<T>& position) :
		position(position)
	{}

	Vertex(const Math::Vector3d<T>& position, const Math::Vector3d<T>& normal) :
		position( position ),
		normal( normal )
	{}

	Vertex(const Math::Vector3d<T>& position, const Math::Vector3d<T>& normal, const Math::Vector3d<T>& texCoord ) :
		position(position),
		normal(normal),
		texCoord( texCoord )
	{}

	Vertex(const Math::Vector3d<T>& position, const Math::Vector3d<T>& normal, const Math::Vector3d<T>& texCoord, const ColorRGBA<T>& color) :
		position(position),
		normal(normal),
		texCoord(texCoord),
		color(color)
	{}

	~Vertex() = default;

	Math::Vector3d<T> getPosition() const { return position; }

	Math::Vector3d<T> getNormal() const { return normal; }

	Math::Vector3d<T> getTexCoord() const { return texCoord; }

	ColorRGBA<T> getColor() const { return color; }

	void move(const Math::Vector3d<T>& v) {
		position += v;
	}

	std::vector<float> toPositionArray() const { return position.toArray(); }

	std::vector<float> toNormalArray() const { return normal.toArray(); }

	std::vector<float> toTexArray() const { return texCoord.toArray(); }

	std::vector<float> toColorArray() const { return color.toArray3(); }

	static std::vector< std::shared_ptr< Vertex<T> > > create(const Math::Vector3dVector<T>& vv) {
		std::vector< std::shared_ptr< Vertex<T> > > vertices;
		for (const Math::Vector3d<T>& v : vv) {
			vertices.push_back(std::make_shared< Vertex >(v));
		}
		return vertices;
	}

private:
	Math::Vector3d<T> position;
	Math::Vector3d<T> normal;
	Math::Vector3d<T> texCoord;
	ColorRGBA<T> color;
};

template<typename T>
using VertexSPtr = std::shared_ptr< Vertex<T> >;

template<typename T>
using VertexSPtrVector = std::vector< VertexSPtr<T> >;

	}
}

#endif