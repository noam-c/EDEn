#include "Exception.h"
#include <sstream>

Exception::Exception()
       : function(""),
         line(-1),
         message("")
{
}

Exception::Exception(const std::string& function,
       int line, const std::string& message)
       : function(function),
         line(line),
         message(message)
{
}

std::string Exception::getFullMessage() const
{
   std::stringstream err;
   err << message << "\n at line " << line << " of function " << function << '\n';

   return err.str();
}

const std::string& Exception::getMessage() const
{
   return message;
}

const std::string& Exception::getFunction() const
{
   return function;
}

int Exception::getLine() const
{
   return line;
}

Exception::~Exception()
{
}
