/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
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
#include "TransitionState.h"
#include "FadeState.h"
#include "BlendState.h"
#include "HomeMenu.h"
#include "SaveMenu.h"
#include "PlayerData.h"
#include "ScreenTexture.h"

#define CHAP1 "chapter1"
#define SAVE_GAME "data/savegames/savegame1.edd"
//Actions for each of the list ops in the title screen

/**
 * 'New Game' was selected. Push a TileEngine state and fade to black.
 * [this will eventually change to a chapter selection list, with the fade
 * and pushed state (field or battle) changing based on the chapter].
 */
void MainMenu::NewGameAction()
{
   auto playerData = std::make_shared<PlayerData>(getMetadata());
   auto tileEngine = std::make_shared<TileEngine>(m_gameContext, playerData, CHAP1);
   auto blendTransition = std::make_shared<FadeTransition>(ScreenTexture::create(*this));

   getExecutionStack()->pushState(tileEngine, std::make_shared<TransitionState>(m_gameContext, blendTransition));
   m_chooseSound->play();
   Music::fadeOutMusic(1000);
}

/**
 * 'Menu Prototype' was selected. Push a Menu state.
 * \todo This will eventually be removed entirely, as it is only a programmer convenience right now.
 */
void MainMenu::MenuPrototypeAction()
{
   // PlayerData is held in a shared_ptr. However, HomeMenu doesn't
   // expect to manage the lifetime of a PlayerData object, and thus
   // takes a reference to PlayerData rather than a pointer.
   // MainMenu will manage the lifetime for now, but after the menu
   // is in good shape, this code will be removed anyway.
   auto loadResult = PlayerData::load(SAVE_GAME, getMetadata());
   m_menuPrototypePlayerData = std::get<0>(loadResult);
   getExecutionStack()->pushState(std::make_shared<HomeMenu>(m_gameContext, *m_menuPrototypePlayerData));
}

/**
 * 'Load Game' was selected. (TODO)
 */
void MainMenu::LoadGameAction()
{
   auto loadGameMenu = std::make_shared<SaveMenu>(m_gameContext);
   getExecutionStack()->pushState(loadGameMenu);
   m_chooseSound->play();
   Music::fadeOutMusic(1000);
}

/**
 * 'Options' was selected.
 * Load up the options menu interface.
 */
void MainMenu::OptionsAction()
{
   auto titleScreenSettingsMenuState = std::make_shared<TitleScreenSettingsMenu>(m_gameContext);

   auto blendTransition = std::make_shared<BlendTransition>(ScreenTexture::create(*this), ScreenTexture::create(*titleScreenSettingsMenuState), 500);
   getExecutionStack()->pushState(
      titleScreenSettingsMenuState,
      std::make_shared<TransitionState>(m_gameContext, blendTransition));
}

/**
 * 'About' was selected. (TODO)
 */
void MainMenu::AboutAction()
{
}

/**
 * 'Quit Game' was selected. Signal state logic termination, fade to black.
 */
void MainMenu::QuitAction()
{
   m_finished = true;
   m_chooseSound->play();
   Music::fadeOutMusic(1000);

   GraphicsUtil::getInstance()->FadeToColor(0.0f, 0.0f, 0.0f, 1000);
}
