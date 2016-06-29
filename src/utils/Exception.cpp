/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "Exception.h"
#include <sstream>

Exception::Exception(const std::string& function,
       int line, const std::string& message) :
   m_function(function),
   m_line(line),
   m_message(message)
{
}

std::string Exception::getFullMessage() const
{
   std::stringstream err;
   err << m_message << "\n at line " << m_line << " of function " << m_function << '\n';

   return err.str();
}

const std::string& Exception::getMessage() const
{
   return m_message;
}

const std::string& Exception::getFunction() const
{
   return m_function;
}

int Exception::getLine() const
{
   return m_line;
}
