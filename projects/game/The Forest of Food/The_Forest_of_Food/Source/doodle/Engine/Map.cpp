/*
File name : Map.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Sieun Choi
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/
#include "../Engine/Engine.h"
#include "../Game/Screens.h"

const int Map::TileRow = 58;

Map::Map() :pixel(nullptr),objectMatrix(Vector2D<>{0, 0}),Tilecheck(false)
{
	Camerarange = { {0,0},{0,0} };
	Map_Size = { {-1000.0,-1000.0},{3000,3000} };
	Map_Season = 0;
	Tile_Size = 3364;
}


void Map::Load(std::string& texture_path, const int season)
{
	

	switch (season)
	{
	case SPRING:{

		pixel = Engine::GetTextureManager().Load(texture_path);
		Map_Season = SPRING;
			
		Camerarange.LeftBottom = Map_Size.LeftBottom;
		Camerarange.TopRight = Map_Size.TopRight - Engine::GetWindow().GetSize();

		for (double j = Map_Size.LeftBottom.y;j < Map_Size.TopRight.y/*screen size로해야함.*/;j += pixel->GetSize().y - 20.0)
		{
			for (double i = Map_Size.LeftBottom.x;i < Map_Size.TopRight.x/*screen size로해야함.*/;i += pixel->GetSize().x - 20.0)
			{
				map_infos.push_back({ Vector2D<>(i,j),false });

				
			}
		}
		
		map_infos[2240].On_Object = true;map_infos[2239].On_Object = true;map_infos[2240 - TileRow].On_Object = true;
		map_infos[2241].On_Object = true;map_infos[2240 + TileRow].On_Object = true;
		Tile_Size = static_cast<int>(map_infos.size());

		break;
	}
	case SUMMER: {

		pixel = Engine::GetTextureManager().Load(texture_path);
		Map_Season = SUMMER;

		Camerarange.LeftBottom = Map_Size.LeftBottom;
		Camerarange.TopRight = Map_Size.TopRight - Engine::GetWindow().GetSize();

		for (double j = Map_Size.LeftBottom.y;j < Map_Size.TopRight.y;j += pixel->GetSize().y - 20.0)
		{
			for (double i = Map_Size.LeftBottom.x;i < Map_Size.TopRight.x;i += pixel->GetSize().x - 20.0)
			{
				map_infos.push_back({ Vector2D<>(i,j),false });
				if (i == 0 && j == 0) { map_infos.push_back({ Vector2D<>(i,j),true }); };
			}
		}
		Tile_Size = static_cast<int>(map_infos.size());

		break;
	}
	case FALL: {

		pixel = Engine::GetTextureManager().Load(texture_path);
		Map_Season = FALL;

		Camerarange.LeftBottom = Map_Size.LeftBottom;
		Camerarange.TopRight = Map_Size.TopRight - Engine::GetWindow().GetSize();

		for (double j = Map_Size.LeftBottom.y;j < Map_Size.TopRight.y;j += pixel->GetSize().y - 20.0)
		{
			for (double i = Map_Size.LeftBottom.x;i < Map_Size.TopRight.x;i += pixel->GetSize().x - 20.0)
			{
				map_infos.push_back({ Vector2D<>(i,j),false });
				if (i == 0 && j == 0) { map_infos.push_back({ Vector2D<>(i,j),true }); };
			}
		}

		Tile_Size = static_cast<int>(map_infos.size());
		break;
	}
	case WINTER: {

		pixel = Engine::GetTextureManager().Load(texture_path);
		Map_Season = WINTER;

		Camerarange.LeftBottom = Map_Size.LeftBottom;
		Camerarange.TopRight = Map_Size.TopRight - Engine::GetWindow().GetSize();

		for (double j = Map_Size.LeftBottom.y;j < Map_Size.TopRight.y;j += pixel->GetSize().y - 20.0)
		{
			for (double i = Map_Size.LeftBottom.x;i < Map_Size.TopRight.x;i += pixel->GetSize().x - 20.0)
			{
				map_infos.push_back({ Vector2D<>(i,j),false });
				if (i == 0 && j == 0) { map_infos.push_back({ Vector2D<>(i,j),true }); };
			}
		}
		Tile_Size = static_cast<int>(map_infos.size());
		break;
	}
	default:
		break;


	}


}


void Map::Draw(TransformMatrix cam)
{

	for (const auto Map_info : map_infos)
	{
		objectMatrix = TranslateMatrix(Map_info.pixel_pos);

		pixel->Draw(cam * objectMatrix);
		
	}


}

void Map::UnLoad()
{
	map_infos.clear();
}

bool Map::Tile_Check(int num)
{
	
	if (map_infos[num].On_Object == false && map_infos[__int64(num) - 1].On_Object == false && map_infos[__int64(num) + 1].On_Object == false)
	{
	if(num >TileRow*2 && num < Tile_Size - (TileRow*2) && map_infos[__int64(num) - TileRow].On_Object == false && map_infos[__int64(num) - (TileRow * 2)].On_Object == false
		&& map_infos[__int64(num) + TileRow].On_Object == false && map_infos[__int64(num) + (TileRow * 2)].On_Object == false)
	{
		return true;
	}

	return true;
	}
	return false;
	
}

unsigned int Map::Random_Number()
{
	std::random_device randomdevice;

	std::mt19937 generator(randomdevice());

	std::uniform_int_distribution<> randomnum(TileRow*2, Tile_Size-(TileRow*2));

	int num = randomnum(generator);
	
	if(Tile_Check(num)==true)
	{
		map_infos[num].On_Object = true;
		return num;
	}
	else
	{
		return num = randomnum(generator);
	}

}