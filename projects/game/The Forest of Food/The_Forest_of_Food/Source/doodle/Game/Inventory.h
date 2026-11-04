/*
File name : Inventory.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Hyeonseo Yoon
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include  "Item.h"
#include  "..\Engine\Rectangle.h"
#include <vector>

std::list<Item> inline InventoryItems;

class Inventory {
public:
	Inventory();
	void Load();
	void Draw();
	void Update();
	void UnLoad();

	void AddItem(Item* item);
	void SetItemInventory(Item* item);

	bool ShowInventory() { return open; }

protected:
	struct InvenRect : public Rectangle {
		InvenRect(std::pair< Vector2D<>, Vector2D<> > pair, Vector2D<int> hotSpot)
		: Rectangle(pair, hotSpot), name(""), haveItem(false) {}
		void SetItemName(Item* Itemname);
		bool haveItem;
		std::string name;
	};
	
	std::vector<InvenRect> square;
	
private:
	Input::InputKey openInventory;
	Camera cameraMatrix;
	int height;
	int width;
	double InventorySize;
	int InfoSize;
	int ItemNum;
	int EndInvenNum;
	bool open;

	Texture* Inven;
	Texture* InvenText;
	
	double WindowX = 0;
	double WindowY = 0;
	double MouseX = 0;
	double MouseY = 0;
};