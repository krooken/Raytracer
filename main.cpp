/*
 * main.cpp
 *
 *  Created on: 15 dec 2012
 *      Author: jonas
 */

#include <iostream>
#include <sstream>
#include <string>
#include <deque>
#include <glm/glm.hpp>
#include <cmath>
#include <pthread.h>

#include <FreeImage.h>

#include "ReadFile.h"
#include "Camera.h"

struct thread_scene_data
{
	std::list <GraphicObject> objects;
	std::list <Light *> lights;
	glm::vec4 origin;
	std::deque <glm::vec4> rays;
	BYTE * pixels;
	int height;
	int width;
	int maxdepth;
};

struct thread_pixel_data
{
	int pixelX;
	int pixelY;
	thread_scene_data * scene;
};

glm::vec3 computeDiffuseLight(const glm::vec4 lightDirection,
		const glm::vec4 surfaceNormal, const glm::vec3 color,
		const float attenuation, const glm::vec3 diffuse)
{
	glm::vec4 direction = lightDirection;
	glm::vec4 normal = surfaceNormal;

	float spreading = std::min(glm::dot(direction,normal),(float)1);
	glm::vec3 diffusion = color * diffuse * attenuation * std::max(spreading, (float)0);

	return diffusion;
}

glm::vec3 computeSpecularLight(const glm::vec4 lightDirection,
		const glm::vec4 surfaceNormal, const glm::vec3 color,
		const float attenuation, glm::vec3 specular,
		const glm::vec4 eyeDirection, float shininess)
{
	glm::vec4 direction = lightDirection;
	glm::vec4 normal = surfaceNormal;
	glm::vec4 eyeDir = eyeDirection;

	glm::vec4 halfAngle = glm::normalize(direction + eyeDir);
	float spreading = std::min(glm::dot(halfAngle,normal),(float)1);
	glm::vec3 s = color * specular * attenuation * std::pow(std::max(spreading,(float)0),shininess);

	return s;
}

glm::vec3 computeReflectiveLighting(const glm::vec3 intensity, const glm::vec4 lightDirection,
		const glm::vec4 surfaceNormal, const glm::vec3 color,
		glm::vec3 diffuse, glm::vec3 specular,
		const glm::vec4 eyeDir, const float shininess,
		const glm::vec3 attenuationVec, const float distanceToLight)
{
	float attenuation;
	//lightDirection = glm::normalize(lightPosition.xyz-vertexPos);
	attenuation = 1.0/(attenuationVec.x + attenuationVec.y*distanceToLight + attenuationVec.z*distanceToLight*distanceToLight);

	//vec3 eyeDirection = glm::normalize(eyePos-vertexPos);

	return (computeDiffuseLight(lightDirection,surfaceNormal,color,attenuation,diffuse) +
				computeSpecularLight(lightDirection,surfaceNormal,color,attenuation,
				specular,eyeDir,shininess));
}

glm::vec3 computeLightning(GraphicObject * hitObject, std::list <GraphicObject> & objects,
		std::list <Light *> & lights,
		glm::vec4 & direction, glm::vec4 & normal, glm::vec4 & intersectionPoint, float intersectionDistance)
{
	glm::vec4 lightDirection;
	glm::vec3 color(0);

	for(std::list<Light *>::iterator lit = lights.begin(); lit != lights.end(); lit++)
	{
		//std::cout << "computeLighting" << std::endl;
		Light * l = *lit;
		lightDirection = l->getDirectionFromPoint(glm::vec4(intersectionPoint.x, intersectionPoint.y, intersectionPoint.z, 0));
		float hit = -1;

		for(std::list<GraphicObject>::iterator it = objects.begin(); it != objects.end(); it++)
		{
			intersection i = it->findIntersection(intersectionPoint, lightDirection);
			//glm::vec4 normal = i.normal;

			/*std::cout << "normal: ";
			for(int k=0; k<4; k++)
			{
				std::cout << normal[k] << " ";
			}
			std::cout << std::endl;*/

			//std::cout << "i: " << i << " j: " << j <<" intersection: " << intersection << std::endl;
			if((i.distance < hit && i.distance >= 0.0001) || (i.distance >= 0.0001 && hit < 0))
			{
				hit = i.distance;
			}
		}
		if(hit > 0 && l->blocked(intersectionPoint, hit))
		{

		}else{
			color += computeReflectiveLighting(glm::vec3(1), lightDirection, normal, l->getColor(),
					hitObject->getDiffuse(),hitObject->getSpecular(),-direction, hitObject->getShininess(),
					l->getAttenuation(), glm::length(l->getPosition()-intersectionPoint));

				/*std::cout << "test: ";
				for(int k=0; k<3; k++)
				{
					std::cout << l->getAttenuation()[k] << " ";
				}
				std::cout << std::endl;*/
		}
	}

	return color;// + hitObject->getAmbient() + hitObject->getEmission();
}

void getColor(std::list <GraphicObject> & objects, std::list<Light *> lights,
		glm::vec4 & origin, glm::vec4 & direction, BYTE pixel[], int depth)
{
	float hit = -1;
	GraphicObject * object;
	intersection intersect;

	for(std::list<GraphicObject>::iterator it = objects.begin(); it != objects.end(); it++)
	{
		intersection i = it->findIntersection(origin, direction);

		//std::cout << "i: " << i << " j: " << j <<" intersection: " << intersection << std::endl;
		if((i.distance < hit && i.distance >= 0.0001) || (i.distance >= 0.0001 && hit < 0))
		{
			object = &(*it);
			hit = i.distance;
			intersect = i;
		}
	}
	if(hit > 0)
	{
		//std::cout << "hit" << std::endl;

		glm::vec4 intersectionPoint = origin + direction*hit;

		glm::vec3 color = computeLightning(object, objects, lights, direction, intersect.normal,
				intersectionPoint,intersect.distance);

		/*std::cout << "color: ";
		for(int k=0; k<3; k++)
		{
			std::cout << color[k] << " ";
		}
		std::cout << std::endl;*/
		BYTE reflectedColor[3];

		if(depth > 0 && glm::length(object->getSpecular()) > 0.001)
		{
			//std::cout << "Recursive step?" << std::endl;
			depth--;
			glm::vec4 reflectedRay = glm::reflect(direction, intersect.normal);
			reflectedRay.w = 0;
			reflectedRay = glm::normalize(reflectedRay);

			getColor(objects, lights, intersectionPoint, reflectedRay, reflectedColor, depth);
		}
		else
		{
			pixel[0] = 0x00;//B
			pixel[1] = 0x00;//G
			pixel[2] = 0x00;//R
		}

		pixel[0] = (BYTE)(std::min(color.z * 0xFF + (float)reflectedColor[0] * object->getSpecular().z, (float)0xFF));
		pixel[1] = (BYTE)(std::min(color.y * 0xFF + (float)reflectedColor[1] * object->getSpecular().y, (float)0xFF));
		pixel[2] = (BYTE)(std::min(color.x * 0xFF + (float)reflectedColor[2] * object->getSpecular().x, (float)0xFF));
	}
	else
	{
		pixel[0] = 0x00;//B
		pixel[1] = 0x00;//G
		pixel[2] = 0x00;//R
	}
}

void * runThread(void * threadarg)
{
	thread_pixel_data * pixel_data;
	pixel_data = (thread_pixel_data *)threadarg;

	//std::cout << "Row: " << pixel_data->pixelY << std::endl;
	//std::cout << "Col: " << pixel_data->pixelX << std::endl;

	BYTE pixel[3];
	int offset = pixel_data->pixelX + pixel_data->pixelY * pixel_data->scene->width;

	getColor(pixel_data->scene->objects, pixel_data->scene->lights,
			pixel_data->scene->origin, pixel_data->scene->rays[offset], pixel, 2);

	pixel_data->scene->pixels[offset*3] = pixel[0];
	pixel_data->scene->pixels[offset*3+1] = pixel[1];
	pixel_data->scene->pixels[offset*3+2] = pixel[2];

	pthread_exit(NULL);

	return NULL;
}

void * runRowThread(void * threadarg)
{
	thread_pixel_data * base_pixel_data;
	thread_pixel_data * pixel_data;
	base_pixel_data = (thread_pixel_data *)threadarg;
	pixel_data = base_pixel_data;

	//std::cout << "Row: " << pixel_data->pixelY << std::endl;
	//std::cout << "Col: " << pixel_data->pixelX << std::endl;

	BYTE pixel[3];
	int offset = base_pixel_data->pixelY * pixel_data->scene->width;

	for(int i=0; i<base_pixel_data->scene->width; i++)
	{

		getColor(pixel_data->scene->objects, pixel_data->scene->lights,
				pixel_data->scene->origin, pixel_data->scene->rays[offset+i], pixel, pixel_data->scene->maxdepth);

		pixel_data->scene->pixels[offset*3+i*3] = pixel[0];
		pixel_data->scene->pixels[offset*3+i*3+1] = pixel[1];
		pixel_data->scene->pixels[offset*3+i*3+2] = pixel[2];

	}

	pthread_exit(NULL);

	return NULL;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: transforms scenefile [grader input (optional)]\n";
        exit(-1);
    }

    FreeImage_Initialise();

    std::list <GraphicObject> objects;
    std::list <Light *> lights;
    Camera camera;
    int width, height, maxdepth;
    std::string outputFile;

    try
    {
    readfile(argv[1], objects, lights, camera, width, height, outputFile, maxdepth);

    std::deque <glm::vec4> rays = camera.getRays(width, height);

    std::cout << "OutputFile: " << outputFile << std::endl;
    std::cout << "Objects: " << objects.size() << std::endl;
    std::cout << "Lights: " << lights.size() << std::endl;
    std::cout << "Maxdepth: " << maxdepth << std::endl;

    BYTE pixels[3 * width * height];
    BYTE pixel[3];

    pthread_t threads[height];
    pthread_attr_t attr;
    // Initialize and set thread joinable
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    thread_pixel_data tpd[height];
    thread_scene_data tsd;

    tsd.objects = objects;
    tsd.lights = lights;
    tsd.origin = camera.getPosition();
    tsd.pixels = pixels;
    tsd.rays = rays;
    tsd.height = height;
    tsd.width = width;
    tsd.maxdepth = maxdepth;

    for(int i=0; i<height; i++)
    {
		tpd[i].pixelX = 0;
		tpd[i].pixelY = i;
		tpd[i].scene = &tsd;

		pthread_create(&threads[i], NULL, runRowThread, (void *)&tpd[i]);

		/*for(int j=0; j<width; j++)
    	{

    		glm::vec4 origin = camera.getPosition();
    		glm::vec4 direction = rays[i*width + j];

    		getColor(objects, lights, origin, direction, pixel, maxdepth);

    		pixels[i*width*3 + j*3] = pixel[0];
    		pixels[i*width*3 + j*3+1] = pixel[1];
    		pixels[i*width*3 + j*3+2] = pixel[2];
    	}*/

		if((i) % 4 == 0 )
		{
			pthread_attr_destroy(&attr);
			for(int k=0; k < 4; k++ ){
				pthread_join(threads[i - k], NULL);
			}
		    pthread_attr_init(&attr);
		    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		}
    }

    /*for(int k=0; k<width*height*3; k++)
    {
    	pixels[k] = 0xFF;
    }*/

	pthread_attr_destroy(&attr);
	for(int i=0; i < height; i++ ){
		pthread_join(threads[i], NULL);
	}

    FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, width, height, width * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);

    bool saved = FreeImage_Save(FIF_PNG, img, outputFile.c_str(), 0);

    std::cout << "Image success: " << saved << std::endl;

    }
    catch(...)
    {
    	std::cout << "Error" << std::endl;
    }

    FreeImage_DeInitialise();

	pthread_exit(NULL);
    return 0;
}
