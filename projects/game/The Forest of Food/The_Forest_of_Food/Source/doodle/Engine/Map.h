/*
File name : Map.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Sieun Choi
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include <string>
#include "Transform.h"
#include "Texture.h"


namespace Vec {

    struct POINT_2
    {

        Vector2D<>LeftBottom;
        Vector2D<>TopRight;

    };
}




class Map {
private:

    unsigned int Map_Season;
    Texture* pixel;
    const static int TileRow;
    Vec::POINT_2 Camerarange;
    TranslateMatrix objectMatrix;
    Vec::POINT_2 Map_Size;
    int Tile_Size = 0;
    bool Tilecheck;
    struct MAP_INFO
    {

        Vector2D<> pixel_pos;
        
        bool On_Object = false;

    };
	
    std::vector<MAP_INFO>map_infos;
public:
   
    Map();
	
    void Load(std::string& texture_path, int season);
    void Draw(TransformMatrix);
    void UnLoad();
    [[nodiscard]]const unsigned int& GetSeason() const { return Map_Season; }
     unsigned int& SetSeason(unsigned int season) { return Map_Season = season; }
    [[nodiscard]] Vec::POINT_2 GetMap_Size()const { return this->Map_Size; }
    [[nodiscard]] Vec::POINT_2 GetCameraRange()const { return this->Camerarange; }
    MAP_INFO& Get_Map_Info(int num) { return map_infos[num]; }
    const int &Get_Tile_Size()const { return Tile_Size; };
    std::vector<MAP_INFO>Get_Map_Array()const { return map_infos; }
    bool Tile_Check(int num);
    unsigned int Random_Number();
    
};

