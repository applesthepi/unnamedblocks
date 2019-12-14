#pragma once

#include "ContentObjects.h"
#include "ContentTextures.h"
#include "ContentTransform.h"
#include "ContentBytes.h"
#include "ContentVariables.h"
#include "ContentThreading.h"
#include "ContentOperations.h"
#include "ContentExecution.h"

class ContentLoader
{
public:
	static void LoadContent()
	{
		{
			RegCatagory* cat = new RegCatagory();
			cat->Color = sf::Color(237, 231, 109);
			cat->DisplayName = "objects";
			cat->UnlocalizedName = "vin_objects";

			BlockRegistry::CreateCatagory(*cat);
		}

		{
			RegCatagory* cat = new RegCatagory();
			cat->Color = sf::Color(68, 212, 68);
			cat->DisplayName = "textures";
			cat->UnlocalizedName = "vin_textures";

			BlockRegistry::CreateCatagory(*cat);
		}

		{
			RegCatagory* cat = new RegCatagory();
			cat->Color = sf::Color(70, 200, 200);
			cat->DisplayName = "transformation";
			cat->UnlocalizedName = "vin_transform";

			BlockRegistry::CreateCatagory(*cat);
		}

		{
			RegCatagory* cat = new RegCatagory();
			cat->Color = sf::Color(140, 200, 170);
			cat->DisplayName = "bytes";
			cat->UnlocalizedName = "vin_bytes";

			BlockRegistry::CreateCatagory(*cat);
		}

		{
			RegCatagory* cat = new RegCatagory();
			cat->Color = sf::Color(255, 161, 74);
			cat->DisplayName = "variables";
			cat->UnlocalizedName = "vin_variables";

			BlockRegistry::CreateCatagory(*cat);
		}

		{
			RegCatagory* cat = new RegCatagory();
			cat->Color = sf::Color(110, 125, 230);
			cat->DisplayName = "threading";
			cat->UnlocalizedName = "vin_threading";

			BlockRegistry::CreateCatagory(*cat);
		}

		{
			RegCatagory* cat = new RegCatagory();
			cat->Color = sf::Color(73, 143, 76);
			cat->DisplayName = "operations";
			cat->UnlocalizedName = "vin_operations";

			BlockRegistry::CreateCatagory(*cat);
		}

		{
			RegCatagory* cat = new RegCatagory();
			cat->Color = sf::Color(255, 113, 74);
			cat->DisplayName = "execution";
			cat->UnlocalizedName = "vin_execution";

			BlockRegistry::CreateCatagory(*cat);
		}

		ContentLoadObjects();
		ContentLoadTextures();
		ContentLoadTransform();
		ContentLoadBytes();
		ContentLoadVariables();
		ContentLoadThreading();
		ContentLoadOperations();
		ContentLoadExecution();
	}
};