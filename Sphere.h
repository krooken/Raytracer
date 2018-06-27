/*
 * Sphere.h
 *
 *  Created on: 8 dec 2012
 *      Author: jonas
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include <glm/glm.hpp>

#include "Primitive.h"

class Sphere : public Primitive
{
public:

	Sphere(glm::vec3 position, float radius);
	Sphere(const Sphere & other);
	virtual ~Sphere();

	virtual Primitive * copy();

	Sphere & operator=(const Sphere & other);

	virtual float findIntersection(const glm::vec4 & origin, const glm::vec4 & direction);
	virtual Sphere * boundingSphere();

	virtual glm::vec3 getNormal(glm::vec4 position);

	virtual std::string string();

private:

	glm::vec4 position;
	float radius;

};


#endif /* SPHERE_H_ */
