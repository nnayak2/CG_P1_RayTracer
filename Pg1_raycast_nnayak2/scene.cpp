#include "scene.h"
#include <math.h>

scene* scene::sceneinstance = NULL;

scene* scene::getScene()
{
	if (sceneinstance == NULL)
		sceneinstance = new scene;
	return sceneinstance;
}

//HACK: This variable is a workaround for bug in tiny obj loader
std::map<std::string, tinyobj::material_t> workaroundMaterialStore;

void scene::loadScene(char *filename)
{
	tinyobj::LoadObj(shapes, filename, "./");
	
	//all this code is because tinyobj was spoiling materials in the shapes structure, so i'm reloadig them with this hack.
	for (std::vector<tinyobj::shape_t>::iterator iter = shapes.begin();
		iter != shapes.end(); iter++)
	{
		std::string shapeName = iter->name;
		std::map<std::string, tinyobj::material_t>::iterator it;
		it = workaroundMaterialStore.find(shapeName);
		iter->material = it->second;
	}
}

struct hitList scene::getHitList(ray *currRay)
{
	struct hitList sceneHitList;

	//Iterate over all meshes and return all intersected objects. 
	for (std::vector<tinyobj::shape_t>::iterator iter = shapes.begin();
		iter != shapes.end(); iter++)
	{
		std::string shapeName = iter->name;
		{
			//for (std::vector<unsigned int>::iterator it = iter->mesh.indices.begin(); it != iter->mesh.indices.end(); it = it++)
			for (unsigned int it = 0; it < (iter->mesh.indices.size()); it++)
			{
				glm::vec3 v1(iter->mesh.positions[3 * iter->mesh.indices[it] + 0], iter->mesh.positions[3 * iter->mesh.indices[it] + 1], iter->mesh.positions[3 * iter->mesh.indices[it] + 2]);
				it++;
				glm::vec3 v2(iter->mesh.positions[3 * iter->mesh.indices[it] + 0], iter->mesh.positions[3 * iter->mesh.indices[it] + 1], iter->mesh.positions[3 * iter->mesh.indices[it] + 2]);
				it++;
				glm::vec3 v3(iter->mesh.positions[3 * iter->mesh.indices[it] + 0], iter->mesh.positions[3 * iter->mesh.indices[it] + 1], iter->mesh.positions[3 * iter->mesh.indices[it] + 2]);

				//HACK: normalizing helps us fit really big models in our small view
				//it spoils normals if you activate. Not sure why.
				//v1 = v1 / 300.0f;//glm::normalize(v1);
				//v2 = v2 / 300.0f;//glm::normalize(v2);
				//v3 = v3 / 300.0f;//glm::normalize(v3);

				//ray plane intersection
				glm::vec3 mNormal = -glm::normalize(glm::cross((v3 - v1), (v2 - v1)));

				// (A - P) . N = 0
				double t = (glm::dot(v1, mNormal) - (glm::dot(currRay->origin, mNormal))) /
					(glm::dot(glm::normalize(currRay->direction), mNormal));


				//check if they are parallel to each other
				if (glm::dot(currRay->direction, mNormal) == 0)
					t = -1;

				//if intersection was positive,check if point in triangle
				if (t > 0)
				{
					glm::vec3 intPt = currRay->origin + (glm::vec3(currRay->direction.r * t, currRay->direction.g * t, currRay->direction.b * t));
					glm::vec3 edge0, edge1, edge2, c0, c1, c2;
					edge0 = v2 - v1; edge1 = v3 - v2; edge2 = v1 - v3;
					c0 = intPt - v1; c1 = intPt - v2; c2 = intPt - v3;

					//the first 3 are for the front face and the next 3 are for back face triangles (reversed normal for backface)
					//commented out the last 3 because we dont need to draw the back face of triangles
					if (((glm::dot(mNormal, glm::cross(edge0, c0)) >= 0) &&
						(glm::dot(mNormal, glm::cross(edge1, c1)) >= 0) &&
						(glm::dot(mNormal, glm::cross(edge2, c2)) >= 0))/*
						||
						((glm::dot(-mNormal, glm::cross(edge0, c0)) >= 0) &&
						(glm::dot(-mNormal, glm::cross(edge1, c1)) >= 0) &&
						(glm::dot(-mNormal, glm::cross(edge2, c2)) >= 0))*/)
					{	
						//TODO: Check if the ray has intersected outside view frustum before adding to hit list
						//UPDATE: Spoke with prof Watson, clipping need not be implemented.
						{
							struct intersectedObject temp;
							temp.name = iter->name;
							temp.normal = mNormal;
							temp.intersectPoint = intPt;
							temp.material = iter->material;
							sceneHitList.intObjs.push_back(temp);
						}
					}
				}
			}
		}
	}

	return sceneHitList;
}

glm::vec3 scene::getColorAtPixel(struct hitList *hitlist)
{
	float tempDepth = 100;
	int tempIndex = -1;
	int dumbTracker = 0;

	//find the object nearest to eye from the list of hit objects
	for (std::vector<intersectedObject>::iterator it = hitlist->intObjs.begin(); it != hitlist->intObjs.end(); it++)
	{
		float distFromeye = glm::length(it->intersectPoint - camera::getInstance()->eye);
		if (distFromeye < tempDepth)
		{
			tempDepth = distFromeye;
			tempIndex = dumbTracker;
		}
		dumbTracker++;
	}

	if (tempIndex > -1)
	{
		//lighting = mat.amb * light.amb + mat.dif * light.dif * (N.L) + light.spec * mat.spec * (L.V)^n
		//have a normalized value, and multiply by 255 before returning. The char buffer expects 0-255 range for each color

		intersectedObject tempobj = hitlist->intObjs[tempIndex];
		glm::vec3 ambient(0,0,0), diffuse(0,0,0), specular(0,0,0);

		camera *cam = camera::getInstance();
		glm::vec3 lookVector = glm::normalize(tempobj.intersectPoint - cam->eye);
		
		for (std::vector<lightSources>::iterator iter = cam->lights.begin(); iter != cam->lights.end(); iter++)
		{
			glm::vec3 lightVec = (iter->pos - tempobj.intersectPoint);
			float distance = ((lightVec.x * lightVec.x) + (lightVec.y * lightVec.y) + (lightVec.z * lightVec.z));
			float c1, c2, c3;
			c1 = 0;// .5;
			c2 = 0.4215;
			c3 = 0.2105;
			float att = glm::min(1.0f, 1 / (c1 + (c2 * distance) + (c3 * distance * distance)));

			lightVec = glm::normalize(lightVec);

			float ndotl = glm::clamp(glm::dot(glm::normalize(lightVec), glm::normalize(tempobj.normal)),0.0f,1.0f);
			//float rdotv = glm::clamp(( glm::dot(glm::reflect(lightVec, tempobj.normal), lookVector)),0.0f, 1.0f );
			float ndoth = glm::clamp( glm::dot(glm::normalize(tempobj.normal), glm::normalize(lightVec + lookVector)), 0.0f, 1.0f);

			glm::vec3 matAmb(tempobj.material.ambient[0], tempobj.material.ambient[1], tempobj.material.ambient[2]);
			glm::vec3 matDif(tempobj.material.diffuse[0], tempobj.material.diffuse[1], tempobj.material.diffuse[2]);
			glm::vec3 matSpec(tempobj.material.specular[0], tempobj.material.specular[1], tempobj.material.specular[2]);

			int shininess = (tempobj.material.shininess > 0) ? (int)tempobj.material.shininess : 0 ;
			ambient  +=  iter->amb  * matAmb          ;
			diffuse += iter->dif  * matDif * ndotl * att;
			//specular +=  iter->spec * matSpec * pow(rdotv, iter->shininess);
			specular += iter->spec * matSpec * pow(ndoth, shininess)  *att;
		}
		glm::vec3 finalLight(ambient + diffuse + specular);
		finalLight = glm::clamp(finalLight, 0, 1);
		finalLight = finalLight * 250.0f;
		return finalLight;
	}

	return glm::vec3(0, 0, 0);
}
