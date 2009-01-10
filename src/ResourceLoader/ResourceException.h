#ifndef __RESOURCE_EXCEPTION_H_
#define __RESOURCE_EXCEPTION_H_

#include "Exception.h"

class ResourceException : public Exception
{   std::string resourceName;

    public:
        ResourceException(Exception& e, const std::string& resourceName) 
                : Exception(e.getFunction(), e.getLine(), 
                      "Failed to load resource: " + resourceName + ".\nReason: " + e.getMessage()),
                             resourceName(resourceName)
        {  
        }

       ResourceException(const std::string& function,
          int line, const std::string& resourceName) 
                : Exception(function, line, 
                      "Failed to load resource: " + resourceName),
                             resourceName(resourceName)
        {  
        }

       ResourceException(const std::string& function,
          int line, const std::string& resourceName, const std::string& reason) 
                : Exception(function, line, 
                      "Failed to load resource: " + resourceName + ".\nReason: " + reason),
                             resourceName(resourceName)
        {  
        }

        ~ResourceException() throw()
        {}
};

#endif
