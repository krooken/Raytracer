/*
 * GraphicObject.h
 *
 *  Created on: 8 dec 2012
 *      Author: jonas
 */

#ifndef GRAPHICOBJECT_H_
#define GRAPHICOBJECT_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Primitive.h"

struct intersection
{
	float distance;
	glm::vec4 normal;
};

class GraphicObject
{
public:

	GraphicObject();
	GraphicObject(const GraphicObject & other);
	~GraphicObject();

	GraphicObject & operator=(const GraphicObject & other);

	glm::vec3 getDiffuse();
	glm::vec3 getEmission();
	glm::vec3 getSpecular();
	glm::vec3 getAmbient();
	float getShininess();

	void setDiffuse(glm::vec3 diffuse);
	void setEmission(glm::vec3 emission);
	void setSpecular(glm::vec3 specular);
	void setShininess(float shininess);
	void setAmbient(glm::vec3 ambient);
	void setTransformation(glm::mat4 transform);

	void addPrimitive(Primitive * primitive);

	intersection findIntersection(const glm::vec4 & origin, const glm::vec4 & direction);

private:

	glm::vec3 diffuse;
	glm::vec3 emission;
	glm::vec3 specular;
	glm::vec3 ambient;
	float shininess;

	glm::mat4 transform;
	glm::mat4 invTransform;

	Primitive * primitive;
};


#endif /* GRAPHICOBJECT_H_ */
