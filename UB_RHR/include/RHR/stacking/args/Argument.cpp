#include "Argument.hpp"

// Argument::Argument()
// {
// 	//m_vertexArrayImage.create(1, 1);
// 	//m_vertexArray.setPrimitiveType(sf::Triangles);

// 	m_mode = BlockArgumentVariableMode::RAW;
// }

// void Argument::SetData(const std::string& data)
// {
// 	m_data = data;
// }

// const std::string& Argument::GetData()
// {
// 	return m_data;
// }

// void Argument::SetMode(const BlockArgumentVariableMode& mode)
// {
// 	m_mode = mode;
// }

// const BlockArgumentVariableMode& Argument::GetMode()
// {
// 	return m_mode;
// }

// bool Argument::PullNext()
// {
// 	return m_next;
// }

// BlockArgumentType Argument::GetType()
// {
// 	return BlockArgumentType::TEXT;
// }

// uint32_t Argument::GetWidth()
// {
// 	return 50;
// }

// bool Argument::HasData()
// {
// 	return false;
// }

// void Argument::Select()
// {

// }

// void Argument::UnSelect()
// {

// }
// /*
// bool Argument::UseVertexArrayTexture()
// {
// 	return false;
// }
// */
// void Argument::UpdateVertexArray()
// {

// }

// void Argument::UpdateData()
// {

// }

// VAO& Argument::GetVAO()
// {
// 	return m_vao;
// }

/*
void Argument::ResetVertices(uint64_t reserve)
{
	m_vertexArray.clear();
	m_vertexCoords.clear();

	m_vertexCoords.reserve(reserve);
}

void Argument::AddTriangle(const sf::Vector2f positions[], const sf::Vector2f textureCoords[])
{
	m_vertexArray.append(sf::Vertex(positions[0], textureCoords[0]));
	m_vertexArray.append(sf::Vertex(positions[1], textureCoords[1]));
	m_vertexArray.append(sf::Vertex(positions[2], textureCoords[2]));

	m_vertexCoords.push_back(true);
	m_vertexCoords.push_back(true);
	m_vertexCoords.push_back(true);
}

void Argument::AddTriangle(const sf::Vector2f positions[], const sf::Color colors[])
{
	m_vertexArray.append(sf::Vertex(positions[0], colors[0], sf::Vector2f(0, 0)));
	m_vertexArray.append(sf::Vertex(positions[1], colors[1], sf::Vector2f(1, 0)));
	m_vertexArray.append(sf::Vertex(positions[2], colors[2], sf::Vector2f(1, 1)));

	m_vertexCoords.push_back(false);
	m_vertexCoords.push_back(false);
	m_vertexCoords.push_back(false);
}
*/