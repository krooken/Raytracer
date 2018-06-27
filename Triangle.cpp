
/*
 * Triangle.cpp
 *
 *  Created on: 16 dec 2012
 *      Author: jonas
 */

#include <iostream>

#include "Triangle.h"
#include "Sphere.h"


Triangle::Triangle(glm::vec3 A, glm::vec3 B, glm::vec3 C) {
	this->A = glm::vec4(A.x, A.y, A.z, 1);
	this->B = glm::vec4(B.x, B.y, B.z, 1);
	this->C = glm::vec4(C.x, C.y, C.z, 1);
}


Triangle::Triangle(const Triangle& other) {
	this->A = other.A;
	this->B = other.B;
	this->C = other.C;
}


Triangle::~Triangle() {
}

Primitive* Triangle::copy() {
	Triangle * t = new Triangle(*this);

	return t;
}


Triangle& Triangle::operator =(const Triangle& other) {
	this->A = other.A;
	this->B = other.B;
	this->C = other.C;

	Triangle & t(*this);

	return t;
}


float Triangle::findIntersection(const glm::vec4& origin,
		const glm::vec4& direction) {

	glm::mat3 Amat(B.x-A.x, C.x-A.x, - direction.x,
				   B.y-A.y, C.y-A.y, - direction.y,
				   B.z-A.z, C.z-A.z, - direction.z);

	/*std::cout << "Amat = " << std::endl;
	for(int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
		{
			std::cout << Amat[i][j] << " ";
		}
		std::cout << std::endl;
	}*/

	glm::vec3 bvec(origin.x-A.x, origin.y-A.y, origin.z-A.z);

	float det = glm::determinant(glm::transpose(Amat));

	if(-0.0001 < det && det < 0.0001)
	{
		//std::cout << "Singular: d = " << det << std::endl;
		return -1;
	}
	else
	{
		glm::vec3 xvec = glm::inverse(glm::transpose(Amat)) * bvec;
		/*std::cout << "Beta = " << xvec.x << std::endl;
		std::cout << "Gamma = " << xvec.y << std::endl;
		std::cout << "t = " << xvec.z << std::endl;*/
		if(xvec.x > 0 && xvec.y > 0 && xvec.x+xvec.y < 1 && xvec.z > 0)
		{
			//std::cout << "Intersection = " << xvec.z << std::endl;
			return xvec.z;
		}
		else
		{
			//std::cout << "No intersection = " << xvec.z << std::endl;
			return -1;
		}
	}
}


Sphere* Triangle::boundingSphere() {
	return new Sphere(glm::vec3(0), 0);
}

glm::vec3 Triangle::getNormal(glm::vec4 position) {
	return glm::normalize(glm::cross(glm::vec3(this->B.x-this->A.x, this->B.y-this->A.y, this->B.z-this->A.z),
			glm::vec3(this->C.x-this->A.x, this->C.y-this->A.y, this->C.z-this->A.z)));
}

std::string Triangle::string() {
	return "Triangle";
}


