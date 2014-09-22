#include "raytracer.h"


void iterateandFill(int startwidth, int startheight, int endwidth, int endheight);

void raytracer::fillbuffer()
{
	scene *scn = scene::getScene();
	camera *cam = camera::getInstance();
	sceneBuffer *scnbuf = sceneBuffer::getsceneBufferInstance();

	//try to get the lights from obj
	cam->setupLights();

	//iterate over all pixels on screen and fill them up
	//try to get the job using 4 threads if screen can be divided into 4 parts nicely
	if (cam->pixwidth % 4 == 0 && cam->pixheight % 4 == 0)
	{
		std::cout << "Woohoo ! got 16 threads to compute all the intersections !!\n";
		std::thread minions[4];
		//for (float i = 0; i < 4; i++)
		//	for (float j = 0; j < 4; j++)
		//	{
		//	float a = i*(1 / 4.0), b = j*(1 / 4.0), c = ((j + 1.0)*(1.0 / 4.0)), d = ((i + 1.0)*(1.0 / 4.0));
		//		minions[(int)j + ((int)i * 4)] = std::thread(&iterateandFill, (int)a, (int)b, (int)cam->pixwidth * (int)c, (int)cam->pixheight * (int)d);
		//		minions[(int)j + ((int)i * 4)].join();
		//	}

		minions[0] = std::thread(&iterateandFill, 0, 0, (int)cam->pixwidth * 0.5, (int)cam->pixheight * 0.5);
		minions[1] = std::thread(&iterateandFill, (int)cam->pixwidth * 0.5, 0, (int)cam->pixwidth, (int)cam->pixheight * 0.5);
		minions[2] = std::thread(&iterateandFill, 0, (int)cam->pixheight * 0.50, (int)cam->pixwidth * 0.5, (int)cam->pixheight);
		minions[3] = std::thread(&iterateandFill, (int)cam->pixwidth * 0.5, (int)cam->pixheight * 0.5, (int)cam->pixwidth, (int)cam->pixheight);

		minions[0].join();
		minions[1].join();
		minions[2].join();
		minions[3].join();
	}
	else
	{
		std::cout << "couldn't divide screen into 4 integral parts. Running single threaded.\n";
		iterateandFill( 0, 0, (int)cam->pixwidth , (int)cam->pixheight );
	}
}

void iterateandFill(int startwidth, int startheight, int endwidth, int endheight)
{
	scene *scn = scene::getScene();
	camera *cam = camera::getInstance();
	sceneBuffer *scnbuf = sceneBuffer::getsceneBufferInstance();

	for (int i = startwidth; i < endwidth; i++)
	{
		for (int j = startheight; j < endheight; j++)
		{
			//construct ray
			glm::vec3 pixpos = cam->getWorldPosofPixel(i, j);
			ray currRay;
			currRay.origin = cam->eye;
			currRay.direction = glm::normalize(pixpos - cam->eye);

			//color at pixel
			struct hitList hitObjects = scn->getHitList(&currRay);
			glm::vec3 color = scn->getColorAtPixel(&hitObjects);

			//store in color buffer
			scnbuf->storecoloratpixel(i, j, &color);
		}
	}

}