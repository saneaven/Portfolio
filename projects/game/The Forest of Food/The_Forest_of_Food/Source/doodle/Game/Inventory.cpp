/*
File name : Inventory.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Hyeonseo Yoon
Author(secondary) : Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Inventory.h"
#include "doodle/doodle.hpp"


Inventory::Inventory() : height(4), width(8), InventorySize(128.0), open(false)
	, openInventory(Input::KeyboardButton::Z), InfoSize(64), ItemNum(24), EndInvenNum(32)
{
	Inven = Engine::GetTextureManager().Load("assets/inventory.png");
	InvenText = Engine::GetTextureManager().Load("assets/inventoryText.png");
	double setX = 15.0;
	double setY = 6.0;
	for (int j = 1; j <= height; ++j) {
		for (int i = 1; i <= width; ++i) {
				square.push_back(InvenRect({ { (InventorySize +5.0)* i + setX,  (InventorySize+5.0) * j + setY}
							, {(InventorySize + 5.0) * i + setX + InventorySize, (InventorySize + 5.0) * j + setY +InventorySize} }, { 0, 0 }));
			
		}
	}
	WindowX = 1440.0;
	WindowY = 800.0;
	ItemNum = 24;
	EndInvenNum = 32;
}

void Inventory::Load() {
	
}

void Inventory::Draw() {


	if(open) {
		for (auto size : square) {
			doodle::push_settings();
			Inven->Draw(TranslateMatrix(Vector2D<>{ WindowX/15.0, WindowY/10.0 }));
			for (auto& some : InventoryItems)
				some.Draw(cameraMatrix);
			doodle::pop_settings();
		}

		for (int num = 0; num < square.size(); num++) {
			if (square[num].isPointInside({ MouseX, MouseY })) {
				doodle::push_settings();
				doodle::set_fill_color(doodle::Color{ 0 });
				doodle::set_font_size(24);
				doodle::no_outline();
				if(square[num].haveItem == true){
					if(num >= 16){
						InvenText->SetPos({ MouseX - InvenText->GetSize().x/2, MouseY - InvenText->GetSize().y });
						InvenText->Draw();
						doodle::draw_text(square[num].name, MouseX-50, MouseY - InvenText->GetSize().y/2);
					}
					else if(num < 16){
						InvenText->SetPos({ MouseX - InvenText->GetSize().x/2, MouseY });
						InvenText->Draw();
						doodle::draw_text(square[num].name, MouseX - 50, MouseY + InvenText->GetSize().y / 2);
					}
				}
				doodle::pop_settings();
			}
		}
	}
	
}

void Inventory::Update() {
	MouseX = doodle::get_mouse_x();
	MouseY = doodle::get_mouse_y();
	if(open == false && openInventory.IsKeyRelease()) {
		open = true;
	}
	else if(open == true && openInventory.IsKeyRelease()) {
		open = false;
	}
}

void Inventory::AddItem(Item* item)
{
	SetItemInventory(item);
	if (ItemNum == EndInvenNum) {
		ItemNum -= width * 2;
		EndInvenNum -= width;
	}
	
	item->SetPostion({ square[ItemNum].GetPositionOf(Direction::leftDown).x, square[ItemNum].GetPositionOf(Direction::leftDown).y });
	square[ItemNum].SetItemName(item);
	ItemNum++;
	
	InventoryItems.push_back(std::move(*item));
}

void Inventory::SetItemInventory(Item* item) {
	switch (item->GetItemName()) {
	case Rice:
		item->SetTexture("cooking/rice_s.spt");
		break;
	case Cabbage:
		item->SetTexture("cooking/cabbage_s.spt");
		break;
	case Carrot:
		item->SetTexture("cooking/carrot_s.spt");
		break;
	case Chwinamul:
		item->SetTexture("cooking/Chwinamul_s.spt");
		break;
	case Egg:
		item->SetTexture("cooking/egg_s.spt");
		break;
	case Garlic:
		item->SetTexture("cooking/garlic_s.spt");
		break;
	case Ginseng:
		item->SetTexture("cooking/ginseng_s.spt");
		break;
	case Glutinous_Rice:
		item->SetTexture("cooking/glutinous_rice_s.spt");
		break;
	case Green_Onion:
		item->SetTexture("cooking/green_onion_s.spt");
		break;
	case Potato:
		item->SetTexture("cooking/potato_s.spt");
		break;
	case Sedum:
		item->SetTexture("cooking/Sedum_sarmentosum_s.spt");
		break;
	case Purse:
		item->SetTexture("cooking/Shepherd's_purse_s.spt");
		break;
	case Pine:
		item->SetTexture("cooking/pine_s.spt");
		break;
	case Enoki:
		item->SetTexture("cooking/enoki_s.spt");
		break;
	case Bean:
		item->SetTexture("cooking/tofu_s.spt");
		break;
	case Onion:
		item->SetTexture("cooking/onion_s.spt");
		break;
	case Turkey:
		item->SetTexture("cooking/turkey_s.spt");
		break;
	case Chicken:
		item->SetTexture("cooking/chicken_s.spt");
		break;
	case Jujube:
		item->SetTexture("cooking/jujube_s.spt");
		break;
	case Beef:
		item->SetTexture("cooking/beef_s.spt");
		break;
	case Pork:
		item->SetTexture("cooking/pork_s.spt");
		break;
	case Butter:
		item->SetTexture("cooking/butter_s.spt");
		break;
	case Broccoli:
		item->SetTexture("cooking/broccoli_s.spt");
		break;
	case Cucumber:
		item->SetTexture("cooking/cucumber_s.spt");
		break;
	case Eggplant:
		item->SetTexture("cooking/eggplant_s.spt");
		break;
	case Pepper:
		item->SetTexture("cooking/pepper_s.spt");
		break;
	case Radish:
		item->SetTexture("cooking/radish_s.spt");
		break;
	case Ramen:
		item->SetTexture("cooking/ramen_s.spt");
		break;
	default:;
	}
}

void Inventory::InvenRect::SetItemName(Item* item) {
	haveItem = true;
	InvenRect::name = item->GetStringName();
}


void Inventory::UnLoad() {
	InventoryItems.clear();
}




