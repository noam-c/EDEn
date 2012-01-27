/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "ConfirmState.h"
#include "ConfirmStateListener.h"
#include "Container.h"
#include "SDL.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_MENU;

ConfirmState::ConfirmDialog::ConfirmDialog(ConfirmState& confirmState, ConfirmStateListener& listener, const std::string& caption) 
   : edwt::Window(caption), confirmStateListener(listener), yesButton("Yes"), noButton("No")
{
   setHeight(200);
   setWidth(300);
   
   yesButton.addActionListener(this);
   noButton.addActionListener(this);
   
   int buttonsY = 50;
   
   add(&messageLabel);
   add(&yesButton, 30, buttonsY);
   add(&noButton, getWidth() - noButton.getWidth() - 30, buttonsY);
}

void ConfirmState::ConfirmDialog::action(const gcn::ActionEvent& event)
{
   distributeActionEvent();
   if(event.getSource() == &yesButton)
   {
      // "Yes" was clicked
      confirmStateListener.yesClicked();
   }
   else if(event.getSource() == &noButton)
   {
      // "No" was clicked
      confirmStateListener.noClicked();
   }
}

void ConfirmState::ConfirmDialog::setMessage(const std::string& message)
{
   messageLabel.setCaption(message);
   messageLabel.adjustSize();
}

ConfirmState::ConfirmState(ExecutionStack& executionStack, edwt::Container* container, ConfirmStateListener& listener, const std::string& caption, const std::string& message) : GameState(executionStack, container)
{
   confirmDialog = new ConfirmDialog(*this, listener, caption);
   confirmDialog->setMessage(message);
   top->add(confirmDialog);

   confirmDialog->addActionListener(this);
   DEBUG("Menu Focus handler: %x", top->_getFocusHandler());
   DEBUG("Menu Internal Focus handler: %x", top->_getInternalFocusHandler());

   DEBUG("Dialog Focus handler: %x", confirmDialog->_getFocusHandler());
   DEBUG("Dialog Internal Focus handler: %x", confirmDialog->_getInternalFocusHandler());

   confirmDialog->requestModalMouseInputFocus();

}

void ConfirmState::action(const gcn::ActionEvent& event)
{
   finished = true;
}

bool ConfirmState::step()
{
   SDL_Event event;

   /* Check for events */
   if(SDL_PollEvent(&event))
   {
      handleEvent(event);
   }

   return !finished;
}

void ConfirmState::draw()
{
}

ConfirmState::~ConfirmState()
{
   confirmDialog->releaseModalMouseInputFocus();
   top->remove(confirmDialog);
   
   delete confirmDialog;
}
