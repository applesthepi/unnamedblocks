#pragma once
#include "ModVin/ModCategories.hpp"

class BlockSystemWindowRender : public ModBlock
{
public:
    const char* get_unlocalized_name() const override;
    const char* GetCategory() const override;

    blockExecution PullExecuteDebug() const override;
    blockExecution PullExecuteRelease() const override;
    
    const std::vector<BlockArgumentInitializer> GetArguments() const override;
};
