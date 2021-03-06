#ifndef __CRYSTAL_SHADER_POINT_RENDERER_H__
#define __CRYSTAL_SHADER_POINT_RENDERER_H__

#include "../Util/UnCopyable.h"
#include "../Graphics/Surface.h"
#include "../Graphics/Camera.h"
#include "ShaderObject.h"
#include "RendererBase.h"

namespace Crystal {
	namespace Shader {

template<typename GeomType, typename ValueType>
class PointRenderer final : public RendererBase<GeomType, ValueType>
{
public:

	PointRenderer() = default;

	~PointRenderer() = default;

	void add(const Graphics::Brush<GeomType, ValueType>& particle) override {
		add(particle.getCenter());
	}

	void add(const Graphics::Surface<GeomType>& surface) override {
		for (const auto& e : surface.getEdges()) {
			add(e->getStartPosition());
		}
	}

	void add(const Math::Vector3d<GeomType>& position) {
		const auto& poss = position.toArray();
		positions.insert(positions.end(), poss.begin(), poss.end());

	}

	void clear() {
		positions.clear();
	}

	void render(const int width, const int height, const Graphics::Camera<float>& camera) override {

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

		const Location& location = getLocations();

		glUniformMatrix4fv(location.projectionMatrix, 1, GL_FALSE, &(projectionMatrix.front()));
		glUniformMatrix4fv(location.modelviewMatrix, 1, GL_FALSE, &(modelviewMatrix.front()));

		glVertexAttribPointer(location.position, 3, GL_FLOAT, GL_FALSE, 0, &(positions.front()));
		//glVertexAttribIPointer(location.id, 1, GL_INT, 0, &(ids.front()));


		//const auto positions = buffer.getPositions();
		glEnableVertexAttribArray(0);

		glDrawArrays(GL_POINTS, 0, positions.size() / 3);

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
			"uniform int isSelected;			\n"
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
	struct Location {
		GLuint projectionMatrix;
		GLuint modelviewMatrix;
		GLuint position;
	};

	Location getLocations() {
		Location location;

		location.projectionMatrix = glGetUniformLocation(shader.getId(), "projectionMatrix");
		location.modelviewMatrix = glGetUniformLocation(shader.getId(), "modelviewMatrix");

		location.position = glGetAttribLocation(shader.getId(), "position");

		return location;
	}


	std::vector<float> positions;

	ShaderObject shader;
};

	}
}

#endif