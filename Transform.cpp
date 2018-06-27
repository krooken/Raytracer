// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
    // YOUR CODE FOR HW2 HERE
    // Please implement this.  Likely the same as in HW 1.  
    float radians = pi/180.0f * -degrees;
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;

	glm::mat3 rotationMatrix = glm::cos(radians) * mat3(1,0,0,0,1,0,0,0,1)
			+ (1-glm::cos(radians))*mat3(x*x,x*y,x*z,y*x,y*y,y*z,z*x,z*y,z*z)
			+ glm::sin(radians)*mat3(0,-z,y,z,0,-x,-y,x,0);

      // You will change this return call
      return rotationMatrix;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
    // YOUR CODE FOR HW2 HERE
    // Likely the same as in HW 1.
	vec3 ortUp = upvector(up, eye);
	glm::mat3 rotationMatrix = rotate(-degrees,ortUp);
	glm::vec3 newEye = eye*rotationMatrix;
	glm::vec3 newUp = up*rotationMatrix;

	eye.x = newEye.x;
	eye.y = newEye.y;
	eye.z = newEye.z;

	up.x = newUp.x;
	up.y = newUp.y;
	up.z = newUp.z;
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
    // YOUR CODE FOR HW2 HERE 
    // Likely the same as in HW 1.
	glm::vec3 rotationVector = glm::normalize(glm::cross(eye,up));
	glm::mat3 rotationMatrix = rotate(-degrees,rotationVector);
	glm::vec3 newEye = eye*rotationMatrix;
	glm::vec3 newUp = up*rotationMatrix;

	eye.x = newEye.x;
	eye.y = newEye.y;
	eye.z = newEye.z;

	up.x = newUp.x;
	up.y = newUp.y;
	up.z = newUp.z;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
	glm::vec3 w = glm::normalize(eye);
	glm::vec3 u = glm::normalize(glm::cross(up,w));
	glm::vec3 v = glm::cross(w,u);

	glm::mat4 rotationMatrix = mat4(
			u.x,u.y,u.z,0,
			v.x,v.y,v.z,0,
			w.x,w.y,w.z,0,
			0,0,0,1
	);
	glm::mat4 translationMatrix = mat4(
			1,0,0,-eye.x,
			0,1,0,-eye.y,
			0,0,1,-eye.z,
			0,0,0,1
	);

	glm::mat4 transformationMatrix = translationMatrix*rotationMatrix;

  // You will change this return call
  return transformationMatrix;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
	float radians = pi/180.0f * fovy;
	float distance = glm::cos(radians/2)/glm::sin(radians/2);
    mat4 ret = mat4(distance/aspect,0,0,0,
    		0,distance,0,0,
    		0,0,-(zFar+zNear)/(zFar-zNear),-2*zFar*zNear/(zFar-zNear),
    		0,0,-1,0);
    // YOUR CODE FOR HW2 HERE
    // New, to implement the perspective transform as well.  
    return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
    mat4 ret = mat4(sx,0,0,0,0,sy,0,0,0,0,sz,0,0,0,0,1);
    // YOUR CODE FOR HW2 HERE
    // Implement scaling 

    return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
    mat4 ret = mat4(1,0,0,0,0,1,0,0,0,0,1,0,tx,ty,tz,1);
    // YOUR CODE FOR HW2 HERE
    // Implement translation 
    return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
    vec3 x = glm::cross(up,zvec); 
    vec3 y = glm::cross(zvec,x); 
    vec3 ret = glm::normalize(y); 
    return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}

mat4 Transform::rotate(const float degrees, const vec4& axis) {
    // YOUR CODE FOR HW2 HERE
    // Please implement this.  Likely the same as in HW 1.
    float radians = pi/180.0f * degrees;
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;

	glm::mat4 rotationMatrix = glm::cos(radians) * mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0)
			+ (1-glm::cos(radians))*mat4(x*x,x*y,x*z,0,y*x,y*y,y*z,0,z*x,z*y,z*z,0,0,0,0,0)
			+ glm::sin(radians)*mat4(0,-z,y,0,z,0,-x,0,-y,x,0,0,0,0,0,0);

      // You will change this return call
      return rotationMatrix;
}
