#pragma once

#include "GLObject.h"

class GLSquare : public GLObject
{
public:
	using GLObject::vertices;
	using GLObject::vertexbuffer;
	using GLObject::genVertexBuffer;

	void update(const glm::vec3& center, const float& dx, const float& dy)
	{
		center_ = center;

		const glm::vec3 v0(center.x - dx, center.y - dy, center.z);
		const glm::vec3 v1(center.x + dx, center.y - dy, center.z);
		const glm::vec3 v2(center.x + dx, center.y + dy, center.z);
		const glm::vec3 v3(center.x - dx, center.y + dy, center.z);

		vertices.push_back(v0);
		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);

		genVertexBuffer();
	}
};
