#ifndef __EXCEPTION_H_
#define __EXCEPTION_H_

#include <exception>
#include <string>

class Exception : public std::exception
{  Exception() {}

   std::string function;
   std::string message;
   int line;

   public:
      Exception(const std::string& function,
             int line, const std::string& message);

      const char* what() const throw();
      const std::string& getFunction() const;
      const std::string& getMessage() const;
      int getLine() const;
      ~Exception() throw();

};

#endif
