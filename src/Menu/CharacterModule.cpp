/**
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "CharacterModule.h"

#include "Character.h"
#include <sstream>
#include "Container.h"
#include "Label.h"
#include "Icon.h"

CharacterModule::CharacterModule() : characterPortrait(NULL)
{
   characterStats = new gcn::contrib::AdjustingContainer();
   characterNameLabel = new edwt::Label();
   characterHPLabel = new edwt::Label();
   characterSPLabel = new edwt::Label();

   characterNameLabel->setForegroundColor(0xFFFFFF);
   characterHPLabel->setForegroundColor(0xFFFFFF);
   characterSPLabel->setForegroundColor(0xFFFFFF);

   characterPortrait = new edwt::Icon();

   characterStats->setNumberOfColumns(1);
   characterStats->setColumnAlignment(0, gcn::contrib::AdjustingContainer::LEFT);
   characterStats->add(characterNameLabel);
   characterStats->add(characterHPLabel);
   characterStats->add(characterSPLabel);
   characterStats->setOpaque(false);

   setNumberOfColumns(2);
   setColumnAlignment(0, gcn::contrib::AdjustingContainer::LEFT);
   setColumnAlignment(1, gcn::contrib::AdjustingContainer::RIGHT);
   setHorizontalSpacing(10);
   setPadding(5, 5, 5, 5);

   add(characterPortrait);
   add(characterStats);
   addMouseListener(this);
   setOpaque(false);
}

void CharacterModule::mouseClicked(gcn::MouseEvent& mouseEvent)
{
   distributeActionEvent();
}

void CharacterModule::setCharacter(Character* character)
{
   if (character != NULL)
   {
      characterName = character->getName();
      characterNameLabel->setCaption(characterName);
      characterNameLabel->adjustSize();

      std::stringstream hpSummary;
      hpSummary << character->getHP() << '/' << character->getMaxHP();
      characterHPLabel->setCaption(hpSummary.str());
      characterHPLabel->adjustSize();

      std::stringstream spSummary;
      spSummary << character->getSP() << '/' << character->getMaxSP();
      characterSPLabel->setCaption(spSummary.str());
      characterSPLabel->adjustSize();

      characterPortrait->setImage(character->getPortraitPath());

      characterStats->adjustContent();
      adjustContent();

      setVisible(true);
   }
   else
   {
      characterNameLabel->setCaption("");
      characterHPLabel->setCaption("");
      characterSPLabel->setCaption("");
      characterPortrait->clearImage();
      setVisible(false);
   }
}

CharacterModule::~CharacterModule()
{
   delete characterNameLabel;
   delete characterPortrait;
   delete characterHPLabel;
   delete characterSPLabel;
}
