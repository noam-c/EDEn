/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef CHOICE_PROVIDER_H
#define CHOICE_PROVIDER_H

#include "ChoiceList.h"

class ChoiceProvider
{
  public:
    /**
     * @return true iff there are choices currently available from this provider.
     */
    virtual bool hasChoices() const = 0;

    /**
     * @return the choice list to show to the player.
     */
    virtual const ChoiceList& getCurrentChoices() const = 0;
   
    virtual void choiceSelected(int choiceIndex) = 0;

    virtual ~ChoiceProvider();
};

#endif
