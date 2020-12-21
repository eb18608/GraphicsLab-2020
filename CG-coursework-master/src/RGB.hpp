#ifndef _RGB_HPP_
#define _RGB_HPP_

#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <CanvasPoint.h>
#include <Colour.h>


#include "Interpolation.hpp"

uint32_t colourToInt(float opaque, float red, float green, float blue);

uint32_t colourToInt(Colour colour);

Colour intToColour(uint32_t i);

void drawGradient(DrawingWindow &window, std::vector<float> gradient);

void drawGradientRGB(DrawingWindow &window);

std::vector<std::vector<glm::vec3>> generateGradientRGB(DrawingWindow &window);

#endif
