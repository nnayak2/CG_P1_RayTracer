#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <iostream>
#include <vector>
#include "glm/glm.hpp"

#include "scenebuffer.h"
#include "scene.h"
#include "camera.h"
#include <thread>

class raytracer
{
public:
	void fillbuffer();
	//void iterateandFill(int startwidth, int startheight, int endwidth, int endheight);
};

struct ray
{
	glm::vec3 origin;
	glm::vec3 direction;
};


#endif