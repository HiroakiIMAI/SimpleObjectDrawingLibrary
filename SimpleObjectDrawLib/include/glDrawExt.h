#pragma once

#include <Eigen\Core>
#include <Eigen\Geometry>
#include "GLTypedef.h"

namespace SmplObjDrwLib {
	void drawCoordSymbol(const unsigned char* name, float size);
	void drawBox(Eigen::Vector3f origin, Eigen::Vector3f length);

	void cvtMatrix_Eigen2GL(const Eigen::Matrix4f &mtx, GLfloat m[16]);

};
