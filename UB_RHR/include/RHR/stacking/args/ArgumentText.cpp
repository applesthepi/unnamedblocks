#include "ArgumentText.hpp"

ArgumentText::ArgumentText()
{
	m_widthNeedsUpdate = false;
	UpdateVertexArray();
}

BlockArgumentType ArgumentText::GetType()
{
	return BlockArgumentType::TEXT;
}

uint32_t ArgumentText::GetWidth()
{
	if (m_widthNeedsUpdate)
	{
		m_widthNeedsUpdate = false;
		m_width = 0;

		for (uint32_t i = 0; i < m_text.length(); i++)
		{
			Global::FontBold.getGlyph(m_text[i], Global::BlockHeight, false);
			m_width += Global::Font.getGlyph(m_text[i], Global::BlockHeight, false).advance;
		}

		return m_width;
	}
	else
		return m_width;
}

bool ArgumentText::HasData()
{
	return false;
}
