/*
 * Triangle.h
 *
 *  Created on: 16 dec 2012
 *      Author: jonas
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "Primitive.h"

class Triangle : public Primitive
{
public:

	Triangle(glm::vec3 A, glm::vec3 B, glm::vec3 C);
	Triangle(const Triangle & other);
	virtual ~Triangle();

	virtual Primitive * copy();

	Triangle & operator=(const Triangle & other);

	virtual float findIntersection(const glm::vec4 & origin, const glm::vec4 & direction);
	virtual Sphere * boundingSphere();

	virtual glm::vec3 getNormal(glm::vec4 position);

	virtual std::string string();

private:

	glm::vec4 A, B, C;

};


#endif /* TRIANGLE_H_ */
