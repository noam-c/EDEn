/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#ifndef CONFIRM_STATE_LISTENER_H
#define CONFIRM_STATE_LISTENER_H

class ConfirmStateListener
{
   public:
      virtual void yesClicked() {}
      virtual void noClicked() {}
};

#endif
