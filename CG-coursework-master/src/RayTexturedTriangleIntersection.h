#include <RayTriangleIntersection.h>

#include <glm/glm.hpp>
#include <iostream>
#include "ObjLoader.hpp"

struct RayTexturedTriangleIntersection : RayTriangleIntersection {
	TexturedModelTriangle texturedTriangle;
	bool isTextured;
	
	RayTexturedTriangleIntersection() : RayTriangleIntersection(), texturedTriangle(intersectedTriangle){};
};
