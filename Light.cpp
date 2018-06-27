/*
 * Light.cpp
 *
 *  Created on: 8 dec 2012
 *      Author: jonas
 */

#include "Light.h"

Light::Light()
{
	this->attenuation = glm::vec3(1,0,0);
}

Light::~Light() {
}

// Setters

void Light::setColor(glm::vec3 color)
{
	this->color = color;
}

void Light::setPosition(glm::vec3 position)
{
	this->position = glm::vec4(position.x, position.y, position.z, 1);
}

void Light::setPosition(glm::vec4 position)
{
	this->position = position;
}

glm::vec4 Light::getPosition() {
	return this->position;
}

void Light::setAttenuation(glm::vec3 attenuation) {
	this->attenuation = attenuation;
}

// Getters

glm::vec3 Light::getColor()
{
	return this->color;
}

/*glm::vec4 Light::getDirectionFromPoint(const glm::vec3 & point) {
	return this->position;
}*/

glm::vec3 Light::getAttenuation() {
	return this->attenuation;
}
