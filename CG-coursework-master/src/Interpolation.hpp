#ifndef _INTERPOLATION_HPP_
#define _INTERPOLATION_HPP_

std::vector<float> interpolateSingleFloats(float first, float last, int num);

std::vector<glm::vec2> interpolateDoubleElementValues(glm::vec2 first, glm::vec2 last, int num);

std::vector<glm::vec3> interpolateThreeElementValues(glm::vec3 first, glm::vec3 last, int num);

std::vector<CanvasPoint> interpolateCanvasPoints(CanvasPoint first, CanvasPoint last, int num);

std::vector<TexturePoint> interpolateTexturePoints(TexturePoint first, TexturePoint last, int num);

#endif
