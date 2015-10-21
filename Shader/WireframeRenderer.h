#ifndef __CRYSTAL_SHADER_WIREFRAME_RENDERER_H__
#define __CRYSTAL_SHADER_WIREFRAME_RENDERER_H__

#include <map>
#include <vector>

#include "ShaderObject.h"
#include "../Graphics/Surface.h"
#include "../Graphics/Camera.h"
#include "../Math/Volume.h"
#include "../Math/Line.h"
#include "../Util/UnCopyable.h"
#include "../Graphics/Brush.h"

#include "RendererBase.h"

namespace {
	struct Location {
		GLuint projectionMatrix;
		GLuint modelviewMatrix;
		GLuint position;
		GLuint id;
	};

	Location getLocations(const Crystal::Shader::ShaderObject& shader)
	{
		Location location;

		location.projectionMatrix = glGetUniformLocation(shader.getId(), "projectionMatrix");
		location.modelviewMatrix = glGetUniformLocation(shader.getId(), "modelviewMatrix");

		location.position = glGetAttribLocation(shader.getId(), "position");
		location.id = glGetAttribLocation(shader.getId(), "id");

		return location;
	}
}


namespace Crystal {
	namespace Shader {

template<typename GeomType, typename ValueType>
class WireframeRenderer final : public RendererBase<GeomType, ValueType>
{
public:
	WireframeRenderer() = default;

	~WireframeRenderer() = default;

	void add(const Graphics::Brush<GeomType, ValueType>& b) override {
		const auto& s = b.getSpace();
		const auto& bb = Math::Box<float>(s.getStart(), s.getEnd());
		add(bb);
	}

	void add(const Math::Volume3d<GeomType, ValueType>& volume) override {
		const Math::Box<float> v(volume.getStart(), volume.getEnd());
		add(v);
	}

	void add(const Graphics::Surface<GeomType>& surface) override {
		for (const auto& e : surface.getEdges()) {
			add(e->getStartPosition());
			add(e->getEndPosition());
		}
	}

	void render(const int width, const int height, const Graphics::Camera<GeomType>& camera) override {
		if (positions.empty()) {
			return;
		}

		const auto& projectionMatrix = camera.getPerspectiveMatrix().toArray();
		const auto& modelviewMatrix = camera.getModelviewMatrix().toArray();

		//assert(GL_NO_ERROR == glGetError());
		const auto error = glGetError();
		if (error != GL_NO_ERROR) {
			assert(false);
			//G glGetString(error);

		}

		glViewport(0, 0, width, height);

		glUseProgram(shader.getId());

		const Location& location = ::getLocations(shader);

		glUniformMatrix4fv(location.projectionMatrix, 1, GL_FALSE, &(projectionMatrix.front()));
		glUniformMatrix4fv(location.modelviewMatrix, 1, GL_FALSE, &(modelviewMatrix.front()));

		glVertexAttribPointer(location.position, 3, GL_FLOAT, GL_FALSE, 0, &(positions.front()));
		//glVertexAttribIPointer(location.id, 1, GL_INT, 0, &(ids.front()));

		glEnableVertexAttribArray(0);

		//const auto positions = buffer.getPositions();

		glDrawArrays(GL_LINES, 0, positions.size() / 3);

		glDisableVertexAttribArray(0);

		glBindFragDataLocation(shader.getId(), 0, "fragColor");

		glUseProgram(0);

	}

	void build() override {
		ShaderUnit vShader;
		const std::string vStr =
			"#version 150						\n"
			"in vec3 position;					\n"
			//		"in int id;							\n"
			"out vec3 color;					\n"
			"uniform mat4 projectionMatrix;		\n"
			"uniform mat4 modelviewMatrix;		\n"
			"void main(void)					\n"
			"{\n"
			"	gl_Position = projectionMatrix * modelviewMatrix * vec4( position, 1.0 );\n"
			"	color.r = 0;			\n"
			//"	color.g = id / 255.0;			\n"
			"	color.g = 0;					\n"
			"	color.b = 0;					\n"
			"}\n"
			;

		vShader.compile(vStr, ShaderUnit::Stage::VERTEX);

		ShaderUnit fShader;

		const std::string fStr =
			"#version 150			\n"
			"in vec3 color;			\n"
			"out vec4 fragColor;	\n"
			"void main(void)		\n"
			"{\n"
			"	fragColor.rgb = color;//color;	\n"
			"	fragColor.a = 1.0;	\n"
			"}						\n"
			;

		fShader.compile(fStr, ShaderUnit::Stage::FRAGMENT);

		shader.link(vShader, fShader);

		std::string log;
		log += vShader.getLog();
		log += fShader.getLog();
		shader.link(vShader, fShader);

	}

private:

	ShaderObject shader;

public:
	void clear() {
		positions.clear();
	}


public:

	void add(const Math::Line3d<GeomType>& line) {
		add(line.getStart());
		add(line.getEnd());
	}

	void add(const Math::Box<GeomType>& box) {

		const auto minx = box.getMinX();
		const auto miny = box.getMinY();
		const auto minz = box.getMinZ();
		const auto maxx = box.getMaxX();
		const auto maxy = box.getMaxY();
		const auto maxz = box.getMaxZ();

		const Math::Vector3dVector<float> vs = {
			Math::Vector3d<float>(minx, miny, minz),
			Math::Vector3d<float>(maxx, miny, minz),
			Math::Vector3d<float>(maxx, maxy, minz),
			Math::Vector3d<float>(minx, maxy, minz),
			Math::Vector3d<float>(minx, miny, maxz),
			Math::Vector3d<float>(maxx, miny, maxz),
			Math::Vector3d<float>(maxx, maxy, maxz),
			Math::Vector3d<float>(minx, maxy, maxz)
		};

		const Math::Line3dVector<float> lines{
			Math::Line3d<float>(vs[0], vs[1]),
			Math::Line3d<float>(vs[1], vs[2]),
			Math::Line3d<float>(vs[2], vs[3]),
			Math::Line3d<float>(vs[3], vs[0]),
			Math::Line3d<float>(vs[4], vs[5]),
			Math::Line3d<float>(vs[5], vs[6]),
			Math::Line3d<float>(vs[6], vs[7]),
			Math::Line3d<float>(vs[7], vs[4]),
			Math::Line3d<float>(vs[0], vs[4]),
			Math::Line3d<float>(vs[1], vs[5]),
			Math::Line3d<float>(vs[2], vs[6]),
			Math::Line3d<float>(vs[3], vs[7])
		};

		for (const auto& l : lines) {
			add(l);
		}
	}


private:

	void add(const Math::Vector3d<GeomType>& position) {
		const auto& poss = position.toArray3();
		positions.insert(positions.end(), std::begin(poss), std::end(poss));
	}


private:
	std::vector<GeomType> positions;


};

	}
}

#endif