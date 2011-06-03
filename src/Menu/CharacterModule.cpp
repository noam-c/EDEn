#include "CharacterModule.h"

#include "Character.h"
#include <sstream>
#include "Container.h"
#include "Label.h"
#include "Icon.h"

CharacterModule::CharacterModule() : characterPortrait(NULL)
{
   characterStats = new gcn::contrib::AdjustingContainer();
   characterName = new edwt::Label();
   characterHPLabel = new edwt::Label();
   characterSPLabel = new edwt::Label();

   characterPortrait = new edwt::Icon();

   characterStats->setNumberOfColumns(1);
   characterStats->setColumnAlignment(0, gcn::contrib::AdjustingContainer::LEFT);
   characterStats->add(characterName);
   characterStats->add(characterHPLabel);
   characterStats->add(characterSPLabel);

   setNumberOfColumns(2);
   setColumnAlignment(0, gcn::contrib::AdjustingContainer::LEFT);
   setColumnAlignment(1, gcn::contrib::AdjustingContainer::RIGHT);
   setHorizontalSpacing(10);
   setPadding(5, 5, 5, 5);

   add(characterPortrait);
   add(characterStats);
}

void CharacterModule::setCharacter(Character* character)
{
   if (character != NULL)
   {
      characterName->setCaption(character->getName());
      characterName->adjustSize();

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
      characterName->setCaption("");
      characterHPLabel->setCaption("");
      characterSPLabel->setCaption("");
      characterPortrait->clearImage();
      setVisible(false);
   }
}

CharacterModule::~CharacterModule()
{
   delete characterName;
   delete characterPortrait;
   delete characterHPLabel;
   delete characterSPLabel;
}
