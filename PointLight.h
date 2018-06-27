/*
 * PointLight.h
 *
 *  Created on: 11 dec 2012
 *      Author: jonas
 */

#ifndef POINTLIGHT_H_
#define POINTLIGHT_H_

#include "Light.h"

class PointLight : public Light
{
public:
	PointLight();

	virtual glm::vec4 getDirectionFromPoint(const glm::vec4 & point);

	virtual bool blocked(const glm::vec4& origin, const float & obstacle);
};

#endif /* POINTLIGHT_H_ */
