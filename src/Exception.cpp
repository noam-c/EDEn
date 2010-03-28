#include "Exception.h"
#include <sstream>

Exception::Exception()
{
}

Exception::Exception(const std::string& function,
       int line, const std::string& message)
       : message(message),
         function(function),
         line(line)
{
}

const std::string& Exception::getMessage() const throw()
{
   std::stringstream err;
   err << message << "\n at line " << line << " of function " << function << '\n';

   return err.str().c_str();
}

const std::string& Exception::getFunction() const
{
   return function;
}

int Exception::getLine() const
{
   return line;
}

Exception::~Exception() throw()
{
}
