#pragma once

#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <vector>

#include <glm/glm.hpp>

#include "Shader.h"
#include "Point.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "VertexHash.h"

#include "GraphicsData.h"

#include "DebugLogger.h"

class LineData : public DebugLogger<LineData>, public GraphicsData
{
public:

	struct PointHash : public VertexHash<Point> {
	public:
		size_t hash_combine(size_t lhs, size_t rhs) const {
			lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
			return lhs;
		}

		bool operator()(const Point& a, const Point& b)const override
		{
			return a.aPos.x == b.aPos.x && a.aPos.y == b.aPos.y && a.aPos.z == b.aPos.z;
		}

		size_t operator()(const Point& k) const override
		{
			return hash_combine(
				hash_combine(std::hash<float>()(k.aPos.x), std::hash<float>()(k.aPos.y)),
				std::hash<float>()(k.aPos.z));
		}
	};
	

	template<class... Args>
	requires (std::conjunction_v<std::is_same<glm::vec3, Args>...> && sizeof...(Args) % 2 == 0)
	LineData(glm::vec4 color, glm::vec3 first, Args... args);

	LineData(glm::vec4 color, std::vector<Point> lines);

	
	glm::vec4 getColor();
	void setColor(glm::vec4 color);

	std::vector<Point> points;
	Indices indices;

	VertexBuffer<Point> vertexBuffer;
	ElementBuffer elementBuffer;

	glm::vec4 color;
	size_t pointCount;
};

template<class... Args>
requires (std::conjunction_v<std::is_same<glm::vec3, Args>...> && sizeof...(Args) % 2 == 0)
inline LineData::LineData(glm::vec4 color, glm::vec3 first, Args... args) :
	vertexBuffer(storePointsAndIndices<PointHash>(indices, points, { first, args... })),
	color(color),
	pointCount(sizeof...(Args) + 1),
	elementBuffer(indices, vertexBuffer.getVAO())
{};

