#include "ProjectHandler.h"

#include <fstream>

void ProjectHandler::Initialize()
{
	CurrentPath = std::string();
}

void ProjectHandler::LoadProject(std::string path, Plane* plane, BlockRegistry* registry)
{
	plane->SetInnerPosition(sf::Vector2i(0, 0));
	plane->DeleteAllBlocks();
	std::ifstream stream;
	stream.open(path);
	std::string* save = new std::string();
	std::getline(stream, *save);
	stream.close();

	unsigned long long i = 0;
	while (i < save->length())
	{
		std::string sx = std::string();
		std::string sy = std::string();

		while ((*save)[i] != ';')
		{
			sx += (*save)[i];
			i++;
		}

		i++;

		while ((*save)[i] != ';')
		{
			sy += (*save)[i];
			i++;
		}

		Stack* stack = new Stack(sf::Vector2i(std::stoi(sx), std::stoi(sy)), registry);
		plane->AddStack(stack);

		i++;

		while ((*save)[i] != ';')
		{
			std::string blockUnlocalizedName = std::string();
			std::vector<std::string> args;

			while ((*save)[i] != ';')
			{
				blockUnlocalizedName += (*save)[i];
				i++;
			}

			i++;

			while ((*save)[i] != ';')
			{
				std::string argument = std::string();
				while ((*save)[i] != ';')
				{
					if ((*save)[i] == '\\')
					{
						i++;
						argument += (*save)[i];
						i++;
						continue;
					}

					argument += (*save)[i];
					i++;
				}

				args.push_back(argument);

				i++;
			}

			Block* block = new Block(blockUnlocalizedName, registry, stack->GetFunctionUpdate());
			stack->AddBlock(block);
			block->SetArgData(args);

			i++;
		}

		i++;
	}
}

void ProjectHandler::SaveProject(std::string path, Plane* plane)
{
	std::string* savePlane = new std::string();

	for (unsigned int i = 0; i < plane->GetStackCount(); i++)
	{
		std::string* saveStack = new std::string();

		*saveStack += std::to_string(plane->GetStack(i)->GetSetPosition().x);
		*saveStack += ";";
		*saveStack += std::to_string(plane->GetStack(i)->GetSetPosition().y);
		*saveStack += ";";

		for (unsigned int a = 0; a < plane->GetStack(i)->GetBlockCount(); a++)
		{
			std::string saveBlock = std::string();

			saveBlock += plane->GetStack(i)->GetBlock(a)->GetUnlocalizedName() + ";";

			for (unsigned int b = 0; b < plane->GetStack(i)->GetBlock(a)->GetArgumentCount(); b++)
			{
				if (plane->GetStack(i)->GetBlock(a)->GetArgument(b)->HasData())
				{
					std::string argumentData = plane->GetStack(i)->GetBlock(a)->GetArgument(b)->GetData();
					std::string arg = std::string();

					for (unsigned int c = 0; c < argumentData.length(); c++)
					{
						if (argumentData[c] == '\\')
							arg += "\\\\";
						else if (argumentData[c] == ';')
							arg += "\\;";
						else
							arg += argumentData[c];
					}

					saveBlock += arg + ";";
				}
			}

			*saveStack += saveBlock + ";";
		}

		*savePlane += *saveStack + ";";

		delete saveStack;
	}

	std::ofstream stream = std::ofstream();
	stream.open(path);
	stream.write(savePlane->c_str(), savePlane->length());
	stream.close();

	delete savePlane;
}

std::string ProjectHandler::CurrentPath;
