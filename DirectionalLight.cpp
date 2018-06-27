
/*
 * DirectionalLight.cpp
 *
 *  Created on: 11 dec 2012
 *      Author: jonas
 */

#include "DirectionalLight.h"

glm::vec4 DirectionalLight::getDirectionFromPoint(const glm::vec4& point) {
	return glm::normalize(glm::vec4(getPosition().x, getPosition().y, getPosition().z, 0));
}

DirectionalLight::DirectionalLight() : Light() {

}

bool DirectionalLight::blocked(const glm::vec4& origin, const float & obstacle) {
	return true;
}

/*glm::vec3 DirectionalLight::getAttenuation()
{
	return Light::getAttenuation();
}*/
