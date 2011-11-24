/**
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#ifndef CONFIRM_STATE_H
#define CONFIRM_STATE_H

#include "GameState.h"
#include "guichan.hpp"
#include "Window.h"
#include <string>

namespace edwt
{
   class ConfirmDialog;
};

class ExecutionStack;
class ConfirmStateListener;

class ConfirmState : public GameState, public gcn::ActionListener
{
   /**
    * A dialog window for asking the user for confirmation.
    *
    * @author Noam Chitayat
    */
   class ConfirmDialog : public edwt::Window, public gcn::ActionListener
   {
      ConfirmStateListener& confirmStateListener;
      
      gcn::Label messageLabel;
      gcn::Button yesButton;
      gcn::Button noButton;
      
      public:
         /**
          * Constructor.
          *
          * @param The window caption for the dialog.
          */
         ConfirmDialog(ConfirmState& confirmState, ConfirmStateListener& listener, const std::string& caption);
         
         /**
          * Responds to one of the two buttons of the confirmation dialog.
          *
          * @param event The action event sent by the GUI.
          */
         void action(const gcn::ActionEvent& event);
      
         /**
          * Set the question to prompt the user with.
          *
          * @param message The message prompt for the user.
          */
         void setMessage(const std::string& message);
   };
   
   ConfirmDialog* confirmDialog;
   
   public:
      ConfirmState(ExecutionStack& executionStack, edwt::Container* container, ConfirmStateListener& listener, const std::string& caption, const std::string& message);
   
      /**
       * Responds to one of the two buttons of the confirmation dialog.
       *
       * @param event The action event sent by the GUI.
       */
      void action(const gcn::ActionEvent& event);

      bool step();
      void draw();
   
      ~ConfirmState();
};

#endif
