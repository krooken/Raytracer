/*
 * ReadFile.h
 *
 *  Created on: 15 dec 2012
 *      Author: jonas
 */

#ifndef READFILE_H_
#define READFILE_H_

#include <list>
#include <string>
#include "Camera.h"
#include "GraphicObject.h"
#include "Light.h"

void readfile(const char* filename,
		std::list <GraphicObject> & objects,
		std::list <Light *> & lights,
		Camera & camera,
		int & width, int & height,
		std::string & outputFile,
		int & maxdepth);


#endif /* READFILE_H_ */
