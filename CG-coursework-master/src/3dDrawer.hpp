#include <CanvasPoint.h>
#include <glm/glm.hpp>
#include <DrawingWindow.h>
#include "RGB.hpp"
//#include "ObjLoader.hpp"
#include "Interpolation.hpp"
#include <math.h>
//#include "Line.hpp"
#include "TriangleDrawer.hpp"
#include <CanvasPoint.h>
#include <CanvasTriangle.h>
#include "RayTexturedTriangleIntersection.h"
#include <limits>

using namespace glm;

enum RenderingMode {
	WIREFRAME,
	RASTERISE,
	RAYTRACING
};

class Light {
	vec3 location;
	float intensity;
	float ambient;
	const float scatter = 0.075;
	
	
	public:
		std::vector<vec3> points;
		inline Light(vec3 location, float intensity){
			this->location = location;
			this->intensity = intensity;
			this->points = {};
			for (int x = -30; x < 31; x++){
				for (int y = -30; y < 31; y++){
					for (int z = -30; z < 31; z++){
						vec3 point(location.x + (x*scatter), location.y + (y*scatter), location.z + (z*scatter));
						points.insert(points.end(), point);
					}
				}
			}
		}
		
		inline Light(vec3 location, float intensity, float ambient){
			this->location = location;
			this->intensity = intensity;
			this->ambient = ambient;
			this->points = {};
			for (int x = -1; x < 2; x++){
				for (int y = -1; y < 2; y++){
					for (int z = -1; z < 2; z++){
						vec3 point(location.x + (x*scatter), location.y + (y*scatter), location.z + (z*scatter));
						points.insert(points.end(), point);
					}
				}
			}
		}
		
		inline void setLocation(vec3 location){
			this->location = location;
		}
		
		inline vec3 getLocation(){
			return this->location;
		}
		
		inline void setIntensity(float intensity){
			this->intensity = intensity;
		}
		
		inline float getIntensity(){
			return this->intensity;
		}
		
		inline void setAmbient(float ambient){
			this->ambient = ambient;
		}
		
		inline float getAmbient(){
			return this->ambient;
		}
};

void draw3dModel(glm::vec3 &cameraPos, Light &light, std::vector<std::vector<TexturedModelTriangle>> &models, double f, DrawingWindow &image, glm::mat3 &matrix, float **depthBuffer, vec3 **rays, RenderingMode mode);
