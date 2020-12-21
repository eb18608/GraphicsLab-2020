#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <CanvasPoint.h>
#include <Colour.h>

#include <TexturePoint.h>
#include <math.h>
#include <ModelTriangle.h>
#include <DrawingWindow.h>

#include "Line.hpp"
#include "RGB.hpp"
#include "Interpolation.hpp"
#include "TextureTriangle.hpp"
#include <TextureMap.h>

void drawTriangle(CanvasTriangle triangle, Colour colour, DrawingWindow &image, float **depthbuffer);

void drawTextureTriangle(CanvasTriangle triangle, TextureTriangle tt, TextureMap map, DrawingWindow &image, float **depthBuffer);
