/*
 * Sphere.cpp
 *
 *  Created on: 8 dec 2012
 *      Author: jonas
 */

#include <cmath>
#include <iostream>

#include "Sphere.h"
#include "Primitive.h"

Sphere::Sphere(glm::vec3 position, float radius) {
	this->radius = radius;
	this->position = glm::vec4(position.x, position.y, position.z, 1);
}

Sphere::~Sphere()
{
}

float Sphere::findIntersection(const glm::vec4& origin,
		const glm::vec4& direction) {
	glm::vec4 distance = origin - position;

	/*std::cout << "origin: ";
	for(int i=0; i<4; i++)
	{
		std::cout << i << ": " << origin[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "direction: ";
	for(int i=0; i<4; i++)
	{
		std::cout << i << ": " << direction[i] << " ";
	}
	std::cout << std::endl;*/

	float a = glm::dot(direction,direction);
	//std::cout << "a: " << a << std::endl;
	float b = 2 * glm::dot(direction,distance);
	//std::cout << "b: " << b << std::endl;
	float c = glm::dot(distance,distance) - std::pow(this->radius,2);
	//std::cout << "c: " << c << std::endl;

	float offset = - b/(2*a);
	//std::cout << "offset: " << offset << std::endl;
	float discriminant = std::pow(b/(2*a),2) - c/a;
	//std::cout << "discriminant: " << discriminant << std::endl;

	if(discriminant <= 0)
	{
		//std::cout << "Discriminant < 0: " << discriminant << std::endl;
		return -1;
	}
	else
	{
		float sqrRoot = std::sqrt(discriminant);
		if(offset-sqrRoot < 0)
		{
			//std::cout << "offset-sqrRoot < 0: " << offset-sqrRoot << std::endl;
			//std::cout << "offset+sqrRoot: " << offset+sqrRoot << std::endl;
			return offset+sqrRoot;
		}
		else
		{
			//std::cout << "offset+sqrRoot: " << offset+sqrRoot << std::endl;
			return offset-sqrRoot;
		}
	}
}

Sphere * Sphere::boundingSphere() {
	return this;
}

Sphere::Sphere(const Sphere& other) {
	this->radius = other.radius;
	this->position = other.position;
}

Sphere& Sphere::operator =(const Sphere& other) {
	this->radius = other.radius;
	this->position = other.position;

	Sphere & s(*this);

	return s;
}

Primitive* Sphere::copy() {
	Sphere * s = new Sphere(*this);

	return s;
}

glm::vec3 Sphere::getNormal(glm::vec4 position) {
	glm::vec4 normal = position - this->position;
	normal.w = 0;
	normal = glm::normalize(normal);
	return glm::vec3(normal.x, normal.y, normal.z);
}

std::string Sphere::string() {
	return "Sphere";
}
