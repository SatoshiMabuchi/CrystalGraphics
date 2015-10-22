#ifndef __CRYSTAL_SHADER_NORMAL_RENDERER_H__
#define __CRYSTAL_SHADER_NORMAL_RENDERER_H__

#include <map>
#include <vector>

#include "ShaderObject.h"

#include "../Graphics/Camera.h"
#include "../Graphics/ColorRGBA.h"
#include "../Graphics/Surface.h"

#include <memory>
#include <sstream>

namespace Crystal {
	namespace Shader {

template<typename GeomType, typename ValueType>
class NormalRenderer {
public:

	NormalRenderer() : scale(1)
	{}

	~NormalRenderer() = default;

	GLint positionLocation;
	GLint vectorLocation;


	void build(const std::string& vSource, const std::string& gSource, const std::string& fSource)
	{
		ShaderUnit vShader;
		ShaderUnit gShader;
		ShaderUnit fShader;
		vShader.compile(vSource, ShaderUnit::Stage::VERTEX);
		gShader.compile(gSource, ShaderUnit::Stage::GEOMETRY);
		fShader.compile(fSource, ShaderUnit::Stage::FRAGMENT);
		std::string log;
		log += vShader.getLog();
		log += gShader.getLog();
		log += fShader.getLog();
		shader.link(vShader, fShader, gShader);

		//positionLocation = glGetUniformLocation( shader.getId(), "position" );
	}

	void render(const int width, const int height, const Graphics::Camera<GeomType>& camera)
	{
		if (positions.empty() || normals.empty()) {
			return;
		}

		const auto& perspectiveMatrix = camera.getPerspectiveMatrix();
		const auto& modelviewMatrix = camera.getModelviewMatrix();

		assert(GL_NO_ERROR == glGetError());

		glViewport(0, 0, width, height);

		//glPointSize( pointSize );
		glLineWidth(1.0f);

		glUseProgram(shader.getId());

		ShaderUtil::setUniformMatrix(shader.getId(), "projectionMatrix", perspectiveMatrix);
		ShaderUtil::setUniformMatrix(shader.getId(), "modelviewMatrix", modelviewMatrix);

		glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, &(positions.front()));
		vectorLocation = glGetAttribLocation(shader.getId(), "vector");
		glVertexAttribPointer(vectorLocation, 3, GL_FLOAT, GL_FALSE, 0, &(normals.front()));
		ShaderUtil::setUniform(shader.getId(), "scale", scale);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glDrawArrays(GL_POINTS, 0, positions.size() / 3);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glBindFragDataLocation(shader.getId(), 0, "fragColor");

		glUseProgram(0);


		assert(GL_NO_ERROR == glGetError());
	}

	float scale;

	void add(const Graphics::Surface<float>& surface) {
		for (const auto& e : surface.getEdges()) {
			add(e->getStartPosition(), e->getStartVertex()->getNormal() );
		}
		//positions.add( surface.)
	}
	void clear() {
		positions.clear();
		normals.clear();
	}


private:
	std::vector< float > positions;
	std::vector< float > normals;

	void add(const Math::Vector3d<float>& position, const Math::Vector3d<float>& normal) {
		const auto& poss = position.toArray();
		positions.insert(positions.end(), poss.begin(), poss.end());
		const auto& norms = normal.toArray();
		normals.insert(normals.end(), norms.begin(), norms.end());
	}


private:
	ShaderObject shader;

};

	}
}

#endif