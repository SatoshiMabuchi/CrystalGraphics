#ifndef __CRYSTAL_SHADER_VOLUME_RENDERER_H__
#define __CRYSTAL_SHADER_VOLUME_RENDERER_H__

#include <map>
#include <vector>

#include "ShaderObject.h"
#include "../Math/Volume.h"
#include "../Graphics/Surface.h"
#include "../Graphics/Camera.h"
#include "../Util/UnCopyable.h"
#include "RendererBase.h"

namespace Crystal {
	namespace Shader {

template<typename GeomType, typename ValueType>
class VolumeRenderer final : public RendererBase<GeomType, ValueType>
{
public:

	VolumeRenderer() = default;

	~VolumeRenderer() = default;


	void add(const Math::Volume3d<GeomType, ValueType>& volume) override {
		const auto& ps = volume.toCenterPositions();
		const auto& vs = volume.getValues();
		assert(ps.size() == vs.size());
		for (size_t i = 0; i < ps.size(); ++i) {
			const auto& pss = ps[i].toArray();
			positions.insert(positions.end(), pss.begin(), pss.end());
			values.emplace_back(vs[i]);
		}
		//positions.add( surface.)
	}

	void render(const int width, const int height, const Graphics::Camera<GeomType>& camera) override {
		const auto& densities = values;

		if (positions.empty()){
			return;
		}

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);


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

		assert(GL_NO_ERROR == glGetError());


		glUniformMatrix4fv(location.projectionMatrix, 1, GL_FALSE, &(projectionMatrix.front()));
		glUniformMatrix4fv(location.modelviewMatrix, 1, GL_FALSE, &(modelviewMatrix.front()));

		glVertexAttribPointer(location.position, 3, GL_FLOAT, GL_FALSE, 0, &(positions.front()));
		glVertexAttribPointer(location.density, 1, GL_FLOAT, GL_FALSE, 0, &(densities.front()));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		assert(GL_NO_ERROR == glGetError());

		glDrawArrays(GL_POINTS, 0, positions.size() / 3);

		assert(GL_NO_ERROR == glGetError());


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glBindFragDataLocation(shader.getId(), 0, "fragColor");

		glUseProgram(0);

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);


		assert(GL_NO_ERROR == glGetError());
	}

	void build() {
		ShaderUnit vShader;
		const std::string vStr =
			"#version 150						\n"
			"in vec3 position;					\n"
			"in int id;							\n"
			"in float density;					\n"
			"out vec4 color;					\n"
			"uniform mat4 projectionMatrix;		\n"
			"uniform mat4 modelviewMatrix;		\n"
			"void main(void)					\n"
			"{\n"
			"	gl_Position = projectionMatrix * modelviewMatrix * vec4( position, 1.0 );\n"
			"	color.r = (density <0) ? 1 : 0;		\n"
			"	color.g = 0.0;			\n"
			"	color.b = (density >0) ? 1 : 0;					\n"
			"	color.a = abs(density);		\n"
			"}\n"
			;

		vShader.compile(vStr, ShaderUnit::Stage::VERTEX);

		ShaderUnit fShader;

		const std::string fStr =
			"#version 150			\n"
			"in vec4 color;			\n"
			"out vec4 fragColor;	\n"
			"void main(void)		\n"
			"{\n"
			"	fragColor = color;//color;	\n"
			"}						\n"
			;

		fShader.compile(fStr, ShaderUnit::Stage::FRAGMENT);

		shader.link(vShader, fShader);

		std::string log;
		log += vShader.getLog();
		log += fShader.getLog();
		shader.link(vShader, fShader);

	}



	void clear() {
		positions.clear();
		values.clear();
	}


private:
	struct Location {
		GLuint projectionMatrix;
		GLuint modelviewMatrix;
		GLuint position;
		GLuint density;
	};

	Location getLocations() {
		VolumeRenderer::Location location;

		location.projectionMatrix = glGetUniformLocation(shader.getId(), "projectionMatrix");
		location.modelviewMatrix = glGetUniformLocation(shader.getId(), "modelviewMatrix");

		location.position = glGetAttribLocation(shader.getId(), "position");
		location.density = glGetAttribLocation(shader.getId(), "density");

		return location;
	}

	ShaderObject shader;


private:
	std::vector<GeomType> positions;
	std::vector<ValueType> values;

};

	}
}

#endif