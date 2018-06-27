/*
 * DirectionalLight.h
 *
 *  Created on: 11 dec 2012
 *      Author: jonas
 */

#ifndef DIRECTIONALLIGHT_H_
#define DIRECTIONALLIGHT_H_

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();

	virtual glm::vec4 getDirectionFromPoint(const glm::vec4 & point);
	virtual bool blocked(const glm::vec4& origin, const float & obstacle);
//	virtual glm::vec3 getAttenuation();
};


#endif /* DIRECTIONALLIGHT_H_ */
