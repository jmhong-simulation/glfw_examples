#pragma once

#include "GLObject.h"

class GLSquare : public GLObject
{
public:
	using GLObject::vertices;
	using GLObject::vertexbuffer;
	using GLObject::genVertexBuffer;

	GLSquare(const glm::vec3& center, const float& dx, const float& dy)
	{
		center_ = center;

		vertices.push_back(center.x - dx);
		vertices.push_back(center.y - dy);
		vertices.push_back(center.z);

		vertices.push_back(center.x + dx);
		vertices.push_back(center.y - dy);
		vertices.push_back(center.z);

		vertices.push_back(center.x + dx);
		vertices.push_back(center.y + dy);
		vertices.push_back(center.z);

		vertices.push_back(center.x - dx);
		vertices.push_back(center.y + dy);
		vertices.push_back(center.z);

		genVertexBuffer();
	}
};
