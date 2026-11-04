# An Omen In The Mirror

*Designing and building the interface of a first-person narrative puzzle game in Unreal Engine.*

## Overview

An Omen In The Mirror is a first-person, narrative-driven puzzle game made in Unreal Engine at DigiPen. Progress comes from reading dialogue, inspecting objects and piecing the story together, which makes the interface load-bearing. The UI is how the player understands the world, not just an overlay on top of it.

This folder documents my part of that game: the user interface, designed end to end and implemented in-engine. It holds the high-fidelity UI concepts and the matching in-game screenshots that prove each surface survived contact with the running build.

## My Role

This was a DigiPen team project. I owned the interface end to end: designing the dialogue, inspection, menu and HUD systems and then implementing them in-engine with UMG. This is a UI/UX scope: I did not write the game's broader gameplay or engine systems. The artifacts here are the UI design work and the in-game results.

## UI systems

Every surface the player touches was designed and built as part of this scope:

- **Dialogue & choice**, the core narrative surface: a dialogue panel with selectable responses and clear hover / clicked states, paced to keep reading comfortable and the choice obvious.
- **Response selection**: selection and hover states give immediate feedback so the player always knows what they are about to pick, without breaking the scene.
- **Inspection & crosshair**: a context-aware crosshair signals when something is interactable, and a dedicated inspection view lets the player examine objects up close to solve puzzles.
- **HUD**: a quiet heads-up display that surfaces only what the player needs in the moment, staying out of the way of a first-person, atmosphere-led game.
- **Menus**: a main menu and pause menu that bookend the experience.
- **Ending & credits**: the closing screen that rolls the team credits and returns the player out, bookending the main menu they came in through.

## Process

The work followed a concept-to-engine pipeline:

1. **Concept first.** Each surface started as a high-fidelity concept (main menu, dialogue, interaction) establishing type, colour and layout before any engine work. The concepts use placeholder copy to focus on layout and state.
2. **Built in UMG.** The approved concepts were rebuilt as UMG widgets in Unreal, wired to game state so they behaved correctly in a running build, not just in a mockup.
3. **Iterated in place.** Seeing the UI inside the actual game surfaced spacing, legibility and pacing issues that only show up in motion, and those fed back into the design.

The `UI Concepts` and `UI Screenshots` folders sit side by side so the concept and its realized in-game counterpart can be compared directly.

## Tech stack

- **Engine:** Unreal Engine
- **UI:** UMG (Unreal Motion Graphics)
- **Design:** UI concepts, custom type and layout (custom fonts bundled with the concept assets)

## Project structure

```
An Omen In The Mirror/
├── UI Concepts/          High-fidelity concept mockups for each surface
│   ├── Ingame Dialogue UI*.png        Dialogue, selection/hover and clicked states
│   ├── Ingame Interaction UI*.png     Crosshair (interactable) and inspection
│   ├── DigiPen_RGB_White*.png         DigiPen logo marks (+ logo zip)
│   ├── New concepts/                  Iterated menu concepts + bundled fonts
│   ├── New dialogue/                  Revised dialogue concepts
│   └── Asset/                         Shared UI assets (musical note, slider thumb,
│                                      polygon, grouped art + legacy bundle)
└── UI Screenshots/       In-engine captures of the built UI
    ├── MainMenu.png / Mainmenu2.png / PauseMenu.png
    ├── Dialogue.png / Selection.png
    ├── Inspection.png
    ├── HUD.png
    └── Ending.png                     End/credits screen
```

## Links

- [Source on GitHub](https://github.com/saneaven/Portfolio/tree/main/projects/game/An%20Omen%20In%20The%20Mirror)

## Context

DigiPen team project. My contribution: UI/UX design and in-game implementation.
