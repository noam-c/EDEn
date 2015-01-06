/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "RocketSDLInputMapping.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_ROCKET;

std::map<SDL_Keycode, Rocket::Core::Input::KeyIdentifier> RocketSDLInputMapping::keyMap;

void RocketSDLInputMapping::initialize()
{
   RocketSDLInputMapping::keyMap[SDLK_BACKSPACE] = Rocket::Core::Input::KI_BACK;
   RocketSDLInputMapping::keyMap[SDLK_TAB] = Rocket::Core::Input::KI_TAB;
   RocketSDLInputMapping::keyMap[SDLK_CLEAR] = Rocket::Core::Input::KI_CLEAR;
   RocketSDLInputMapping::keyMap[SDLK_RETURN] = Rocket::Core::Input::KI_RETURN;
   RocketSDLInputMapping::keyMap[SDLK_PAUSE] = Rocket::Core::Input::KI_PAUSE;
   RocketSDLInputMapping::keyMap[SDLK_ESCAPE] = Rocket::Core::Input::KI_ESCAPE;
   RocketSDLInputMapping::keyMap[SDLK_SPACE] = Rocket::Core::Input::KI_SPACE;
   RocketSDLInputMapping::keyMap[SDLK_EXCLAIM] = Rocket::Core::Input::KI_1;
   RocketSDLInputMapping::keyMap[SDLK_HASH] = Rocket::Core::Input::KI_3;
   RocketSDLInputMapping::keyMap[SDLK_DOLLAR] = Rocket::Core::Input::KI_4;
   RocketSDLInputMapping::keyMap[SDLK_AMPERSAND] = Rocket::Core::Input::KI_7;
   RocketSDLInputMapping::keyMap[SDLK_LEFTPAREN] = Rocket::Core::Input::KI_9;
   RocketSDLInputMapping::keyMap[SDLK_RIGHTPAREN] = Rocket::Core::Input::KI_0;
   RocketSDLInputMapping::keyMap[SDLK_ASTERISK] = Rocket::Core::Input::KI_8;
   RocketSDLInputMapping::keyMap[SDLK_PLUS] = Rocket::Core::Input::KI_OEM_PLUS;
   RocketSDLInputMapping::keyMap[SDLK_COMMA] = Rocket::Core::Input::KI_OEM_COMMA;
   RocketSDLInputMapping::keyMap[SDLK_MINUS] = Rocket::Core::Input::KI_OEM_MINUS;
   RocketSDLInputMapping::keyMap[SDLK_PERIOD] = Rocket::Core::Input::KI_OEM_PERIOD;
   RocketSDLInputMapping::keyMap[SDLK_SLASH] = Rocket::Core::Input::KI_OEM_2;
   RocketSDLInputMapping::keyMap[SDLK_0] = Rocket::Core::Input::KI_0;
   RocketSDLInputMapping::keyMap[SDLK_1] = Rocket::Core::Input::KI_1;
   RocketSDLInputMapping::keyMap[SDLK_2] = Rocket::Core::Input::KI_2;
   RocketSDLInputMapping::keyMap[SDLK_3] = Rocket::Core::Input::KI_3;
   RocketSDLInputMapping::keyMap[SDLK_4] = Rocket::Core::Input::KI_4;
   RocketSDLInputMapping::keyMap[SDLK_5] = Rocket::Core::Input::KI_5;
   RocketSDLInputMapping::keyMap[SDLK_6] = Rocket::Core::Input::KI_6;
   RocketSDLInputMapping::keyMap[SDLK_7] = Rocket::Core::Input::KI_7;
   RocketSDLInputMapping::keyMap[SDLK_8] = Rocket::Core::Input::KI_8;
   RocketSDLInputMapping::keyMap[SDLK_9] = Rocket::Core::Input::KI_9;
   RocketSDLInputMapping::keyMap[SDLK_COLON] = Rocket::Core::Input::KI_OEM_1;
   RocketSDLInputMapping::keyMap[SDLK_SEMICOLON] = Rocket::Core::Input::KI_OEM_1;
   RocketSDLInputMapping::keyMap[SDLK_LESS] = Rocket::Core::Input::KI_OEM_COMMA;
   RocketSDLInputMapping::keyMap[SDLK_EQUALS] = Rocket::Core::Input::KI_OEM_PLUS;
   RocketSDLInputMapping::keyMap[SDLK_GREATER] = Rocket::Core::Input::KI_OEM_PERIOD;
   RocketSDLInputMapping::keyMap[SDLK_QUESTION] = Rocket::Core::Input::KI_OEM_2;
   RocketSDLInputMapping::keyMap[SDLK_AT] = Rocket::Core::Input::KI_2;

   RocketSDLInputMapping::keyMap[SDLK_LEFTBRACKET] = Rocket::Core::Input::KI_OEM_4;
   RocketSDLInputMapping::keyMap[SDLK_BACKSLASH] = Rocket::Core::Input::KI_OEM_5;
   RocketSDLInputMapping::keyMap[SDLK_RIGHTBRACKET] = Rocket::Core::Input::KI_OEM_6;
   RocketSDLInputMapping::keyMap[SDLK_CARET] = Rocket::Core::Input::KI_6;
   RocketSDLInputMapping::keyMap[SDLK_UNDERSCORE] = Rocket::Core::Input::KI_OEM_MINUS;
   RocketSDLInputMapping::keyMap[SDLK_BACKQUOTE] = Rocket::Core::Input::KI_OEM_3;
   RocketSDLInputMapping::keyMap[SDLK_a] = Rocket::Core::Input::KI_A;
   RocketSDLInputMapping::keyMap[SDLK_b] = Rocket::Core::Input::KI_B;
   RocketSDLInputMapping::keyMap[SDLK_c] = Rocket::Core::Input::KI_C;
   RocketSDLInputMapping::keyMap[SDLK_d] = Rocket::Core::Input::KI_D;
   RocketSDLInputMapping::keyMap[SDLK_e] = Rocket::Core::Input::KI_E;
   RocketSDLInputMapping::keyMap[SDLK_f] = Rocket::Core::Input::KI_F;
   RocketSDLInputMapping::keyMap[SDLK_g] = Rocket::Core::Input::KI_G;
   RocketSDLInputMapping::keyMap[SDLK_h] = Rocket::Core::Input::KI_H;
   RocketSDLInputMapping::keyMap[SDLK_i] = Rocket::Core::Input::KI_I;
   RocketSDLInputMapping::keyMap[SDLK_j] = Rocket::Core::Input::KI_J;
   RocketSDLInputMapping::keyMap[SDLK_k] = Rocket::Core::Input::KI_K;
   RocketSDLInputMapping::keyMap[SDLK_l] = Rocket::Core::Input::KI_L;
   RocketSDLInputMapping::keyMap[SDLK_m] = Rocket::Core::Input::KI_M;
   RocketSDLInputMapping::keyMap[SDLK_n] = Rocket::Core::Input::KI_N;
   RocketSDLInputMapping::keyMap[SDLK_o] = Rocket::Core::Input::KI_O;
   RocketSDLInputMapping::keyMap[SDLK_p] = Rocket::Core::Input::KI_P;
   RocketSDLInputMapping::keyMap[SDLK_q] = Rocket::Core::Input::KI_Q;
   RocketSDLInputMapping::keyMap[SDLK_r] = Rocket::Core::Input::KI_R;
   RocketSDLInputMapping::keyMap[SDLK_s] = Rocket::Core::Input::KI_S;
   RocketSDLInputMapping::keyMap[SDLK_t] = Rocket::Core::Input::KI_T;
   RocketSDLInputMapping::keyMap[SDLK_u] = Rocket::Core::Input::KI_U;
   RocketSDLInputMapping::keyMap[SDLK_v] = Rocket::Core::Input::KI_V;
   RocketSDLInputMapping::keyMap[SDLK_w] = Rocket::Core::Input::KI_W;
   RocketSDLInputMapping::keyMap[SDLK_x] = Rocket::Core::Input::KI_X;
   RocketSDLInputMapping::keyMap[SDLK_y] = Rocket::Core::Input::KI_Y;
   RocketSDLInputMapping::keyMap[SDLK_z] = Rocket::Core::Input::KI_Z;
   RocketSDLInputMapping::keyMap[SDLK_DELETE] = Rocket::Core::Input::KI_DELETE;

   RocketSDLInputMapping::keyMap[SDLK_KP_0] = Rocket::Core::Input::KI_NUMPAD0;
   RocketSDLInputMapping::keyMap[SDLK_KP_1] = Rocket::Core::Input::KI_NUMPAD1;
   RocketSDLInputMapping::keyMap[SDLK_KP_2] = Rocket::Core::Input::KI_NUMPAD2;
   RocketSDLInputMapping::keyMap[SDLK_KP_3] = Rocket::Core::Input::KI_NUMPAD3;
   RocketSDLInputMapping::keyMap[SDLK_KP_4] = Rocket::Core::Input::KI_NUMPAD4;
   RocketSDLInputMapping::keyMap[SDLK_KP_5] = Rocket::Core::Input::KI_NUMPAD5;
   RocketSDLInputMapping::keyMap[SDLK_KP_6] = Rocket::Core::Input::KI_NUMPAD6;
   RocketSDLInputMapping::keyMap[SDLK_KP_7] = Rocket::Core::Input::KI_NUMPAD7;
   RocketSDLInputMapping::keyMap[SDLK_KP_8] = Rocket::Core::Input::KI_NUMPAD8;
   RocketSDLInputMapping::keyMap[SDLK_KP_9] = Rocket::Core::Input::KI_NUMPAD9;
   RocketSDLInputMapping::keyMap[SDLK_KP_PERIOD] = Rocket::Core::Input::KI_DECIMAL;
   RocketSDLInputMapping::keyMap[SDLK_KP_DIVIDE] = Rocket::Core::Input::KI_DIVIDE;
   RocketSDLInputMapping::keyMap[SDLK_KP_MULTIPLY] = Rocket::Core::Input::KI_MULTIPLY;
   RocketSDLInputMapping::keyMap[SDLK_KP_MINUS] = Rocket::Core::Input::KI_SUBTRACT;
   RocketSDLInputMapping::keyMap[SDLK_KP_PLUS] = Rocket::Core::Input::KI_ADD;
   RocketSDLInputMapping::keyMap[SDLK_KP_ENTER] = Rocket::Core::Input::KI_NUMPADENTER;
   RocketSDLInputMapping::keyMap[SDLK_KP_EQUALS] = Rocket::Core::Input::KI_OEM_NEC_EQUAL;

   RocketSDLInputMapping::keyMap[SDLK_UP] = Rocket::Core::Input::KI_UP;
   RocketSDLInputMapping::keyMap[SDLK_DOWN] = Rocket::Core::Input::KI_DOWN;
   RocketSDLInputMapping::keyMap[SDLK_RIGHT] = Rocket::Core::Input::KI_RIGHT;
   RocketSDLInputMapping::keyMap[SDLK_LEFT] = Rocket::Core::Input::KI_LEFT;
   RocketSDLInputMapping::keyMap[SDLK_INSERT] = Rocket::Core::Input::KI_INSERT;
   RocketSDLInputMapping::keyMap[SDLK_HOME] = Rocket::Core::Input::KI_HOME;
   RocketSDLInputMapping::keyMap[SDLK_END] = Rocket::Core::Input::KI_END;
   RocketSDLInputMapping::keyMap[SDLK_PAGEUP] = Rocket::Core::Input::KI_PRIOR;
   RocketSDLInputMapping::keyMap[SDLK_PAGEDOWN] = Rocket::Core::Input::KI_NEXT;

   RocketSDLInputMapping::keyMap[SDLK_F1] = Rocket::Core::Input::KI_F1;
   RocketSDLInputMapping::keyMap[SDLK_F2] = Rocket::Core::Input::KI_F2;
   RocketSDLInputMapping::keyMap[SDLK_F3] = Rocket::Core::Input::KI_F3;
   RocketSDLInputMapping::keyMap[SDLK_F4] = Rocket::Core::Input::KI_F4;
   RocketSDLInputMapping::keyMap[SDLK_F5] = Rocket::Core::Input::KI_F5;
   RocketSDLInputMapping::keyMap[SDLK_F6] = Rocket::Core::Input::KI_F6;
   RocketSDLInputMapping::keyMap[SDLK_F7] = Rocket::Core::Input::KI_F7;
   RocketSDLInputMapping::keyMap[SDLK_F8] = Rocket::Core::Input::KI_F8;
   RocketSDLInputMapping::keyMap[SDLK_F9] = Rocket::Core::Input::KI_F9;
   RocketSDLInputMapping::keyMap[SDLK_F10] = Rocket::Core::Input::KI_F10;
   RocketSDLInputMapping::keyMap[SDLK_F11] = Rocket::Core::Input::KI_F11;
   RocketSDLInputMapping::keyMap[SDLK_F12] = Rocket::Core::Input::KI_F12;
   RocketSDLInputMapping::keyMap[SDLK_F13] = Rocket::Core::Input::KI_F13;
   RocketSDLInputMapping::keyMap[SDLK_F14] = Rocket::Core::Input::KI_F14;
   RocketSDLInputMapping::keyMap[SDLK_F15] = Rocket::Core::Input::KI_F15;

   RocketSDLInputMapping::keyMap[SDLK_NUMLOCKCLEAR] = Rocket::Core::Input::KI_NUMLOCK;
   RocketSDLInputMapping::keyMap[SDLK_CAPSLOCK] = Rocket::Core::Input::KI_CAPITAL;
   RocketSDLInputMapping::keyMap[SDLK_SCROLLLOCK] = Rocket::Core::Input::KI_SCROLL;
   RocketSDLInputMapping::keyMap[SDLK_RSHIFT] = Rocket::Core::Input::KI_RSHIFT;
   RocketSDLInputMapping::keyMap[SDLK_LSHIFT] = Rocket::Core::Input::KI_LSHIFT;
   RocketSDLInputMapping::keyMap[SDLK_RGUI] = Rocket::Core::Input::KI_RMETA;
   RocketSDLInputMapping::keyMap[SDLK_LGUI] = Rocket::Core::Input::KI_LMETA;

   RocketSDLInputMapping::keyMap[SDLK_HELP] = Rocket::Core::Input::KI_HELP;
   RocketSDLInputMapping::keyMap[SDLK_PRINTSCREEN] = Rocket::Core::Input::KI_PRINT;
   RocketSDLInputMapping::keyMap[SDLK_PAUSE] = Rocket::Core::Input::KI_PAUSE;
}

int RocketSDLInputMapping::getKeyModifierState(const SDL_Keymod& modifiers)
{
   int rocketModifierState = 0;

   if ((modifiers & KMOD_SHIFT) != 0)
   {
      rocketModifierState |= Rocket::Core::Input::KM_SHIFT;
   }

   if ((modifiers & KMOD_CTRL) != 0)
   {
      rocketModifierState |= Rocket::Core::Input::KM_CTRL;
   }

   if ((modifiers & KMOD_ALT) != 0)
   {
      rocketModifierState |= Rocket::Core::Input::KM_ALT;
   }

   if ((modifiers & KMOD_GUI) != 0)
   {
      rocketModifierState |= Rocket::Core::Input::KM_META;
   }

   if ((modifiers & KMOD_CAPS) != 0)
   {
      rocketModifierState |= Rocket::Core::Input::KM_CAPSLOCK;
   }

   if ((modifiers & KMOD_NUM) != 0)
   {
      rocketModifierState |= Rocket::Core::Input::KM_NUMLOCK;
   }

   if ((modifiers & KMOD_MODE) != 0)
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
         context->ProcessMouseButtonUp(RocketSDLInputMapping::mapMouseButton(event.button), RocketSDLInputMapping::getKeyModifierState(SDL_GetModState()));
         break;
      }
      case SDL_MOUSEBUTTONDOWN:
      {
         context->ProcessMouseButtonDown(RocketSDLInputMapping::mapMouseButton(event.button), RocketSDLInputMapping::getKeyModifierState(SDL_GetModState()));
         break;
      }
      case SDL_MOUSEWHEEL:
      {
         context->ProcessMouseWheel(event.wheel.y, RocketSDLInputMapping::getKeyModifierState(SDL_GetModState()));
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
         const auto& keyMapResult = RocketSDLInputMapping::keyMap.find(event.key.keysym.sym);
         const auto& keyMapping = keyMapResult != RocketSDLInputMapping::keyMap.end() ? keyMapResult->second : Rocket::Core::Input::KI_UNKNOWN;

         if(event.type == SDL_KEYUP)
         {
            context->ProcessKeyUp(keyMapping, RocketSDLInputMapping::getKeyModifierState(SDL_GetModState()));
         }
         else
         {
            context->ProcessKeyDown(keyMapping, RocketSDLInputMapping::getKeyModifierState(SDL_GetModState()));
         }
         break;
      }
      case SDL_TEXTINPUT:
      {
         context->ProcessTextInput(event.text.text);
         break;
      }
   }
}
