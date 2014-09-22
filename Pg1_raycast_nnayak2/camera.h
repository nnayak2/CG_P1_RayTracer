#ifndef CAMERA_H
#define CAMERA_H

#include <vector>
#include "scene.h"
#include "glm/glm.hpp"
#include <iostream>

class camera
{
	camera(glm::vec3 eye, glm::vec3 look, glm::vec3 up, int pixwidth, int pixheight, int worldwidth, int worldheight, float znear, float zfar);
public:
	static camera *instance;
	glm::vec3 eye, look, up;
	int pixwidth, pixheight, worldwidth, worldheight;
	float znear, zfar;
	std::vector<glm::vec2> pixelPosTable;
	std::vector<struct lightSources> lights;

	static camera* getInstance(glm::vec3 eye, glm::vec3 look, glm::vec3 up, int pixwidth, int pixheight, int worldwidth, int worldheight, float znear, float zfar);
	static camera* getInstance();
	glm::vec3 getWorldPosofPixel(int i, int j);
	void setupLights();
};

struct lightSources
{
	glm::vec3 pos;
	glm::vec3 amb;
	glm::vec3 dif;
	glm::vec3 spec;
	//int shininess;
};

static inline float _parseFloat(char*& token)
{
	token += strspn(token, " \t");
	float f = (float)atof(token);
	token += strcspn(token, " \t\r");
	return f;
}

static inline int _parseInt(char*& token)
{
	token += strspn(token, " \t");
	int f = (int)atoi(token);
	token += strcspn(token, " \t\r");
	return f;
}

static inline void _parseInt2(
	int& x, int& y,
	char*& token)
{
	x = _parseInt(token);
	y = _parseInt(token);
}

static inline void _parseFloat3(
	float& x, float& y, float& z,
	char*& token)
{
	x = _parseFloat(token);
	y = _parseFloat(token);
	z = _parseFloat(token);
}

#endif