#ifndef MODULE_SELECT_LISTENER_H
#define MODULE_SELECT_LISTENER_H

#include <string>

/**
 * An interface for classes that detect and act on savegame selection events in the in-game menu.
 *
 * @author Noam Chitayat
 */
class ModuleSelectListener
{
   public:
      /**
       * Fires when a module is selected from a list in the in-game menu.
       *
       * @param path The index of the GUI module that was selected.
       */
      virtual void moduleSelected(int index) = 0;
};

#endif
