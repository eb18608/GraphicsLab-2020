//#include "3dDrawer.hpp"
#include "Interpolation.hpp"
#include <math.h>
#include <DrawingWindow.h>
#include "RGB.hpp"

class Line{
	public:
		std::vector<CanvasPoint> pixels;
		Colour colour;
		inline Line(CanvasPoint start, CanvasPoint end, Colour colour){
			this->colour = colour;
			this->pixels = {};
			float diffX = end.x - start.x;
			float diffY = end.y - start.y;
			float numOfSteps = fmax(abs(diffX), abs(diffY));
			//std::cout << numOfSteps << "\n";
			pixels = interpolateCanvasPoints(start, end, numOfSteps * 2);
		}
		
		inline std::vector<std::vector<Colour>> draw(std::vector<std::vector<Colour>> image){
			for (int i = 0; i < (int) pixels.size(); i++){
				CanvasPoint pixel = pixels.at(i);
				//std::cout << pixel.x << ", " << pixel.y << "\n";
				try {
					image[floor(pixel.y)][floor(pixel.x)] = colour;
				} catch (int e){
				}
			}
			return image;
		}
		
		inline void draw(DrawingWindow &image, float **depthBuffer){
			uint32_t c = colourToInt(colour);
			for (int i = 0; i < (int) pixels.size(); i++){
				CanvasPoint pixel = pixels[i];
				//std::cout << pixel.x << ", " << pixel.y << "\n";
				int yRound = floor(pixel.y);
				int xRound = floor(pixel.x);
				if (xRound >= 0 && xRound < image.width && yRound >= 0 && yRound < image.height && depthBuffer[yRound][xRound] < pixel.depth){
					try {
						image.setPixelColour(xRound, yRound, c);
						depthBuffer[yRound][xRound] = pixel.depth;
					} catch (int e){
					}
				}
			}
			//return image;
		}
		
		inline static void drawNewLine(CanvasPoint start, CanvasPoint end, Colour colour, DrawingWindow &image, float **depthBuffer){
			float diffX = end.x - start.x;
			float diffY = end.y - start.y;
			float numOfSteps = fmax(abs(diffX), abs(diffY));
			//std::cout << numOfSteps << "\n";
			std::vector<CanvasPoint> pixels = interpolateCanvasPoints(start, end, numOfSteps * 2);
			uint32_t c = colourToInt(colour);
			for (int i = 0; i < (int) pixels.size(); i++){
				CanvasPoint pixel = pixels[i];
				//std::cout << pixel.x << ", " << pixel.y << "\n";
				int yRound = floor(pixel.y);
				int xRound = floor(pixel.x);
				if (xRound >= 0 && xRound < image.width && yRound >= 0 && yRound < image.height && depthBuffer[yRound][xRound] < pixel.depth){
					try {
						image.setPixelColour(xRound, yRound, c);
						depthBuffer[yRound][xRound] = pixel.depth;
					} catch (int e){
					}
				}
			}
			//return image;
		}
};
