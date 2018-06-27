/*
 * Light.h
 *
 *  Created on: 8 dec 2012
 *      Author: jonas
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Light
{
public:

	Light();
	virtual ~Light();

	void setPosition(glm::vec3 position);
	void setPosition(glm::vec4 position);
	void setColor(glm::vec3 color);
	void setAttenuation(glm::vec3 attenuation);

	glm::vec4 getPosition();
	virtual glm::vec4 getDirectionFromPoint(const glm::vec4 & point) = 0;
	glm::vec3 getColor();
	glm::vec3 getAttenuation();

	virtual bool blocked(const glm::vec4& origin, const float & obstacle) = 0;

private:

	glm::vec4 position;
	glm::vec3 color;
	glm::vec3 attenuation;

};


#endif /* LIGHT_H_ */
