#include "EquipPane.h"
#include "EquipSlot.h"
#include "ModuleSelectListener.h"
#include "Item.h"
#include "ItemListModel.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

const std::string EquipPane::SlotModuleEventId = "SlotModuleEvent";
const std::string EquipPane::ItemListEventId = "ItemListEvent";

EquipPane::EquipSlotModule::EquipSlotModule()
{
   addMouseListener(this);
}

EquipPane::EquipSlotModule::EquipSlotModule(const EquipSlotModule& module) : edwt::Label()
{
   addMouseListener(this);
}

void EquipPane::EquipSlotModule::mouseClicked(gcn::MouseEvent& event)
{
   distributeActionEvent();
}

EquipPane::EquipPane(std::vector<EquipSlot*>& slotList, ItemListModel& equippableItemList, const gcn::Rectangle& rect) : MenuPane(rect), invalidated(false), equipSlots(slotList), equippableItems(equippableItemList)
{
   slotContainer.setNumberOfColumns(1);
   slotContainer.setHeight(rect.height / 2);

   itemListBox.setWidth(300);
   itemListBox.setHeight(300);
   itemListBox.setBaseColor(0xFFFFFF);
   itemListBox.setActionEventId(ItemListEventId);
   itemListBox.setOpaque(true);
   itemListBox.setListModel(&equippableItems);

   add(&slotContainer);
   add(&itemListBox, 0, rect.height / 2);

   refresh();
}

void EquipPane::setModuleSelectListener(edwt::ModuleSelectListener* listener)
{
   moduleSelectListener = listener;
}

void EquipPane::action(const gcn::ActionEvent& event)
{
   if(event.getId() == SlotModuleEventId)
   {
      for(unsigned int i = 0; i < slotLabels.size(); ++i)
      {
         if(event.getSource() == &slotLabels[i])
         {
            moduleSelectListener->moduleSelected(i, SlotModuleEventId);
            break;
         }
      }
   }
   else if(event.getId() == ItemListEventId)
   {
      moduleSelectListener->moduleSelected(itemListBox.getSelected(), ItemListEventId);
   }
}

void EquipPane::resizeEquipSlotLabels(int numSlotsRequired)
{
   int numCurrentSlots = slotLabels.size();

   if(numCurrentSlots == numSlotsRequired)
   {
      // Nothing to do here if we have exactly as many slots as we need.
      return;
   }
   else if(numSlotsRequired < numCurrentSlots)
   {
      // At least one slot was removed since the last refresh,
      // so unregister and remove the extra slot label widgets.
      for(int i = numSlotsRequired; i < numCurrentSlots; ++i)
      {
         slotContainer.remove(&slotLabels[i]);
      }

      slotLabels.resize(numSlotsRequired);
   }
   else if(numCurrentSlots < numSlotsRequired)
   {
      slotLabels.resize(numSlotsRequired);
      for(int i = numCurrentSlots; i < numSlotsRequired; ++i)
      {
         slotContainer.add(&slotLabels[i]);
         slotLabels[i].setActionEventId(SlotModuleEventId);
         slotLabels[i].addActionListener(this);
      }
   }
}

void EquipPane::refreshEquipSlots()
{
   int numSlots = equipSlots.size();
   resizeEquipSlotLabels(numSlots);

   for(int i = 0; i < numSlots; ++i)
   {
      const Item* item = equipSlots[i]->equipped;
      if(item != NULL)
      {
         slotLabels[i].setCaption(item->getName());
      }
      else
      {
         slotLabels[i].setCaption("(unequipped)");
      }

      slotLabels[i].adjustSize();
   }

   slotContainer.adjustContent();
}

void EquipPane::refresh()
{
   refreshEquipSlots();
   itemListBox.adjustSize();
   invalidated = false;
}

void EquipPane::invalidate()
{
   invalidated = true;
}

void EquipPane::logic()
{
   MenuPane::logic();
   if(invalidated)
   {
      refresh();
   }
}

EquipPane::~EquipPane()
{
}
