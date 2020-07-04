#pragma once
#include "RHR/ui/ITransformable.h"
#include "VAO.h"

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
	//const sf::VertexArray* GetVertexArray();

	// a list of the vertices as booleans for whether or not they are intended to have texture coordinates
	//const std::vector<bool>* GetVertexArrayCoords();

	// if using textures, then this will be what you refer to using the texture coordinates. Later it will get processed with everything.
	//const sf::Image* GetVertexArrayTexture();

	// how to interpret the data
	virtual BlockArgumentType GetType();

	// get width for surrounding argument spacing
	virtual uint32_t GetWidth();

	// whether or not the argument contains data for storing
	virtual bool HasData();

	// select the argument and enable it
	virtual void Select();

	// when the focus turns off of the argument
	virtual void UnSelect();

	// whether to use the vertexArrayImage or not
	//virtual bool UseVertexArrayTexture();

	// update geometry
	virtual void UpdateVertexArray();

	// update data should also cause an update in geometry
	virtual void UpdateData();

	// contains all data including: position, colors, and characters.
	VAO& GetVAO();
protected:
	bool m_next;
	std::string m_data;
	BlockArgumentVariableMode m_mode;
private:
	VAO m_vao;
};
