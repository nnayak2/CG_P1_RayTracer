#include "scenebuffer.h"

sceneBuffer* sceneBuffer::instance = NULL;

sceneBuffer* sceneBuffer::getsceneBufferInstance(int width, int height)
{
	if (instance == NULL)
		instance = new sceneBuffer(width, height);
	return instance;
}

sceneBuffer* sceneBuffer::getsceneBufferInstance()
{
	return instance;
}

char* sceneBuffer::getBuffer()
{
	return colorBuffer;
}

void sceneBuffer::storecoloratpixel(int i, int j, glm::vec3 *color)
{
	//i is width
	//j is height
	int width = camera::getInstance()->pixwidth;
	*(colorBuffer + (3 * i) + (3 * j * width) + 0) = (char)color->r;
	*(colorBuffer + (3 * i) + (3 * j * width) + 1) = (char)color->g;
	*(colorBuffer + (3 * i) + (3 * j * width) + 2) = (char)color->b;
}