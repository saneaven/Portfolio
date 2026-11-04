#include <pch.h>
#include "Projects/ProjectOne.h"
#include "Agent/CameraAgent.h"

void ProjectOne::setup()
{
    /*
    // Create an agent (using the default "Agent::AgentModel::Man" model)
    auto man = agents->create_behavior_agent("ExampleAgent", BehaviorTreeTypes::Example);

    // You can change properties here or at runtime from a behavior tree leaf node
    // Look in Agent.h for all of the setters, like these:
    // man->set_color(Vec3(1, 0, 1));
    // man->set_scaling(Vec3(7,7,7));
    // man->set_position(Vec3(100, 0, 100));

    // Create an agent with a different 3D model:
    // 1. (optional) Add a new 3D model to the framework other than the ones provided:
    //    A. Find a ".sdkmesh" model or use https://github.com/walbourn/contentexporter
    //       to convert fbx files (many end up corrupted in this process, so good luck!)
    //    B. Add a new AgentModel enum for your model in Agent.h (like the existing Man or Tree).
    // 2. Register the new model with the engine, so it associates the file path with the enum
    //    A. Here we are registering all of the extra models that already come in the package.
    Agent::add_model("Assets\\tree.sdkmesh", Agent::AgentModel::Tree);
    Agent::add_model("Assets\\car.sdkmesh", Agent::AgentModel::Car);
    Agent::add_model("Assets\\bird.sdkmesh", Agent::AgentModel::Bird);
    Agent::add_model("Assets\\ball.sdkmesh", Agent::AgentModel::Ball);
    Agent::add_model("Assets\\hut.sdkmesh", Agent::AgentModel::Hut);
    // 3. Create the agent, giving it the correct AgentModel type.
    auto tree = agents->create_behavior_agent("ExampleAgent2", BehaviorTreeTypes::Example, Agent::AgentModel::Car);
    // 4. (optional) You can also set the pitch of the model, if you want it to be rotated differently
    tree->set_pitch(PI / 2);
    // 5. (optional) Set other aspects to make it start out correctly
    tree->set_color(Vec3(0, 0.5, 0));   // Set the tree to green

    // You can technically load any map you want, even create your own map file,
    // but behavior agents won't actually avoid walls or anything special, unless you code
    // that yourself (that's the realm of project 2)
    terrain->goto_map(0);

    // You can also enable the pathing layer and set grid square colors as you see fit.
    // Works best with map 0, the completely blank map
    terrain->pathLayer.set_enabled(true);
    terrain->pathLayer.set_value(0, 0, Colors::Red);

    // Camera position can be modified from this default
    auto camera = agents->get_camera_agent();
    camera->set_position(Vec3(-62.0f, 70.0f, terrain->mapSizeInWorld * 0.5f));
    camera->set_pitch(0.610865); // 35 degrees

    // Sound control (these sound functions can be kicked off in a behavior tree node - see the example in L_PlaySound.cpp)
    audioManager->SetVolume(0.5f);
    audioManager->PlaySoundEffect(L"Assets\\Audio\\retro.wav");
    // Uncomment for example on playing music in the engine (must be .wav)
    // audioManager->PlayMusic(L"Assets\\Audio\\motivate.wav");
    // audioManager->PauseMusic(...);
    // audioManager->ResumeMusic(...);
    // audioManager->StopMusic(...);
    */
    
    Agent::add_model("Assets\\tree.sdkmesh", Agent::AgentModel::Tree);
    Agent::add_model("Assets\\car.sdkmesh", Agent::AgentModel::Car);
    Agent::add_model("Assets\\bird.sdkmesh", Agent::AgentModel::Bird);
    Agent::add_model("Assets\\ball.sdkmesh", Agent::AgentModel::Ball);
    Agent::add_model("Assets\\hut.sdkmesh", Agent::AgentModel::Hut);

    auto runningLeader = agents->create_behavior_agent("ParticipantLeader", BehaviorTreeTypes::ParticipantLeader);
    runningLeader->set_position(RNG::world_position());
    runningLeader->set_yaw(RNG::range(-PI, PI));
    Blackboard& bboard1 = runningLeader->get_blackboard();
    bboard1.set_value<std::string>("Order", "");
    bboard1.set_value<std::string>("Status", "Normal");
    bboard1.set_value<float>("Circular Position", 0.);
    runningLeader->set_color({ .33,.45,.75 });


    auto runningTroopR1 = agents->create_behavior_agent("ParticipantFollower", BehaviorTreeTypes::ParticipantFollower);
    runningTroopR1->set_position(RNG::world_position());
    runningTroopR1->set_yaw(RNG::range(-PI, PI));
    Blackboard& bboardR1 = runningTroopR1->get_blackboard();
    bboardR1.set_value<size_t>("Leader ID", runningLeader->get_id());
    bboardR1.set_value<Vec3>("Delta Position", { - 12.0, 0., 12.0 });
    bboardR1.set_value<float>("Circular Position", 2*PI/5.);

    auto runningTroopR2 = agents->create_behavior_agent("ParticipantFollower", BehaviorTreeTypes::ParticipantFollower);
    runningTroopR2->set_position(RNG::world_position());
    runningTroopR2->set_yaw(RNG::range(-PI, PI));
    Blackboard& bboardR2 = runningTroopR2->get_blackboard();
    bboardR2.set_value<size_t>("Leader ID", runningLeader->get_id());
    bboardR2.set_value<Vec3>("Delta Position", { - 24.0, 0., 24.0 });
    bboardR2.set_value<float>("Circular Position", 4 * PI / 5.);
    bboardR2.set_value<size_t>("Partner ID", runningTroopR1->get_id());
    bboardR1.set_value<size_t>("Partner ID", runningTroopR2->get_id());


    auto runningTroopL1 = agents->create_behavior_agent("ParticipantFollower", BehaviorTreeTypes::ParticipantFollower);
    runningTroopL1->set_position(RNG::world_position());
    runningTroopL1->set_yaw(RNG::range(-PI, PI));
    Blackboard& bboardL1 = runningTroopL1->get_blackboard();
    bboardL1.set_value<size_t>("Leader ID", runningLeader->get_id());
    bboardL1.set_value<Vec3>("Delta Position", { - 12.0, 0., -12.0 });
    bboardL1.set_value<float>("Circular Position", 6 * PI / 5.);

    auto runningTroopL2 = agents->create_behavior_agent("ParticipantFollower", BehaviorTreeTypes::ParticipantFollower);
    runningTroopL2->set_position(RNG::world_position());
    runningTroopL2->set_yaw(RNG::range(-PI, PI));
    Blackboard& bboardL2 = runningTroopL2->get_blackboard();
    bboardL2.set_value<size_t>("Leader ID", runningLeader->get_id());
    bboardL2.set_value<Vec3>("Delta Position", { -24.0, 0., -24.0 });
    bboardL2.set_value<float>("Circular Position", 8 * PI / 5.);
    bboardL2.set_value<size_t>("Partner ID", runningTroopL1->get_id());
    bboardL1.set_value<size_t>("Partner ID", runningTroopL2->get_id());


    // You can technically load any map you want, even create your own map file,
    // but behavior agents won't actually avoid walls or anything special, unless you code
    // that yourself (that's the realm of project 2)
    terrain->goto_map(0);

    // You can also enable the pathing layer and set grid square colors as you see fit.
    // Works best with map 0, the completely blank map
    terrain->pathLayer.set_enabled(true);
    terrain->pathLayer.set_value(0, 0, Colors::Red);

    //// Camera position can be modified from this default
    auto camera = agents->get_camera_agent();
    camera->set_position(Vec3(-62.0f, 70.0f, terrain->mapSizeInWorld * 0.5f));
    camera->set_pitch(0.610865); // 35 degrees

    // Sound control (these sound functions can be kicked off in a behavior tree node - see the example in L_PlaySound.cpp)
    audioManager->SetVolume(0.5f);
    audioManager->PlaySoundEffect(L"Assets\\Audio\\retro.wav");

}