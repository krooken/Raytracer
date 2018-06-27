/*
 * Primitive.h
 *
 *  Created on: 8 dec 2012
 *      Author: jonas
 */

#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

class Sphere;

class Primitive
{
public:

	virtual ~Primitive() = 0;

	virtual Primitive * copy() = 0;

	virtual float findIntersection(const glm::vec4 & origin, const glm::vec4 & direction) = 0;
	virtual Sphere * boundingSphere() = 0;

	virtual glm::vec3 getNormal(glm::vec4 position) = 0;

	virtual std::string string();

private:
};



#endif /* PRIMITIVE_H_ */
