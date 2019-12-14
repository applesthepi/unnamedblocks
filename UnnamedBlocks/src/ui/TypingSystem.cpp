#include "TypingSystem.h"

#include <SFML/Window/Keyboard.hpp>
#include <chrono>
#include <iostream>

static void GetStrokes(bool* press)
{
	press[8] = sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace);
	press[127] = sf::Keyboard::isKeyPressed(sf::Keyboard::Delete);
	press[10] = sf::Keyboard::isKeyPressed(sf::Keyboard::Enter);
	press[13] = sf::Keyboard::isKeyPressed(sf::Keyboard::Enter);
	press[27] = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);
	press[32] = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[9] = false; press[128] = true; } else { press[128] = false; press[9] = true; } } else { press[9] = false; press[128] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tilde)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[96] = false; press[126] = true; } else { press[126] = false; press[96] = true; } } else { press[96] = false; press[126] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::RBracket)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[93] = false; press[125] = true; } else { press[125] = false; press[93] = true; } } else { press[93] = false; press[125] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSlash)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[92] = false; press[124] = true; } else { press[124] = false; press[92] = true; } } else { press[92] = false; press[124] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LBracket)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[91] = false; press[123] = true; } else { press[123] = false; press[91] = true; } } else { press[91] = false; press[123] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::SemiColon)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[59] = false; press[58] = true; } else { press[58] = false; press[59] = true; } } else { press[59] = false; press[58] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Slash)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[47] = false; press[63] = true; } else { press[63] = false; press[47] = true; } } else { press[47] = false; press[63] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[46] = false; press[62] = true; } else { press[62] = false; press[46] = true; } } else { press[46] = false; press[62] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Hyphen)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[45] = false; press[95] = true; } else { press[95] = false; press[45] = true; } } else { press[45] = false; press[95] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Comma)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[44] = false; press[60] = true; } else { press[60] = false; press[44] = true; } } else { press[44] = false; press[60] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[61] = false; press[43] = true; } else { press[43] = false; press[61] = true; } } else { press[61] = false; press[43] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Quote)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[39] = false; press[34] = true; } else { press[34] = false; press[39] = true; } } else { press[39] = false; press[34] = false; }

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[97] = false; press[65] = true; press[130] = false; } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) { press[65] = false; press[97] = false; press[130] = true; } else { press[65] = false; press[97] = true; press[130] = false; }} else {press[65] = false; press[97] = false; press[130] = false;}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[98] = false; press[66] = true; } else { press[66] = false; press[98] = true; } } else { press[98] = false; press[66] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[99] = false; press[67] = true; press[131] = false; } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) { press[67] = false; press[99] = false; press[131] = true; } else { press[67] = false; press[99] = true; press[131] = false; }} else {press[67] = false; press[99] = false; press[131] = false;}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[100] = false; press[68] = true; } else { press[68] = false; press[100] = true; } } else { press[100] = false; press[68] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[101] = false; press[69] = true; } else { press[69] = false; press[101] = true; } } else { press[101] = false; press[69] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[102] = false; press[70] = true; } else { press[70] = false; press[102] = true; } } else { press[102] = false; press[70] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[103] = false; press[71] = true; } else { press[71] = false; press[103] = true; } } else { press[103] = false; press[71] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[104] = false; press[72] = true; } else { press[72] = false; press[104] = true; } } else { press[104] = false; press[72] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[105] = false; press[73] = true; } else { press[73] = false; press[105] = true; } } else { press[105] = false; press[73] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[106] = false; press[74] = true; } else { press[74] = false; press[106] = true; } } else { press[106] = false; press[74] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[107] = false; press[75] = true; } else { press[75] = false; press[107] = true; } } else { press[107] = false; press[75] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[108] = false; press[76] = true; } else { press[76] = false; press[108] = true; } } else { press[108] = false; press[76] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[109] = false; press[77] = true; } else { press[77] = false; press[109] = true; } } else { press[109] = false; press[77] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[110] = false; press[78] = true; } else { press[78] = false; press[110] = true; } } else { press[110] = false; press[78] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[111] = false; press[79] = true; } else { press[79] = false; press[111] = true; } } else { press[111] = false; press[79] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[112] = false; press[80] = true; } else { press[80] = false; press[112] = true; } } else { press[112] = false; press[80] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[113] = false; press[81] = true; } else { press[81] = false; press[113] = true; } } else { press[113] = false; press[81] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[114] = false; press[82] = true; } else { press[82] = false; press[114] = true; } } else { press[114] = false; press[82] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[115] = false; press[83] = true; } else { press[83] = false; press[115] = true; } } else { press[115] = false; press[83] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[116] = false; press[84] = true; } else { press[84] = false; press[116] = true; } } else { press[116] = false; press[84] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[117] = false; press[85] = true; } else { press[85] = false; press[117] = true; } } else { press[117] = false; press[85] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[118] = false; press[86] = true; press[132] = false; } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) { press[86] = false; press[118] = false; press[132] = true; } else { press[86] = false; press[118] = true; press[132] = false; }} else {press[86] = false; press[118] = false; press[132] = false;}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[119] = false; press[87] = true; } else { press[87] = false; press[119] = true; } } else { press[119] = false; press[87] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[120] = false; press[88] = true; } else { press[88] = false; press[120] = true; } } else { press[120] = false; press[88] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[121] = false; press[89] = true; } else { press[89] = false; press[121] = true; } } else { press[121] = false; press[89] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[122] = false; press[90] = true; } else { press[90] = false; press[122] = true; } } else { press[122] = false; press[90] = false; }

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[48] = false; press[41] = true; } else { press[41] = false; press[48] = true; } } else { press[48] = false; press[41] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[49] = false; press[33] = true; } else { press[33] = false; press[49] = true; } } else { press[49] = false; press[33] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[50] = false; press[64] = true; } else { press[64] = false; press[50] = true; } } else { press[50] = false; press[64] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[51] = false; press[35] = true; } else { press[35] = false; press[51] = true; } } else { press[51] = false; press[35] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[52] = false; press[36] = true; } else { press[36] = false; press[52] = true; } } else { press[52] = false; press[36] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[53] = false; press[37] = true; } else { press[37] = false; press[53] = true; } } else { press[53] = false; press[37] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[54] = false; press[94] = true; } else { press[94] = false; press[54] = true; } } else { press[54] = false; press[94] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[55] = false; press[38] = true; } else { press[38] = false; press[55] = true; } } else { press[55] = false; press[38] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[56] = false; press[42] = true; } else { press[42] = false; press[56] = true; } } else { press[56] = false; press[42] = false; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9)) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { press[57] = false; press[40] = true; } else { press[40] = false; press[57] = true; } } else { press[57] = false; press[40] = false; }

	press[129] = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
	
	press[133] = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
	press[134] = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
	press[135] = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	press[136] = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
}

std::vector<std::function<void(int)>*>* TypingSystem::PressFunctions;
bool* TypingSystem::WasDown;
int* TypingSystem::PressRepeatIndex;

std::chrono::time_point<std::chrono::steady_clock>* TypingSystem::PressRepeatStartTime;
std::chrono::time_point<std::chrono::steady_clock>* TypingSystem::PressRepeatIncTime;

void TypingSystem::Initialization()
{
	PressFunctions = new std::vector<std::function<void(int)>*>();
	WasDown = new bool[137];
	GetStrokes(WasDown);
	PressRepeatIndex = new int(-1);
	PressRepeatStartTime = nullptr;
	PressRepeatIncTime = nullptr;
}

void TypingSystem::Update()
{
	bool* currentPress = new bool[137];
	GetStrokes(currentPress);

	RunStroke(8, currentPress);
	RunStroke(9, currentPress);
	RunStroke(10, currentPress);
	RunStroke(13, currentPress);
	RunStroke(27, currentPress);

	for (unsigned char i = 32; i < 129; i++)
	{
		RunStroke(i, currentPress);
	}

	for (unsigned char i = 130; i < 133; i++)
	{
		RunStroke(i, currentPress);
	}

	if (currentPress[129])
	{
		if (!WasDown[129])
		{
			WasDown[129] = true;

			for (unsigned long long int i = 0; i < PressFunctions->size(); i++)
			{
				(*(*PressFunctions)[i])(129);
			}
		}
	}
	else
	{
		if (WasDown[129])
		{
			WasDown[129] = false;

			for (unsigned long long int i = 0; i < PressFunctions->size(); i++)
			{
				(*(*PressFunctions)[i])(129);
			}
		}
	}

	for (unsigned char i = 133; i < 137; i++)
	{
		RunArrowStroke(i, currentPress);
	}

	delete[] currentPress;

	if (PressRepeatStartTime != nullptr)
	{
		if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - *PressRepeatStartTime).count() > 400)
		{
			if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - *PressRepeatIncTime).count() > 20)
			{
				for (unsigned long long int i = 0; i < PressFunctions->size(); i++)
				{
					if (*PressRepeatIndex >= 133)
						(*(*PressFunctions)[i])((int)(*PressRepeatIndex - 137));
					else
						(*(*PressFunctions)[i])((int)*PressRepeatIndex);
				}

				*PressRepeatIncTime = std::chrono::high_resolution_clock::now();
			}
		}
	}
}

void TypingSystem::AddKeypressRegister(std::function<void(int)>* fun)
{
	PressFunctions->push_back(fun);
}

void TypingSystem::RemoveKeypressRegister(std::function<void(int)>* fun)
{
	for (unsigned long long int i = 0; i < PressFunctions->size(); i++)
	{
		if ((*PressFunctions)[i] == fun)
		{
			PressFunctions->erase(PressFunctions->begin() + i);
			return;
		}
	}
}

void TypingSystem::RunStroke(unsigned int c, bool* currentPress)
{
	if (WasDown[c] && !currentPress[c])
	{
		WasDown[c] = false;

		if (*PressRepeatIndex == c)
		{
			delete PressRepeatStartTime;
			delete PressRepeatIncTime;

			PressRepeatStartTime = nullptr;
		}
	}
	else if (!WasDown[c] && currentPress[c])
	{
		WasDown[c] = true;
		*PressRepeatIndex = c;
		PressRepeatStartTime = new std::chrono::time_point<std::chrono::steady_clock>();
		*PressRepeatStartTime = std::chrono::high_resolution_clock::now();

		PressRepeatIncTime = new std::chrono::time_point<std::chrono::steady_clock>();
		*PressRepeatIncTime = std::chrono::high_resolution_clock::now();

		for (unsigned long long int i = 0; i < PressFunctions->size(); i++)
		{
			(*(*PressFunctions)[i])((int)c);
		}
	}
}

void TypingSystem::RunArrowStroke(unsigned int c, bool* currentPress)
{
	if (WasDown[c] && !currentPress[c])
	{
		WasDown[c] = false;

		if (*PressRepeatIndex == c)
		{
			delete PressRepeatStartTime;
			delete PressRepeatIncTime;

			PressRepeatStartTime = nullptr;
		}
	}
	else if (!WasDown[c] && currentPress[c])
	{
		WasDown[c] = true;
		*PressRepeatIndex = c;
		PressRepeatStartTime = new std::chrono::time_point<std::chrono::steady_clock>();
		*PressRepeatStartTime = std::chrono::high_resolution_clock::now();

		PressRepeatIncTime = new std::chrono::time_point<std::chrono::steady_clock>();
		*PressRepeatIncTime = std::chrono::high_resolution_clock::now();

		for (unsigned long long int i = 0; i < PressFunctions->size(); i++)
		{
			(*(*PressFunctions)[i])((int)(((int)c) - 137));
		}
	}
}