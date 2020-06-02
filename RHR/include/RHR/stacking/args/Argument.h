#pragma once
#include "RHR/ui/ITransformable.h"

#include <Cappuccino/block/ModBlock.h>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Image.hpp>

class Argument : public ITransformable
{
public:
	Argument();

	// data of the argument must be stored as string for saving
	void SetData(const std::string& data);
	const std::string& GetData();

	// raw or var
	void SetMode(const BlockArgumentVariableMode& mode);
	const BlockArgumentVariableMode& GetMode();

	// pull whether or not the argument is requesting to goto the next argument
	bool PullNext();

	// the geometry, colors, and texture coordinates of the argument
	const sf::VertexArray* GetVertexArray();

	// a list of the vertices as booleans for whether or not they are intended to have texture coordinates
	const std::vector<bool>* GetVertexArrayCoords();

	// if using textures, then this will be what you refer to using the texture coordinates. Later it will get processed with everything.
	const sf::Image* GetVertexArrayTexture();

	// how to interpret the data
	virtual const BlockArgumentType GetType();

	// get width for surrounding argument spacing
	virtual uint32_t GetWidth();

	// whether or not the argument contains data for storing
	virtual bool HasData();

	// select the argument and enable it
	virtual void Select();

	// when the focus turns off of the argument
	virtual void UnSelect();

	// whether to use the vertexArrayImage or not
	virtual bool UseVertexArrayTexture();
protected:
	virtual void UpdateVertexArray();

	void ResetVertices(uint64_t reserve);
	void AddVertex(const sf::Vertex& vertex, bool hasTextureCoords);

	bool m_next;
	std::string m_data;
	BlockArgumentVariableMode m_mode;
	
	sf::Image m_vertexArrayImage;
private:
	sf::VertexArray m_vertexArray;
	std::vector<bool> m_vertexCoords;
};
