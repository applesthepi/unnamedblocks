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

#define HEADER_HEIGHT (32 + 10)

enum GpuMan
{
	INTEL, NVIDIA
};

enum ContextType
{
	NONE, BLOCK
};

class ContextSystem
{
public:
	ContextSystem()
	{
		Type = ContextType::NONE;
		Position = sf::Vector2i(0, 0);
		Callback = nullptr;
	}

	ContextSystem(ContextType type, sf::Vector2i position, std::function<void(unsigned int index)>* callback)
		:Type(type), Position(position), Callback(callback)
	{

	}

	ContextType Type;
	sf::Vector2i Position;
	std::function<void(unsigned int index)>* Callback;
};

class Global
{
public:
	static void LoadDefaults()
	{
		BlockHeight = 18;
		BlockBorder = 4;
		Font = new sf::Font();
		Font->loadFromFile("res/AnonymousPro-Regular.ttf");
		Dragging = false;
		DraggingStack = nullptr;
		DraggingStackConnected = nullptr;
		DraggingStackConnectedIndex = 0;
		CutRenderingPlane = false;
		DraggingPlane = nullptr;
		DraggingPlaneOver = nullptr;
		SelectedArgument = nullptr;
		ToolbarWidth = 300;
		BlockConnectDistance = 60;
		ApplicationRunning = true;
		ProjectPath = "";
		SkipFrame = false;
		ContextActive = false;
		Context = ContextSystem(ContextType::NONE, sf::Vector2i(0, 0), nullptr);
		ContextUpdate = false;
		ContextData = "";
		MousePosition = sf::Vector2i(0, 0);
		SelectedBlock = nullptr;
		SelectedStack = nullptr;
	}

	static unsigned int BlockHeight;
	static unsigned int BlockBorder;
	static sf::Font* Font;
	static bool Dragging;
	static void* DraggingStack;
	static void* DraggingStackConnected;
	static unsigned int DraggingStackConnectedIndex;
	static bool CutRenderingPlane;
	static void* DraggingPlane;
	static void* DraggingPlaneOver;
	static void* SelectedArgument;
	static unsigned int ToolbarWidth;
	static unsigned int BlockConnectDistance;
	static bool ApplicationRunning;
	static std::string ProjectPath;
	static bool SkipFrame;
	static bool ContextActive;
	static ContextSystem Context;
	static GpuMan GpuType;
	static std::string ContextData;
	static bool ContextUpdate;
	static sf::Vector2i MousePosition;
	static void* SelectedBlock;
	static void* SelectedStack;
};

