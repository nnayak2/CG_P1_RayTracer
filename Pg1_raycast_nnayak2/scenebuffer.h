#ifndef SCENEBUFFER_H
#define SCENEBUFFER_H

#include <iostream>
#include "glm/glm.hpp"
#include "camera.h"

class sceneBuffer
{
	char *colorBuffer;
	static sceneBuffer *instance;
	sceneBuffer(int width, int height)
	{
		colorBuffer = (char*)malloc(width * height * 3 * sizeof(char));
	}
public:
	static sceneBuffer* getsceneBufferInstance(int width, int height);
	static sceneBuffer* getsceneBufferInstance();
	char* getBuffer();
	void storecoloratpixel(int i, int j, glm::vec3 *);
};

#endif