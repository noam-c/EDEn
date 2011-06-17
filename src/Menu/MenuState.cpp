#include "MenuState.h"

#include "guichan.hpp"
#include "Container.h"
#include <SDL.h>

MenuState::MenuState(edwt::Container* top, edwt::Container& menuPane) : GameState(top), menuPane(menuPane)
{
}

bool MenuState::step()
{
   if(finished) return false;
   bool done = false;

   pollInputEvent(done);
   return !done;
}

void MenuState::pollInputEvent(bool& finishState)
{  
   /* Check for events */
   SDL_Event event;
   if(SDL_PollEvent(&event))
   {
      switch (event.type)
      {
         case SDL_KEYDOWN:
         {
            switch(event.key.keysym.sym)
            {
               case SDLK_ESCAPE:
               {
                  finishState = true;
                  return;
               }
            }

            break;
         }
         default:
         {
            break;
         }
      }

      // If the menu pane didn't consume this event at all, then propagate to the generic input handling
      handleEvent(event);
   }
}

void MenuState::draw()
{
   /* Don't run too fast */
   SDL_Delay (1);
}

MenuState::~MenuState()
{
}