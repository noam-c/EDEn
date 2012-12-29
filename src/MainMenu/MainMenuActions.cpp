/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "MainMenu.h"
#include "GameContext.h"
#include "ExecutionStack.h"
#include "GraphicsUtil.h"
#include "Music.h"
#include "Sound.h"

#include <Rocket/Core.h>
#include "MenuShell.h"
#include "TileEngine.h"
#include "FadeState.h"
#include "HomeMenu.h"
#include "PlayerData.h"

#define CHAP1 "chapter1"
#define SAVE_GAME "data/savegames/savegamejson.edd"
//Actions for each of the list ops in the title screen

/**
 * 'New Game' was selected. Push a TileEngine state and fade to black.
 * [this will eventually change to a chapter selection list, with the fade
 * and pushed state (field or battle) changing based on the chapter].
 */
void MainMenu::NewGameAction(Rocket::Core::Event* event)
{
   TileEngine* tileEngine = new TileEngine(gameContext, CHAP1);
   gameContext.getExecutionStack().pushState(tileEngine, new FadeState(gameContext, this));
   chooseSound->play();
   Music::fadeOutMusic(1000);
}

/**
 * 'Menu Prototype' was selected. Push a Menu state.
 * \todo This will eventually be removed entirely, as it is only a programmer convenience right now.
 */
void MainMenu::MenuPrototypeAction(Rocket::Core::Event* event)
{
   PlayerData* playerData = new PlayerData();
   playerData->load(SAVE_GAME);

   gameContext.getExecutionStack().pushState(new HomeMenu(gameContext, *playerData));
}

/**
 * 'Load Game' was selected. (TODO)
 */
void MainMenu::LoadGameAction(Rocket::Core::Event* event)
{
}

/**
 * 'Options' was selected.
 * Perform any transitions necessary and load up the options menu interface. (TODO)
 */
void MainMenu::OptionsAction(Rocket::Core::Event* event)
{
}

/**
 * 'About' was selected. (TODO)
 */
void MainMenu::AboutAction(Rocket::Core::Event* event)
{
}

/**
 * 'Quit Game' was selected. Signal state logic termination, fade to black.
 */
void MainMenu::QuitAction(Rocket::Core::Event* event)
{
   finished = true;
   chooseSound->play();

   if (!MUSIC_OFF)
   {
      Music::fadeOutMusic(1000);
   }

   GraphicsUtil::getInstance()->FadeToColor(0.0f, 0.0f, 0.0f, 1000);
   titleDocument->Close();
}
