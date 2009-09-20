#ifndef __STRING_LIST_MODEL_H_
#define __STRING_LIST_MODEL_H_

#include "guichan.hpp"
#include <vector>

namespace edwt
{
   /**
    * List model that holds strings for a Guichan gcn::ListBox
    * Each item is associated with an action number, which is thrown to
    * the SDL input stack (typically checked by the currently running GameState)
    * as an SDL user event.
    *
    * @author Noam Chitayat
    */
   class StringListModel : public gcn::ListModel
   {
      /**
       * A list item holds both a name and an associated action flag.
       *
       * @author Noam Chitayat
       */
      struct ListItem
      {
         /** The name of the option */
         std::string name;

         /** The action associated with the option */
         int actionFlag;

         /** Constructor. */
         ListItem(std::string label, int action) : name(label), actionFlag(action) {}
      };

      /** The string items (and associated action flags) in the list */ 
      std::vector<ListItem> listOps;

      public:

         /**
          * @return the number of items in this list model
          */
         int getNumberOfElements();

         /**
          * @param i the index of a list item
          * @return the list item at index i
          */
         std::string getElementAt(int i);

         /**
          * @param i the index of a list item
          * @return the action associated with the list item at index i
          */
         int getActionAt(int i);

         /**
          * Add an item to the list model
          *
          * @param label the string label for the new item
          * @param action the action flag for selecting the new item
          */
         void add(std::string label, int action);

         /**
          * Removes all items from the list model
          */
         void clear();
    };
};

#endif
