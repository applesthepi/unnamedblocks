#pragma once
//#include "config.h"
#include <SFML/Graphics/Font.hpp>
#include <string>
#include <functional>

#define MOD_BACKGROUND_LOW_C 30
#define MOD_BACKGROUND_LOW sf::Color(MOD_BACKGROUND_LOW_C, MOD_BACKGROUND_LOW_C, MOD_BACKGROUND_LOW_C, 255)
#define MOD_BACKGROUND_HIGH_C 51
#define MOD_BACKGROUND_HIGH sf::Color(MOD_BACKGROUND_HIGH_C, MOD_BACKGROUND_HIGH_C, MOD_BACKGROUND_HIGH_C, 255)

#define MOD_BUTTON_TEXT_FG_C 150
#define MOD_BUTTON_TEXT_FG sf::Color(MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, 255)
#define MOD_BUTTON_TEXT_BG_C 40
#define MOD_BUTTON_TEXT_BG sf::Color(MOD_BUTTON_TEXT_BG_C, MOD_BUTTON_TEXT_BG_C, MOD_BUTTON_TEXT_BG_C, 255)
#define MOD_BUTTON_TEXT_BG_ACCENT_WEAK sf::Color(36, 41, 45, 255)
#define MOD_BUTTON_TEXT_BG_ACCENT_STRONG sf::Color(35, 40, 115, 255)

#define MOD_VAR sf::Color(144, 50, 47)

#define MOD_HIGHLIGHT_COLOR sf::Color(0, 120, 200, 80)

#define HEADER_HEIGHT (32 + 10)

/*
enum GpuMan
{
	INTEL, NVIDIA
};
*/

class Global
{
public:
	static void LoadDefaults()
	{
		// these are in use and updated for "the_nuking_of_rhr_stacking"

		BlockHeight = 18;
		BlockBorder = 4;
		Font.loadFromFile("res/AnonymousPro-Regular.ttf");

		// these may be old and unused

		//Dragging = false;
		//DraggingStack = nullptr;
		//DraggingStackConnected = nullptr;
		//DraggingStackConnectedIndex = 0;
		//CutRenderingPlane = false;
		//DraggingPlane = nullptr;
		//DraggingPlaneOver = nullptr;
		//SelectedArgument = nullptr;
		//ToolbarWidth = 300;
		//BlockConnectDistance = 60;
		//ApplicationRunning = true;
		//ProjectPath = "";
		//SkipFrame = false;
		//MousePosition = sf::Vector2i(0, 0);
		//SelectedBlock = nullptr;
		//SelectedStack = nullptr;
	}

	// these are in use and updated for "the_nuking_of_rhr_stacking"

	static uint8_t BlockHeight;
	static uint8_t BlockBorder;// must be divisible by 2
	static sf::Font Font;
	static sf::Vector2i MousePosition;
	static sf::Vector2u WindowSize;

	// these may be old and unused

	//static bool Dragging;
	//static void* DraggingStack;
	//static void* DraggingStackConnected;
	//static unsigned int DraggingStackConnectedIndex;
	//static bool CutRenderingPlane;
	//static void* DraggingPlane;
	//static void* DraggingPlaneOver;
	//static void* SelectedArgument;
	//static unsigned int ToolbarWidth;
	//static unsigned int BlockConnectDistance;
	//static bool ApplicationRunning;
	//static std::string ProjectPath;
	//static bool SkipFrame;
	//static GpuMan GpuType;
	//static sf::Vector2i MousePosition;
	//static void* SelectedBlock;
	//static void* SelectedStack;
};

