#include <ModelTriangle.h>
#include <TextureMap.h>
#include <glm/glm.hpp>

//#include "ModelMap.hpp"

class TexturedModelTriangle{
	
	bool textured;
	
	public:
		ModelTriangle triangle;
		TextureMap texture;
		float averageDepth;
		inline TexturedModelTriangle(ModelTriangle triangle){
			this->triangle = triangle;
			textured = false;
		}
		
		inline TexturedModelTriangle(ModelTriangle triangle, TextureMap texture){
			this->triangle = triangle;
			this->texture = texture;
			textured = true;
		}
		
		inline bool isTextured(){
			return textured;
		}
		
		inline void setTexture(TextureMap texture){
			this->texture = texture;
			textured = true;
		}
		
		inline void setIsTextured(bool textured){
			this->textured = textured;
		}
		
};

//std::vector<TexturedModelTriangle> readObjFile(std::string file);

std::vector<TexturedModelTriangle> readObjFile(glm::vec3 &position, std::string file, std::string directory = "", float scale = 1.0);
