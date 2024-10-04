#pragma once

#include <glm/vec2.hpp>

struct rect
{
	glm::vec2 pos1; //left bottom
	glm::vec2 pos2; //right top
};

bool pointVsRect(const glm::vec2& p, const rect& r)
{
	return (p.x >= r.pos1.x && p.y >= r.pos1.y && p.x < r.pos2.x && p.y < r.pos2.y);
}

bool RectVsRect(const rect& r1, const rect& r2)
{
	return (r1.pos1.x < r2.pos2.x && r1.pos2.x > r2.pos1.x &&
			r1.pos1.y < r2.pos2.y && r1.pos2.y > r2.pos1.y);
}