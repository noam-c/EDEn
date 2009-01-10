#include "MainMenu.h"
#include "ExecutionStack.h"
#include "TileEngine.h"
#include "ScriptEngine.h"

#define CHAP1 "data/chapters/chapter1"
//Actions for each of the list ops in the title screen

/**
 * 'New Game' was selected. Push a TileEngine state and fade to black.
 * [this will eventually change to a chapter selection list, with the fade
 * and pushed state (field or battle) changing based on the chapter].
 */
void MainMenu::NewGameAction()
{  TileEngine* tileEngine = new TileEngine();
   ScriptEngine::getInstance()->setTileEngine(tileEngine);
   ScriptEngine::getInstance()->runScript(CHAP1);

   ExecutionStack::getInstance()->pushState(tileEngine);
   Mix_PlayChannel(-1, chooseSound, 0);
   Mix_FadeOutMusic(1000);
   GraphicsUtil::getInstance()->FadeToColor(0.0f, 0.0f, 0.0f, 1000);
}

/**
 * 'Load Game' was selected. (TODO)
 */
void MainMenu::LoadGameAction()
{
}

/**
 * 'Options' was selected.
 * Perform any transitions necessary and load up the options menu interface. (TODO)
 */
void MainMenu::OptionsAction()
{
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
{  finished = true;
   Mix_PlayChannel(-1, chooseSound, 0);
   Mix_FadeOutMusic(1000);
   GraphicsUtil::getInstance()->FadeToColor(0.0f, 0.0f, 0.0f, 1000);
}
