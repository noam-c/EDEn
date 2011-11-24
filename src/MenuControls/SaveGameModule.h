/**
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#ifndef CHARACTER_MODULE_H_
#define CHARACTER_MODULE_H_

#include "guichan.hpp"

namespace edwt
{
   class Container;
   class Icon;
   class Label;
};

class PlayerData;

/**
 * A GUI element used to display summarized character information in a small area of a menu.
 *
 * @author Noam Chitayat
 */
class SaveGameModule : public gcn::contrib::AdjustingContainer, public gcn::MouseListener
{
   /** The player save data to show. */
   PlayerData& playerData;
   
   /** Path to the save game file. */
   const std::string path;

   /** The icons holding the portraits of the characters. */
   std::vector<edwt::Icon*> characterPortraits;

   public:
      /**
       * Constructor for creating an empty character module.
       *
       * @param playerData The player data contained in the save game.
       * @param path The path to the save game file.
       */
      SaveGameModule(PlayerData& playerData);

      void mouseClicked(gcn::MouseEvent& event);
   
      /**
       * Destructor.
       */
      ~SaveGameModule();
};

#endif
