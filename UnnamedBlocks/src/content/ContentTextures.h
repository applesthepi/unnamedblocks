#pragma once

#include <RHR/RHR.h>

void ContentLoadTextures()
{
	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();
		BlockArgument arg2 = BlockArgument();
		BlockArgument arg3 = BlockArgument();

		arg0.SetupTEXT("add texture");
		arg1.SetupSTRING("0path");
		arg2.SetupTEXT("for");
		arg3.SetupREAL("1var");
		
		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);
		argList.push_back(arg3);

		block->Args = argList;
		block->Catagory = "vin_textures";
		block->UnlocalizedName = "vin_texture_add";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string pathText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string objText = (*args)[1].substr(1, (*args)[1].length() - 1);

			if ((*args)[1][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + objText + "\"");
				return false;
			}

			std::string path = std::string();
			
			if ((*args)[0][0] == '0')
				path = pathText;
			else
			{
				std::string* value = VariableHandler::GetString(pathText.c_str());
				if (value == nullptr)
				{
					Logger::Error("variable \"" + pathText + "\" does not exist");
					return false;
				}

				path = *value;
			}
			
			double* objectId = VariableHandler::GetReal(objText.c_str());
			if (objectId == nullptr)
			{
				Logger::Error("variable \"" + objText + "\" does not exist");
				return false;
			}

			RuntimeObject* run = ObjectHandler::GetObject(*objectId);
			if (run == nullptr)
			{
				Logger::Error("object \"" + std::to_string(*objectId) + "\" does not exist");
				return false;
			}

			sf::Texture* txt = new sf::Texture();
			txt->loadFromFile(pathText);

			sf::Sprite* sp = new sf::Sprite();
			sp->setTexture(*txt);

			ObjectHandler::ObjectMutex->lock();
			run->Textures.push_back(txt);
			run->Sprites.push_back(sp);
			ObjectHandler::ObjectMutex->unlock();

			return true;
		};

		block->Execute = execute;
		BlockRegistry::CreateBlock(*block);
	}
	
	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();
		BlockArgument arg2 = BlockArgument();
		BlockArgument arg3 = BlockArgument();

		arg0.SetupTEXT("remove texture at");
		arg1.SetupREAL("00");
		arg2.SetupTEXT("for");
		arg3.SetupREAL("1var");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);
		argList.push_back(arg3);

		block->Args = argList;
		block->Catagory = "vin_textures";
		block->UnlocalizedName = "vin_texture_remove";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string indexText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string objText = (*args)[1].substr(1, (*args)[1].length() - 1);

			if ((*args)[1][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + objText + "\"");
				return false;
			}
			
			double* objectId = VariableHandler::GetReal(objText.c_str());
			if (objectId == nullptr)
			{
				Logger::Error("variable \"" + objText + "\" does not exist");
				return false;
			}

			RuntimeObject* run = ObjectHandler::GetObject(*objectId);
			if (run == nullptr)
			{
				Logger::Error("object \"" + std::to_string(*objectId) + "\" does not exist");
				return false;
			}

			unsigned int textureId = 0;
			
			if ((*args)[0][0] == '0')
			{
				try
				{
					textureId = std::stod(indexText);
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + indexText + "\"");
					return false;
				}
			}
			else
			{
				double* value = VariableHandler::GetReal(indexText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + indexText + "\" does not exist");
					return false;
				}

				textureId = *value;
			}

			if (textureId >= run->Textures.size())
			{
				Logger::Error("trying to remove a texture out of range. Object \"" + std::to_string(*objectId) + "\" only has " + std::to_string(run->Textures.size()) + " textures. Trying to remove texture " + std::to_string(textureId));
				return false;
			}

			ObjectHandler::ObjectMutex->lock();
			run->Textures.erase(run->Textures.begin() + textureId);
			run->Sprites.erase(run->Sprites.begin() + textureId);
			ObjectHandler::ObjectMutex->unlock();

			return true;
		};

		block->Execute = execute;
		BlockRegistry::CreateBlock(*block);
	}
	
	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();
		BlockArgument arg2 = BlockArgument();
		BlockArgument arg3 = BlockArgument();
		BlockArgument arg4 = BlockArgument();
		BlockArgument arg5 = BlockArgument();

		arg0.SetupTEXT("insert texture at");
		arg1.SetupREAL("00");
		arg2.SetupTEXT("for");
		arg3.SetupREAL("1var");
		arg4.SetupTEXT("with");
		arg5.SetupSTRING("0path");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);
		argList.push_back(arg3);
		argList.push_back(arg4);
		argList.push_back(arg5);

		block->Args = argList;
		block->Catagory = "vin_textures";
		block->UnlocalizedName = "vin_texture_insert";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string indexText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string objText = (*args)[1].substr(1, (*args)[1].length() - 1);
			std::string pathText = (*args)[2].substr(1, (*args)[2].length() - 1);

			if ((*args)[1][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + objText + "\"");
				return false;
			}

			double* objectId = VariableHandler::GetReal(objText.c_str());
			if (objectId == nullptr)
			{
				Logger::Error("variable \"" + objText + "\" does not exist");
				return false;
			}

			RuntimeObject* run = ObjectHandler::GetObject(*objectId);
			if (run == nullptr)
			{
				Logger::Error("object \"" + std::to_string(*objectId) + "\" does not exist");
				return false;
			}

			unsigned int textureId = 0;

			if ((*args)[0][0] == '0')
			{
				try
				{
					textureId = std::stod(indexText);
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + indexText + "\"");
					return false;
				}
			}
			else
			{
				double* value = VariableHandler::GetReal(indexText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + indexText + "\" does not exist");
					return false;
				}

				textureId = *value;
			}

			std::string path = std::string();

			if ((*args)[2][0] == '0')
				path = pathText;
			else
			{
				std::string* value = VariableHandler::GetString(pathText.c_str());
				if (value == nullptr)
				{
					Logger::Error("variable \"" + pathText + "\" does not exist");
					return false;
				}

				path = *value;
			}

			if (textureId > run->Textures.size())
			{
				Logger::Error("trying to insert a texture out of range. Object \"" + std::to_string(*objectId) + "\" only has " + std::to_string(run->Textures.size()) + " textures. Trying to insert texture \"" + pathText + "\"" + std::to_string(textureId));
				return false;
			}

			sf::Texture* txt = new sf::Texture();
			txt->loadFromFile(path);

			sf::Sprite* sp = new sf::Sprite();
			sp->setTexture(*txt);
			 
			ObjectHandler::ObjectMutex->lock();
			run->Textures.insert(run->Textures.begin() + textureId, txt);
			run->Sprites.insert(run->Sprites.begin() + textureId, sp);
			ObjectHandler::ObjectMutex->unlock();

			return true;
		};

		block->Execute = execute;
		BlockRegistry::CreateBlock(*block);
	}
	
	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();
		BlockArgument arg2 = BlockArgument();
		BlockArgument arg3 = BlockArgument();
		BlockArgument arg4 = BlockArgument();
		BlockArgument arg5 = BlockArgument();

		arg0.SetupTEXT("replace texture at");
		arg1.SetupREAL("00");
		arg2.SetupTEXT("for");
		arg3.SetupREAL("1var");
		arg4.SetupTEXT("with");
		arg5.SetupSTRING("0path");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);
		argList.push_back(arg3);
		argList.push_back(arg4);
		argList.push_back(arg5);

		block->Args = argList;
		block->Catagory = "vin_textures";
		block->UnlocalizedName = "vin_texture_replace";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string indexText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string objText = (*args)[1].substr(1, (*args)[1].length() - 1);
			std::string pathText = (*args)[2].substr(1, (*args)[2].length() - 1);

			if ((*args)[1][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + objText + "\"");
				return false;
			}

			double* objectId = VariableHandler::GetReal(objText.c_str());
			if (objectId == nullptr)
			{
				Logger::Error("variable \"" + objText + "\" does not exist");
				return false;
			}

			RuntimeObject* run = ObjectHandler::GetObject(*objectId);
			if (run == nullptr)
			{
				Logger::Error("object \"" + std::to_string(*objectId) + "\" does not exist");
				return false;
			}

			unsigned int textureId = 0;

			if ((*args)[0][0] == '0')
			{
				try
				{
					textureId = std::stod(indexText);
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + indexText + "\"");
					return false;
				}
			}
			else
			{
				double* value = VariableHandler::GetReal(indexText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + indexText + "\" does not exist");
					return false;
				}

				textureId = *value;
			}

			std::string path = std::string();

			if ((*args)[2][0] == '0')
				path = pathText;
			else
			{
				std::string* value = VariableHandler::GetString(pathText.c_str());
				if (value == nullptr)
				{
					Logger::Error("variable \"" + pathText + "\" does not exist");
					return false;
				}

				path = *value;
			}

			if (textureId >= run->Textures.size())
			{
				Logger::Error("trying to insert a texture out of range. Object \"" + std::to_string(*objectId) + "\" only has " + std::to_string(run->Textures.size()) + " textures. Trying to insert texture \"" + pathText + "\"" + std::to_string(textureId));
				return false;
			}

			ObjectHandler::ObjectMutex->lock();
			run->Textures[textureId]->loadFromFile(path);
			run->Sprites[textureId]->setTexture(*run->Textures[textureId]);
			ObjectHandler::ObjectMutex->unlock();
			
			return true;
		};

		block->Execute = execute;
		BlockRegistry::CreateBlock(*block);
	}
	
	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();
		BlockArgument arg2 = BlockArgument();
		BlockArgument arg3 = BlockArgument();

		arg0.SetupTEXT("switch texture to");
		arg1.SetupREAL("00");
		arg2.SetupTEXT("for");
		arg3.SetupSTRING("1var");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);
		argList.push_back(arg3);

		block->Args = argList;
		block->Catagory = "vin_textures";
		block->UnlocalizedName = "vin_texture_switch";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string indexText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string objText = (*args)[1].substr(1, (*args)[1].length() - 1);

			if ((*args)[1][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + objText + "\"");
				return false;
			}

			unsigned int textureId = 0;

			if ((*args)[0][0] == '0')
			{
				try
				{
					textureId = std::stod(indexText);
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + indexText + "\"");
					return false;
				}
			}
			else
			{
				double* value = VariableHandler::GetReal(indexText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + indexText + "\" does not exist");
					return false;
				}

				textureId = *value;
			}

			double* objectId = VariableHandler::GetReal(objText.c_str());
			if (objectId == nullptr)
			{
				Logger::Error("variable \"" + objText + "\" does not exist");
				return false;
			}

			RuntimeObject* run = ObjectHandler::GetObject(*objectId);
			if (run == nullptr)
			{
				Logger::Error("object \"" + std::to_string(*objectId) + "\" does not exist");
				return false;
			}

			run->ImageIndex = textureId;

			return true;
		};

		block->Execute = execute;
		BlockRegistry::CreateBlock(*block);
	}
}