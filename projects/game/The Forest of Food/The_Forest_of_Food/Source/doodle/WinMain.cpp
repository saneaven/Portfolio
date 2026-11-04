/*
File name : WinMain.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Rudy Castan
Author(Second):Hyeonseo Yoon, Minjae Kyung, Sieun Choi
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/


#include "Engine/Engine.h"
#include "Game/Splash.h"
#include "Game/Story.h"
#include "Game/Spring.h"
#include "Game/Summer.h"
#include "Game/Fall.h"
#include "Game/Winter.h"
#include "Game/Cooking.h"
#include <doodle/window.hpp>
#include <doodle/doodle.hpp>
#include <exception>
#include <sdkddkver.h>
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <Windows.h>
#include "Game/Credit.h"
#include "Game/Ending.h"
#include "Game/HowToplay.h"
#include "Game/Main_UI.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow)
try
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    doodle::create_window(1400, 900);
    Engine& engine = Engine::Instance();
    engine.Init("The forest of food");
    while (!doodle::is_window_closed())
    {
        doodle::update_window();
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
    MessageBox(nullptr, e.what(), "Error", MB_OK | MB_ICONERROR);
    return -1;
}
