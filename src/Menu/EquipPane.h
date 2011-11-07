#ifndef EQUIP_PANE_H_
#define EQUIP_PANE_H_

#include "MenuPane.h"
#include "Label.h"
#include "ListBox.h"
#include <vector>

namespace edwt
{
   class Container;
   class ModuleSelectListener;
};

class EquipSlot;
class Item;
class ItemListModel;

/**
 * The pane containing the GUI elements for the character equip menu.
 *
 * @author Noam Chitayat
 */
class EquipPane : public MenuPane, public gcn::ActionListener
{
   class EquipSlotModule : public edwt::Label, public gcn::MouseListener
   {  
      public:
         EquipSlotModule();
         EquipSlotModule(const EquipSlotModule& module);
         void mouseClicked(gcn::MouseEvent& event);
   };
 
   bool invalidated;
   
   std::vector<EquipSlot*>& equipSlots;
   ItemListModel& equippableItems;

   edwt::ListBox itemListBox;

   std::vector<EquipSlotModule> slotLabels;
   gcn::contrib::AdjustingContainer slotContainer;

   edwt::ModuleSelectListener* moduleSelectListener;
   
   void resizeEquipSlotLabels(int numSlotsRequired);
   
   void refreshEquipSlots();
   
   void refresh();
   
   protected:
      
      void logic();
   
   public:
      static const std::string SlotModuleEventId;
      static const std::string ItemListEventId;
   
      /**
       * Constructor.
       *
       * @param slotList The list of character slots to display in the equipment pane.
       * @param rect The preferred dimensions for this menu pane.
       */
      EquipPane(std::vector<EquipSlot*>& slotList, ItemListModel& equippableItemList, const gcn::Rectangle& rect);

      void setModuleSelectListener(edwt::ModuleSelectListener* listener);
   
      /**
       * Response to UI actions.
       *
       * @param event The GUI action event.
       */
      void action(const gcn::ActionEvent& event);
   
      /**
       * Request a refresh of the equip points and item list displayed by the equipment panel.
       */
      void invalidate();

      /**
       * Destructor.
       */
      ~EquipPane();
};

#endif
