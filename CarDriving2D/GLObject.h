#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

class GLObject
{
public:
	glm::vec3 center_;
	glm::mat4 model_matrix_ = glm::mat4(1.0f);

	std::vector<float> vertices;

	GLuint vertexbuffer;

	~GLObject()
	{
		glDeleteBuffers(1, &vertexbuffer);
	}

	void genVertexBuffer()
	{
		if (vertices.size() > 0)
		{
			glGenBuffers(1, &vertexbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0])*vertices.size(), vertices.data(), GL_STATIC_DRAW);
		}
	}

	void drawLineLoop(const GLint& MatrixID, const glm::mat4 vp)
	{
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(vp*model_matrix_)[0][0]);
		
		glEnableVertexAttribArray(0); //TODO: not quite sure if this need to be called before all object drawing

		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

		glVertexAttribPointer
		(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size (x, y, z)
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glDrawArrays(GL_LINE_LOOP, 0, vertices.size() / 3);
	}

	void drawLines(const GLint& MatrixID, const glm::mat4 vp)
	{
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(vp*model_matrix_)[0][0]);

		glEnableVertexAttribArray(0); //TODO: not quite sure if this need to be called before all object drawing

		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

		glVertexAttribPointer
		(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size (x, y, z)
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glDrawArrays(GL_LINES, 0, vertices.size() / 3);
	}

	void rotateCenteredZAxis(const float& angle_degree)
	{
		// TODo; define center
		//model_matrix_ = glm::translate(model_matrix_ , -center_);
		//model_matrix_ = glm::rotate(model_matrix_, glm::radians(angle_degree), glm::vec3(0, 0, 1));
		//model_matrix_ = glm::translate(model_matrix_ , +center_);

		model_matrix_ = glm::translate(center_) * glm::rotate(glm::mat4(), glm::radians(angle_degree), glm::vec3(0, 0, 1)) * glm::translate(-center_) * model_matrix_;
	}
};
