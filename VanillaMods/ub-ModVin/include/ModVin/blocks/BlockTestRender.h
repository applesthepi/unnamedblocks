#pragma once
#include "ModVin/ModCategories.h"
#include <Espresso/Espresso.h>

class BlockTestRender : public ModBlock
{
public:
    const char* GetUnlocalizedName() const override;
    blockExecution PullExecuteDebug() const override;
    blockExecution PullExecuteRelease() const override;
    const char* GetCategory() const override;
    const ModBlockFlags GetFlags() const override;
    
    const std::vector<BlockArgumentInitializer> GetArguments() const override;
};
