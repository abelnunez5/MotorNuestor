#pragma once

#include <limits>

#include <glm/glm.hpp>

struct AABB
{
	glm::vec3 min { std::numeric_limits<float>::max() };
	glm::vec3 max { std::numeric_limits<float>::lowest() };
	
	bool IsValid() const
	{
		return min.x <= max.x && min.y <= max.y && min.z <= max.z;
	}

	void Expand(const glm::vec3& point)
	{
		min = glm::min(min, point);
		max = glm::max(max, point);
	}

	void Expand(const AABB& other)
	{
		if (other.IsValid())
		{
			Expand(other.min);
			Expand(other.max);
		}
	}

	glm::vec3 Center() const
	{
		return (min + max) * 0.5f;
	}

	glm::vec3 Size() const
	{
		return max - min;
	}

	float Radius() const
	{
		return glm::length(Size()) * 0.5f;
	}
};