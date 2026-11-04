/*
File name : Cooking.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Hyeonseo Yoon
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#include "Cooking.h"
#include "Screens.h"


Cooking::Cooking() {}

void Cooking::Load() {
	square.clear();
	materials.clear();
	controlMaterialBool(false);
	WindowX = 1500;
	WindowY = 800;
	Inven = Engine::GetTextureManager().Load("assets/noInven.png");
	Mark = Engine::GetTextureManager().Load("assets/Mark.png");
	CheckItems();
	MakeCookingBox();
	SetIems();
	maincharacter.SetPosition({ -200, 0 });
	maincharacter.SetFace(MainCharacterFace::HAPPY);
	if(Engine::GetMap().GetSeason() == FALL)
	{
		CookingPot.Load("assets/cookingoven_s.spt");
	}
	else {
		CookingPot.Load("assets/cookingPot_s.spt");
	}
	cameraMatrix = TranslateMatrix({ Engine::GetWindow().GetSize()/2 });
	SetBGM("sounds/cookingStage.ogg");
	EndCookingTime = 3.0;
	finishChoose = false;
}

void Cooking::Draw() {
	if(finishChoose == false){
		doodle::clear_background(doodle::HexColor{ 0xE8E5B5ff });
		maincharacter.Draw();
		Inven->Draw({ WindowX / 4.0 + 40, WindowY / 3.0 - 10 });
		for (auto item : materials)
			item->Draw();
		DrawCookingBox();
		conversation();
	}
	else{
		doodle::clear_background(doodle::HexColor{ 0xffffffff });
		CookingPot.Draw(cameraMatrix);
	}
}

void Cooking::Update(double dt)
{
	MouseX = doodle::get_mouse_x();
	MouseY = doodle::get_mouse_y();

	if (NextCheck.isPointInside({ MouseX, MouseY })){
		if (mouseClick.MouseOneClick()) {
			CookingPot.PlayAnimation(0);
			UnloadBGM();
			SetBGM("sounds/pot.ogg");
			finishChoose = true;
		}
	}

	if(finishChoose){
		CookingPot.Update(dt);
		EndCookingTime -= dt;
		if (EndCookingTime <= 0) {
			if (CheckFood()){
				if(square[Ramen].OnClick){
					Engine::GetGameStateManager().SetNextState(HIDDENENDING);
				}
				else{
					Engine::GetGameStateManager().SetNextState(GOODENDING);
				}
			}
			else{
				
				Engine::GetGameStateManager().SetNextState(BADENDING);
			}
		}
	}


	if(openMaterial.IsKeyRelease()){
		controlMaterialBool(true);
		materials.clear();
		SetIems();
	}
}

void Cooking::UnLoad()
{
	UnloadBGM();
	InventoryItems.clear();
}

void Cooking::conversation()
{
	doodle::push_settings();
	doodle::set_outline_color(doodle::Color{ 0 });
	doodle::set_outline_width(1);
	doodle::set_fill_color(doodle::Color{ 255 , 230 });
	doodle::draw_rectangle(WindowX / 15.0, WindowY / 15.0
		, (WindowX / 15.0) * 13.0, (WindowY / 15.0) * 4.0);
	doodle::draw_rectangle(WindowX / 15.0, WindowY / 3.0
		, (WindowX / 15.0) * 2.0, (WindowY / 15.0) * 1.0);
	doodle::set_font(pixelFontStyle);
	doodle::set_font_size(32);
	doodle::set_fill_color(doodle::Color{ 0 });
	switch (ConverationNum)
	{
	case 0:
		maincharacter.DrawPlayerNameWithLoad();
		doodle::draw_text("After selecting the ingredients, click the dialog.", (WindowX / 15) + 10, (WindowY / 15.0) * 4.0);
		break;
	default:
		break;
	}
	doodle::pop_settings();
}

void Cooking::CheckItems() {
	for (auto items : InventoryItems) {
		switch (items.GetItemName()) {
		case ItemName::Rice:
			rice = true;
			break;
		case ItemName::Chwinamul:
			chwinalmul = true;
			break;
		case ItemName::Sedum:
			sedum = true;
			break;
		case ItemName::Purse:
			purse = true;
			break;
		case ItemName::Bean:
			tofu = true;
			break;
		case ItemName::Enoki:
			enoki = true;
			break;
		case ItemName::Pine:
			pine = true;
			break;
		case ItemName::Cabbage:
			cabbage = true;
			break;
		case ItemName::Green_Onion:
			green_onion = true;
			break;
		case ItemName::Onion:
			onion = true;
			break;
		case ItemName::Potato:
			potato = true;
			break;
		case ItemName::Carrot:
			carrot = true;
			break;
		case ItemName::Garlic:
			garlic = true;
			break;
		case ItemName::Egg:
			egg = true;
			break;
		case ItemName::Ginseng:
			ginseng = true;
			break;
		case ItemName::Glutinous_Rice:
			glutinous = true;
			break;
		case ItemName::Butter:
			butter = true;
			break;
		case ItemName::Turkey:
			turkey = true;
			break;
		case ItemName::Chicken:
			chicken = true;
			break;
		case ItemName::Jujube:
			jujube = true;
			break;
		case ItemName::Beef:
			beef = true;
			break;
		case ItemName::Pork:
			pork = true;
			break;
		case ItemName::Broccoli:
			brocooli = true;
			break;
		case ItemName::Cucumber:
			cucumber = true;
			break;
		case ItemName::Eggplant:
			eggplant = true;
			break;
		case ItemName::Pepper:
			pepper = true;
			break;
		case ItemName::Radish:
			radish = true;
			break;
		case ItemName::Ramen:
			ramen = true;
			break;
		default:
			break;
		}
	}
}

void Cooking::MakeCookingBox() {
	for (int j = 1; j <= height; ++j) {
		for (int i = 1; i <= width; ++i) {
			square.push_back(Square({ {128.0 * i + WindowX / 5.0, 128.0 * j - WindowY / 250.0 + 150}
						, {128.0 * i + WindowX / 5.0 + BoxSize, 128.0 * j - WindowY / 250.0 + 150 + BoxSize} }, { 0, 0 }));

		}
	}
}

void Cooking::DrawCookingBox() {
	doodle::push_settings();
	for (auto& box : square) {
		if (box.isPointInside({ MouseX, MouseY })) {
			if (doodle::MouseIsPressed && breakMouseClick) {
				MouseRelese = true;
			}
			else if (doodle::MouseIsPressed == false) {
				breakMouseClick = true;
			}

			if (box.OnClick && MouseRelese && box.have) {
				box.OnClick = false;
				breakMouseClick = false;
				MouseRelese = false;
			}
			else if (box.OnClick == false && MouseRelese && box.have) {
				box.OnClick = true;
				breakMouseClick = false;
				MouseRelese = false;
			}
		}

		if (box.OnClick == false) {
			doodle::no_fill();
		}
		else if (box.OnClick) {
			doodle::set_fill_color(doodle::Color{ 228, 238, 139, 200 });
			Mark->Draw({ box.GetPositionOf(Direction::leftDown).x, box.GetPositionOf(Direction::leftDown).y });
		}
	}
	doodle::pop_settings();
}

void Cooking::SetIems() {
	int start = 21;
	int end = 27;
	PushItems("cooking/rice.png", rice);
	PushItems("cooking/cabbage.png", cabbage);
	PushItems("cooking/carrot.png", carrot);
	PushItems("cooking/Chwinamul.png", chwinalmul);
	PushItems("cooking/egg.png", egg);
	PushItems("cooking/garlic.png", garlic);
	PushItems("cooking/ginseng.png", ginseng);
	PushItems("cooking/glutinous_rice.png", glutinous);
	PushItems("cooking/green_onion.png", green_onion);
	PushItems("cooking/potato.png", potato);
	PushItems("cooking/Sedum_sarmentosum.png", sedum);
	PushItems("cooking/Shepherd's_purse.png", purse);
	PushItems("cooking/pine.png", pine);
	PushItems("cooking/enoki.png", enoki);
	PushItems("cooking/tofu.png", tofu);
	PushItems("cooking/onion.png", onion);
	PushItems("cooking/turkey.png", turkey);
	PushItems("cooking/chicken.png", chicken);
	PushItems("cooking/jujube.png", jujube);
	PushItems("cooking/beef.png", beef);
	PushItems("cooking/pork.png", pork);
	PushItems("cooking/butter.png", butter);
	PushItems("cooking/broccoli.png", brocooli);
	PushItems("cooking/cucumber.png", cucumber);
	PushItems("cooking/eggplant.png", eggplant);
	PushItems("cooking/pepper.png", pepper);
	PushItems("cooking/radish.png", radish);
	PushItems("cooking/ramen.png", ramen);


	for(auto temp: materials) {
		temp->SetPos(square[start].GetPositionOf(Direction::leftDown));
		if(temp->have) {
			square[start].have = true;
		}
		if (start == end)
		{
			start -= width * 2;
			end -= width;
		}
		start++;
	}

}

void Cooking::PushItems(const std::filesystem::path& filePath, bool some) {
	if(some) {
		materials.push_back(new CookingItem(filePath, true));
	}
	else {
		materials.push_back(new CookingItem("cooking/something.png"));
	}
}


bool Cooking::CheckFood() {
	switch (CheckSeaonCooking) {
	case SPRING:
		if (square[Rice].OnClick && square[Egg].OnClick && square[Purse].OnClick
			&& square[Chwinamul].OnClick && square[Sedum].OnClick && square[Beef].OnClick)
			return true;
		else
			return false;
	case SUMMER:
		if (square[Glutinous_Rice].OnClick && square[Jujube].OnClick && square[Ginseng].OnClick
			&& square[Green_Onion].OnClick && square[Chicken].OnClick)
			return true;
		else
			return false;
	case FALL:
		if (square[Turkey].OnClick && square[Onion].OnClick && square[Potato].OnClick 
			&& square[Carrot].OnClick && square[Garlic].OnClick && square[Butter].OnClick)
			return true;
		else
			return false;
	case WINTER:
		if (square[Bean].OnClick && square[Pork].OnClick && square[Enoki].OnClick 
			&& square[Pine].OnClick && square[Cabbage].OnClick && square[Green_Onion].OnClick)
			return true;
		else
			return false;
	default:
		break;
	}
	return false;
}

void Cooking::controlMaterialBool(bool some) {
	if(some){
		rice = true;
		chwinalmul = true;
		cabbage = true;
		carrot = true;
		sedum = true;
		egg = true;
		purse = true;
		pine = true;
		enoki = true;
		garlic = true;
		ginseng = true;
		glutinous = true;
		green_onion = true;
		potato = true;
		tofu = true;
		butter = true;
		onion = true;
		turkey = true;
		chicken = true;
		jujube = true;
		beef = true;
		pork = true;
		brocooli = true;
		cucumber = true;
		eggplant = true;
		pepper = true;
		radish = true;
		ramen = true;
	}
	else{
		rice = false;
		chwinalmul = false;
		cabbage = false;
		carrot = false;
		sedum = false;
		egg = false;
		purse = false;
		pine = false;
		enoki = false;
		garlic = false;
		ginseng = false;
		glutinous = false;
		green_onion = false;
		potato = false;
		tofu = false;
		butter = false;
		onion = false;
		turkey = false;
		chicken = false;
		jujube = false;
		beef = false;
		pork = false;
		brocooli = false;
		cucumber = false;
		eggplant = false;
		pepper = false;
		radish = false;
		ramen = false;
	}
}
