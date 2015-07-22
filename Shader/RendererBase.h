#ifndef __CRYSTAL_SHADER_RENDERER_BASE_H__
#define __CRYSTAL_SHADER_RENDERER_BASE_H__

#include "../Util/UnCopyable.h"

#include "../Graphics/Camera.h"
#include "../Graphics/Brush.h"
#include "../Math/Volume.h"

#include <memory>
#include <list>
#include <vector>

namespace Crystal {
	namespace Shader {

template<typename GeomType, typename ValueType>
class RendererBase : private UnCopyable
{
public:
	RendererBase() = default;

	virtual ~RendererBase(){};

	virtual void render(const int width, const int height, const Graphics::Camera<float>& camera) = 0;

	virtual void add(const Graphics::Brush<GeomType, ValueType>& brush){};

	virtual void add(const Math::Volume3d<GeomType, ValueType>& volume){};

	virtual void add(const Graphics::Surface<GeomType>& surface){};

	virtual void build() = 0;

};

template<typename GeomType, typename ValueType>
using RendererBaseSPtr = std::shared_ptr < RendererBase<GeomType, ValueType> > ;
template<typename GeomType, typename ValueType>
using RendererBaseSPtrVector = std::vector < RendererBaseSPtr<GeomType, ValueType> > ;
template<typename GeomType, typename ValueType>
using RendererBaseSPtrList = std::list < RendererBaseSPtr<GeomType, ValueType> >;

template<typename GeomType, typename ValueType>
class NullRenderer : public RendererBase<GeomType, ValueType>
{
public:
	NullRenderer() = default;

	~NullRenderer() = default;

	void render(const int width, const int height, const Graphics::Camera<float>& camera) override {};

	void add(const Graphics::Brush<GeomType, ValueType>& brush) override{};

	void add(const Math::Volume3d<GeomType, ValueType>& volume) override{};

	void add(const Graphics::Surface<GeomType>& surface) override{};

	void build() override{};

};
	}
}

#endif