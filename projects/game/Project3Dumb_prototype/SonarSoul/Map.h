/*
File name : Map.h
Game name : Sonar Soul
the course name  : GAM100
the term : Fall 2019
Author : Hoseob.jeong
¡°All content (C) 2019 DigiPen (USA) Corporation, all rights reserved.¡±
*/

#pragma once
#include <vector>
#include <doodle/doodle.hpp>
#include <fstream>
#include "classes.h"


const std::filesystem::path  mapFilePath{ "map.txt" };



class Map
{
private:
    int map_height =0;
    int map_width  = 0;
    

public:
    std::vector<int> tiles;
    int              width_position =0;
    int              height_position =0;
    int              get_width_num();
    int              get_height_num();
    int              put_width_num(int& width);
    int              put_height_num(int& height);
   
};
Map  ReadMap(const std::filesystem::path& map_filepath);
void setup_Map(Map& maps, std::vector<Block>& blocks, std::vector<Enemy>& enemies, Player& player, Gate& gate);
std::istream& operator>>(std::istream& instream, Map& map);
   