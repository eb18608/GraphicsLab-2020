#include "3dDrawer.hpp"
#include <cmath>
#include <chrono>

using namespace std::chrono;

CanvasPoint threeDToTwoD(glm::vec3 &cameraPos, glm::vec3 &vec, double f, DrawingWindow &image, glm::mat3 &matrix){
	glm::vec3 newVec = (vec - cameraPos) * matrix;
	double u = f * (-newVec.x * image.height / newVec.z) + (image.width/2.0);
	double v = f * (newVec.y * image.height / newVec.z) + (image.height/2.0);
	
	CanvasPoint answer(floor(u), floor(v), -1/newVec.z);
	return answer;
}

vec3 canvasPointToVec3(CanvasPoint point){
	vec3 answer(point.x, point.y, point.depth);
	return answer;
}

void getClosestIntersection(RayTexturedTriangleIntersection &closest, vec3 &source, vec3 &direction, std::vector<TexturedModelTriangle> &model, double f, glm::mat3 &matrix, float **depthBuffer, int x, int y){
	closest.distanceFromCamera = INFINITY;
	float t;
	float u;
	float v;
	vec3 e0, e1, SPVector, possibleSolution;
	for (int i = 0; i < model.size(); i++){
		if (model[i].averageDepth > depthBuffer[y][x]){
			ModelTriangle triangle = model[i].triangle;
			e0 = triangle.vertices[1] - triangle.vertices[0];
			e1 = triangle.vertices[2] - triangle.vertices[0];
			SPVector = source - triangle.vertices[0];
			mat3 DEMatrix(-direction, e0, e1);
			possibleSolution = inverse(DEMatrix) * SPVector;
			t = possibleSolution.x;
			u = possibleSolution.y;
			v = possibleSolution.z;
			if (t < closest.distanceFromCamera
				&& u >= 0.0 && u <= 1.0
				&& v >= 0.0 && v <= 1.0
				&& u + v <= 1.0
				&& t > 0){
				closest.intersectionPoint = triangle.vertices[0] + (u * e0) + (v * e1);
				closest.distanceFromCamera = t;
				closest.intersectedTriangle = triangle;
				closest.triangleIndex = i;
				if (model[i].isTextured()){
					closest.texturedTriangle = model[i];
					closest.isTextured = true;
				} else {
					closest.isTextured = false;
				}
				//depthBuffer[y][x] = model[i].averageDepth;
				depthBuffer[y][x] = closest.intersectionPoint.z;
				//std::cout << closest.distanceFromCamera << "\n";
				//std::cout << triangle.colour << "\n";
			}
		}
		
	}
}

void getClosestIntersection(RayTexturedTriangleIntersection &closest, vec3 &source, vec3 &direction, std::vector<TexturedModelTriangle> &model, double f, glm::mat3 &matrix){
	closest.distanceFromCamera = INFINITY;
	float t;
	float u;
	float v;
	vec3 e0, e1, SPVector, possibleSolution;
	for (int i = 0; i < model.size(); i++){
		ModelTriangle triangle = model[i].triangle;
		e0 = triangle.vertices[1] - triangle.vertices[0];
		e1 = triangle.vertices[2] - triangle.vertices[0];
		SPVector = source - triangle.vertices[0];
		mat3 DEMatrix(-direction, e0, e1);
		possibleSolution = inverse(DEMatrix) * SPVector;
		//float distance = length(possibleSolution - source);
		t = possibleSolution.x;
		u = possibleSolution.y;
		v = possibleSolution.z;
		//std::cout << "Values: " << t << ", " << u << ", " << v << "\n";
		if (t < closest.distanceFromCamera
			&& u >= 0.0 && u <= 1.0
			&& v >= 0.0 && v <= 1.0
			&& u + v <= 1.0
			&& t > 0){
			closest.intersectionPoint = triangle.vertices[0] + (u * e0) + (v * e1);
			closest.distanceFromCamera = t;
			closest.intersectedTriangle = triangle;
			if (model[i].isTextured()){
				closest.texturedTriangle = model[i];
				closest.isTextured = true;
			} else {
				closest.isTextured = false;
			}
			closest.triangleIndex = i;
			//std::cout << closest.distanceFromCamera << "\n";
			//std::cout << triangle.colour << "\n";
		}
	}
}

void getClosestIntersection(RayTexturedTriangleIntersection &closest, vec3 &source, vec3 &direction, std::vector<std::vector<TexturedModelTriangle>> &models, double f, glm::mat3 &matrix){
	closest.distanceFromCamera = INFINITY;
	float t;
	float u;
	float v;
	vec3 e0, e1, SPVector, possibleSolution;
	for (int i = 0; i < models.size(); i++){
		for (int j = 0; j < models[i].size(); j++){
			ModelTriangle triangle = models[i][j].triangle;
			e0 = triangle.vertices[1] - triangle.vertices[0];
			e1 = triangle.vertices[2] - triangle.vertices[0];
			SPVector = source - triangle.vertices[0];
			mat3 DEMatrix(-direction, e0, e1);
			possibleSolution = inverse(DEMatrix) * SPVector;
			//float distance = length(possibleSolution - source);
			t = possibleSolution.x;
			u = possibleSolution.y;
			v = possibleSolution.z;
			//std::cout << "Values: " << t << ", " << u << ", " << v << "\n";
			if (t < closest.distanceFromCamera
				&& u >= 0.0 && u <= 1.0
				&& v >= 0.0 && v <= 1.0
				&& u + v <= 1.0
				&& t > 0){
				closest.intersectionPoint = triangle.vertices[0] + (u * e0) + (v * e1);
				closest.distanceFromCamera = t;
				closest.intersectedTriangle = triangle;
				if (models[i][j].isTextured()){
					closest.texturedTriangle = models[i][j];
					closest.isTextured = true;
				} else {
					closest.isTextured = false;
				}
				closest.triangleIndex = i;
				//std::cout << closest.distanceFromCamera << "\n";
				//std::cout << triangle.colour << "\n";
			}
		}
	}
	
}

//----------------------------------------------------//

void fillFlat3dTriangle(CanvasTriangle &triangle, Colour colour, DrawingWindow &image, float **depthBuffer){

	Line line1(triangle.v1(), triangle.v0(), colour);
	Line line2(triangle.v1(), triangle.v2(), colour);
	Line flatLine(triangle.v2(), triangle.v0(), colour);
	int startY = floor(line1.pixels[0].y);
	int endY = floor(line1.pixels[line1.pixels.size() - 1].y);
	//std::cout << startY << ", " << endY << "\n";
	
	int i1 = 0;
	int i2 = 0;
	int i = startY;
	bool end = false;
	
	while (!end){
		//std::cout << "Y: " << i << ", ";
		if (startY <= endY) {
			while (floor(line1.pixels[i1].y) < i && i1 < floor(line1.pixels.size())){
				i1++;
			}
			while (floor(line2.pixels[i2].y) < i && i2 < floor(line2.pixels.size())){
				i2++;
			}
		} else {
			while (floor(line1.pixels[i1].y) > i && i1 < floor(line1.pixels.size())){
				i1++;
			}
			while (floor(line2.pixels[i2].y) > i && i2 < floor(line2.pixels.size())){
				i2++;
			}
		}
		
		//std::cout << i1 << ", " << i2 << "; ";
		if (i1 < floor(line1.pixels.size()) && i2 < floor(line2.pixels.size())){
			CanvasPoint p1(line1.pixels[i1].x, i, line1.pixels[i1].depth);
			CanvasPoint p2(line2.pixels[i2].x, i, line2.pixels[i2].depth);
			
			int num = line2.pixels[i2].x - line1.pixels[i1].x;
			if (num < 0) {
				num = -num;
			}
			if (num > 1){
				float diffX = p2.x - p1.x;
				float diffY = p2.y - p1.y;
				float numOfSteps = fmax(abs(diffX), abs(diffY));
				//std::cout << numOfSteps << "\n";
				std::vector<CanvasPoint> pixels = interpolateCanvasPoints(p1, p2, numOfSteps * 2);
				for (int i = 0; i < (int) pixels.size(); i++){
					CanvasPoint pixel = pixels[i];
					//std::cout << pixel.x << ", " << pixel.y << "\n";
					int yRound = floor(pixel.y);
					int xRound = floor(pixel.x);
					if (xRound >= 0 && xRound < image.width && yRound >= 0 && yRound < image.height && depthBuffer[yRound][xRound] < pixel.depth){
						try {
							//image[yRound][xRound] = colour;
							image.setPixelColour(xRound, yRound, colourToInt(colour));
							depthBuffer[yRound][xRound] = pixel.depth;
						} catch (int e){
						}
					}
				}
			} else if (num > 0) {
				int x = floor(p1.x);
				if (x >= 0 && x < image.width && i >= 0 && i < image.height && line1.pixels[i1].depth > depthBuffer[i][x]){
					//image[i][x] = colour;
					image.setPixelColour(x, i, colourToInt(colour));
				}
			}
			
			//std::cout << p1 << ", " << p2;
		}
		//std::cout << "\n";
		
		if (startY <= endY){
			if (i >= endY){
				end = true;
			} else {
				i++;
			}
		} else {
			if (i <= endY){
				end = true;
			} else {
				i--;
			}
		}
	}
	return;
	
	//image = flatLine.draw(image);
	//return image;
}

void draw3dTriangle(CanvasTriangle &triangle, Colour colour, DrawingWindow &image, float **depthBuffer, RenderingMode mode){
	//std::cout << "..." << "\n";
	// sort from top to bottom
	// insertion sort because I'm lazy and it's literally like 3 elements
	
	for (int i = 1; i < 3; i++){
		for (int j = i; j > 0; j--){
			//std::cout << j << "\n";
			if (triangle.vertices[j].y < triangle.vertices[j-1].y){
				CanvasPoint temp = triangle.vertices[j];
				triangle.vertices[j] = triangle.vertices[j-1];
				triangle.vertices[j-1] = temp;
			}
		}
	}
	//std::cout << "..." << "\n";
	// Draw lines
	std::vector<Line> lines = {};
	auto iv = lines.end();
	for (int i = 0; i < 3; i++){
		Line l(triangle.vertices[i], triangle.vertices[(i+1)%3], colour);
		lines.insert(iv, l);
		iv = lines.end();
	}
	
	//std::cout << "..." << "\n";
	
	// Find our new point
	CanvasPoint newPoint;
	for (int i = 0; i < floor(lines[2].pixels.size()); i++){
		if (floor(lines[2].pixels[i].y) == floor(triangle.vertices[1].y)){
			newPoint = lines[2].pixels[i];
			break;
		}
	}
	
	//std::cout << "..." << "\n";
	
	CanvasTriangle flat1(triangle.vertices[1], triangle.vertices[0], newPoint);
	CanvasTriangle flat2(triangle.vertices[1], triangle.vertices[2], newPoint);
	if (mode == RASTERISE){
		fillFlat3dTriangle(flat1, colour, image, depthBuffer);
		fillFlat3dTriangle(flat2, colour, image, depthBuffer);
	}
	Colour testBorder(0xff, 0xff, 0xff);
	//image = drawTriangle(flat1, testBorder, image);
	//image = drawTriangle(flat2, testBorder, image);
	drawTriangle(triangle, colour, image, depthBuffer);
	
	//std::cout << "..." << "\n";
	
	//return image;
}

//----------------------------------------------------//

bool isNear(vec3 point1, vec3 point2){
	return (length(point1 - point2) < 0.00001);
}

uint32_t getColourComponent(uint32_t component, double multiplier, float ambient, float specular){
	long newValue = (component * multiplier) + (component * specular);
	if (newValue > 0xff){
		return 0xff;
	} else if (newValue < component * ambient){
		return component * ambient;
	} else {
		return (uint32_t) newValue;
	}
}

uint32_t getColourComponent(uint32_t component, double multiplier, float specular){
	long newValue = (component * multiplier) + (component * specular);
	if (newValue > 0xff){
		return 0xff;
	} else if (newValue < 0){
		return 0;
	} else {
		return (uint32_t) newValue;
	}
}

double getAreaOfTriangle(vec3 v0, vec3 v1, vec3 v2){
	double result = abs(v0.x * (v1.y - v2.y) + v1.x * (v2.y - v0.y) + v2.x * (v0.y - v1.y)) / 2.0;
	if (result <= 0){
		return 0.0;
	} else {
		return result;
	}
}

void handleRayWithLightSoft(Light &light, RayTexturedTriangleIntersection &possibleSolution, double f, mat3 &matrix, vec3 &direction, std::vector<std::vector<TexturedModelTriangle>> &models, DrawingWindow &image, float x, float y){
	Colour colour;

	if (possibleSolution.isTextured){
		ModelTriangle t = possibleSolution.intersectedTriangle;
		//std::cout << "Triangle: (" << t.vertices[0].x << "," << t.vertices[0].y << "),  (" << t.vertices[1].x << "," << t.vertices[1].y << "), ";
		//std::cout << "(" << t.vertices[2].x << "," << t.vertices[2].y << ")\n";
		double area = getAreaOfTriangle(t.vertices[0], t.vertices[1], t.vertices[2]);
		if (area == 0.0){
			colour = possibleSolution.intersectedTriangle.colour;
		} else {
			double u = getAreaOfTriangle(t.vertices[2], t.vertices[0], possibleSolution.intersectionPoint) / area;
			double v = getAreaOfTriangle(t.vertices[0], t.vertices[1], possibleSolution.intersectionPoint) / area;
			double w = 1.0 - u - v;
			TexturePoint p(w * t.texturePoints[0].x + u * t.texturePoints[1].x + v * t.texturePoints[2].x,
						   w * t.texturePoints[0].y + u * t.texturePoints[1].y + v * t.texturePoints[2].y);
			//std::cout << "u: " << u << ", v: " << v << ", w: " << w << "\n";
			//std::cout << p << "\n";
			TextureMap map = possibleSolution.texturedTriangle.texture;
			if (floor(p.x) + (map.width * floor(p.y)) < map.pixels.size()){
				colour = intToColour(possibleSolution.texturedTriangle.texture.pixels[floor(p.x) + (map.width * floor(p.y))]);
			} else {
				colour = possibleSolution.intersectedTriangle.colour;
			}
		}
		
	} else {
		colour = possibleSolution.intersectedTriangle.colour;
	}
	
	int size = light.points.size();
	colour.red = colour.red / (float (size) / 4.0);
	colour.green = colour.green / (float (size) / 4.0);
	colour.blue = colour.blue / (float (size) / 4.0);
	uint32_t r = colour.red;	
	uint32_t g = colour.green;	
	uint32_t b = colour.blue;
	
	RayTexturedTriangleIntersection fromLight = {};
	float ambient = light.getAmbient();
	for (int i = 0; i < size; i++){
		vec3 lightLocation = light.points[i];
		vec3 lightDirection = normalize(possibleSolution.intersectionPoint - lightLocation);	// direction from LIGHT to POINT
		
		getClosestIntersection(fromLight, lightLocation, lightDirection, models, f, matrix);
		
		float distanceFromLight = length(possibleSolution.intersectionPoint - lightLocation);
		double multiplier;
		
		
		// for proximity lighting
		multiplier = (light.getIntensity()/(4.0 * M_PI * distanceFromLight * distanceFromLight));
		//std::cout << "First: " << multiplier << "\n";
		
		// for angle of incidence lighting
		ModelTriangle triangle = fromLight.intersectedTriangle;
		vec3 normal = cross(triangle.vertices[2] - triangle.vertices[1], triangle.vertices[0] - triangle.vertices[1]);
		float dotProduct = dot(-lightDirection, normal);
		float angleOfIncidence = std::acos(dotProduct / (length(lightDirection) * length(normal)));
		multiplier *= (((M_PI / 2) - angleOfIncidence) / (M_PI / 2));
		//multiplier *= dotProduct;
		
		// for specular lighting
		vec3 reflection = -lightDirection - (2.0f * normal * dotProduct);
		//float specular = pow(dot(direction, reflection), 256);
		
		float specular = 0;
		
		
		
		
		if (fromLight.distanceFromCamera > distanceFromLight
			|| isNear(possibleSolution.intersectionPoint, fromLight.intersectionPoint)){
			specular = pow(dot(direction, reflection), 256);
			colour.red = getColourComponent(colour.red + getColourComponent(r, multiplier, specular), 1, 0);
			colour.green = getColourComponent(colour.green + getColourComponent(g, multiplier, specular), 1, 0);
			colour.blue = getColourComponent(colour.blue + getColourComponent(b, multiplier, specular), 1, 0);
			
		}
		
	}
	colour.red = getColourComponent(colour.red, 1, ambient, 0);
	colour.green = getColourComponent(colour.green, 1, ambient, 0);
	colour.blue = getColourComponent(colour.blue, 1, ambient, 0);
	
	image.setPixelColour(x, y, colourToInt(colour));
}

void handleRayWithLight(Light &light, RayTexturedTriangleIntersection &possibleSolution, double f, mat3 &matrix, vec3 &direction, std::vector<std::vector<TexturedModelTriangle>> &models, DrawingWindow &image, float x, float y){
	Colour colour;

	if (possibleSolution.isTextured){
		ModelTriangle t = possibleSolution.intersectedTriangle;
		//std::cout << "Triangle: (" << t.vertices[0].x << "," << t.vertices[0].y << "),  (" << t.vertices[1].x << "," << t.vertices[1].y << "), ";
		//std::cout << "(" << t.vertices[2].x << "," << t.vertices[2].y << ")\n";
		double area = getAreaOfTriangle(t.vertices[0], t.vertices[1], t.vertices[2]);
		if (area == 0.0){
			colour = possibleSolution.intersectedTriangle.colour;
		} else {
			double u = getAreaOfTriangle(t.vertices[2], t.vertices[0], possibleSolution.intersectionPoint) / area;
			double v = getAreaOfTriangle(t.vertices[0], t.vertices[1], possibleSolution.intersectionPoint) / area;
			double w = 1.0 - u - v;
			TexturePoint p(w * t.texturePoints[0].x + u * t.texturePoints[1].x + v * t.texturePoints[2].x,
						   w * t.texturePoints[0].y + u * t.texturePoints[1].y + v * t.texturePoints[2].y);
			//std::cout << "u: " << u << ", v: " << v << ", w: " << w << "\n";
			//std::cout << p << "\n";
			TextureMap map = possibleSolution.texturedTriangle.texture;
			if (floor(p.x) + (map.width * floor(p.y)) < map.pixels.size()){
				colour = intToColour(possibleSolution.texturedTriangle.texture.pixels[floor(p.x) + (map.width * floor(p.y))]);
			} else {
				colour = possibleSolution.intersectedTriangle.colour;
			}
		}
		
	} else {
		colour = possibleSolution.intersectedTriangle.colour;
	}
	
	
	RayTexturedTriangleIntersection fromLight = {};
	float ambient = light.getAmbient();
	//for (int i = 0; i < light.points.size(); i++){
	vec3 lightLocation = light.getLocation();
	vec3 lightDirection = normalize(possibleSolution.intersectionPoint - lightLocation);	// direction from LIGHT to POINT
	
	getClosestIntersection(fromLight, lightLocation, lightDirection, models, f, matrix);
	
	float distanceFromLight = length(possibleSolution.intersectionPoint - lightLocation);
	double multiplier;
	
	
	// for proximity lighting
	multiplier = (light.getIntensity()/(4.0 * M_PI * distanceFromLight * distanceFromLight));
	//std::cout << "First: " << multiplier << "\n";
	
	// for angle of incidence lighting
	ModelTriangle triangle = fromLight.intersectedTriangle;
	vec3 normal = cross(triangle.vertices[2] - triangle.vertices[1], triangle.vertices[0] - triangle.vertices[1]);
	float dotProduct = dot(-lightDirection, normal);
	float angleOfIncidence = std::acos(dotProduct / (length(lightDirection) * length(normal)));
	multiplier *= (((M_PI / 2) - angleOfIncidence) / (M_PI / 2));
	//multiplier *= dotProduct;
	
	// for specular lighting
	vec3 reflection = -lightDirection - (2.0f * normal * dotProduct);
	//float specular = pow(dot(direction, reflection), 256);
	
	float specular = 0;
	
	
	
	
	if (fromLight.distanceFromCamera > distanceFromLight
		|| isNear(possibleSolution.intersectionPoint, fromLight.intersectionPoint)){
		specular = pow(dot(direction, reflection), 256);
		colour.red = getColourComponent(colour.red, multiplier, ambient, specular);
		colour.green = getColourComponent(colour.green, multiplier, ambient, specular);
		colour.blue = getColourComponent(colour.blue, multiplier, ambient, specular);
		
	} else {
		colour.red = getColourComponent(colour.red, ambient, ambient, 0);
		colour.green = getColourComponent(colour.green, ambient, ambient, 0);
		colour.blue = getColourComponent(colour.blue, ambient, ambient, 0);
	}
		
	//}
	
	
	image.setPixelColour(x, y, colourToInt(colour));
}

void draw3dModel(vec3 &cameraPos, Light &light, std::vector<std::vector<TexturedModelTriangle>> &models, double f, DrawingWindow &image, mat3 &matrix, float **depthBuffer, vec3 **rays, RenderingMode mode){
	
	int h = image.height;
	int w = image.width;
	
	glm::vec3 newPos = cameraPos;
	
	if (mode == RAYTRACING){
		RayTexturedTriangleIntersection possibleSolution = {};
		
		for (int y = 0; y < image.height; y++){
			for (int x = 0; x < image.width; x++){
				vec3 direction = normalize(rays[y][x] * inverse(matrix));
				getClosestIntersection(possibleSolution, cameraPos, direction, models, f, matrix);
				if (possibleSolution.distanceFromCamera != INFINITY){
					handleRayWithLightSoft(light, possibleSolution, f, matrix, direction, models, image, x, y);
				}
				
				
			}
			std::cout << "Finished row " << y << "\n";
		}
	} else {
		for (int i = 0; i < int(models.size()); i++){
			for (int j = 0; j < int(models[i].size()); j++){
			
				std::vector<CanvasPoint> newPoints = {threeDToTwoD(newPos, models[i][j].triangle.vertices[0], f, image, matrix),
													threeDToTwoD(newPos, models[i][j].triangle.vertices[1], f, image, matrix),
													threeDToTwoD(newPos, models[i][j].triangle.vertices[2], f, image, matrix)};
				CanvasTriangle triangle(newPoints[0], newPoints[1], newPoints[2]);
				
				
				if (models[i][j].isTextured() && mode == RASTERISE){
					TextureTriangle tt(models[i][j].triangle.texturePoints[0], models[i][j].triangle.texturePoints[1], models[i][j].triangle.texturePoints[2]);
					drawTextureTriangle(triangle, tt, models[i][j].texture, image, depthBuffer);
				} else {
					
					draw3dTriangle(triangle, models[i][j].triangle.colour, image, depthBuffer, mode);
				}
				
			}
		}
		
	}
}

