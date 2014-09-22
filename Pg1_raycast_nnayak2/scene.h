#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "glm/glm.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "raytracer.h"
#include "tiny_obj_loader.h"
#include <map>

class scene
{
	static scene *sceneinstance;
	scene(){}
public:
	//std::vector<glm::vec3> vertices;
	//std::vector<glm::vec3> indices;
	std::vector<tinyobj::shape_t> shapes;
public:
	static scene* getScene();
	void loadScene(char *file);
	struct hitList getHitList(struct ray *currRay);
	glm::vec3 getColorAtPixel(struct hitList *hitlist);
};

struct intersectedObject
{
	std::string  name;
	glm::vec3 normal;
	glm::vec3 intersectPoint;
	tinyobj::material_t   material;
};

struct hitList
{
	std::vector<intersectedObject> intObjs;
};

#endif