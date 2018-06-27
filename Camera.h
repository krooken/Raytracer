/*
 * Camera.h
 *
 *  Created on: 11 dec 2012
 *      Author: jonas
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>
#include <deque>

class Camera
{
public:

	Camera();

	Camera(float lookFromX, float lookFromY, float lookFromZ,
			float lookAtX, float lookAtY, float lookAtZ,
			float upX, float upY, float upZ, float fieldOfViewY);

	void initialize(float lookFromX, float lookFromY, float lookFromZ,
			float lookAtX, float lookAtY, float lookAtZ,
			float upX, float upY, float upZ, float fieldOfViewY);

	void initialize(glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 up, float fovy);

	void setLookFrom(glm::vec3 lookFrom);
	void setLookAt(glm::vec3 lookAt);
	void setUp(glm::vec3 up);
	void setFovy(float fovy);

	glm::vec4 getPosition();
	glm::vec4 getOrthoNormalLookAt();
	glm::vec4 getOrthoNormalUp();
	glm::vec4 getOrthoNormalY();
	glm::mat4 getOrthoNormalMatrix();

	float getFovY();

	std::deque<glm::vec4> getRays(const int &width, const int &height);

private:

	glm::vec4 position;
	glm::vec4 orthoNormLookAt;
	glm::vec4 orthoNormUp;
	glm::vec4 orthoNormSide;

	glm::mat4 orthoNormMatrix;

	float fovy;
};


#endif /* CAMERA_H_ */
