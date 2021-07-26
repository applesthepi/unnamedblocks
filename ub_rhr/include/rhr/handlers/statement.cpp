#include "statement.hpp"

/*
std::vector<StatmentIf> StatmentHandler::ProcessIfStatments(Stack* stack)
{
	std::vector<StatmentIf> ifStatments;
	std::vector<StatmentIf> currentIfs;
	std::vector<std::vector<u32>> currentIfElse;

	for (u32 a = 0; a < stack->GetBlockCount(); a++)
	{
		if (stack->GetBlock(a)->GetUnlocalizedName() == "vin_execution_if")
		{
			currentIfs.push_back(StatmentIf());
			currentIfs.back().Location = a;

			continue;
		}
		
		if (currentIfs.size() > 0)
		{
			if (stack->GetBlock(a)->GetUnlocalizedName() == "vin_execution_if_end")
			{
				currentIfs.back().LocationEnd = a;

				if (currentIfs.back().HasElseIf)
				{
					currentIfs.back().LocationElseIf = currentIfElse.back();
					currentIfElse.pop_back();
				}

				ifStatments.push_back(currentIfs.back());
				currentIfs.pop_back();
			}
			else if (stack->GetBlock(a)->GetUnlocalizedName() == "vin_execution_else")
			{
				currentIfs.back().HasElse = true;
				currentIfs.back().LocationElse = a;
			}
			else if (stack->GetBlock(a)->GetUnlocalizedName() == "vin_execution_else_if")
			{
				if (currentIfs.back().HasElse)
				{
					//if (!muteErrors)
					//	Logger::Error("failed to construct if statement track, you can not have an \"else if\" block after an \"else\" block in an if statement");

					return std::vector<StatmentIf>();
				}

				if (!currentIfs.back().HasElseIf)
					currentIfElse.push_back(std::vector<u32>());

				currentIfs.back().HasElseIf = true;
				currentIfElse.back().push_back(a);
			}
		}
	}

	if (currentIfs.size() > 0)
	{
		//if (!muteErrors)
		//	Logger::Error("failed to construct if statement track, unbalanced statements");

		return std::vector<StatmentIf>();
	}

	return ifStatments;
}
*/