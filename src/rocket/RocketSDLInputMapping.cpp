/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "RocketSDLInputMapping.h"

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_ROCKET

std::map<SDL_Keycode, Rml::Core::Input::KeyIdentifier> RocketSDLInputMapping::keyMap;

void RocketSDLInputMapping::initialize()
{
   RocketSDLInputMapping::keyMap[SDLK_BACKSPACE] = Rml::Core::Input::KI_BACK;
   RocketSDLInputMapping::keyMap[SDLK_TAB] = Rml::Core::Input::KI_TAB;
   RocketSDLInputMapping::keyMap[SDLK_CLEAR] = Rml::Core::Input::KI_CLEAR;
   RocketSDLInputMapping::keyMap[SDLK_RETURN] = Rml::Core::Input::KI_RETURN;
   RocketSDLInputMapping::keyMap[SDLK_PAUSE] = Rml::Core::Input::KI_PAUSE;
   RocketSDLInputMapping::keyMap[SDLK_ESCAPE] = Rml::Core::Input::KI_ESCAPE;
   RocketSDLInputMapping::keyMap[SDLK_SPACE] = Rml::Core::Input::KI_SPACE;
   RocketSDLInputMapping::keyMap[SDLK_EXCLAIM] = Rml::Core::Input::KI_1;
   RocketSDLInputMapping::keyMap[SDLK_HASH] = Rml::Core::Input::KI_3;
   RocketSDLInputMapping::keyMap[SDLK_DOLLAR] = Rml::Core::Input::KI_4;
   RocketSDLInputMapping::keyMap[SDLK_AMPERSAND] = Rml::Core::Input::KI_7;
   RocketSDLInputMapping::keyMap[SDLK_LEFTPAREN] = Rml::Core::Input::KI_9;
   RocketSDLInputMapping::keyMap[SDLK_RIGHTPAREN] = Rml::Core::Input::KI_0;
   RocketSDLInputMapping::keyMap[SDLK_ASTERISK] = Rml::Core::Input::KI_8;
   RocketSDLInputMapping::keyMap[SDLK_PLUS] = Rml::Core::Input::KI_OEM_PLUS;
   RocketSDLInputMapping::keyMap[SDLK_COMMA] = Rml::Core::Input::KI_OEM_COMMA;
   RocketSDLInputMapping::keyMap[SDLK_MINUS] = Rml::Core::Input::KI_OEM_MINUS;
   RocketSDLInputMapping::keyMap[SDLK_PERIOD] = Rml::Core::Input::KI_OEM_PERIOD;
   RocketSDLInputMapping::keyMap[SDLK_SLASH] = Rml::Core::Input::KI_OEM_2;
   RocketSDLInputMapping::keyMap[SDLK_0] = Rml::Core::Input::KI_0;
   RocketSDLInputMapping::keyMap[SDLK_1] = Rml::Core::Input::KI_1;
   RocketSDLInputMapping::keyMap[SDLK_2] = Rml::Core::Input::KI_2;
   RocketSDLInputMapping::keyMap[SDLK_3] = Rml::Core::Input::KI_3;
   RocketSDLInputMapping::keyMap[SDLK_4] = Rml::Core::Input::KI_4;
   RocketSDLInputMapping::keyMap[SDLK_5] = Rml::Core::Input::KI_5;
   RocketSDLInputMapping::keyMap[SDLK_6] = Rml::Core::Input::KI_6;
   RocketSDLInputMapping::keyMap[SDLK_7] = Rml::Core::Input::KI_7;
   RocketSDLInputMapping::keyMap[SDLK_8] = Rml::Core::Input::KI_8;
   RocketSDLInputMapping::keyMap[SDLK_9] = Rml::Core::Input::KI_9;
   RocketSDLInputMapping::keyMap[SDLK_COLON] = Rml::Core::Input::KI_OEM_1;
   RocketSDLInputMapping::keyMap[SDLK_SEMICOLON] = Rml::Core::Input::KI_OEM_1;
   RocketSDLInputMapping::keyMap[SDLK_LESS] = Rml::Core::Input::KI_OEM_COMMA;
   RocketSDLInputMapping::keyMap[SDLK_EQUALS] = Rml::Core::Input::KI_OEM_PLUS;
   RocketSDLInputMapping::keyMap[SDLK_GREATER] = Rml::Core::Input::KI_OEM_PERIOD;
   RocketSDLInputMapping::keyMap[SDLK_QUESTION] = Rml::Core::Input::KI_OEM_2;
   RocketSDLInputMapping::keyMap[SDLK_AT] = Rml::Core::Input::KI_2;

   RocketSDLInputMapping::keyMap[SDLK_LEFTBRACKET] = Rml::Core::Input::KI_OEM_4;
   RocketSDLInputMapping::keyMap[SDLK_BACKSLASH] = Rml::Core::Input::KI_OEM_5;
   RocketSDLInputMapping::keyMap[SDLK_RIGHTBRACKET] = Rml::Core::Input::KI_OEM_6;
   RocketSDLInputMapping::keyMap[SDLK_CARET] = Rml::Core::Input::KI_6;
   RocketSDLInputMapping::keyMap[SDLK_UNDERSCORE] = Rml::Core::Input::KI_OEM_MINUS;
   RocketSDLInputMapping::keyMap[SDLK_BACKQUOTE] = Rml::Core::Input::KI_OEM_3;
   RocketSDLInputMapping::keyMap[SDLK_a] = Rml::Core::Input::KI_A;
   RocketSDLInputMapping::keyMap[SDLK_b] = Rml::Core::Input::KI_B;
   RocketSDLInputMapping::keyMap[SDLK_c] = Rml::Core::Input::KI_C;
   RocketSDLInputMapping::keyMap[SDLK_d] = Rml::Core::Input::KI_D;
   RocketSDLInputMapping::keyMap[SDLK_e] = Rml::Core::Input::KI_E;
   RocketSDLInputMapping::keyMap[SDLK_f] = Rml::Core::Input::KI_F;
   RocketSDLInputMapping::keyMap[SDLK_g] = Rml::Core::Input::KI_G;
   RocketSDLInputMapping::keyMap[SDLK_h] = Rml::Core::Input::KI_H;
   RocketSDLInputMapping::keyMap[SDLK_i] = Rml::Core::Input::KI_I;
   RocketSDLInputMapping::keyMap[SDLK_j] = Rml::Core::Input::KI_J;
   RocketSDLInputMapping::keyMap[SDLK_k] = Rml::Core::Input::KI_K;
   RocketSDLInputMapping::keyMap[SDLK_l] = Rml::Core::Input::KI_L;
   RocketSDLInputMapping::keyMap[SDLK_m] = Rml::Core::Input::KI_M;
   RocketSDLInputMapping::keyMap[SDLK_n] = Rml::Core::Input::KI_N;
   RocketSDLInputMapping::keyMap[SDLK_o] = Rml::Core::Input::KI_O;
   RocketSDLInputMapping::keyMap[SDLK_p] = Rml::Core::Input::KI_P;
   RocketSDLInputMapping::keyMap[SDLK_q] = Rml::Core::Input::KI_Q;
   RocketSDLInputMapping::keyMap[SDLK_r] = Rml::Core::Input::KI_R;
   RocketSDLInputMapping::keyMap[SDLK_s] = Rml::Core::Input::KI_S;
   RocketSDLInputMapping::keyMap[SDLK_t] = Rml::Core::Input::KI_T;
   RocketSDLInputMapping::keyMap[SDLK_u] = Rml::Core::Input::KI_U;
   RocketSDLInputMapping::keyMap[SDLK_v] = Rml::Core::Input::KI_V;
   RocketSDLInputMapping::keyMap[SDLK_w] = Rml::Core::Input::KI_W;
   RocketSDLInputMapping::keyMap[SDLK_x] = Rml::Core::Input::KI_X;
   RocketSDLInputMapping::keyMap[SDLK_y] = Rml::Core::Input::KI_Y;
   RocketSDLInputMapping::keyMap[SDLK_z] = Rml::Core::Input::KI_Z;
   RocketSDLInputMapping::keyMap[SDLK_DELETE] = Rml::Core::Input::KI_DELETE;

   RocketSDLInputMapping::keyMap[SDLK_KP_0] = Rml::Core::Input::KI_NUMPAD0;
   RocketSDLInputMapping::keyMap[SDLK_KP_1] = Rml::Core::Input::KI_NUMPAD1;
   RocketSDLInputMapping::keyMap[SDLK_KP_2] = Rml::Core::Input::KI_NUMPAD2;
   RocketSDLInputMapping::keyMap[SDLK_KP_3] = Rml::Core::Input::KI_NUMPAD3;
   RocketSDLInputMapping::keyMap[SDLK_KP_4] = Rml::Core::Input::KI_NUMPAD4;
   RocketSDLInputMapping::keyMap[SDLK_KP_5] = Rml::Core::Input::KI_NUMPAD5;
   RocketSDLInputMapping::keyMap[SDLK_KP_6] = Rml::Core::Input::KI_NUMPAD6;
   RocketSDLInputMapping::keyMap[SDLK_KP_7] = Rml::Core::Input::KI_NUMPAD7;
   RocketSDLInputMapping::keyMap[SDLK_KP_8] = Rml::Core::Input::KI_NUMPAD8;
   RocketSDLInputMapping::keyMap[SDLK_KP_9] = Rml::Core::Input::KI_NUMPAD9;
   RocketSDLInputMapping::keyMap[SDLK_KP_PERIOD] = Rml::Core::Input::KI_DECIMAL;
   RocketSDLInputMapping::keyMap[SDLK_KP_DIVIDE] = Rml::Core::Input::KI_DIVIDE;
   RocketSDLInputMapping::keyMap[SDLK_KP_MULTIPLY] = Rml::Core::Input::KI_MULTIPLY;
   RocketSDLInputMapping::keyMap[SDLK_KP_MINUS] = Rml::Core::Input::KI_SUBTRACT;
   RocketSDLInputMapping::keyMap[SDLK_KP_PLUS] = Rml::Core::Input::KI_ADD;
   RocketSDLInputMapping::keyMap[SDLK_KP_ENTER] = Rml::Core::Input::KI_NUMPADENTER;
   RocketSDLInputMapping::keyMap[SDLK_KP_EQUALS] = Rml::Core::Input::KI_OEM_NEC_EQUAL;

   RocketSDLInputMapping::keyMap[SDLK_UP] = Rml::Core::Input::KI_UP;
   RocketSDLInputMapping::keyMap[SDLK_DOWN] = Rml::Core::Input::KI_DOWN;
   RocketSDLInputMapping::keyMap[SDLK_RIGHT] = Rml::Core::Input::KI_RIGHT;
   RocketSDLInputMapping::keyMap[SDLK_LEFT] = Rml::Core::Input::KI_LEFT;
   RocketSDLInputMapping::keyMap[SDLK_INSERT] = Rml::Core::Input::KI_INSERT;
   RocketSDLInputMapping::keyMap[SDLK_HOME] = Rml::Core::Input::KI_HOME;
   RocketSDLInputMapping::keyMap[SDLK_END] = Rml::Core::Input::KI_END;
   RocketSDLInputMapping::keyMap[SDLK_PAGEUP] = Rml::Core::Input::KI_PRIOR;
   RocketSDLInputMapping::keyMap[SDLK_PAGEDOWN] = Rml::Core::Input::KI_NEXT;

   RocketSDLInputMapping::keyMap[SDLK_F1] = Rml::Core::Input::KI_F1;
   RocketSDLInputMapping::keyMap[SDLK_F2] = Rml::Core::Input::KI_F2;
   RocketSDLInputMapping::keyMap[SDLK_F3] = Rml::Core::Input::KI_F3;
   RocketSDLInputMapping::keyMap[SDLK_F4] = Rml::Core::Input::KI_F4;
   RocketSDLInputMapping::keyMap[SDLK_F5] = Rml::Core::Input::KI_F5;
   RocketSDLInputMapping::keyMap[SDLK_F6] = Rml::Core::Input::KI_F6;
   RocketSDLInputMapping::keyMap[SDLK_F7] = Rml::Core::Input::KI_F7;
   RocketSDLInputMapping::keyMap[SDLK_F8] = Rml::Core::Input::KI_F8;
   RocketSDLInputMapping::keyMap[SDLK_F9] = Rml::Core::Input::KI_F9;
   RocketSDLInputMapping::keyMap[SDLK_F10] = Rml::Core::Input::KI_F10;
   RocketSDLInputMapping::keyMap[SDLK_F11] = Rml::Core::Input::KI_F11;
   RocketSDLInputMapping::keyMap[SDLK_F12] = Rml::Core::Input::KI_F12;
   RocketSDLInputMapping::keyMap[SDLK_F13] = Rml::Core::Input::KI_F13;
   RocketSDLInputMapping::keyMap[SDLK_F14] = Rml::Core::Input::KI_F14;
   RocketSDLInputMapping::keyMap[SDLK_F15] = Rml::Core::Input::KI_F15;

   RocketSDLInputMapping::keyMap[SDLK_NUMLOCKCLEAR] = Rml::Core::Input::KI_NUMLOCK;
   RocketSDLInputMapping::keyMap[SDLK_CAPSLOCK] = Rml::Core::Input::KI_CAPITAL;
   RocketSDLInputMapping::keyMap[SDLK_SCROLLLOCK] = Rml::Core::Input::KI_SCROLL;
   RocketSDLInputMapping::keyMap[SDLK_RSHIFT] = Rml::Core::Input::KI_RSHIFT;
   RocketSDLInputMapping::keyMap[SDLK_LSHIFT] = Rml::Core::Input::KI_LSHIFT;
   RocketSDLInputMapping::keyMap[SDLK_RGUI] = Rml::Core::Input::KI_RMETA;
   RocketSDLInputMapping::keyMap[SDLK_LGUI] = Rml::Core::Input::KI_LMETA;

   RocketSDLInputMapping::keyMap[SDLK_HELP] = Rml::Core::Input::KI_HELP;
   RocketSDLInputMapping::keyMap[SDLK_PRINTSCREEN] = Rml::Core::Input::KI_PRINT;
   RocketSDLInputMapping::keyMap[SDLK_PAUSE] = Rml::Core::Input::KI_PAUSE;
}

int RocketSDLInputMapping::getKeyModifierState(const SDL_Keymod& modifiers)
{
   int rocketModifierState = 0;

   if ((modifiers & KMOD_SHIFT) != 0)
   {
      rocketModifierState |= Rml::Core::Input::KM_SHIFT;
   }

   if ((modifiers & KMOD_CTRL) != 0)
   {
      rocketModifierState |= Rml::Core::Input::KM_CTRL;
   }

   if ((modifiers & KMOD_ALT) != 0)
   {
      rocketModifierState |= Rml::Core::Input::KM_ALT;
   }

   if ((modifiers & KMOD_GUI) != 0)
   {
      rocketModifierState |= Rml::Core::Input::KM_META;
   }

   if ((modifiers & KMOD_CAPS) != 0)
   {
      rocketModifierState |= Rml::Core::Input::KM_CAPSLOCK;
   }

   if ((modifiers & KMOD_NUM) != 0)
   {
      rocketModifierState |= Rml::Core::Input::KM_NUMLOCK;
   }

   if ((modifiers & KMOD_MODE) != 0)
   {
      rocketModifierState |= Rml::Core::Input::KM_CAPSLOCK;
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

void RocketSDLInputMapping::handleSDLEvent(Rml::Core::Context* context, const SDL_Event& event)
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
         const auto& keyMapping = keyMapResult != RocketSDLInputMapping::keyMap.end() ? keyMapResult->second : Rml::Core::Input::KI_UNKNOWN;

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
