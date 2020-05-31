#pragma once
#include "IRenderable.h"
#include "IMouseUpdatable.h"

class IUI : public IRenderable, public IMouseUpdatable
{

};