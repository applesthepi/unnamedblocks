#include "Global.h"

unsigned int Global::BlockHeight;

unsigned int Global::BlockBorder;

sf::Font* Global::Font;

bool Global::Dragging;

void* Global::DraggingStack;

void* Global::DraggingStackConnected;

unsigned int Global::DraggingStackConnectedIndex;

bool Global::CutRenderingPlane;

void* Global::DraggingPlane;

void* Global::DraggingPlaneOver;

void* Global::SelectedArgument;

unsigned int Global::ToolbarWidth;

unsigned int Global::BlockConnectDistance;

bool Global::ApplicationRunning;

std::string Global::ProjectPath;

bool Global::SkipFrame;

bool Global::ContextActive;

ContextSystem Global::Context;

GpuMan Global::GpuType;

std::string Global::ContextData;

sf::Vector2i Global::MousePosition;

void* Global::SelectedBlock;

