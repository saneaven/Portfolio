/*
File name : Story.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Hyeonseo Yoon
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#include "Story.h"
#include "Cooking.h"
#include "Screens.h"


void Story::Load() {
	
	set_frame_of_reference(doodle::FrameOfReference::RightHanded_OriginBottomLeft);
	WindowX = Engine::GetWindow().GetSize().x;
	WindowY = Engine::GetWindow().GetSize().y;
	MouseX = doodle::get_mouse_x();
	MouseY = doodle::get_mouse_y();
	ConverationNum = 0;
	OnCapsLock = false;
	objectMatrix = TranslateMatrix(Vector2D<>{0, 0});
	SetBGM("sounds/story.ogg");
	room = Engine::GetTextureManager().Load("assets/room.jpg");
	ramen = Engine::GetTextureManager().Load("assets/ramen.png");
	ramen->SetPos({ (WindowX/4.0) * 1.0 , (WindowY/5.0)*3.0 - 5.0 });
	boolRamenCheck = false;
	itemRamen = new Item({ 0, 0 },{ 0, 0 }, "cooking/ramen_s.spt", "Ramen", ItemName::Ramen, { 0, 0 }, { 0,  0 });
	rectRamenCheck = { {{(WindowX / 4.0) * 1.0 , (WindowY / 5.0) * 3.0 - 5.0}
		, {(WindowX / 4.0) * 1.0 + ramen->GetSize().x , (WindowY / 5.0) * 3.0 - 5.0 + ramen->GetSize().y}}, {0, 0} };

	spring = Engine::GetTextureManager().Load("assets/notSpring.png");
	summer = Engine::GetTextureManager().Load("assets/notSummer.png");
	fall = Engine::GetTextureManager().Load("assets/notFall.png");
	winter = Engine::GetTextureManager().Load("assets/notWinter.png");
	InventoryItems.clear();
	storyInven = new Inventory;
}

void Story::Draw() {
	doodle::clear_background(doodle::HexColor{ 0xFFFFFFFF });
	if(ConverationNum != 13){
		if (ConverationNum != 0) {
			room->Draw(objectMatrix);
			if (boolRamenCheck == false) {
				ramen->Draw();
			}
			doodle::push_settings();
			doodle::set_fill_color(255, 200);
			doodle::draw_rectangle(0, 0, 1600, 900);
			doodle::pop_settings();
			if (ConverationNum > 0) {
				character.Draw();
			}
			if (ConverationNum > 3) {
				faity.Draw();
			}
		}
		conversation();
	}
	else if(ConverationNum == 13){
		DrawChooseMap();
	}
}

void Story::Update(double) {
	set_frame_of_reference(doodle::FrameOfReference::RightHanded_OriginBottomLeft);
	WindowX = Engine::GetWindow().GetSize().x;
	WindowY = Engine::GetWindow().GetSize().y;
	MouseX = doodle::get_mouse_x();
	MouseY = doodle::get_mouse_y();
	if(boolRamenCheck == false){
		if (rectRamenCheck.isPointInside({MouseX, MouseY}) && mouseClick.MouseOneClick()){
			storyInven->AddItem(itemRamen);
			boolRamenCheck = true;
		}
	}
	
	objectMatrix = TranslateMatrix(Vector2D<>{0, 0})* scale;
	if (NextCheck.isPointInside({ MouseX, MouseY }) && ConverationNum != 13 && ConverationNum != 0) {
		if (mouseClick.MouseOneClick()) {
			ConverationNum++;
		}
	}

	UpdateChooseMap();
}

void Story::StateChange() {
	switch (map.GetSeason())
	{
	case SPRING:
		Engine::GetGameStateManager().SetNextState(SPRING);
		break;
	case SUMMER:
		Engine::GetGameStateManager().SetNextState(SUMMER);
		break;
	case FALL:
		Engine::GetGameStateManager().SetNextState(FALL);
		break;
	case WINTER:
		Engine::GetGameStateManager().SetNextState(WINTER);
		break;
	}
}

void Story::conversation() {
	double conversationSetX = (WindowX / 15) + 10;
	double conversationSetY = WindowY / 15.0 * 4.0;
	doodle::push_settings();
	doodle::set_outline_color(doodle::Color{ 0 });
	doodle::set_outline_width(1);
	doodle::set_fill_color(doodle::Color{ 255 , 230 });
	doodle::draw_rectangle(WindowX / 15.0, WindowY / 15.0
		, (WindowX / 15.0) * 13.0, (WindowY / 15.0) * 4.0);
	doodle::draw_rectangle(WindowX / 15.0, WindowY / 3.0
		, (WindowX / 15.0) * 2.0, (WindowY / 15.0) * 1.0);
	doodle::pop_settings();
	doodle::set_font(pixelFontStyle);
	doodle::push_settings();
	doodle::set_font_size(32);
	doodle::set_fill_color(doodle::Color{ 0 });
	switch (ConverationNum)
	{
	case 0:
		MakePlayerName();
		character.DrawPlayerNameWithEngine();
		break;
	case 1:
		doodle::draw_text("so, hungry", conversationSetX, conversationSetY);
		character.SetFace(MainCharacterFace::HUNGRY);
		character.DrawPlayerNameWithEngine();
		break;
	case 2:
		doodle::draw_text("I bought new games a while ago. I have no money and no food at home...", conversationSetX, conversationSetY);
		character.SetFace(MainCharacterFace::HUNGRY);
		character.DrawPlayerNameWithEngine();
		break;
	case 3:
		doodle::draw_text("Because of the Steam sale....", conversationSetX, conversationSetY);
		character.SetFace(MainCharacterFace::SOSO);
		character.DrawPlayerNameWithEngine();
		break;
	case 4:
		doodle::draw_text("Are you hungry?", conversationSetX, conversationSetY);
		faity.SetFace(FairyFace::SOSO);
		faity.DrawFairyName();
		break;
	case 5:
		doodle::draw_text("Who are you?!", conversationSetX, conversationSetY);
		character.SetFace(MainCharacterFace::DISAPPOINT);
		character.DrawPlayerNameWithEngine();
		break;
	case 6:
		doodle::draw_text("I am cute fairy of The Forest of Food. I heard you were hungry.", conversationSetX, conversationSetY);
		faity.SetFace(FairyFace::HAPPY);
		faity.DrawFairyName();
		break;
	case 7:
		doodle::draw_text("Fairy?? what nonsense.", conversationSetX, conversationSetY);
		character.SetFace(MainCharacterFace::SOSO);
		character.DrawPlayerNameWithEngine();
		break;
	case 8:
		doodle::draw_text("It doesn't matter, aren't you hungry?", conversationSetX, conversationSetY);
		faity.SetFace(FairyFace::SOSO);
		faity.DrawFairyName();
		break;
	case 9:
		doodle::draw_text("Yeah...I'm hungry. I don't have the money to buy food.", conversationSetX, conversationSetY);
		character.SetFace(MainCharacterFace::HUNGRY);
		character.DrawPlayerNameWithEngine();
		break;
	case 10:
		doodle::draw_text("Then I'll give you food?", conversationSetX, conversationSetY);
		faity.SetFace(FairyFace::SOSO);
		faity.DrawFairyName();
		break;
	case 11:
		doodle::draw_text("Yes!! Give me anything!!", conversationSetX, conversationSetY);
		character.SetFace(MainCharacterFace::HAPPY);
		character.DrawPlayerNameWithEngine();
		break;
	case 12:
		doodle::draw_text("Choose the season you like.", conversationSetX, conversationSetY);
		faity.SetFace(FairyFace::HAPPY);
		faity.DrawFairyName();
		break;
	case 13:
		
		break;
	case 14:
		if(map.GetSeason() == SPRING){
			doodle::draw_text("You picked Spring, I'll give you Bom Namul Bibimbap.", conversationSetX, conversationSetY);
		}
		else if(map.GetSeason() == SUMMER){
			doodle::draw_text("You picked Summer, I'll give you Samgyetang.", conversationSetX, conversationSetY);
		}
		else if(map.GetSeason() == FALL){
			doodle::draw_text("You picked Fall, I'll give you Grilled Turkey.", conversationSetX, conversationSetY);
		}
		else if(map.GetSeason() == WINTER){
			doodle::draw_text("You picked Winter, I'll give you Jeongol.", conversationSetX, conversationSetY);
		}
		faity.SetFace(FairyFace::HAPPY);
		faity.DrawFairyName();
		break;
	case 15:
		if (map.GetSeason() == SPRING) {
			doodle::draw_text("Wow! It's the rice I've seen in a long time! \nAnd there are some soft-boiled eggs!", conversationSetX, conversationSetY);
		}
		else if (map.GetSeason() == SUMMER) {
			doodle::draw_text("Wow! The glutinous rice in Samgyetang is the best! \nChicken is also delicious!", conversationSetX, conversationSetY);
		}
		else if (map.GetSeason() == FALL) {
			doodle::draw_text("This is my first time to see Grilled Turkey. Vegetables that I've seen in a long \ntime. And I really like onions!", conversationSetX, conversationSetY);
		}
		else if (map.GetSeason() == WINTER) {
			doodle::draw_text("Jeongol! There's beef instead of pork in this food, right! \nThere are also various mushrooms.", conversationSetX, conversationSetY);
		}
		character.SetFace(MainCharacterFace::HAPPY);
		character.DrawPlayerNameWithEngine();
		break;
	case 16:
		if (map.GetSeason() == SPRING) {
			doodle::draw_text("There are three types of Bom Namul.", conversationSetX, conversationSetY);
		}
		else if (map.GetSeason() == SUMMER) {
			doodle::draw_text("There are healthy ginseng, jujube, and crispy green onion.", conversationSetX, conversationSetY);
		}
		else if (map.GetSeason() == FALL) {
			doodle::draw_text("There are also in Potatoes, carrots, garlic, and even sweet butter!", conversationSetX, conversationSetY);
		}
		else if (map.GetSeason() == WINTER) {
			doodle::draw_text("Cabbage, green onions, and soft tofu are indispensable!", conversationSetX, conversationSetY);
		}
		faity.SetFace(FairyFace::HAPPY);
		faity.DrawFairyName();
		break;
	case 17:
		doodle::draw_text("I had a good meal! Thank you fairy.", conversationSetX, conversationSetY);
		character.SetFace(MainCharacterFace::HAPPY);
		character.DrawPlayerNameWithEngine();
		break;
	case 18:
		doodle::draw_text("I'm glad you had a good meal. Shall we go to the forest now?", conversationSetX, conversationSetY);
		faity.SetFace(FairyFace::HAPPY);
		faity.DrawFairyName();
		break;
	case 19:
		doodle::draw_text("Forest? What are you talking about?", conversationSetX, conversationSetY);
		character.SetFace(MainCharacterFace::DISAPPOINT);
		character.DrawPlayerNameWithEngine();
		break;
	case 20:
		doodle::draw_text("There's nothing for free in the world. Since you got food from me and ate it, \nyou have to cook it too.", conversationSetX, conversationSetY);
		faity.SetFace(FairyFace::HAPPY);
		faity.DrawFairyName();
		break;
	case 21:
		doodle::draw_text("No, then I didn't eat it!! I'm not going!!", conversationSetX, conversationSetY);
		character.SetFace(MainCharacterFace::DISAPPOINT);
		character.DrawPlayerNameWithEngine();
		break;
	case 22:
		doodle::draw_text("You already ate food, so you have no choice! Let's go to The Forest of Food!", conversationSetX, conversationSetY);
		faity.SetFace(FairyFace::HAPPY);
		faity.DrawFairyName();
		break;
	default:
		StateChange();
		doodle::draw_text("You already have food, so you have no choice! Let's go to The Forest of Food!", conversationSetX, conversationSetY);
		faity.DrawFairyName();
		break;
	}
	doodle::pop_settings();
}

void Story::UnLoad() {
	UnloadBGM();
	itemRamen = nullptr;
	ramen = nullptr;
	room = nullptr;
	spring = nullptr;
	summer = nullptr;
	fall = nullptr;
	winter = nullptr;
	storyInven = nullptr;
}


void Story::UpdateChooseMap() {
	double BoxX = (WindowX / 20.0) * 8;
	double SpringY = (WindowY / 30.0) * 20;
	double SummerY = (WindowY / 30.0) * 15;
	double FallY = (WindowY / 30.0) * 10;
	double WinterY = (WindowY / 30.0) * 5;
	Vector2D<> BoxSize = { 250, 100 };
	
	if(rectSpringCheck.isPointInside({MouseX, MouseY})){
		spring = Engine::GetTextureManager().Load("assets/spring.png");
		if(mouseClick.MouseOneClick()){
			SetChooseBox(SPRING);
		}
	}
	else{
		spring = Engine::GetTextureManager().Load("assets/notSpring.png");
	}
	if (rectSummerCheck.isPointInside({ MouseX, MouseY })) {
		summer = Engine::GetTextureManager().Load("assets/summer.png");
		if (mouseClick.MouseOneClick()) {
			SetChooseBox(SUMMER);
		}
	}
	else {
		summer = Engine::GetTextureManager().Load("assets/notSummer.png");
	}
	if (rectFallCheck.isPointInside({ MouseX, MouseY })) {
		fall = Engine::GetTextureManager().Load("assets/fall.png");
		if (mouseClick.MouseOneClick()) {
			SetChooseBox(FALL);
		}
	}
	else {
		fall = Engine::GetTextureManager().Load("assets/notFall.png");
	}
	if (rectWinterCheck.isPointInside({ MouseX, MouseY })) {
		winter = Engine::GetTextureManager().Load("assets/winter.png");
		if (mouseClick.MouseOneClick()) {
			SetChooseBox(WINTER);
		}
	}
	else {
		winter = Engine::GetTextureManager().Load("assets/notWinter.png");
	}


	rectSpringCheck = { {{BoxX, SpringY},{BoxSize.x + BoxX, BoxSize.y + SpringY} }, {0, 0} };
	rectSummerCheck = { {{BoxX, SummerY},{BoxX + BoxSize.x, SummerY + BoxSize.y}}, {0, 0} };
	rectFallCheck = { {{BoxX, FallY}, {BoxX + BoxSize.x, FallY + BoxSize.y}}, {0, 0} };
	rectWinterCheck = { {{BoxX, WinterY},{BoxX + BoxSize.x, WinterY + BoxSize.y}}, {0, 0} };
	spring->SetPos({ BoxX, SpringY });
	summer->SetPos({ BoxX, SummerY });
	fall->SetPos({ BoxX, FallY });
	winter->SetPos({ BoxX, WinterY });
}

void Story::DrawChooseMap() {
	doodle::clear_background(doodle::HexColor{ 0x6ab6f6FF });
	spring->Draw();
	summer->Draw();
	fall->Draw();
	winter->Draw();
}



void Story::SetChooseBox(int season) {
	map.SetSeason(season);
	CheckSeaonCooking = season;
	ConverationNum++;
}




void Story::MakePlayerName() {
	doodle::push_settings();
	doodle::set_fill_color(0);
	doodle::set_font_size(50);
	doodle::draw_text("Once you have entered your name, \n   click on the dialog below", WindowX / 4.0, (WindowY / 5.0) * 3.0);

	if (PlayerName.size() < 6) {
		if (OnCapsLock == false) {
			if (A_key.IsKeyRelease()) {
				PlayerName += "a";
			}
			else if (B_key.IsKeyRelease()) {
				PlayerName += "b";
			}
			else if (C_key.IsKeyRelease()) {
				PlayerName += "c";
			}
			else if (D_key.IsKeyRelease()) {
				PlayerName += "d";
			}
			else if (E_key.IsKeyRelease()) {
				PlayerName += "e";
			}
			else if (F_key.IsKeyRelease()) {
				PlayerName += "f";
			}
			else if (G_key.IsKeyRelease()) {
				PlayerName += "g";
			}
			else if (H_key.IsKeyRelease()) {
				PlayerName += "h";
			}
			else if (I_key.IsKeyRelease()) {
				PlayerName += "i";
			}
			else if (J_key.IsKeyRelease()) {
				PlayerName += "j";
			}
			else if (K_key.IsKeyRelease()) {
				PlayerName += "k";
			}
			else if (L_key.IsKeyRelease()) {
				PlayerName += "l";
			}
			else if (M_key.IsKeyRelease()) {
				PlayerName += "m";
			}
			else if (N_key.IsKeyRelease()) {
				PlayerName += "n";
			}
			else if (O_key.IsKeyRelease()) {
				PlayerName += "o";
			}
			else if (P_key.IsKeyRelease()) {
				PlayerName += "p";
			}
			else if (Q_key.IsKeyRelease()) {
				PlayerName += "q";
			}
			else if (R_key.IsKeyRelease()) {
				PlayerName += "r";
			}
			else if (S_key.IsKeyRelease()) {
				PlayerName += "s";
			}
			else if (T_key.IsKeyRelease()) {
				PlayerName += "t";
			}
			else if (U_key.IsKeyRelease()) {
				PlayerName += "u";
			}
			else if (V_key.IsKeyRelease()) {
				PlayerName += "v";
			}
			else if (W_key.IsKeyRelease()) {
				PlayerName += "w";
			}
			else if (X_key.IsKeyRelease()) {
				PlayerName += "x";
			}
			else if (Y_key.IsKeyRelease()) {
				PlayerName += "y";
			}
			else if (Z_key.IsKeyRelease()) {
				PlayerName += "z";
			}
			else if (Caps_Key.IsKeyRelease() && OnCapsLock == false) {
				OnCapsLock = true;
			}
			else if (Caps_Key.IsKeyRelease() && OnCapsLock == true) {
				OnCapsLock = false;
			}
			else if (Back_Key.IsKeyRelease() && PlayerName.size() > 0) {
				PlayerName.erase(PlayerName.size() - 1);
			}
		}
		else if (OnCapsLock) {
			if (A_key.IsKeyRelease()) {
				PlayerName += "A";
			}
			else if (B_key.IsKeyRelease()) {
				PlayerName += "B";
			}
			else if (C_key.IsKeyRelease()) {
				PlayerName += "C";
			}
			else if (D_key.IsKeyRelease()) {
				PlayerName += "D";
			}
			else if (E_key.IsKeyRelease()) {
				PlayerName += "E";
			}
			else if (F_key.IsKeyRelease()) {
				PlayerName += "F";
			}
			else if (G_key.IsKeyRelease()) {
				PlayerName += "G";
			}
			else if (H_key.IsKeyRelease()) {
				PlayerName += "H";
			}
			else if (I_key.IsKeyRelease()) {
				PlayerName += "I";
			}
			else if (J_key.IsKeyRelease()) {
				PlayerName += "J";
			}
			else if (K_key.IsKeyRelease()) {
				PlayerName += "K";
			}
			else if (L_key.IsKeyRelease()) {
				PlayerName += "L";
			}
			else if (M_key.IsKeyRelease()) {
				PlayerName += "M";
			}
			else if (N_key.IsKeyRelease()) {
				PlayerName += "N";
			}
			else if (O_key.IsKeyRelease()) {
				PlayerName += "O";
			}
			else if (P_key.IsKeyRelease()) {
				PlayerName += "P";
			}
			else if (Q_key.IsKeyRelease()) {
				PlayerName += "Q";
			}
			else if (R_key.IsKeyRelease()) {
				PlayerName += "R";
			}
			else if (S_key.IsKeyRelease()) {
				PlayerName += "S";
			}
			else if (T_key.IsKeyRelease()) {
				PlayerName += "T";
			}
			else if (U_key.IsKeyRelease()) {
				PlayerName += "U";
			}
			else if (V_key.IsKeyRelease()) {
				PlayerName += "V";
			}
			else if (W_key.IsKeyRelease()) {
				PlayerName += "W";
			}
			else if (X_key.IsKeyRelease()) {
				PlayerName += "X";
			}
			else if (Y_key.IsKeyRelease()) {
				PlayerName += "Y";
			}
			else if (Z_key.IsKeyRelease()) {
				PlayerName += "Z";
			}
			else if (Caps_Key.IsKeyRelease() && OnCapsLock == false) {
				OnCapsLock = true;
			}
			else if (Caps_Key.IsKeyRelease() && OnCapsLock == true) {
				OnCapsLock = false;
			}
			else if (Back_Key.IsKeyRelease() && PlayerName.size() > 0) {
				PlayerName.erase(PlayerName.size() - 1);
			}

		}
	}
	else {
		if (Back_Key.IsKeyRelease() && PlayerName.size() > 0) {
			PlayerName.erase(PlayerName.size() - 1);
		}
	}
	if (NextCheck.isPointInside({ MouseX, MouseY })) {
		if (mouseClick.MouseOneClick() && PlayerName != "") {
			ConverationNum++;
		}
	}
	doodle::pop_settings();

}


