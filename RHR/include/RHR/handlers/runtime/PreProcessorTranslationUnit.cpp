#include "PreProcessorTranslationUnit.h"

#include <fstream>

void ThreadPreProcessorTranslationUnit(PreProcessorTranslationUnit& unit)
{
	unit.SetTranslationUnitStatus(PreProcessorTranslationUnitStatus::RUNNING);

	const Stack& stack = unit.GetStack();
	BlockRegistry* blockRegistry = unit.GetBlockRegistry();

	for (uint64_t i = 0; i < stack.GetBlockCount(); i++)
	{
		Block* block = stack.GetBlock(i);

		const std::string& unlocalizedName = block->GetUnlocalizedName();
		BlockRuntimeReturn runtimeReturn = block->GetUsedArgumentsRuntime();

		const ModBlock& modBlock = blockRegistry->GetBlock(unlocalizedName);
		char* blockPath = (char*)"mods/";
		strcat(blockPath, modBlock.GetPath());

		std::vector<std::string> lines;

		{
			std::ifstream stream(blockPath);
			std::string editedLine;
			std::string line;
			uint64_t lastCut = 0;

			bool firstHit = false;

			while (std::getline(stream, line))
			{
				for (uint64_t a = 0; a < line.length(); a++)
				{
					if (firstHit)
					{
						if (line[a] == '$')
						{
							firstHit = false;
							std::string number;
							editedLine += line.substr(lastCut, a - lastCut - 2);

							while (line[a] != '$')
							{
								number += line[a];
								a++;

								if (a == line.length())
								{
									Logger::Error("failed to preprocess script; invalid argument syntax");
									unit.SetTranslationUnitStatus(PreProcessorTranslationUnitStatus::DONE_ERROR);
									return;
								}
							}

							uint32_t argIdx = std::stoul(number);

							if (runtimeReturn.Args->at(argIdx).Mode == BlockArgumentVariableMode::VAR)
							{

							}
							else
							{
								editedLine += runtimeReturn.Args->at(argIdx).Value;
								lastCut = a;
							}
						}
						else
							firstHit = false;
					}
					
					if (line[a] == '$')
						firstHit = true;
				}

				editedLine += line.substr(lastCut, line.length() - lastCut);
				lines.push_back(editedLine);
			}

			stream.close();
		}

		{
			std::ofstream stream(unit.GetFinishedPath());

			for (uint64_t a = 0; a < lines.size(); a++)
			{
				stream << lines[a];
			}

			stream.close();
		}
	}

	unit.SetTranslationUnitStatus(PreProcessorTranslationUnitStatus::DONE);
}

PreProcessorTranslationUnit::PreProcessorTranslationUnit()
	:m_status(PreProcessorTranslationUnitStatus::NOT_READY)
{
	
}

PreProcessorTranslationUnit::PreProcessorTranslationUnit(const uint64_t& idx, const Stack* stack, BlockRegistry* blockRegistry)
	:m_status(PreProcessorTranslationUnitStatus::NOT_READY), m_stack(stack), m_blockRegistry(blockRegistry)
{
	m_path = "runtime/build/tu_" + std::to_string(idx) + ".ubb";
	m_thread = std::thread(ThreadPreProcessorTranslationUnit, *this);
	m_thread.detach();
}

void PreProcessorTranslationUnit::SetTranslationUnitStatus(PreProcessorTranslationUnitStatus status)
{
	std::unique_lock<std::mutex> lock(m_statusMutex);
	m_status = status;
}

PreProcessorTranslationUnitStatus PreProcessorTranslationUnit::GetTranslationUnitStatus()
{
	std::unique_lock<std::mutex> lock(m_statusMutex);
	return m_status;
}

const std::string& PreProcessorTranslationUnit::GetFinishedPath() const
{
	return m_path;
}

const Stack& PreProcessorTranslationUnit::GetStack() const
{
	return m_stack;
}

BlockRegistry* PreProcessorTranslationUnit::GetBlockRegistry() const
{
	return m_blockRegistry;
}
