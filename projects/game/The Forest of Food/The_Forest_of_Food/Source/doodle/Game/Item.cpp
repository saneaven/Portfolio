/*
File name : Item.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Hyeonseo Yoon
Author(secondary) : Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/
#include  "Item.h"
#include "../Engine/Engine.h"
#include "../Engine/GameObjectManager.h"
#include "Screens.h"

void Items::Load(unsigned int)
{
	items.clear();

	for (int i = 0;i < 2;i++) {

		//ect.
		items.push_front(new Item({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos },
			{ 0, 0 }, "assets/broccoli_s.spt", "Brocooli", ItemName::Broccoli, { 4, 5 }, { 81,  57}));
		items.push_front(new Item({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos },
			{ 0, 0 }, "assets/cucumber_s.spt", "Cucumber", ItemName::Cucumber, { 10, 0 }, { 75,  78 }));
		items.push_front(new Item({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos },
			{ 0, 0 }, "assets/eggplant_s.spt", "Eggplant", ItemName::Eggplant, { 7, 3 }, { 66,  72 }));
		items.push_front(new Item({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos },
			{ 0, 0 }, "assets/pepper_s.spt", "Pepper", ItemName::Pepper, { 5, 0 }, { 75,  66 }));
		items.push_front(new Item({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos },
			{ 0, 0 }, "assets/radish_s.spt", "Radish", ItemName::Radish, { 7, 5 }, { 78,  75 }));
		
		//Spring
		items.push_front(new Item({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos },
			{ 0, 0 }, "assets/rice_s.spt", "Rice", ItemName::Rice, { 6, 1 }, { 92, 77 }));
		items.push_front(new Item({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos },
			{ 0, 0 }, "assets/Chwinamul_s.spt", "Chwinamul", ItemName::Chwinamul, { 27, 35 }, { 68, 90 }));
		items.push_front(new Item({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos },
			{ 0, 0 }, "assets/sedum_s.spt", "Sedum\n sarmentosum", ItemName::Sedum, { 27, 43 }, { 75, 66 }));
		items.push_front(new Item({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos },
			{ 0, 0 }, "assets/shepherd_s.spt", "Shepherd's\n purse", ItemName::Purse, { 30, 43 }, { 65, 67 }));
		if (Engine::GetMap().GetSeason() == SPRING)
		{
			items.push_front(new Item({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos },
				{ 0,0 }, "assets/egg_s.spt", "Egg", ItemName::Egg, { 9,0 }, { 39,37 }));
			items.push_front(new Item({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos },
				{ 0,0 }, "assets/egg_s.spt", "Egg", ItemName::Egg, { 9,0 }, { 39,37 }));
			
		}
		//Summer
		items.push_front(new Item({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos },
			{ 0, 0 }, "assets/ginseng_s.spt", "Ginseng", ItemName::Ginseng, { 33, 7 }, { 60, 51 }));
		items.push_front(new Item({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos },
			{ 0, 0 }, "assets/glutinous_rice_s.spt", "Glutinous", ItemName::Glutinous_Rice, { 6, 1 }, { 92, 77 }));
		if (Engine::GetMap().GetSeason() == SUMMER)
		{
			items.push_front(new Item({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos },
				{ 0,0 }, "assets/jujube_s.spt", "Jujube", ItemName::Jujube, { 0,0 }, { 100,100 }));
		}

		//Fall
		items.push_front(new Item({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos },
			{ 0, 0 }, "assets/onion_s.spt", "Onion", ItemName::Onion, { 30, 1 }, { 69, 69 }));
		items.push_front(new Item({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos },
			{ 0, 0 }, "assets/potato_s.spt", "Potato", ItemName::Potato, { 18, 8 }, { 77, 58 }));
		items.push_front(new Item({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos },
			{ 0, 0 }, "assets/carrot_s.spt", "Carrot", ItemName::Carrot, { 32, 4 }, { 63, 46 }));
		items.push_front(new Item({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos },
			{ 0, 0 }, "assets/garlic_s.spt", "Garlic", ItemName::Garlic, { 33, 4 }, { 65, 78 }));
		if (Engine::GetMap().GetSeason() == FALL)
		{
			items.push_front(new Item({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos },
				{ 0,0 }, "assets/butter_s.spt", "Butter", ItemName::Butter, { 0,0 }, { 100,100 }));
		}
		//Winter
		items.push_front(new Item({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos },
			{ 0, 0 }, "assets/bean_s.spt", "Bean(Tofu)", ItemName::Bean, { 13, 10 }, { 76, 82 }));
		items.push_front(new Item({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos },
			{ 0, 0 }, "assets/cabbage_s.spt", "Cabbage", ItemName::Cabbage, { 16, 38 }, { 80, 74 }));

		if (Engine::GetMap().GetSeason() == WINTER)
		{
			items.push_front(new Item({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos },
				{ 0,0 }, "assets/Enoki_s.spt", "Enoki\n Mushroom", ItemName::Enoki, { 0,0 }, { 100,100 }));
			items.push_front(new Item({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos },
				{ 0,0 }, "assets/pine_mushroom_s.spt", "Pine\n mushroom", ItemName::Pine, { 0,0 }, { 100,100 }));
		}
		items.push_front(new Item({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos },
			{ 0, 0 }, "assets/Green_onion_s.spt", "Green\n Onion", ItemName::Green_Onion, { 36, 1 }, { 61, 60 }));

	}

	for (Item* i : items)
	{
		i->Load();
	}
}

void Items::Draw(Camera CameraMatrix, unsigned int)
{

	for (auto& temp : items)
		temp->Draw(CameraMatrix);

}
void Items::Update(GameObjectManager& objManager, double dt)
{
	for (auto& temp : items)
		temp->Update(objManager, dt);

}

void Items::UnLoad() {

	for (auto temp : items)
	{
		temp->Unload();
		delete temp;
	}

	items.clear();
}


void Knife::Update(GameObjectManager& objManager, double dt)
{
	position = objManager.player.GetPosition() + objManager.player.GetHotSpot(1);

	sprite.Update(dt);

    if (durationTimer > 0)
    {
        Direction temp;
       for(int i = 0; i< objManager.animalObjects.GetAnimals().size();i++)
       {
           Creature* copy = objManager.animalObjects.GetAnimals()[i];
           temp = CollisionCheck(copy, dt);
           std::cout << temp;

           if (temp != Direction::notD)
           {
               std::cout << "Direction : " << temp << std::endl;
               std::cout << "[Animal] \n";
			   copy->CollisionInteractWithKnife(objManager, dt);
           }
       }
        durationTimer -= dt;
    }
	coolTimer -= dt;

	objectMatrix = TranslateMatrix(position);
	if (isFlipped == true)
	{
		objectMatrix *= ScaleMatrix({ -1.0, 1.0 });
	}
	else if (isFlipped == false)
	{
		objectMatrix *= ScaleMatrix({ 1.0,1.0 });
	}


	HitboxUpdate();
}

#include "doodle/doodle.hpp"
void Knife::useKnife()
{
	if (coolTimer <= 0)
	{
		durationTimer = duration;
		coolTimer = coolTime;
		PlaySFX("Attack");
		sprite.PlayAnimation(1);
	}
}

void Item::SetPostion(Vector2D<> pos) {
	position = pos;
}

void Item::SetTexture(const std::string& filePath) {
	sprite.Load(filePath);
}

void Item::Update(GameObjectManager&, double)
{
	HitboxUpdate();

}
