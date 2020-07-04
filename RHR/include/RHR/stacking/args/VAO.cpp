#include "VAO.h"
#include <Espresso/Global.h>

VAO::VAO()
{
	m_vertexArray.setPrimitiveType(sf::Triangles);
}

void VAO::AddTriangle(const sf::Vector2f positions[], const sf::Color colors[])
{
	m_vertexArray.append(sf::Vertex(positions[0], colors[0], sf::Vector2f(0, 0)));
	m_vertexArray.append(sf::Vertex(positions[1], colors[1], sf::Vector2f(1, 0)));
	m_vertexArray.append(sf::Vertex(positions[2], colors[2], sf::Vector2f(1, 1)));

	m_char.push_back(false);
	m_char.push_back(false);
	m_char.push_back(false);
}

void VAO::AddChar(const sf::Glyph& gl, char character, uint32_t* offset)
{
	sf::FloatRect rect = sf::FloatRect(
		*offset,
		gl.bounds.top + (Global::BlockHeight - Global::BlockBorder - 1),
		gl.bounds.width,
		gl.bounds.height
	);

	m_vertexArray.append(sf::Vertex(sf::Vector2f(rect.left, rect.top), sf::Color::Black, sf::Vector2f(character, 0)));
	m_vertexArray.append(sf::Vertex(sf::Vector2f(rect.left + rect.width, rect.top), sf::Color::Black, sf::Vector2f(0, 0)));
	m_vertexArray.append(sf::Vertex(sf::Vector2f(rect.left + rect.width, rect.top + rect.height), sf::Color::Black, sf::Vector2f(0, 0)));

	m_vertexArray.append(sf::Vertex(sf::Vector2f(rect.left, rect.top), sf::Color::Black, sf::Vector2f(0, 0)));
	m_vertexArray.append(sf::Vertex(sf::Vector2f(rect.left + rect.width, rect.top + rect.height), sf::Color::Black, sf::Vector2f(0, 0)));
	m_vertexArray.append(sf::Vertex(sf::Vector2f(rect.left, rect.top + rect.height), sf::Color::Black, sf::Vector2f(0, 0)));

	m_char.push_back(true);
	m_char.push_back(false);
	m_char.push_back(false);
	m_char.push_back(false);
	m_char.push_back(false);
	m_char.push_back(false);

	*offset += gl.advance;
}

void VAO::Reset()
{
	m_vertexArray.clear();
	m_char.clear();
}

void VAO::Reserve(size_t triCount)
{
	m_char.reserve(triCount * 3);
}

sf::VertexArray* VAO::GetVertexArray()
{
	return &m_vertexArray;
}

std::vector<bool>* VAO::GetChar()
{
	return &m_char;
}
