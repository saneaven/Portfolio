/*
File name : Item.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Hyeonseo Yoon
Author(secondary) : Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "..\Engine\Object.h"
#include  <string>
#include <list>
#include <vector>
#include "ItemName.h"


class Knife : public Object {
public:

    Knife(Vector2D<> position, Vector2D<int> hotspot, std::pair< Vector2D<int>, Vector2D<int> > hitbox
        , const std::string& filePath, std::string name) : Object(position, hotspot, hitbox, filePath, name),isFlipped(true),coolTimer(NULL),durationTimer(NULL)
    {
        AddSFX("Attack", "sounds/stick.ogg");
        sprite.PlayAnimation(0);
        objectMatrix *= ScaleMatrix({ -1.0, 1.0 });
    }
    void Update(GameObjectManager& objectManager, double dt) override;

    void useKnife();

    bool isFlipped;
private:

    constexpr static double duration = 0.25, coolTime = 0.5;
    double durationTimer, coolTimer;
};



class Item :public Object {
public:
    Item(Vector2D<> pos, Vector2D<int>hotspot, const std::string& filePath, std::string name, ItemName itemName, Vector2D<> leftDown, Vector2D<> rightUp)
        :Object(pos, hotspot, { leftDown,rightUp }, filePath, name),itemName(itemName) 
    {};


    void Draw(Camera CameraMatrix) {
        objectMatrix = TranslateMatrix(position);
        Object::Draw(CameraMatrix);
    }
    void SetPostion(Vector2D<> pos);
    void SetTexture(const std::string& filePath);
    void Update(GameObjectManager& objectManager, double dt) override;
    ItemName GetItemName() { return itemName; }
    std::string GetStringName() { return GetName(); }
    void SetExplanation(std::string Setexplanation) { explanation = Setexplanation; }
    std::string GetExplanation() { return explanation; }
	
private:
    ItemName itemName;
    std::string explanation;
};

class Items {
public:
    
    void Load(unsigned int season);
    void Draw(Camera CameraMatrix, unsigned int season);
    void Update(GameObjectManager& objectManager, double dt);
    void UnLoad();
    void AddItems(Item* item) { items.push_back(item); }
    std::list<Item*> items;

private:

};

