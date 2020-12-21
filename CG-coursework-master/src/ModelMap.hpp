#include <vector>
#include <Colour.h>
#include <unordered_map>
#include <TextureMap.h>

class ModelMap{
	std::unordered_map<std::string, Colour> colourMap;
	std::unordered_map<std::string, TextureMap> textureMap;
	
	public:
		inline ModelMap(){
			colourMap = {};
			textureMap = {};
		}
		
		inline void insert(std::string name, Colour colour){
			colourMap.insert({name, colour});
		}
		
		inline void insert(std::string name, TextureMap texture){
			textureMap.insert({name, texture});
		}
		
		inline Colour getColour(std::string name){
			return colourMap[name];
		}
		
		inline TextureMap getTexture(std::string name){
			return textureMap[name];
		}
		
		inline bool hasTexture(std::string name){
			if (textureMap.find(name) == textureMap.end()){
				return false;
			} else {
				return true;
			}
		}
		
		inline bool hasOneTexture(){
			return textureMap.size() == 1;
		}
		
		inline TextureMap getTexture(){
			return textureMap.begin()->second;
		}
};
