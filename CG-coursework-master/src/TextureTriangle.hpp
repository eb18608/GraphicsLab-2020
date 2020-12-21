class TextureTriangle{
	public:
		std::array<TexturePoint, 3> vertices;
		inline TextureTriangle(TexturePoint v0, TexturePoint v1, TexturePoint v2){
			vertices = {v0, v1, v2};
		}
};
