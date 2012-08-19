/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "RocketSDLInputMapping.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_ROCKET;

std::map<SDLKey, Rocket::Core::Input::KeyIdentifier> RocketSDLInputMapping::keyMap;

void RocketSDLInputMapping::initialize()
{
   keyMap[SDLK_BACKSPACE] = Rocket::Core::Input::KI_BACK;
   keyMap[SDLK_TAB] = Rocket::Core::Input::KI_TAB;
   keyMap[SDLK_CLEAR] = Rocket::Core::Input::KI_CLEAR;
   keyMap[SDLK_RETURN] = Rocket::Core::Input::KI_RETURN;
   keyMap[SDLK_PAUSE] = Rocket::Core::Input::KI_PAUSE;
   keyMap[SDLK_ESCAPE] = Rocket::Core::Input::KI_ESCAPE;
   keyMap[SDLK_SPACE] = Rocket::Core::Input::KI_SPACE;
   keyMap[SDLK_EXCLAIM] = Rocket::Core::Input::KI_1;
   keyMap[SDLK_HASH] = Rocket::Core::Input::KI_3;
   keyMap[SDLK_DOLLAR] = Rocket::Core::Input::KI_4;
   keyMap[SDLK_AMPERSAND] = Rocket::Core::Input::KI_7;
   keyMap[SDLK_LEFTPAREN] = Rocket::Core::Input::KI_9;
   keyMap[SDLK_RIGHTPAREN] = Rocket::Core::Input::KI_0;
   keyMap[SDLK_ASTERISK] = Rocket::Core::Input::KI_8;
   keyMap[SDLK_PLUS] = Rocket::Core::Input::KI_OEM_PLUS;
   keyMap[SDLK_COMMA] = Rocket::Core::Input::KI_OEM_COMMA;
   keyMap[SDLK_MINUS] = Rocket::Core::Input::KI_OEM_MINUS;
   keyMap[SDLK_PERIOD] = Rocket::Core::Input::KI_OEM_PERIOD;
   keyMap[SDLK_SLASH] = Rocket::Core::Input::KI_OEM_2;
   keyMap[SDLK_0] = Rocket::Core::Input::KI_0;
   keyMap[SDLK_1] = Rocket::Core::Input::KI_1;
   keyMap[SDLK_2] = Rocket::Core::Input::KI_2;
   keyMap[SDLK_3] = Rocket::Core::Input::KI_3;
   keyMap[SDLK_4] = Rocket::Core::Input::KI_4;
   keyMap[SDLK_5] = Rocket::Core::Input::KI_5;
   keyMap[SDLK_6] = Rocket::Core::Input::KI_6;
   keyMap[SDLK_7] = Rocket::Core::Input::KI_7;
   keyMap[SDLK_8] = Rocket::Core::Input::KI_8;
   keyMap[SDLK_9] = Rocket::Core::Input::KI_9;
   keyMap[SDLK_COLON] = Rocket::Core::Input::KI_OEM_1;
   keyMap[SDLK_SEMICOLON] = Rocket::Core::Input::KI_OEM_1;
   keyMap[SDLK_LESS] = Rocket::Core::Input::KI_OEM_COMMA;
   keyMap[SDLK_EQUALS] = Rocket::Core::Input::KI_OEM_PLUS;
   keyMap[SDLK_GREATER] = Rocket::Core::Input::KI_OEM_PERIOD;
   keyMap[SDLK_QUESTION] = Rocket::Core::Input::KI_OEM_2;
   keyMap[SDLK_AT] = Rocket::Core::Input::KI_2;

   keyMap[SDLK_LEFTBRACKET] = Rocket::Core::Input::KI_OEM_4;
   keyMap[SDLK_BACKSLASH] = Rocket::Core::Input::KI_OEM_5;
   keyMap[SDLK_RIGHTBRACKET] = Rocket::Core::Input::KI_OEM_6;
   keyMap[SDLK_CARET] = Rocket::Core::Input::KI_6;
   keyMap[SDLK_UNDERSCORE] = Rocket::Core::Input::KI_OEM_MINUS;
   keyMap[SDLK_BACKQUOTE] = Rocket::Core::Input::KI_OEM_3;
   keyMap[SDLK_a] = Rocket::Core::Input::KI_A;
   keyMap[SDLK_b] = Rocket::Core::Input::KI_B;
   keyMap[SDLK_c] = Rocket::Core::Input::KI_C;
   keyMap[SDLK_d] = Rocket::Core::Input::KI_D;
   keyMap[SDLK_e] = Rocket::Core::Input::KI_E;
   keyMap[SDLK_f] = Rocket::Core::Input::KI_F;
   keyMap[SDLK_g] = Rocket::Core::Input::KI_G;
   keyMap[SDLK_h] = Rocket::Core::Input::KI_H;
   keyMap[SDLK_i] = Rocket::Core::Input::KI_I;
   keyMap[SDLK_j] = Rocket::Core::Input::KI_J;
   keyMap[SDLK_k] = Rocket::Core::Input::KI_K;
   keyMap[SDLK_l] = Rocket::Core::Input::KI_L;
   keyMap[SDLK_m] = Rocket::Core::Input::KI_M;
   keyMap[SDLK_n] = Rocket::Core::Input::KI_N;
   keyMap[SDLK_o] = Rocket::Core::Input::KI_O;
   keyMap[SDLK_p] = Rocket::Core::Input::KI_P;
   keyMap[SDLK_q] = Rocket::Core::Input::KI_Q;
   keyMap[SDLK_r] = Rocket::Core::Input::KI_R;
   keyMap[SDLK_s] = Rocket::Core::Input::KI_S;
   keyMap[SDLK_t] = Rocket::Core::Input::KI_T;
   keyMap[SDLK_u] = Rocket::Core::Input::KI_U;
   keyMap[SDLK_v] = Rocket::Core::Input::KI_V;
   keyMap[SDLK_w] = Rocket::Core::Input::KI_W;
   keyMap[SDLK_x] = Rocket::Core::Input::KI_X;
   keyMap[SDLK_y] = Rocket::Core::Input::KI_Y;
   keyMap[SDLK_z] = Rocket::Core::Input::KI_Z;
   keyMap[SDLK_DELETE] = Rocket::Core::Input::KI_DELETE;

   keyMap[SDLK_KP0] = Rocket::Core::Input::KI_NUMPAD0;
   keyMap[SDLK_KP1] = Rocket::Core::Input::KI_NUMPAD1;
   keyMap[SDLK_KP2] = Rocket::Core::Input::KI_NUMPAD2;
   keyMap[SDLK_KP3] = Rocket::Core::Input::KI_NUMPAD3;
   keyMap[SDLK_KP4] = Rocket::Core::Input::KI_NUMPAD4;
   keyMap[SDLK_KP5] = Rocket::Core::Input::KI_NUMPAD5;
   keyMap[SDLK_KP6] = Rocket::Core::Input::KI_NUMPAD6;
   keyMap[SDLK_KP7] = Rocket::Core::Input::KI_NUMPAD7;
   keyMap[SDLK_KP8] = Rocket::Core::Input::KI_NUMPAD8;
   keyMap[SDLK_KP9] = Rocket::Core::Input::KI_NUMPAD9;
   keyMap[SDLK_KP_PERIOD] = Rocket::Core::Input::KI_DECIMAL;
   keyMap[SDLK_KP_DIVIDE] = Rocket::Core::Input::KI_DIVIDE;
   keyMap[SDLK_KP_MULTIPLY] = Rocket::Core::Input::KI_MULTIPLY;
   keyMap[SDLK_KP_MINUS] = Rocket::Core::Input::KI_SUBTRACT;
   keyMap[SDLK_KP_PLUS] = Rocket::Core::Input::KI_ADD;
   keyMap[SDLK_KP_ENTER] = Rocket::Core::Input::KI_NUMPADENTER;
   keyMap[SDLK_KP_EQUALS] = Rocket::Core::Input::KI_OEM_NEC_EQUAL;

   keyMap[SDLK_UP] = Rocket::Core::Input::KI_UP;
   keyMap[SDLK_DOWN] = Rocket::Core::Input::KI_DOWN;
   keyMap[SDLK_RIGHT] = Rocket::Core::Input::KI_RIGHT;
   keyMap[SDLK_LEFT] = Rocket::Core::Input::KI_LEFT;
   keyMap[SDLK_INSERT] = Rocket::Core::Input::KI_INSERT;
   keyMap[SDLK_HOME] = Rocket::Core::Input::KI_HOME;
   keyMap[SDLK_END] = Rocket::Core::Input::KI_END;
   keyMap[SDLK_PAGEUP] = Rocket::Core::Input::KI_PRIOR;
   keyMap[SDLK_PAGEDOWN] = Rocket::Core::Input::KI_NEXT;

   keyMap[SDLK_F1] = Rocket::Core::Input::KI_F1;
   keyMap[SDLK_F2] = Rocket::Core::Input::KI_F2;
   keyMap[SDLK_F3] = Rocket::Core::Input::KI_F3;
   keyMap[SDLK_F4] = Rocket::Core::Input::KI_F4;
   keyMap[SDLK_F5] = Rocket::Core::Input::KI_F5;
   keyMap[SDLK_F6] = Rocket::Core::Input::KI_F6;
   keyMap[SDLK_F7] = Rocket::Core::Input::KI_F7;
   keyMap[SDLK_F8] = Rocket::Core::Input::KI_F8;
   keyMap[SDLK_F9] = Rocket::Core::Input::KI_F9;
   keyMap[SDLK_F10] = Rocket::Core::Input::KI_F10;
   keyMap[SDLK_F11] = Rocket::Core::Input::KI_F11;
   keyMap[SDLK_F12] = Rocket::Core::Input::KI_F12;
   keyMap[SDLK_F13] = Rocket::Core::Input::KI_F13;
   keyMap[SDLK_F14] = Rocket::Core::Input::KI_F14;
   keyMap[SDLK_F15] = Rocket::Core::Input::KI_F15;

   keyMap[SDLK_NUMLOCK] = Rocket::Core::Input::KI_NUMLOCK;
   keyMap[SDLK_CAPSLOCK] = Rocket::Core::Input::KI_CAPITAL;
   keyMap[SDLK_SCROLLOCK] = Rocket::Core::Input::KI_SCROLL;
   keyMap[SDLK_RSHIFT] = Rocket::Core::Input::KI_RSHIFT;
   keyMap[SDLK_LSHIFT] = Rocket::Core::Input::KI_LSHIFT;
   keyMap[SDLK_RMETA] = Rocket::Core::Input::KI_RMETA;
   keyMap[SDLK_LMETA] = Rocket::Core::Input::KI_LMETA;
   keyMap[SDLK_LSUPER] = Rocket::Core::Input::KI_LWIN;
   keyMap[SDLK_RSUPER] = Rocket::Core::Input::KI_RWIN;

   keyMap[SDLK_HELP] = Rocket::Core::Input::KI_HELP;
   keyMap[SDLK_PRINT] = Rocket::Core::Input::KI_PRINT;
   keyMap[SDLK_BREAK] = Rocket::Core::Input::KI_PAUSE;
}

int RocketSDLInputMapping::getKeyModifierState(const SDLMod& modifiers)
{
   int rocketModifierState = 0;

   if (modifiers & KMOD_SHIFT != 0)
   {
      rocketModifierState |= Rocket::Core::Input::KM_SHIFT;
   }

   if (modifiers & KMOD_CTRL != 0)
   {
      rocketModifierState |= Rocket::Core::Input::KM_CTRL;
   }

   if (modifiers & KMOD_ALT != 0)
   {
      rocketModifierState |= Rocket::Core::Input::KM_ALT;
   }

   if (modifiers & KMOD_META != 0)
   {
      rocketModifierState |= Rocket::Core::Input::KM_META;
   }

   if (modifiers & KMOD_CAPS != 0)
   {
      rocketModifierState |= Rocket::Core::Input::KM_CAPSLOCK;
   }

   if (modifiers & KMOD_NUM != 0)
   {
      rocketModifierState |= Rocket::Core::Input::KM_NUMLOCK;
   }

   if (modifiers & KMOD_MODE != 0)
   {
      rocketModifierState |= Rocket::Core::Input::KM_CAPSLOCK;
   }

   // No scroll lock support available from SDL, so scroll lock is ignored.

   return rocketModifierState;
}

int RocketSDLInputMapping::mapMouseButton(const SDL_MouseButtonEvent& mouseButtonEvent)
{
   if(mouseButtonEvent.button == SDL_BUTTON_LEFT)
   {
      return 0;
   }
   else if(mouseButtonEvent.button == SDL_BUTTON_RIGHT)
   {
      return 1;
   }
   else if(mouseButtonEvent.button == SDL_BUTTON_MIDDLE)
   {
      return 2;
   }

   return 3;
}

void RocketSDLInputMapping::handleSDLEvent(Rocket::Core::Context* context, const SDL_Event& event)
{
   switch(event.type)
   {
      case SDL_MOUSEBUTTONUP:
      {
         if(event.button.button != SDL_BUTTON_WHEELUP && event.button.button != SDL_BUTTON_WHEELDOWN)
         {
            context->ProcessMouseButtonUp(RocketSDLInputMapping::mapMouseButton(event.button), RocketSDLInputMapping::getKeyModifierState(SDL_GetModState()));
         }

         break;
      }
      case SDL_MOUSEBUTTONDOWN:
      {
         if(event.button.button == SDL_BUTTON_WHEELUP)
         {
            context->ProcessMouseWheel(-1, RocketSDLInputMapping::getKeyModifierState(SDL_GetModState()));
         }
         else if(event.button.button == SDL_BUTTON_WHEELDOWN)
         {
            context->ProcessMouseWheel(1, RocketSDLInputMapping::getKeyModifierState(SDL_GetModState()));
         }
         else
         {
            context->ProcessMouseButtonDown(RocketSDLInputMapping::mapMouseButton(event.button), RocketSDLInputMapping::getKeyModifierState(SDL_GetModState()));
         }

         break;
      }
      case SDL_MOUSEMOTION:
      {
         context->ProcessMouseMove(event.motion.x, event.motion.y, RocketSDLInputMapping::getKeyModifierState(SDL_GetModState()));
         break;
      }
      case SDL_KEYUP:
      case SDL_KEYDOWN:
      {
         std::map<SDLKey, Rocket::Core::Input::KeyIdentifier>::const_iterator keyMapResult = keyMap.find(event.key.keysym.sym);
         Rocket::Core::Input::KeyIdentifier keyMapping = keyMapResult != keyMap.end() ? keyMapResult->second : Rocket::Core::Input::KI_UNKNOWN;

         if(event.type == SDL_KEYUP)
         {
            context->ProcessKeyUp(keyMapping, RocketSDLInputMapping::getKeyModifierState(event.key.keysym.mod));
         }
         else
         {
            context->ProcessKeyDown(keyMapping, RocketSDLInputMapping::getKeyModifierState(event.key.keysym.mod));
         }
         break;
      }
   }
}
