/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

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

   void StringListModel::add(const std::string& label, int a)
   {
      listOps.push_back(ListItem(label,a));
   }
    
   void StringListModel::clear()
   {
      listOps.clear();
   }
};
