#ifndef __CRYSTAL_SHADER_SMOOTH_RENDERER_H__
#define __CRYSTAL_SHADER_SMOOTH_RENDERER_H__

#include <map>
#include <vector>

#include "../Shader/ShaderObject.h"
#include "../Math/Matrix.h"
#include "../Graphics/Light.h"
#include "../Graphics/Camera.h"
#include "../Graphics/Material.h"
#include "../Graphics/Surface.h"

#include "RendererBase.h"

namespace Crystal {
	namespace Shader {

template<typename GeomType, typename ValueType>
class SmoothRenderer final : public RendererBase<GeomType, ValueType>
{
public:

	SmoothRenderer() = default;

	~SmoothRenderer() = default;

	void build() override {
		ShaderUnit vShader;
		const std::string vStr =
			"#version 150						\n"
			"in vec3 position;					\n"
			"in vec3 normal;					\n"
			"out vec3 vColor;					\n"
			"uniform mat4 projectionMatrix;		\n"
			"uniform mat4 modelviewMatrix;		\n"
			"uniform vec3 lightPosition;		\n"
			"uniform vec3 eyePosition;			\n"
			"void main(void)					\n"
			"{\n"
			"	vec3 n = normalize( normal );	\n"
			"	vec3 s = normalize( lightPosition - eyePosition );						\n"
			"   vec3 diffuseColor = max(dot(s, n), 0.0) * vec3(0.0, 0.0, 1.0);		\n"
			"   vec3 ambientColor = vec3( 0.2, 0.2, 0.2); \n"
			"	gl_Position = projectionMatrix * modelviewMatrix * vec4( position, 1.0 );	\n"
			"	vColor = diffuseColor + ambientColor;				\n"
			"}\n"
			;

		vShader.compile(vStr, ShaderUnit::Stage::VERTEX);

		const std::string fStr =
			"#version 150						\n"
			"in vec3 vColor;					\n"
			"out vec4 fragColor;				\n"
			"void main(void)											\n"
			"{															\n"
			"	fragColor = vec4( vColor, 1.0);					\n"
			"}															\n"
			;

		ShaderUnit fShader;
		fShader.compile(fStr, ShaderUnit::Stage::FRAGMENT);

		shader.link(vShader, fShader);
	}

	void add(const Math::Volume3d<GeomType, ValueType>& volume) override {};

	void add(const Graphics::Surface<float>& surface) override {
		for (const auto& v : surface.getVertices()) {
			const auto& pos = v->getPosition();
			const auto& norms = v->getNormal();
			add(pos, norms);
		}
	}

	void clear() {
		positions.clear();
		normals.clear();
	}


	void render(const int width, const int height, const Graphics::Camera<float>& camera) override {
		const auto& projectionMatrix = camera.getPerspectiveMatrix().toArray4x4();
		const auto& modelviewMatrix = camera.getModelviewMatrix().toArray4x4();
		const auto& eyePos = camera.getPos().toArray();

		glViewport(0, 0, width, height);

		const Location& location = getLocations();


		if (positions.empty()) {
			return;
		}

		assert(GL_NO_ERROR == glGetError());

		glViewport(0, 0, width, height);

		glUseProgram(shader.getId());

		const std::vector< float > lightPos = { 10.0f, 10.0f, 10.0f };

		const GLint lightLoc = glGetUniformLocation(shader.getId(), "lightPosition");
		glUniform3fv(lightLoc, 1, &(lightPos.front()));


		glBindFragDataLocation(shader.getId(), 0, "fragColor");

		glUniformMatrix4fv(location.projectionMatrix, 1, GL_FALSE, &(projectionMatrix.front()));
		glUniformMatrix4fv(location.modelviewMatrix, 1, GL_FALSE, &(modelviewMatrix.front()));
		glUniform3fv(location.eyePos, 1, &(eyePos.front()));


		glVertexAttribPointer(location.position, 3, GL_FLOAT, GL_FALSE, 0, &(positions.front()));
		glVertexAttribPointer(location.normal, 3, GL_FLOAT, GL_FALSE, 0, &(normals.front()));
		//glVertexAttribPointer(location.)

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glDrawArrays(GL_TRIANGLES, 0, positions.size() / 3);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);


		glUseProgram(0);

		const GLenum error = glGetError();
		assert(GL_NO_ERROR == error);

	}

private:
	ShaderObject shader;

	struct Location {
		GLuint projectionMatrix;
		GLuint modelviewMatrix;
		GLuint position;
		GLuint normal;
		GLuint eyePos;
		GLuint matAmbient;
		GLuint matSpecular;
		GLuint matDiffuse;
		GLuint shininess;
	};

	std::vector<float> positions;
	std::vector<float> normals;

	void add(const Math::Vector3d<float>& position, const Math::Vector3d<float>& normal) {
		const auto& poss = position.toArray3();
		positions.insert(positions.end(), std::begin(poss), std::end(poss));
		const auto& norms = normal.toArray3();
		normals.insert(normals.end(), std::begin(poss), std::end(poss));
	}


	Location getLocations() {
		Location location;

		location.projectionMatrix = glGetUniformLocation(shader.getId(), "projectionMatrix");
		location.modelviewMatrix = glGetUniformLocation(shader.getId(), "modelviewMatrix");
		location.eyePos = glGetUniformLocation(shader.getId(), "eyePosition");

		location.position = glGetAttribLocation(shader.getId(), "position");
		location.normal = glGetAttribLocation(shader.getId(), "normal");

		return location;

	}
};


	}
}

#endif