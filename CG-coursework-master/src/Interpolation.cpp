#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <CanvasPoint.h>
#include <Colour.h>

#include "Interpolation.hpp"

//--------------------------------------------Interpolation stuff--------------------------------------//

std::vector<float> interpolateSingleFloats(float first, float last, int num){
	assert(num >= 2);
	std::vector<float> result = {first};
	auto iv = result.end();
	float div = (last - first) / (float) (num - 1);
	//std::cout << "Div: " << div << "\n";
	for (int i = 1; i <= num - 2; i++){
		iv = result.end();
		float n = first + (div * i);
		//std::cout << n << "\n";
		result.insert(iv, n);
	}
	iv = result.end();
	result.insert(iv, last);
	
	//for(size_t i=0; i<result.size(); i++) std::cout << result[i] << " ";
	//std::cout << std::endl;
	return result;
	
}

std::vector<glm::vec2> interpolateDoubleElementValues(glm::vec2 first, glm::vec2 last, int num){
	assert(num >= 2);
	glm::vec2 elem = {first.x, first.y};
	std::vector<glm::vec2> result = {elem};
	auto iv = result.end();
	float divX = (last.x - first.x) / (float) (num - 1);
	float divY = (last.y - first.y) / (float) (num - 1);
	//std::cout << "DivX: " << divX << ", DivY: " << divY << "\n";
	for (int i = 1; i <= num - 2; i++){
		iv = result.end();
		elem = {first.x + (divX * i), first.y + (divY * i)};
		//std::cout << elem.x << ", " << elem.y << "\n";
		result.insert(iv, elem);
	}
	elem = {last.x, last.y};
	iv = result.end();
	result.insert(iv, elem);
	
	return result;
}

std::vector<glm::vec3> interpolateThreeElementValues(glm::vec3 first, glm::vec3 last, int num){
	std::vector<float> x = interpolateSingleFloats(first.x, last.x, num);
	std::vector<float> y = interpolateSingleFloats(first.y, last.y, num);
	std::vector<float> z = interpolateSingleFloats(first.z, last.z, num);
	
	std::vector<glm::vec3> result = {};
	auto iv = result.end();
	for (int i = 0; i < num; i++){
		iv = result.end();
		glm::vec3 elem = {x.at(i), y.at(i), z.at(i)};
		result.insert(iv, elem);
	}
	//iv = result.end();
	//result.insert(iv, last);
	return result;
}

std::vector<CanvasPoint> interpolateCanvasPoints(CanvasPoint first, CanvasPoint last, int num){
	CanvasPoint elem = {first.x, first.y, first.depth};
	std::vector<CanvasPoint> result = {elem};
	if(num >= 2){
		auto iv = result.end();
		float divX = (last.x - first.x) / (float) (num - 1);
		float divY = (last.y - first.y) / (float) (num - 1);
		float divDepth = (last.depth - first.depth) / (float) (num - 1);
		//std::cout << "DivX: " << divX << ", DivY: " << divY << "\n";
		for (int i = 1; i <= num - 2; i++){
			iv = result.end();
			elem = {first.x + (divX * i), first.y + (divY * i), first.depth + (divDepth * i)};
			//std::cout << elem.x << ", " << elem.y << "\n";
			result.insert(iv, elem);
		}
		elem = {last.x, last.y, last.depth};
		iv = result.end();
		result.insert(iv, elem);
	}
	
	return result;
}

std::vector<TexturePoint> interpolateTexturePoints(TexturePoint first, TexturePoint last, int num){
	TexturePoint elem = {first.x, first.y};
	std::vector<TexturePoint> result = {elem};
	if(num >= 2){
		auto iv = result.end();
		float divX = (last.x - first.x) / (float) (num - 1);
		float divY = (last.y - first.y) / (float) (num - 1);
		//std::cout << "DivX: " << divX << ", DivY: " << divY << "\n";
		for (int i = 1; i <= num - 2; i++){
			iv = result.end();
			elem = {first.x + (divX * i), first.y + (divY * i)};
			//std::cout << elem.x << ", " << elem.y << "\n";
			result.insert(iv, elem);
		}
		elem = {last.x, last.y};
		iv = result.end();
		result.insert(iv, elem);
	}
	
	return result;
}

//-----------------------------------------------------------------------------------------------------//
