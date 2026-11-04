/*
File name : ConsoleMain.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Rudy Castan
Author(Second):Hyeonseo Yoon, Minjae Kyung, Sieun Choi
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/


#include <doodle/window.hpp>
#include <doodle/doodle.hpp>
#include <exception>
#include <iostream>
#include "Engine/Engine.h"
#include "Game/Ending.h"
#include "Game/Splash.h"
#include "Game/Story.h"
#include "Game/Spring.h"
#include "Game/Summer.h"
#include "Game/Fall.h"
#include "Game/Winter.h"
#include "Game/Cooking.h"
#include "Game/Credit.h"
#include "Game/HowToplay.h"
#include "Game/Main_UI.h"


int main(void)
try
{
    doodle::create_window(1600, 900);
    Engine& engine = Engine::Instance();
    engine.Init("The forest of food");
    
    while (!doodle::is_window_closed())
    {
        Splash splash;
        MainMenu main;
        Story story;
        Tutorial howtoplay;
        CreditScreen credit_screen;
        Spring spring;
        Summer summer;
        Fall fall;
        Winter winter;
        Cooking cooking;
        GoodEnding  good_ending;
        BadEnding  bad_ending;
        HiddenEnding hidden_ending;

        engine.GetGameStateManager().AddGameState(splash);
        engine.GetGameStateManager().AddGameState(main);
        engine.GetGameStateManager().AddGameState(story);
        engine.GetGameStateManager().AddGameState(howtoplay);
        engine.GetGameStateManager().AddGameState(credit_screen);
        engine.GetGameStateManager().AddGameState(spring);
        engine.GetGameStateManager().AddGameState(summer);
        engine.GetGameStateManager().AddGameState(fall);
        engine.GetGameStateManager().AddGameState(winter);
        engine.GetGameStateManager().AddGameState(cooking);
        engine.GetGameStateManager().AddGameState(good_ending);
        engine.GetGameStateManager().AddGameState(bad_ending);
        engine.GetGameStateManager().AddGameState(hidden_ending);


        while (engine.HasGameEnded() == false) {
            engine.Update();
        }
        engine.Shutdown();


    }
    return 0;
}
catch (std::exception& e)
{
    std::cerr << e.what() << "\n";
    return -1;
}
