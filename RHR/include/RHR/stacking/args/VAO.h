#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class VAO
{
public:
	VAO();

	void AddTriangle(const sf::Vector2f positions[], const sf::Color colors[]);
	void AddChar(const sf::Glyph& gl, char character, uint32_t* offset);

	void Reset();
	void Reserve(size_t triCount);

	sf::VertexArray* GetVertexArray();
	std::vector<bool>* GetChar();
private:
	std::vector<bool> m_char;
	sf::VertexArray m_vertexArray;
};