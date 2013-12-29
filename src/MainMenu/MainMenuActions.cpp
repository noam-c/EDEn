/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "MainMenu.h"
#include "ExecutionStack.h"
#include "GraphicsUtil.h"
#include "Music.h"
#include "Sound.h"

#include <Rocket/Core.h>
#include "MenuShell.h"
#include "TitleScreenSettingsMenu.h"
#include "TileEngine.h"
#include "FadeState.h"
#include "BlendState.h"
#include "HomeMenu.h"
#include "PlayerData.h"

#define CHAP1 "chapter1"
#define SAVE_GAME "data/savegames/savegame1.edd"
//Actions for each of the list ops in the title screen

/**
 * 'New Game' was selected. Push a TileEngine state and fade to black.
 * [this will eventually change to a chapter selection list, with the fade
 * and pushed state (field or battle) changing based on the chapter].
 */
void MainMenu::NewGameAction(Rocket::Core::Event* event)
{
   TileEngine* tileEngine = new TileEngine(m_gameContext, CHAP1);
   getExecutionStack()->pushState(tileEngine, new FadeState(m_gameContext, this));
   m_chooseSound->play();
   Music::fadeOutMusic(1000);
}

/**
 * 'Menu Prototype' was selected. Push a Menu state.
 * \todo This will eventually be removed entirely, as it is only a programmer convenience right now.
 */
void MainMenu::MenuPrototypeAction(Rocket::Core::Event* event)
{
   getCurrentPlayerData().load(SAVE_GAME);
   getExecutionStack()->pushState(new HomeMenu(m_gameContext));
}

/**
 * 'Load Game' was selected. (TODO)
 */
void MainMenu::LoadGameAction(Rocket::Core::Event* event)
{
}

/**
 * 'Options' was selected.
 * Load up the options menu interface.
 */
void MainMenu::OptionsAction(Rocket::Core::Event* event)
{
   TitleScreenSettingsMenu* titleScreenSettingsMenuState = new TitleScreenSettingsMenu(m_gameContext);
   getExecutionStack()->pushState(
      titleScreenSettingsMenuState,
      new BlendState(m_gameContext, this, titleScreenSettingsMenuState, 500));
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
   m_finished = true;
   m_chooseSound->play();
   Music::fadeOutMusic(1000);

   GraphicsUtil::getInstance()->FadeToColor(0.0f, 0.0f, 0.0f, 1000);
}
