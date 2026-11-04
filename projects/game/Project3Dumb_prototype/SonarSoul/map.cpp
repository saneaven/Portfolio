/*
File name : map.cpp
Game name : Sonar Soul
the course name  : GAM100
the term : Fall 2019
Author :  Hoseob Jeong
¡°All content (C) 2019 DigiPen (USA) Corporation, all rights reserved.¡±
*/

#include "Map.h"





int Map::get_width_num() 
{
	return this->map_width; 
}

int Map::get_height_num() 
{ 
	return this->map_height; 
}

int Map::put_width_num(int& width) 
{ 
	return map_width = width;
}

int Map::put_height_num(int& height) 
{
	return map_height = height;
}



std::istream& operator>>(std::istream& instream, Map& map)
{
	std::string str;
	int width;
	int height;
	instream >> str;
	if (!instream || str != "width")
	{
		std::cout << "Failed to read an width string" << std::endl;
	}
	instream >> width;
	if (!instream)
	{
		std::cout << "Failed to read an int for the map width" << std::endl;
	}
	instream >> str;
	if (!instream || str != "height")
	{
		std::cout<<"Failed to read an height string"<<std::endl;
	}
	instream >> height;
	if (!instream)
	{
		std::cout<<"Failed to read an int for the map height" <<std::endl;
	}
	map.put_height_num(height);
	map.put_width_num(width);

	map.tiles.resize(map.get_height_num() * map.get_width_num());
	for (int i = 0; i < map.tiles.size(); ++i)
		instream >> map.tiles[i];

	return instream;
}



Map ReadMap(const std::filesystem::path& map_filepath)
{
	Map map;
	std::ifstream inputFileStream{ map_filepath };
	if (!inputFileStream)
		std::cout<<"failed to open file" << std::endl;
	inputFileStream.exceptions(inputFileStream.exceptions() | std::ios_base::badbit);

	inputFileStream >> map;

	if (!inputFileStream && !inputFileStream.eof())
		std::cout <<"failed to read map" <<std::endl;

	return map;
}

void setup_Map( Map & maps, std::vector<Block>&blocks, std::vector<Enemy>& enemies, Player& player, Gate& gate) {

    
	for (int y = 0; y <maps.get_height_num(); y++)
	{
		for (int x = 0; x < maps.get_width_num(); x++)
		{
            switch (maps.tiles[y * maps.get_width_num() + x])
            {
                case 0: 
					break;
				case -1:
					player = Player(x * BLOCKSIZE, -y * BLOCKSIZE, 25, 40, 1);
					player.spawnPoint = Vec2(x * BLOCKSIZE, -y * BLOCKSIZE);
					break;
                case 1: 
                    blocks.push_back(Block(x * BLOCKSIZE ,-y * BLOCKSIZE ));
                    break;
                case 2:
					enemies.push_back(Enemy(x * BLOCKSIZE, -y * BLOCKSIZE, 30, 30, 30, Syllable::Do));
					break;
				case 3:
					enemies.push_back(Enemy(x * BLOCKSIZE, -y * BLOCKSIZE, 30, 30, 30, Syllable::Re));
					break;
				case 4:
					enemies.push_back(Enemy(x * BLOCKSIZE, -y * BLOCKSIZE, 30, 30, 30, Syllable::Mi));
					break;
				case 5:
					enemies.push_back(Enemy(x * BLOCKSIZE, -y * BLOCKSIZE, 30, 30, 30, Syllable::Fa));
					break;
				case 6:
					enemies.push_back(Enemy(x * BLOCKSIZE, -y * BLOCKSIZE, 30, 30, 30, Syllable::Sol));
					break;
				case 7:
					enemies.push_back(Enemy(x * BLOCKSIZE, -y * BLOCKSIZE, 30, 30, 30, Syllable::La));
					break;
				case 8:
					enemies.push_back(Enemy(x * BLOCKSIZE, -y * BLOCKSIZE, 30, 30, 30, Syllable::Ti));
					break;
				case 9:
					enemies.push_back(Enemy(x * BLOCKSIZE, -y * BLOCKSIZE, 30, 30, 30, Syllable::Do_high));
					break;
				case 10:
					gate = Gate(Vec2(x * BLOCKSIZE, -y * BLOCKSIZE), { Syllable::Do,Syllable::Re ,Syllable::Mi });
					break;
            }
		}
	}
}
