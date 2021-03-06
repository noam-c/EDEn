/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef DEBUG_COMMAND_MESSAGE_H
#define DEBUG_COMMAND_MESSAGE_H

#include <string>

struct DebugCommandMessage final
{
   const std::string command;

   DebugCommandMessage(const std::string& command);
};

#endif
