/*
 * GraphicObject.cpp
 *
 *  Created on: 8 dec 2012
 *      Author: jonas
 */

#include <iostream>

#include "GraphicObject.h"

GraphicObject::~GraphicObject()
{
	delete this->primitive;
}

// Setters

void GraphicObject::setDiffuse(glm::vec3 diffuse)
{
	this->diffuse = diffuse;
}

void GraphicObject::setEmission(glm::vec3 emission)
{
	this->emission = emission;
}

void GraphicObject::setSpecular(glm::vec3 specular)
{
	this->specular = specular;
}

void GraphicObject::setShininess(float shininess)
{
	this->shininess = shininess;
}

// Getters

glm::vec3 GraphicObject::getDiffuse()
{
	return this->diffuse;
}

glm::vec3 GraphicObject::getEmission()
{
	return this->emission;
}

glm::vec3 GraphicObject::getSpecular()
{
	return this->specular;
}

float GraphicObject::getShininess()
{
	return this->shininess;
}

void GraphicObject::addPrimitive(Primitive * primitive) {
	this->primitive = primitive;
}

glm::vec3 GraphicObject::getAmbient() {
	return this->ambient;
}

void GraphicObject::setAmbient(glm::vec3 ambient) {
	this->ambient = ambient;
}

GraphicObject::GraphicObject(const GraphicObject& other) {
	this->ambient = other.ambient;
	this->diffuse = other.diffuse;
	this->emission = other.emission;
	this->shininess = other.shininess;
	this->specular = other.specular;
	this->transform = other.transform;
	this->invTransform = other.invTransform;

	this->primitive = other.primitive->copy();

	//std::cout << "Other: " << other.primitive << std::endl;
	//std::cout << "This: " << this->primitive << std::endl;
}

GraphicObject& GraphicObject::operator =(const GraphicObject& other) {
	this->ambient = other.ambient;
	this->diffuse = other.diffuse;
	this->emission = other.emission;
	this->shininess = other.shininess;
	this->specular = other.specular;
	this->transform = other.transform;
	this->invTransform = other.invTransform;

	this->primitive = other.primitive;

	GraphicObject & g(*this);

	return g;
}

GraphicObject::GraphicObject() {
	this->shininess = 0;

	this->primitive = NULL;
}

void GraphicObject::setTransformation(glm::mat4 transform) {
	this->transform = transform;
	this->invTransform = glm::inverse(transform);
}

intersection GraphicObject::findIntersection(const glm::vec4& origin,
		const glm::vec4& direction) {
	intersection i;

	//std::cout << "FindIntersection" << std::endl;
	//std::cout << "primitive: " << this->primitive->string() << std::endl;
	glm::vec4 invOrigin = invTransform*origin;
	glm::vec4 invDirection = invTransform*direction;
	float result = this->primitive->findIntersection(invOrigin, invDirection);
	//std::cout << "result: " << result << std::endl;
	glm::vec3 n = this->primitive->getNormal(invOrigin + invDirection*result);
	glm::vec4 normal(n.x, n.y, n.z, 0);
	normal = (glm::transpose(invTransform)) * normal;
	normal.w = 0;

	i.distance = result;
	i.normal = glm::normalize(normal);

	return i;
}
