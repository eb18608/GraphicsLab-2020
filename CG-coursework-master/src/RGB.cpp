
#include "RGB.hpp"

uint32_t colourToInt(float opaque, float red, float green, float blue){
	return (int(opaque) << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
}

uint32_t colourToInt(Colour colour){
	return (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
}

Colour intToColour(uint32_t i){
	Colour colour(((i >> 16) & 0xff), ((i >> 8) & 0xff), (i&0xff));
	return colour;
}

void drawGradient(DrawingWindow &window, std::vector<float> gradient){
	window.clearPixels();
	if (gradient.size() != window.width){
		std::cout << "...\n";
		return;
	}
	for (size_t y = 0; y < window.height; y++){
		for (size_t x = 0; x < window.width; x++){
			float value = gradient.at(x);
			uint32_t colour = colourToInt(255, value, value, value);
			//std::cout << colour << " ";
			window.setPixelColour(x, y, colour);
		}
		//std::cout << "\n";
	}
}

void drawGradientRGB(DrawingWindow &window){
	glm::vec3 red(255, 0, 0);
	glm::vec3 yellow(255, 255, 0);
	glm::vec3 green(0, 255, 0);
	glm::vec3 blue(0, 0, 255);
	
	std::vector<glm::vec3> firstColumn = interpolateThreeElementValues(red, yellow, window.height);
	std::vector<glm::vec3> lastColumn = interpolateThreeElementValues(blue, green, window.height);
	
	std::vector<glm::vec3> elem;
	
	for (size_t y = 0; y < window.height; y++){
		elem = interpolateThreeElementValues(firstColumn.at(y), lastColumn.at(y), window.width);
		for (size_t x = 0; x < window.width; x++){
			glm::vec3 c = elem.at(x);
			uint32_t colour = colourToInt(255, c.x, c.y, c.z);
			window.setPixelColour(x, y, colour);
		} 
	}
}

std::vector<std::vector<glm::vec3>> generateGradientRGB(DrawingWindow &window){
	glm::vec3 red(255, 0, 0);
	glm::vec3 yellow(255, 255, 0);
	glm::vec3 green(0, 255, 0);
	glm::vec3 blue(0, 0, 255);
	
	std::vector<glm::vec3> firstColumn = interpolateThreeElementValues(red, yellow, window.height);
	std::vector<glm::vec3> lastColumn = interpolateThreeElementValues(blue, green, window.height);
	
	std::vector<std::vector<glm::vec3>> matrix = {};
	auto i = matrix.end();
	for (size_t y = 0; y < window.height; y++){
		matrix.insert(i, interpolateThreeElementValues(firstColumn.at(y), lastColumn.at(y), window.width));
		i = matrix.end();
	}
	return matrix;
}

