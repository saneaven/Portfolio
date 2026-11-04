/*
File name : MainCharacter.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Hyeonseo Yoon
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/
#include "MainCharacter.h"

MainCharacter::MainCharacter() :position({ 100, 0 }), scale({ 0.65, 0.65 }) {

    hungry = Engine::GetTextureManager().Load("Character/MainCharacter_Hungry.png");
    happy = Engine::GetTextureManager().Load("Character/mainCharacter_happy.png");
    soso = Engine::GetTextureManager().Load("Character/mainCharacter_soso.png");
    disappoint = Engine::GetTextureManager().Load("Character/mainCharacter_disappoint.png");
    WindowX = Engine::GetWindow().GetSize().x;
    WindowY = Engine::GetWindow().GetSize().y;
    objectMatrix = TranslateMatrix(position) * scale;
    face = MainCharacterFace::HUNGRY;
}


void MainCharacter::Draw() {
    switch (face)
    {
    case MainCharacterFace::HUNGRY:
        hungry->Draw(objectMatrix);
        break;
    case MainCharacterFace::HAPPY:
        happy->Draw(objectMatrix);
        break;
    case MainCharacterFace::DISAPPOINT:
        disappoint->Draw(objectMatrix);
        break;
    case MainCharacterFace::SOSO:
        soso->Draw(objectMatrix);
        break;;
    }
}

void MainCharacter::SetFace(MainCharacterFace some) {
    face = some;
}

void MainCharacter::SetPosition(Vector2D<> pos)
{
    position = pos;
    objectMatrix = TranslateMatrix(position) * scale;
}

void MainCharacter::SetScale(double , double )
{
    
}

void MainCharacter::DrawPlayerNameWithEngine() {
    doodle::push_settings();
    doodle::set_font(pixelFontStyle);
    doodle::set_font_size(48);
    doodle::set_fill_color(doodle::Color{ 0 });
    doodle::draw_text(PlayerName, (Engine::GetWindow().GetSize().x / 15.0) + 10, (Engine::GetWindow().GetSize().y / 3.0) + 5);
    doodle::pop_settings();
}

void MainCharacter::DrawPlayerNameWithLoad() {
    doodle::push_settings();
    doodle::set_font(pixelFontStyle);
    doodle::set_font_size(48);
    doodle::set_fill_color(doodle::Color{ 0 });
    doodle::draw_text(PlayerName, (1440.0 / 15.0) + 10, (800.0 / 3.0) + 5);
    doodle::pop_settings();
}


