#pragma once


class LineTechnique
{
public:
	static void setModel(glm::mat4 matrix);
	static void setView(glm::mat4 matrix);
	static void setProjection(glm::mat4 matrix);
	static void setColor(glm::vec4 matrix);
};

