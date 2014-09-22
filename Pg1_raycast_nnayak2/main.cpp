#include "main.h"

void draw()
{
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawPixels(camera::getInstance()->pixwidth, camera::getInstance()->pixheight, GL_RGB, GL_UNSIGNED_BYTE,
		sceneBuffer::getsceneBufferInstance()->getBuffer());
	glutSwapBuffers();
}

void initialiseGLUT(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(camera::getInstance()->pixwidth,camera::getInstance()->pixheight);
	glutCreateWindow("\t CSC561: Ray Tracing");
	glutDisplayFunc(draw);
	glutMainLoop();
}

int main(int argc, char* argv[])
{
	glm::vec3 eye(0, 0, -2), look(0, 0, 1), up(0, 1, 0);
	int pixwidth=256, pixheight=256, worldwidth=2, worldheight=2;
	float znear = 1, zfar = 3;
	camera *cam = NULL;

	std::ifstream fileIn;
	fileIn.open("./window.txt");
	if (!fileIn)
	{
		std::cout << "Could not find window.txt file, using assignment defaults."<<std::endl;
	}
	else
	{
		while (!fileIn.eof())
		{
			char line[50];
			fileIn.getline(line, 50);
			if (line[0] == 'p')
			{
				char *token = &line[2];
				_parseInt2(pixwidth, pixheight, token);
			}
			if (line[0] == 'w')
			{
				char *token = &line[2];
				_parseInt2(worldwidth, worldheight, token);
			}
		}
	}

	std::ifstream fileIn2;
	fileIn2.open("./view.txt");
	if (!fileIn2)
	{
		std::cout << "Could not find view.txt file, using assignment defaults."<<std::endl;
	}
	else
	{
		while (!fileIn2.eof())
		{
			char line[50];
			fileIn2.getline(line, 50);
			if (line[0] == 'e')
			{
				char *token = &line[2];
				_parseFloat3(eye.x, eye.y, eye.z, token);
			}
			if (line[0] == 'l')
			{
				char *token = &line[2];
				_parseFloat3(look.x, look.y, look.z, token);
			}
			if (line[0] == 'u')
			{
				char *token = &line[2];
				_parseFloat3(up.x, up.y, up.z, token);
			}
			if (line[0] == 'n')
			{
				char *token = &line[2];
				znear = _parseInt(token);
			}
			if (line[0] == 'f')
			{
				char *token = &line[2];
				zfar = _parseInt(token);
			}
		}
	}

	cam = camera::getInstance(eye, look, up, pixwidth, pixheight, worldwidth, worldheight, znear, zfar);

	//This class loads the obj file, and does the intersection calculations
	scene *scn = scene::getScene();

	//This class just has the buffer to store pixel color information
	sceneBuffer *scnBuf = sceneBuffer::getsceneBufferInstance(cam->pixwidth, cam->pixheight);

	//This class defines the rays and calls intersection test methods and fills the color buffer
	raytracer *rytrc = new raytracer;

	std::string obj = "cube2.obj";
	int userObjNeeded = 0;
	std::cout << "Do you want to manually specify obj (press 1) or use cube2.obj (press 0):\n";
	std::cin >> userObjNeeded;
	if (userObjNeeded)
	{
		std::cout << "enter object to load: (obj file has to be present in current working directory)\n";
		std::cin >> obj;
	}

	//load the obj file
	scn->loadScene(const_cast<char*>(obj.c_str()));

	//Start ray tracing andfill the color buffer
	rytrc->fillbuffer();

	initialiseGLUT(argc, argv);
}