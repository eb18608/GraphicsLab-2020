#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <CanvasPoint.h>
#include <Colour.h>
#include <CanvasTriangle.h>
#include <TextureMap.h>
#include <TexturePoint.h>
#include <math.h>
#include <ModelTriangle.h>
#include <limits>
#include <string>

#include "Interpolation.hpp"
#include "RGB.hpp"
#include "3dDrawer.hpp"

#define WIDTH 640	//original 320
#define HEIGHT 480 //original 240

using namespace glm;

double degreesToRadians(float deg){
	return (deg / 180) * M_PI;
}

void drawRedNoise(DrawingWindow &window) {
	window.clearPixels();
	for (size_t y = 0; y < window.height; y++) {
		for (size_t x = 0; x < window.width; x++) {
			float red = rand() % 256;
			float green = 0.0;
			float blue = 255.0;
			//uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
			uint32_t colour = colourToInt(255, red, green, blue);
			//std::cout << colour << " ";
			window.setPixelColour(x, y, colour);
		}
	}
}

void generateEmptyImage(DrawingWindow &window){
	for (size_t y = 0; y < window.height; y++){
		for (size_t x = 0; x < window.width; x++){
			window.setPixelColour(x, y, 0);
		}
	}
	//return image;
}



void rotate(float xDeg, float yDeg, float zDeg, mat3 &matrix){
	double xRad = degreesToRadians(xDeg);
	double yRad = degreesToRadians(yDeg);
	double zRad = degreesToRadians(zDeg);
	const double limit = 0.05;
	// x rotation
	if (xDeg >= limit || xDeg <= -limit){
		mat3 xMat = mat3(1.0);
		xMat[1][1] = cos(xRad);
		xMat[2][2] = cos(xRad);
		xMat[1][2] = sin(xRad);
		xMat[2][1] = -sin(xRad);
		matrix = xMat * matrix;
	}
	
	
	// y rotation
	if (yDeg >= limit || yDeg <= -limit){
		mat3 yMat = mat3(1.0);
		yMat[0][0] = cos(yRad);
		yMat[2][2] = cos(yRad);
		yMat[0][2] = -sin(yRad);
		yMat[2][0] = sin(yRad);
		matrix = yMat * matrix;
	}
	
	//z rotation
	if (zDeg >= limit || zDeg <= -limit){
		mat3 zMat = mat3(1.0);
		zMat[0][0] = cos(zRad);
		zMat[1][1] = cos(zRad);
		zMat[0][1] = sin(zRad);
		zMat[1][0] = -sin(zRad);
		matrix = zMat * matrix;
	}
}

void rotate(float xDeg, float yDeg, float zDeg, mat3 &matrix, bool t){
	double xRad = degreesToRadians(xDeg);
	double yRad = degreesToRadians(yDeg);
	double zRad = degreesToRadians(zDeg);
	const double limit = 0.05;
	// x rotation
	if (xDeg >= limit || xDeg <= -limit){
		mat3 xMat = mat3(1.0);
		xMat[1][1] = cos(xRad);
		xMat[2][2] = cos(xRad);
		xMat[1][2] = sin(xRad);
		xMat[2][1] = -sin(xRad);
		matrix = xMat * matrix;
	}
	
	
	// y rotation
	if (yDeg >= limit || yDeg <= -limit){
		mat3 yMat = mat3(1.0);
		yMat[0][0] = cos(yRad);
		yMat[2][2] = cos(yRad);
		yMat[0][2] = -sin(yRad);
		yMat[2][0] = sin(yRad);
		matrix = yMat * matrix;
	}
	
	//z rotation
	if (zDeg >= limit || zDeg <= -limit){
		mat3 zMat = mat3(1.0);
		zMat[0][0] = cos(zRad);
		zMat[1][1] = cos(zRad);
		zMat[0][1] = sin(zRad);
		zMat[1][0] = -sin(zRad);
		matrix = zMat * matrix;
	}
	//return matrix;
}

vec3 crossProduct(vec3 a, vec3 b){
	vec3 ans((a.y*b.z) - (a.z*b.y), (a.z*b.x) - (a.x*b.z), (a.x*b.y) - (a.y*b.x));
	return ans;
}

void lookAt(vec3 cameraPos, vec3 target, mat3 &cameraMatrix){
	vec3 vertical(0,1,0);
	vec3 forward = cameraPos - target;
	forward = forward / (std::sqrt(forward.x*forward.x + forward.y*forward.y + forward.z*forward.z));
	cameraMatrix[2] = forward;
	vec3 right = crossProduct(vertical, forward);
	cameraMatrix[0] = right;
	vec3 up = crossProduct(forward, right);
	cameraMatrix[1] = up;
}

void printMatrix(mat3 matrix){
	std::cout << matrix[0][0] << ", " << matrix[1][0] << ", " << matrix[2][0] << ",\n";
	std::cout << matrix[0][1] << ", " << matrix[1][1] << ", " << matrix[2][1] << ",\n";
	std::cout << matrix[0][2] << ", " << matrix[1][2] << ", " << matrix[2][2] << "\n";
}

void handleEvent(SDL_Event event, DrawingWindow &window, RenderingMode &mode, vec3 &cameraPos, mat3 &orientation) {
	const float speed = 0.1;
	vec3 movement(0, 0, 0);
	//vec3 lightLocation = light.getLocation();
	
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_a){
			movement.x -= speed;
		}
		else if (event.key.keysym.sym == SDLK_d){
			movement.x += speed;
		}
		else if (event.key.keysym.sym == SDLK_w){
			movement.z -= speed;
		}
		else if (event.key.keysym.sym == SDLK_s){
			movement.z += speed;
		}
		else if (event.key.keysym.sym == SDLK_q){
			rotate(0, 1.0, 0, orientation);
		}
		else if (event.key.keysym.sym == SDLK_e){
			rotate(0, -1.0, 0, orientation);
		}
		else if (event.key.keysym.sym == SDLK_1){
			mode = WIREFRAME;
		} else if (event.key.keysym.sym == SDLK_2){
			mode = RASTERISE;
		} else if (event.key.keysym.sym == SDLK_3){
			mode = RAYTRACING;
		}
		//light.setLocation(lightLocation);
		movement = orientation * movement;
		cameraPos = cameraPos + movement;
		std::cout << "Current position: (" << cameraPos.x << "," << cameraPos.y << "," << cameraPos.z << ")\n";
		std::cout << "Current orientation: \n\n";
		std::cout << orientation[0][0] << " " << orientation[1][0] << " " << orientation[2][0] << "\n";
		std::cout << orientation[0][1] << " " << orientation[1][1] << " " << orientation[2][1] << "\n";
		std::cout << orientation[0][2] << " " << orientation[1][2] << " " << orientation[2][2] << "\n\n";
	} else if (event.type == SDL_MOUSEBUTTONDOWN){
		window.savePPM("output.ppm");
	}
}

void update(DrawingWindow &window) {
	// Function for performing animation (shifting artifacts or moving the camera)
}



void drawImage(DrawingWindow &window, std::vector<std::vector<glm::vec3>> image){
	glm::vec3 pixel;
	for (size_t y = 0; y < window.height; y++){
		for(size_t x = 0; x < window.width; x++){
			pixel = image.at(y).at(x);
			uint32_t colour = colourToInt(255, pixel.x, pixel.y, pixel.z);
			window.setPixelColour(x, y, colour);
		}
	}
}

void drawImage(DrawingWindow &window, std::vector<std::vector<Colour>> image){
	Colour pixel;
	for (size_t y = 0; y < window.height; y++){
		for(size_t x = 0; x < window.width; x++){
			pixel = image.at(y).at(x);
			uint32_t colour = colourToInt(255, pixel.red, pixel.green, pixel.blue);
			window.setPixelColour(x, y, colour);
		}
	}
}

void orbit(vec3 &cameraPos, vec3 target, mat3 &orientation, float mag){
	vec3 direction(mag, 0, mag * -0.02);
	lookAt(cameraPos, target, orientation);
	cameraPos += orientation * direction;
	lookAt(cameraPos, target, orientation);
}

void rotZ(vec3 &cameraPos, mat3 &orientation, float mag){
	rotate(mag, 0, mag, orientation);
	//cameraPos = cameraPos * orientation;
}

int main(int argc, char *argv[]) {	
	
	//Line line({0, 0}, {WIDTH - 1, HEIGHT - 1}, {0xff,0xff,0xff});
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;

	//std::vector<std::vector<Colour>> image = generateEmptyImage(window);
	
	//TextureMap map("logo/texture.ppm");
	TextureMap map("texture.ppm");
	TextureTriangle tt(TexturePoint(195, 5), TexturePoint(395, 380), TexturePoint(65, 330));
	
	CanvasTriangle triangle(CanvasPoint(160, 10), CanvasPoint(300, 230), CanvasPoint(10, 150));
	
	CanvasTriangle triangle2(CanvasPoint(480, 10), CanvasPoint(340, 230), CanvasPoint(630, 150));
	
	CanvasTriangle triangle3(CanvasPoint(160, 460), CanvasPoint(300, 240), CanvasPoint(10, 320));

	CanvasTriangle triangle4(CanvasPoint(480, 460), CanvasPoint(340, 240), CanvasPoint(630, 320));
	
	
	vec3 column1(1.0, 0.0, 0.0);
	vec3 column2(0.0, 1.0, 0.0);
	vec3 column3(0.0, 0.0, 1.0);
	mat3 rotationMatrix(column1, column2, column3);
	//mat3 cameraMatrix(column1, column2, column3);
	mat3 orientation(column1, column2, column3);
	//rotationMatrix = rotate(0.0, 0.0, 0.0, rotationMatrix);
	//vec3 column1(0.0, 0.0, 0.0);
	//vec3 column2(0.0, 0.0, 0.0);
	//vec3 column3(0.0, 0.0, 0.0);
	//draw3dModel(cameraPos, "textured-cornell-box.obj", 2.0, window, rotationMatrix);
	std::cout << rotationMatrix[0][0] << " " << rotationMatrix[1][0] << " " << rotationMatrix[2][0] << "\n";
	std::cout << rotationMatrix[0][1] << " " << rotationMatrix[1][1] << " " << rotationMatrix[2][1] << "\n";
	std::cout << rotationMatrix[0][2] << " " << rotationMatrix[1][2] << " " << rotationMatrix[2][2] << "\n";
	
	vec3 cameraPos(-1.0, 0.0, 2.4);
	
	std::vector<std::vector<TexturedModelTriangle>> models = {};
	
	vec3 logoPos(0.0, -0.2, 0.2);
	std::vector<TexturedModelTriangle> logo = readObjFile(logoPos, "logo.obj", "logo/", 0.005f);
	models.insert(models.end(), logo);
	
	vec3 roomPos(0.1, 0.0, 0.0);
	std::vector<TexturedModelTriangle> box = readObjFile(roomPos, "textured-cornell-box.obj");
	models.insert(models.end(), box);
	
	//std::vector<TexturedModelTriangle> model = readObjFile("sphere.obj");
	
	vec3 lightLocation(-0.3, 0.3, 0.7);
	
	//vec3 lightLocation(0.0, 0.4, 0.5);
	Light light(lightLocation, 4.0, 0.3);
	//image = drawFilledTriangle(triangle, tt, map, image);
	
	float f = 2.0;
	
	float **depthBuffer = new float *[HEIGHT];
	vec3 **rays = new vec3 *[HEIGHT];
	for (int y = 0; y < window.height; y++){
		float *row = new float[WIDTH];
		depthBuffer[y] = row;
		vec3 *rayRow = new vec3[WIDTH];
		for (int x = 0; x < window.width; x++){
			vec3 ray((x - (window.width/2.0))/window.height, ((window.height/2.0) - y)/window.height, -f);
			rayRow[x] = ray;
		}
		rays[y] = rayRow;
	}
	RenderingMode mode = WIREFRAME;
	
	
	
	
	vec3 target(0.0, 0.0, 0.0);
	std::cout << "Drawing...\n";
	
	std::string outputDirectoryName = "video";
	std::string outputFileName;
	std::string numString;
	long frames = 0;
	while (frames <= 300) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)){
			handleEvent(event, window, mode, cameraPos, orientation);

		}
		generateEmptyImage(window);
		for (int y = 0; y < HEIGHT; y++){
			for (int x = 0; x < WIDTH; x++){
				depthBuffer[y][x] = -INFINITY;
			}
		}
		//orbit(cameraPos, target, orientation, 0.15);
		cameraPos.x += 0.02;
		lookAt(cameraPos, target, orientation);
		draw3dModel(cameraPos, light, models, f, window, orientation, depthBuffer, rays, mode);
		update(window);

		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
		
		if (frames < 10){
			numString = "00" + std::to_string(frames);
		} else if (frames < 100){
			
			numString = "0" + std::to_string(frames);
		} else {
			
			numString = std::to_string(frames);
		}
		

		if (frames % 100 == 0 && frames != 0){
			cameraPos.x = -1.0;
			if (mode == WIREFRAME){
				mode = RASTERISE;
			} else if (mode == RASTERISE){
				mode = RAYTRACING;
			}
		}
		
		std::cout << "Frame " << frames << " completed.\n";
		outputFileName = outputDirectoryName + "/output" + numString + ".ppm";
		window.savePPM(outputFileName);
		frames++;
	}
	for (int i = 0; i < HEIGHT; i++){
		delete depthBuffer[i];
		delete rays[i];
	}
	delete[] depthBuffer;
	delete[] rays;
}
