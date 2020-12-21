#include "ObjLoader.hpp"
#include "ModelMap.hpp"
#include <fstream>
#include <vector>
#include <Utils.h>
#include <Colour.h>
#include <unordered_map>
#include <TexturePoint.h>


ModelMap readMtlFile(std::string file, std::string directory = ""){
	//std::unordered_map<std::string, Colour> map = {};
	ModelMap map;
	
	std::ifstream infile(directory + file);
	std::string line;
	
	std::string name;
	
	//enum State {recording, reading};
	//State state = reading;
	std::cout << "Reading: " << file << " in " << directory << "\n";
	while(std::getline(infile, line)){
		//std::cout << line << "\n";
		std::vector<std::string> lineVector = split(line, ' ');
		if (lineVector[0] == "newmtl"){
			//state = recording;
			name = lineVector[1];
		} else if (lineVector[0] == "Kd"){
			Colour colour(stof(lineVector[1]) * 255.0f, stof(lineVector[2]) * 255.0f, stof(lineVector[3]) * 255.0f);
			map.insert(name, colour);
			//state = reading;
		} else if (lineVector[0] == "map_Kd"){
			TextureMap texture(directory + lineVector[1]);
			std::cout << directory + lineVector[1] << "\n";
			map.insert(name, texture);
		}
	}
	return map;
}

/*
std::vector<TexturedModelTriangle> readObjFile(std::string file){
	std::vector<glm::vec3> vectors = {};
	std::vector<TexturePoint> textureVectors = {};
	std::vector<TexturedModelTriangle> triangles = {};
	
	float scalingFactor = 0.17f;

	std::ifstream infile(file);
	std::string line;
	
	ModelMap map;
	
	Colour *colour;
	TextureMap texture;
	
	while (std::getline(infile, line)){
		// process line data
		std::vector<std::string> lineVector = split(line, ' ');
		if (lineVector[0] == "v"){
			glm::vec3 v = {stof(lineVector[1]), stof(lineVector[2]), stof(lineVector[3])};
			v = v * scalingFactor;
			vectors.insert(vectors.end(), v);
		} else if (lineVector[0] == "f"){
			std::array<glm::vec3, 3> vs{};
			std::vector<TexturePoint> ts = {};
			for (int i = 0; i < 3; i++){
				std::vector<std::string> data = split(lineVector[i+1], '/');
				vs[i] = vectors[stoi(data[0]) - 1];
				if (data.size() == 2){
					try {
						ts.insert(ts.end(), textureVectors[stoi(data[1]) - 1]);
						//std::cout << textureVectors[stoi(data[1]) - 1] << "\n";
					} catch (const std::invalid_argument& e){
						//do nothing
					}
				}
			}
			ModelTriangle tri(vs[0], vs[1], vs[2], *colour);
			TexturedModelTriangle t(tri);
			if (ts.size() == 3){
				//std::cout << "mkay\n";
				t.setTexture(texture);
				for (int i = 0; i < 3; i++){
					t.triangle.texturePoints[i] = ts[i];
					//std::cout << t.triangle.texturePoints[i] << "\n";
				}
			}
			triangles.insert(triangles.end(), t);
		} else if (lineVector[0] == "mtllib"){
			map = readMtlFile(lineVector[1]);
		} else if (lineVector[0] == "usemtl"){
			Colour c = map.getColour(lineVector[1]);
			colour = &c;
			if (map.hasTexture(lineVector[1])){
				texture = map.getTexture(lineVector[1]);
			}
		} else if (lineVector[0] == "vt"){
			TexturePoint v = {floor(stof(lineVector[1]) * texture.width), floor(stof(lineVector[2]) * texture.height)};
			//v = v * scalingFactor;
			//std::cout << v.x << ", " << v.y << "\n";
			textureVectors.insert(textureVectors.end(), v);
		}
	}

	return triangles;
}*/

std::vector<TexturedModelTriangle> readObjFile(glm::vec3 &position, std::string file, std::string directory, float scale){
	std::vector<glm::vec3> vectors = {};
	std::vector<TexturePoint> textureVectors = {};
	std::vector<TexturedModelTriangle> triangles = {};
	
	float scalingFactor = 0.17f * scale;

	std::ifstream infile(directory + file);
	std::string line;
	
	ModelMap map;
	
	Colour defaultColour(0xaa, 0xaa, 0xaa);
	Colour *colour = &defaultColour;
	TextureMap texture;
	
	bool hasOneTexture = true;
	
	while (std::getline(infile, line)){
		// process line data
		//std::cout << line << "\n";
		std::vector<std::string> lineVector = split(line, ' ');
		if (lineVector[0] == "v"){
			glm::vec3 v = {stof(lineVector[1]), stof(lineVector[2]), stof(lineVector[3])};
			v = (v * scalingFactor) + position;
			vectors.insert(vectors.end(), v);
			std::cout << v.x << ", " << v.y << ", " << v.z << "\n";
		} else if (lineVector[0] == "f"){
			std::array<glm::vec3, 3> vs{};
			std::vector<TexturePoint> ts = {};
			for (int i = 0; i < 3; i++){
				//std::cout << lineVector[i+1] << "\n";
				std::vector<std::string> data = split(lineVector[i+1], '/');
				for (int i = 0; i < data.size(); i++){
					//std::cout << data[i] << "\n";
				}
				vs[i] = vectors[stoi(data[0]) - 1];
				if (data.size() == 2){
					try {
						ts.insert(ts.end(), textureVectors[stoi(data[1]) - 1]);
						//std::cout << textureVectors[stoi(data[1]) - 1] << "\n";
					} catch (const std::invalid_argument& e){
						//do nothing
					}
				}
			}
			ModelTriangle tri(vs[0], vs[1], vs[2], *colour);
			TexturedModelTriangle t(tri);
			if (ts.size() == 3){
				//std::cout << "mkay\n";
				t.setTexture(texture);
				for (int i = 0; i < 3; i++){
					t.triangle.texturePoints[i] = ts[i];
					//std::cout << t.triangle.texturePoints[i] << "\n";
				}
			}
			triangles.insert(triangles.end(), t);
			//std::cout << triangles[triangles.size() - 1].triangle.texturePoints[0] << "\n";
		} else if (lineVector[0] == "mtllib"){
			map = readMtlFile(lineVector[1], directory);
		} else if (lineVector[0] == "usemtl"){
			Colour c = map.getColour(lineVector[1]);
			colour = &c;
			hasOneTexture = false;
			if (map.hasTexture(lineVector[1])){
				texture = map.getTexture(lineVector[1]);
			}
		} else if (lineVector[0] == "vt"){
			TexturePoint v = {stof(lineVector[1]), stof(lineVector[2])};
			//v = v * scalingFactor;
			//std::cout << v.x << ", " << v.y << "\n";
			textureVectors.insert(textureVectors.end(), v);
		}
	}
	
	if (hasOneTexture && map.hasOneTexture()){
		std::cout << "This has only 1 texture" << "\n";
		texture = map.getTexture();
		for (int i = 0; i < triangles.size(); i++){
			triangles[i].setTexture(texture);
			//std::cout << triangles[i].triangle.texturePoints[i] << "\n";
		}
	}
	for (int i = 0; i < triangles.size(); i++){
		if (triangles[i].isTextured()){
			texture = triangles[i].texture;
			//std::cout << texture << "\n";
			for (int j = 0; j < 3; j++){
				std::cout << triangles[i].triangle.texturePoints[j] << "\n";
				triangles[i].triangle.texturePoints[j].x = floor(triangles[i].triangle.texturePoints[j].x * texture.width);
				triangles[i].triangle.texturePoints[j].y = floor(triangles[i].triangle.texturePoints[j].y * texture.height);
				//std::cout << texture.height << "\n";
				//std::cout << triangles[i].triangle.texturePoints[j] << "\n";
			}
		}
		
		//std::cout << triangles[i].triangle.texturePoints << "\n";
	}

	return triangles;
}

