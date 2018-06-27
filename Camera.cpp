/*
 * Camera.cpp
 *
 *  Created on: 11 dec 2012
 *      Author: jonas
 */

//#define GLM_SWIZZLE_XYZW
//#include <glm/glm.hpp>

#include <iostream>

#include "Camera.h"

Camera::Camera() {
	this->position = glm::vec4(0);
	this->orthoNormLookAt = glm::vec4(0);
	this->orthoNormUp = glm::vec4(0);
	this->orthoNormMatrix = glm::mat4(0);
	this->fovy = 0;
}

Camera::Camera(float lookFromX, float lookFromY, float lookFromZ, float lookAtX,
		float lookAtY, float lookAtZ, float upX, float upY, float upZ,
		float fieldOfViewY) {
	this->initialize(lookFromX, lookFromY, lookFromZ,
					 lookAtX, lookAtY, lookAtZ,
					 upX, upY, upZ,
					 fieldOfViewY);
}

void Camera::initialize(float lookFromX, float lookFromY, float lookFromZ, float lookAtX,
		float lookAtY, float lookAtZ, float upX, float upY, float upZ,
		float fieldOfViewY) {
	this->position = glm::vec4(lookFromX, lookFromY, lookFromZ, 1);

	glm::vec3 direction = glm::vec3(position.x, position.y, position.z) - glm::vec3(lookAtX, lookAtY, lookAtZ);
	glm::vec3 up(upX, upY, upZ);

	glm::vec3 w = glm::normalize(direction);
	glm::vec3 u = glm::normalize(glm::cross(up,w));
	glm::vec3 v = glm::normalize(glm::cross(w,u));


	this->orthoNormLookAt = glm::vec4(w.x, w.y, w.z, 0);
	this->orthoNormUp = glm::vec4(v.x, v.y, v.z, 0);
	this->orthoNormSide = glm::vec4(u.x, u.y, u.z, 0);

	this->orthoNormMatrix = glm::mat4(0);
	this->fovy = glm::radians(fieldOfViewY);
}

void Camera::initialize(glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 up, float fovy)
{
	this->initialize(lookFrom.x, lookFrom.y, lookFrom.z,
					lookAt.x, lookAt.y, lookAt.z,
					up.x, up.y, up.z,
					fovy);

}

void Camera::setLookFrom(glm::vec3 lookFrom) {

}

void Camera::setLookAt(glm::vec3 lookAt) {
}

void Camera::setUp(glm::vec3 up) {
}

void Camera::setFovy(float fovy) {
}

glm::vec4 Camera::getOrthoNormalLookAt() {
	return this->orthoNormLookAt;
}

glm::vec4 Camera::getOrthoNormalUp() {
	return this->orthoNormUp;
}

glm::vec4 Camera::getOrthoNormalY() {
	return this->orthoNormLookAt;
}

glm::mat4 Camera::getOrthoNormalMatrix() {
	return this->orthoNormMatrix;
}

float Camera::getFovY() {
	return fovy;
}

glm::vec4 Camera::getPosition() {
	return this->position;
}

std::deque<glm::vec4> Camera::getRays(const int& width, const int& height) {
	std::deque <glm::vec4> rays;

	//std::cout << "fovy: " << fovy << std::endl;
	//float fovx = fovy * (float)height/(float)width;
	float fovx = 2 * glm::atan((float)width*glm::tan(fovy/2.0)/(float)height);
	//std::cout << "fovx: " << fovx << std::endl;
	float alphaFactor = glm::tan(fovx/2);
	//std::cout << "alphaFactor: " << alphaFactor << std::endl;
	float betaFactor = glm::tan(fovy/2);
	//std::cout << "betaFactor: " << betaFactor << std::endl;
	float halfHeight = height/2.0;
	//std::cout << "halfHeight: " << halfHeight << std::endl;
	float halfWidth = width/2.0;
	//std::cout << "halfWidth: " << halfWidth << std::endl;

	float alpha, beta;
	glm::vec4 ray;

	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			alpha = alphaFactor * (j+.5-halfWidth)/halfWidth;
			//std::cout << "alpha: " << alpha << std::endl;
			beta = betaFactor * (halfHeight-(i+.5))/halfHeight;
			//std::cout << "beta: " << beta << std::endl;
			ray = glm::normalize(alpha*orthoNormSide + beta*orthoNormUp - orthoNormLookAt);
			rays.push_back(ray);
		}
	}

	return rays;
}
