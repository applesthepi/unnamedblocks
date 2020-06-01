#include "Argument.h"

#include <Espresso/Global.h>
#include <Cappuccino/Logger.h>

Argument::Argument()
{
	m_vertexArrayImage.create(1, 1);
	m_vertexArray.setPrimitiveType(sf::Triangles);
}

void Argument::SetData(const std::string& data)
{
	m_data = data;
}

const std::string& Argument::GetData()
{
	return m_data;
}

void Argument::SetMode(const BlockArgumentVariableMode& mode)
{
	m_mode = mode;
}

const BlockArgumentVariableMode& Argument::GetMode()
{
	return m_mode;
}

const bool Argument::PullNext()
{
	return m_next;
}

const sf::VertexArray& Argument::GetVertexArray()
{
	return m_vertexArray;
}

const std::vector<bool>& Argument::GetVertexArrayCoords()
{
	return m_vertexCoords;
}

const sf::Image& Argument::GetVertexArrayTexture()
{
	return m_vertexArrayImage;
}

const BlockArgumentType Argument::GetType()
{
	return BlockArgumentType::TEXT;
}

const uint32_t Argument::GetWidth()
{
	return 50;
}

const bool Argument::HasData()
{
	return false;
}

void Argument::Select()
{

}

void Argument::UnSelect()
{

}

void Argument::UpdateVertexArray()
{

}

void Argument::ResetVertices()
{
	m_vertexArray.clear();
	m_vertexCoords.clear();
}

void Argument::AddVertex(const sf::Vertex& vertex, const bool& hasTextureCoords)
{
	m_vertexArray.append(vertex);
	m_vertexCoords.push_back(hasTextureCoords);
}