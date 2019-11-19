#include "Block.h"
#include "args/ArgumentText.h"

Block::Block(std::string type)
{
	std::vector<BlockArgument>* args = BlockRegistry::GetHeapedBlockArguments(type);

	unsigned int offset = 0;

	for (unsigned int i = 0; i < args->size(); i++)
	{
		if ((*args)[i].Type == BlockArgumentType::TEXT)
		{
			ArgumentText* arg = new ArgumentText(sf::Vector2u(m_position.x + offset, m_position.y), (*args)[i].T_STRING);
			offset += arg->GetArgumentRawWidth();

			m_args.push_back(arg);
		}
	}

	delete args;
}

void Block::Render(sf::RenderWindow* window)
{
	sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(300, Global::BlockHeight));
	shape.setPosition(m_position.x, m_position.y);
	shape.setFillColor(sf::Color::Green);

	window->draw(shape);

	for (unsigned int i = 0; i < m_args.size(); i++)
	{
		m_args[i]->Update(&m_position);
		m_args[i]->Render(window);
	}
}
