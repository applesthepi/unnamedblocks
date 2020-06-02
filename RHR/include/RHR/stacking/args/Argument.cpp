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
	UpdateData();
	UpdateVertexArray();
}

const std::string& Argument::GetData()
{
	return m_data;
}

void Argument::SetMode(const BlockArgumentVariableMode& mode)
{
	m_mode = mode;
	UpdateVertexArray();
}

const BlockArgumentVariableMode& Argument::GetMode()
{
	return m_mode;
}

bool Argument::PullNext()
{
	return m_next;
}

const sf::VertexArray* Argument::GetVertexArray()
{
	return &m_vertexArray;
}

const std::vector<bool>* Argument::GetVertexArrayCoords()
{
	return &m_vertexCoords;
}

const sf::Image* Argument::GetVertexArrayTexture()
{
	if (UseVertexArrayTexture())
		return &m_vertexArrayImage;
	else
		return nullptr;
}

BlockArgumentType Argument::GetType()
{
	return BlockArgumentType::TEXT;
}

uint32_t Argument::GetWidth()
{
	return 50;
}

bool Argument::HasData()
{
	return false;
}

void Argument::Select()
{

}

void Argument::UnSelect()
{

}

bool Argument::UseVertexArrayTexture()
{
	return false;
}

void Argument::UpdateVertexArray()
{

}

void Argument::UpdateData()
{

}

void Argument::ResetVertices(uint64_t reserve)
{
	m_vertexArray.clear();
	m_vertexCoords.clear();

	m_vertexCoords.reserve(reserve);
}

void Argument::AddVertex(const sf::Vertex& vertex, bool hasTextureCoords)
{
	m_vertexArray.append(vertex);
	m_vertexCoords.push_back(hasTextureCoords);
}
