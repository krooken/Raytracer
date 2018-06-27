/*
 * PointLight.cpp
 *
 *  Created on: 11 dec 2012
 *      Author: jonas
 */

#include "PointLight.h"

PointLight::PointLight() : Light()
{

}

glm::vec4 PointLight::getDirectionFromPoint(const glm::vec4 & point) {
	glm::vec4 direction = getPosition() - point;
	return glm::normalize(glm::vec4(direction.x, direction.y, direction.z, 0));
}

bool PointLight::blocked(const glm::vec4& origin, const float& obstacle) {
	float distance = glm::length(getPosition() - origin);

	return distance > obstacle;
}
