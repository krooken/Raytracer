/*
 * ReadFile.cpp
 *
 *  Created on: 8 dec 2012
 *      Author: jonas
 */

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <stack>
#include <list>
#include <deque>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ReadFile.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Transform.h"
#include "Light.h"
#include "PointLight.h"
#include "DirectionalLight.h"

/*
// The function below applies the appropriate transform to a 4-vector
void matransform(stack<mat4> &transfstack, GLfloat* values)
{
    mat4 transform = transfstack.top();
    vec4 valvec = vec4(values[0],values[1],values[2],values[3]);
    vec4 newval = valvec * transform;
    for (int i = 0; i < 4; i++) values[i] = newval[i];
}*/

void rightmultiply(const mat4 & M, std::stack<mat4> &transfstack)
{
    mat4 &T = transfstack.top();
    // Right multiply M, but do this left to account for row/column major
    //T = M * T;
    T = T * M;
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.
bool readvals(std::stringstream &s, const int numvals, float * values)
{
    for (int i = 0; i < numvals; i++) {
        s >> values[i];
        if (s.fail()) {
            std::cout << "Failed reading value " << i << " will skip\n";
            return false;
        }
    }
    return true;
}

void readfile(const char* filename,
		std::list <GraphicObject> & objects,
		std::list <Light *> & lights,
		Camera & camera,
		int & width, int & height,
		std::string & outputFile,
		int & maxdepth)
{
    std::string str, cmd;
    std::ifstream in;
    in.open(filename);
    if (in.is_open()) {

        // I need to implement a matrix stack to store transforms.
        // This is done using standard STL Templates
        std::stack <glm::mat4> transfstack;
        transfstack.push(glm::mat4(1.0));  // identity

        std::deque <glm::vec3> vertices;

        GraphicObject object;
        glm::vec3 ambient, specular, diffuse, emission, attenuation(1,0,0);
        float shininess;
        maxdepth = 5;

        getline (in, str);
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != std::string::npos) && (str[0] != '#')) {
                // Ruled out comment and blank lines

                std::stringstream s(str);
                s >> cmd;
                std::cout << str << std::endl;
                int i;
                float values[10]; // Position and color for light, colors for others
                                    // Up to 10 params for cameras.
                bool validinput; // Validity of input

                // Process the light, add it to database.
                // Lighting Command
                if (cmd == "light") {

                    std::cerr << "Unknown Command: " << cmd << " Skipping \n";
                	/*
                    if (numused == numLights) { // No more Lights
                    	std::cerr << "Reached Maximum Number of Lights " << numused << " Will ignore further lights\n";
                    } else {
                        validinput = readvals(s, 8, values); // Position/color for lts.
                        if (validinput) {

                            // YOUR CODE FOR HW 2 HERE.
                            // Note that values[0...7] shows the read in values
                            // Make use of lightposn[] and lightcolor[] arrays in variables.h
                            // Those arrays can then be used in display too.

                        	int offset = numused * 4;
                        	cout << numused << endl;
                        	cout << offset << endl;
                        	for(int i=0; i<4; i++){
                        		lightposn[offset+i] = values[i];
                        		lightcolor[offset+i] = values[i+4];
                        	}

                            ++numused;
                        }
                    }*/
                }

                // Material Commands
                // Ambient, diffuse, specular, shininess properties for each object.
                // Filling this in is pretty straightforward, so I've left it in
                // the skeleton, also as a hint of how to do the more complex ones.
                // Note that no transforms/stacks are applied to the colors.

                else if (cmd == "point") {
					validinput = readvals(s, 6, values); // colors
					if (validinput) {
						Light * l = new PointLight();
						l->setPosition(glm::vec3(values[0],values[1],values[2]));
						l->setColor(glm::vec3(values[3],values[4],values[5]));
						l->setAttenuation(attenuation);
						lights.push_back(l);
					}
				}else if (cmd == "directional") {
					validinput = readvals(s, 6, values); // colors
					if (validinput) {
						Light * l = new DirectionalLight();
						l->setPosition(glm::vec3(values[0],values[1],values[2]));
						l->setColor(glm::vec3(values[3],values[4],values[5]));
						l->setAttenuation(glm::vec3(1,0,0));
						lights.push_back(l);
					}
				}else if (cmd == "maxdepth") {
					validinput = readvals(s, 1, values); // colors
					if (validinput) {
						maxdepth = values[0];
					}
				}
				else if (cmd == "attenuation") {
					validinput = readvals(s, 3, values); // colors
					if (validinput) {
						for (i = 0; i < 3; i++) {
							attenuation[i] = values[i];
						}
					}
				}
                else if (cmd == "ambient") {
                    validinput = readvals(s, 3, values); // colors
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            ambient[i] = values[i];
                        }
                    }
                } else if (cmd == "diffuse") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            diffuse[i] = values[i];
                        	//diffuse[i] = 0;
                        }
                    }
                } else if (cmd == "specular") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            specular[i] = values[i];
                        	//specular[i] = 0.0;
                        }
                    }
                } else if (cmd == "emission") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            emission[i] = values[i];
                        }
                    }
                } else if (cmd == "shininess") {
                    validinput = readvals(s, 1, values);
                    if (validinput) {
                        shininess = values[0];
                    }
                } else if (cmd == "size") {
                    validinput = readvals(s,2,values);
                    if (validinput) {
                        width = (int) values[0]; height = (int) values[1];
                    }
                } else if (cmd == "camera") {
                	validinput = readvals(s,10,values); // 10 values eye cen up fov
                	if (validinput) {
                		glm::vec3 eye = glm::vec3(values[0], values[1], values[2]);
                		glm::vec3 lookAt = glm::vec3(values[3], values[4], values[5]);
                		glm::vec3 up = glm::vec3(values[6], values[7], values[8]);
                		float fovy = values[9];

                		/*camera.setLookAt(lookAt);
                		camera.setLookFrom(eye);
                		camera.setUp(up);
                		camera.setFovy(fovy);
                		std::cout << fovy << std::endl;*/
                		camera.initialize(eye,lookAt,up,fovy);
                	}

                    /*validinput = readvals(s,10,values); // 10 values eye cen up fov
                    if (validinput) {

                        // YOUR CODE FOR HW 2 HERE
                        // Use all of values[0...9]
                        // You may need to use the upvector fn in Transform.cpp
                        // to set up correctly.
                        // Set eyeinit upinit center fovy in variables.h

                    	eyeinit = vec3(values[0],values[1],values[2]);
                    	center = vec3(values[3],values[4],values[5]);
                    	upinit = vec3(values[6],values[7],values[8]);
                    	fovy = values[9];

                    }*/
                } else if (cmd == "output") {
                    std::string output;
                    s >> output;
                    outputFile += output;
                }

                // I've left the code for loading objects in the skeleton, so
                // you can get a sense of how this works.
                // Also look at demo.txt to get a sense of why things are done this way.

                else if (cmd == "sphere")
                {
                	validinput = readvals(s, 4, values);
                	if (validinput){
						float radius;
						glm::vec3 position;
						for(int i=0; i<3; i++)
						{
							position[i] = values[i];
						}
						radius = values[3];
						object.setAmbient(ambient);
						object.setDiffuse(diffuse);
						object.setEmission(emission);
						object.setSpecular(specular);
						object.setShininess(shininess);
						object.addPrimitive(new Sphere(position, radius));
						object.setTransformation(transfstack.top());
						objects.push_back(object);
						object = GraphicObject();
                	}

                }

                else if (cmd == "vertex")
                {
                	validinput = readvals(s, 3, values);
                	if (validinput){
						glm::vec3 vertex(values[0],values[1],values[2]);
						/*std::cout << "vertex: " << std::endl;
						for(int i=0; i<3; i++)
						{
							std::cout << values[i] << " ";
						}
						std::cout << std::endl;*/
						vertices.push_back(vertex);
                	}

                }

                else if (cmd == "tri")
                {
                	validinput = readvals(s, 3, values);
                	if (validinput){
						glm::vec3 v[3];
						for(i=0; i<3; i++)
						{
							v[i] = vertices[values[i]];
						}
						object.setAmbient(ambient);
						object.setDiffuse(diffuse);
						object.setEmission(emission);
						object.setSpecular(specular);
						object.setShininess(shininess);
						object.addPrimitive(new Triangle(v[0], v[1], v[2]));
						object.setTransformation(transfstack.top());
						objects.push_back(object);
						object = GraphicObject();
                	}

                }

                //else if (cmd == "sphere" || cmd == "cube" || cmd == "teapot") {
                    /*if (numobjects == maxobjects) { // No more objects
                        cerr << "Reached Maximum Number of Objects " << numobjects << " Will ignore further objects\n";
                    } else {
                        validinput = readvals(s, 1, values);
                        if (validinput) {
                            object * obj = &(objects[numobjects]);
                            obj->size = values[0];

                            // Set the object's light properties
                            for (i = 0; i < 4; i++) {
                                (obj->ambient)[i] = ambient[i];
                                (obj->diffuse)[i] = diffuse[i];
                                (obj->specular)[i] = specular[i];
                                (obj->emission)[i] = emission[i];
                            }
                            obj->shininess = shininess;

                            // Set the object's transform
                            obj->transform = transfstack.top();

                            // Set the object's type
                            if (cmd == "sphere") {
                                obj->type = sphere;
                            } else if (cmd == "cube") {
                                obj->type = cube;
                            } else if (cmd == "teapot") {
                                obj->type = teapot;
                            }
                        }
                        ++numobjects;
                    }*/
                //}

                else if (cmd == "translate") {
                    validinput = readvals(s,3,values);
                    if (validinput) {

                    	float x, y, z;
                    	x = values[0];
                    	y = values[1];
                    	z = values[2];

                    	mat4 translate = Transform::translate(x,y,z);
                    	/*for(int i=0; i<4; i++) {
                    		for(int j=0; j<4; j++) {
                    			cout << translate[i][j] << " ";
                    		}
                    		cout << endl;
                    	}
                    	cout << endl;*/

                    	rightmultiply((translate),transfstack);

                        // YOUR CODE FOR HW 2 HERE.
                        // Think about how the transformation stack is affected
                        // You might want to use helper functions on top of file.
                        // Also keep in mind what order your matrix is!

                    }
                }
                else if (cmd == "scale") {
                    validinput = readvals(s,3,values);
                    if (validinput) {

                    	float x, y, z;
                    	x = values[0];
                    	y = values[1];
                    	z = values[2];

                    	mat4 scale = Transform::scale(x,y,z);
                    	rightmultiply((scale),transfstack);

                        // YOUR CODE FOR HW 2 HERE.
                        // Think about how the transformation stack is affected
                        // You might want to use helper functions on top of file.
                        // Also keep in mind what order your matrix is!

                    }
                }
                else if (cmd == "rotate") {
                    validinput = readvals(s,4,values);
                    if (validinput) {

                    	float x, y, z, theta;
                    	x = values[0];
                    	y = values[1];
                    	z = values[2];
                    	theta = values[3];

                    	vec3 axis = glm::normalize(vec3(x,y,z));
                    	//mat3 rotate3(1.0);
                    	mat3 rotate3 = Transform::rotate(-theta,axis);
                    	mat4 rotate(1.0);
                    	for(int i=0; i<3; i++) {
                    		for(int j=0; j<3; j++) {
                    			rotate[i][j] = rotate3[j][i];
                    		}
                    	}
                    	rightmultiply((rotate),transfstack);

                        // YOUR CODE FOR HW 2 HERE.
                        // values[0..2] are the axis, values[3] is the angle.
                        // You may want to normalize the axis (or in Transform::rotate)
                        // See how the stack is affected, as above.
                        // Note that rotate returns a mat3.
                        // Also keep in mind what order your matrix is!

                    }
                }

                // I include the basic push/pop code for matrix stacks
                else if (cmd == "pushTransform") {
                    transfstack.push(transfstack.top());
                } else if (cmd == "popTransform") {
                    if (transfstack.size() <= 1) {
                        std::cerr << "Stack has no elements.  Cannot Pop\n";
                    } else {
                        transfstack.pop();
                    }
                }

                else {
                    //std::cerr << "Unknown Command: " << cmd << " Skipping \n";
                }
            }
            getline (in, str);
        }
    } else {
        std::cerr << "Unable to Open Input Data File " << filename << "\n";
        throw 2;
    }
    in.close();
}
