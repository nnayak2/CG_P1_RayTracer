#include "camera.h"

camera* camera::instance = NULL;

camera* camera::getInstance(glm::vec3 eye, glm::vec3 look, glm::vec3 up, int width, int height, int worldwidth, int worldheight, float znear, float zfar)
{
	if (instance == NULL)
		instance = new camera(eye, look, up, width, height, worldwidth, worldheight, znear, zfar);
	return instance;
}

camera* camera::getInstance()
{
	return instance;
}

glm::vec3 camera::getWorldPosofPixel(int i, int j)
{
	//assumptions:
	//start from bottom left of screen

	glm::vec3 screenCenter = eye + (glm::normalize(look)*znear);
	glm::vec3 upcrosslook = glm::normalize(glm::cross(up, look));
	glm::vec3 down = glm::normalize(-up);
	glm::vec3 traverseWidth = glm::normalize(-upcrosslook) * ((float)worldwidth / (float)pixwidth);
	glm::vec3 traverseHeight = glm::normalize(up) * ((float)worldheight / (float)pixheight);

	glm::vec3 zerozero = screenCenter + (upcrosslook*(float)worldwidth/2.0f + down*(float)worldheight/2.0f);
	glm::vec3 wid = (traverseWidth * (float)i );
	glm::vec3 hei = (traverseHeight * (float)j );
	glm::vec3 returnVal = zerozero + wid+ hei;
	return returnVal;
}

camera::camera(glm::vec3 _eye, glm::vec3 _look, glm::vec3 _up, int _pixwidth, int _pixheight, int _worldwidth, int _worldheight, float _znear, float _zfar)
{
	eye = _eye;
	look = _look;
	up = _up;
	pixwidth = _pixwidth;
	pixheight = _pixheight;
	worldwidth = _worldwidth;
	worldheight = _worldheight;
	znear = _znear;
	zfar = _zfar;
}

void camera::setupLights()
{
	std::ifstream fileIn;
	fileIn.open("./lights.txt");
	if (!fileIn)
	{
		std::cout << "could not find lights.txt file, using defaults." << std::endl;
		//def light parameters
		lightSources temp;
		temp.pos = glm::vec3(0, 1, 2);
		temp.amb = glm::vec3(1, 1, 1);
		temp.dif = glm::vec3(1, 1, 1);
		temp.spec = glm::vec3(1.0, 1.0, 1.0);
		lights.push_back(temp);
	}
	else
	{
		while (!fileIn.eof())
		{
			char line[50];
			fileIn.getline(line, 50);
			lightSources temp;
			if (line[0] == 'p')
			{
				char *token = &line[2];
				_parseFloat3(temp.pos.x, temp.pos.y, temp.pos.z, token);

				for (int i= 0; i < 4; i++)
				{
					fileIn.getline(line, 50);
					if (line[0] == 'a')
					{
						char *token = &line[2];
						_parseFloat3(temp.amb.x, temp.amb.y, temp.amb.z, token);
					}
					if (line[0] == 'd')
					{
						char *token = &line[2];
						_parseFloat3(temp.dif.x, temp.dif.y, temp.dif.z, token);
					}
					if (line[0] == 's')
					{
						char *token = &line[2];
						_parseFloat3(temp.spec.x, temp.spec.y, temp.spec.z, token);
					}
				}
				lights.push_back(temp);
			}

		}
	}
}