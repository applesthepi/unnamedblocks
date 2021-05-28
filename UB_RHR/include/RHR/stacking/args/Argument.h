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
