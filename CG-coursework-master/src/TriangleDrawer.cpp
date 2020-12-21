#include "TriangleDrawer.hpp"

double getAreaOfTriangle(CanvasPoint v0, CanvasPoint v1, CanvasPoint v2){
	double result = abs(v0.x * (v1.y - v2.y) + v1.x * (v2.y - v0.y) + v2.x * (v0.y - v1.y)) / 2.0;
	if (result <= 0){
		return 0.0;
	} else {
		return result;
	}
}

void drawTriangle(CanvasTriangle triangle, Colour colour, DrawingWindow &image, float **depthBuffer){
	Line line1(triangle.v0(), triangle.v1(), colour);
	Line line2(triangle.v1(), triangle.v2(), colour);
	Line line3(triangle.v2(), triangle.v0(), colour);
	line1.draw(image, depthBuffer);
	line2.draw(image, depthBuffer);
	line3.draw(image, depthBuffer);
}

void getTextureColour(Colour &colour, CanvasTriangle &triangle, TextureTriangle &tt, TextureMap &map, double area, float x, float y){
	if (area == 0.0){
		colour = Colour(0xaa, 0xaa, 0xaa);
		//std::cout << "hlp\n";
	} else {
		double u = getAreaOfTriangle(triangle.vertices[2], triangle.vertices[0], CanvasPoint(x, y)) / area;
		double v = getAreaOfTriangle(triangle.vertices[0], triangle.vertices[1], CanvasPoint(x, y)) / area;
		double w = 1.0 - u - v;
		TexturePoint p(w * tt.vertices[0].x + u * tt.vertices[1].x + v * tt.vertices[2].x,
					   w * tt.vertices[0].y + u * tt.vertices[1].y + v * tt.vertices[2].y);
					   
		//TextureMap map = possibleSolution.texturedTriangle.texture;
		if (floor(p.x) + (map.width * floor(p.y)) < map.pixels.size()){
			colour = intToColour(map.pixels[floor(p.x) + (map.width * floor(p.y))]);
		} else {
			colour = Colour(0xaa, 0xaa, 0xaa);
		}
		
	}
}

void fillFlatTextureTriangle(CanvasTriangle &triangle, TextureTriangle &tt, TextureMap &map, DrawingWindow &image, float **depthBuffer){

	Colour colour(0xff, 0xff, 0xff);
	Line line1(triangle.v1(), triangle.v0(), colour);
	Line line2(triangle.v1(), triangle.v2(), colour);
	Line flatLine(triangle.v2(), triangle.v0(), colour);
	
	std::vector<TexturePoint> tLine1 = interpolateTexturePoints(tt.vertices[1], tt.vertices[0], line1.pixels.size());
	std::vector<TexturePoint> tLine2 = interpolateTexturePoints(tt.vertices[1], tt.vertices[2], line2.pixels.size());
	std::vector<TexturePoint> tFlatLine = interpolateTexturePoints(tt.vertices[2], tt.vertices[0], flatLine.pixels.size());
	
	//std::cout << triangle.v0() << ", " << tt.vertices[0] << "\n";
	//std::cout << triangle.v1() << ", " << tt.vertices[1] << "\n";
	//std::cout << triangle.v2() << ", " << tt.vertices[2] << "\n";
	
	int startY = line1.pixels[0].y;
	int endY = line1.pixels[line1.pixels.size() - 1].y;
	//std::cout << startY << ", " << endY << "\n";
	
	int i1 = 0;
	int i2 = 0;
	int i = startY;
	bool end = false;
	
	std::vector<TexturePoint> current;
	
	while (!end){
		//std::cout << "Y: " << i << ", ";
		if (startY <= endY) {
			while (line1.pixels[i1].y < i && i1 < (int)line1.pixels.size()){
				i1++;
			}
			while (line2.pixels[i2].y < i && i2 < (int)line2.pixels.size()){
				i2++;
			}
		} else {
			while (line1.pixels[i1].y > i && i1 < (int)line1.pixels.size()){
				i1++;
			}
			while (line2.pixels[i2].y > i && i2 < (int)line2.pixels.size()){
				i2++;
			}
		}
		
		//std::cout << i1 << ", " << i2 << "; ";
		CanvasPoint p1(line1.pixels[i1].x, i);
		CanvasPoint p2(line2.pixels[i2].x, i);
		//current = interpolateTexturePoints(tLine1[i1], tLine2[i2], abs(p2.x - p1.x + 1));
		
		Colour colour;
		
		//std::cout << "Triangle: (" << t.vertices[0].x << "," << t.vertices[0].y << "),  (" << t.vertices[1].x << "," << t.vertices[1].y << "), ";
		//std::cout << "(" << t.vertices[2].x << "," << t.vertices[2].y << ")\n";
		double area = getAreaOfTriangle(triangle.vertices[0], triangle.vertices[1], triangle.vertices[2]);
		
		
		int num = p1.x - p2.x;
		if (num < 0){
			num = -num;
		}
		if (num > 1){
			std::vector<float> depths = interpolateSingleFloats(line1.pixels[i1].depth, line2.pixels[i2].depth, num);
			//std::cout << current[0].y << "\n";
			for (int x = floor(p1.x); x < floor(p2.x); x++){
				getTextureColour(colour, triangle, tt, map, area, x, i);
				int u = floor(x-p1.x);
				//std::cout << u << "\n";
				if (u >= 0){
					
					if (i >= 0 && i < (int)image.height && x >= 0 && x < (int)image.width && depthBuffer[i][x] <= depths[u]){
						//image.setPixelColour(x, i, map.pixels[index]);
						
						image.setPixelColour(x, i, colourToInt(colour));
						depthBuffer[i][x] = depths[u];
					}
				}
				
			}
			for (int x = floor(p2.x); x < floor(p1.x); x++){
				getTextureColour(colour, triangle, tt, map, area, x, i);
				int u = floor(x-p2.x);
				if (u >= 0){
					
					if (i >= 0 && i < (int)image.height && x >= 0 && x < (int)image.width && depthBuffer[i][x] <= depths[u]){
						//image.setPixelColour(x, i, map.pixels[index]);
						
						image.setPixelColour(x, i, colourToInt(colour));
						depthBuffer[i][x] = depths[u];
					}
					
				}
			}
		} else if (num > 0) {
			int x = floor(p1.x);
			if (x >= 0 && x < image.width && i >= 0 && i < image.height && line1.pixels[i1].depth > depthBuffer[i][x]){
				//image[i][x] = colour;
				image.setPixelColour(x, i, colourToInt(colour));
				depthBuffer[i][x] = line1.pixels[i1].depth;
			}
		}
		
		//std::cout << p1 << ", " << p2 << "\n";
		
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
	
	//image = flatLine.draw(image);
	//return image;
}

void drawTextureTriangle(CanvasTriangle triangle, TextureTriangle tt, TextureMap map, DrawingWindow &image, float **depthBuffer){
//std::cout << "..." << "\n";
	// sort from top to bottom
	// insertion sort because I'm lazy and it's literally like 3 elements
	
	for (int i = 1; i < 3; i++){
		for (int j = i; j > 0; j--){
			//std::cout << j << "\n";
			if (triangle.vertices[j].y < triangle.vertices[j-1].y){
				std::swap(triangle.vertices[j], triangle.vertices[j-1]);
				std::swap(tt.vertices[j], tt.vertices[j-1]);
			}
		}
	}
	//std::cout << "..." << "\n";
	// Draw border
	std::vector<Line> lines = {};
	std::vector<std::vector<TexturePoint>> textureLines = {};
	Colour white(0xff, 0xff, 0xff);
	auto iv = lines.end();
	auto jv = textureLines.end();
	for (int i = 0; i < 3; i++){
		Line l(triangle.vertices[i], triangle.vertices[(i+1)%3], white);
		lines.insert(iv, l);
		
		std::vector<TexturePoint> tLine = interpolateTexturePoints(tt.vertices[i], tt.vertices[(i+1)%3], l.pixels.size());
		textureLines.insert(jv, tLine);
		iv = lines.end();
		jv = textureLines.end();
	}
	
	/*
	std::cout << "interpolation: " << "\n{";
	for (int i = 0; i < (int)textureLines[2].size(); i++){
		std::cout << textureLines[2][i].x << ", ";
	}
	std::cout << "}\n";
	*/
	
	// Find our new point
	CanvasPoint newPoint(-1, triangle.vertices[1].y);
	/*CanvasPoint newPoint(-1, 0);
	if (triangle.vertices[0].x > triangle.vertices[1].x){
		newPoint.y = triangle.vertices[0].y;
	} else {
		newPoint.y = triangle.vertices[1].y;
	}*/
	TexturePoint newTPoint;
	for (int i = 0; i < (int)lines[2].pixels.size(); i++){
		if ((int) lines[2].pixels[i].y == (int) newPoint.y){
			newPoint.x = lines[2].pixels[i].x;
			//newTPoint.x = textureLines[2][i].x;
			//newTPoint.y = textureLines[2][i].y;
			newTPoint = textureLines[2][i];
			break;
		}
	}
	
	//std::cout << "New point: " << newPoint << ", " << newTPoint << "\n";
	
	if (triangle.vertices[0].x >= triangle.vertices[1].x) {
		CanvasTriangle flat1(triangle.vertices[1], triangle.vertices[0], newPoint);
		TextureTriangle t1(tt.vertices[1], tt.vertices[0], newTPoint);
		fillFlatTextureTriangle(flat1, t1, map, image, depthBuffer);
	} else {
		CanvasTriangle flat1(newPoint, triangle.vertices[0], triangle.vertices[1]);
		TextureTriangle t1(newTPoint, tt.vertices[0], tt.vertices[1]);
		fillFlatTextureTriangle(flat1, t1, map, image, depthBuffer);
	}
	
	
	if (triangle.vertices[2].x >= triangle.vertices[1].x){
	
		CanvasTriangle flat2(triangle.vertices[1], triangle.vertices[2], newPoint);
		TextureTriangle t2(tt.vertices[1], tt.vertices[2], newTPoint);
		fillFlatTextureTriangle(flat2, t2, map, image, depthBuffer);
	} else {
		CanvasTriangle flat2(newPoint, triangle.vertices[2], triangle.vertices[1]);
		TextureTriangle t2(newTPoint, tt.vertices[2], tt.vertices[1]);
		fillFlatTextureTriangle(flat2, t2, map, image, depthBuffer);
	}
}
