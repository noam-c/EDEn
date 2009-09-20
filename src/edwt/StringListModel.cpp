#include "StringListModel.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_EDWT;

namespace edwt
{
   int StringListModel::getNumberOfElements()
   {
      return listOps.size();
   }

   std::string StringListModel::getElementAt(int i)
   {
      return listOps[i].name;
   }
    
   int StringListModel::getActionAt(int i)
   {
      return listOps[i].actionFlag;
   }

   void StringListModel::add(std::string label, int a)
   {
      listOps.push_back(ListItem(label,a));
   }
    
   void StringListModel::clear()
   {
      listOps.clear();
   }
};
