/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>

/**
 * The Exception class is used for throwing generic exceptions that do not fall
 * under a topic covered by Exception's subclasses.
 * This class inherits functionality from std::exception and should always be
 * used instead of std::exception.
 * Where possible, do not use this Exception, and instead use a subclass.
 * If it seems like a bunch of Exceptions fall under the same topic, then create
 * a new subclass to encompass these.
 *
 * DebugUtils.h contains macros for throwing generic Exceptions easily.
 *
 * @author Noam Chitayat
 */
class Exception : public std::exception
{  
   /**
    * Making the default constructor private to force people to throw detailed exceptions.
    */
   Exception();
   
   /**
    * The name of the function from which this Exception was thrown.
    */
   const std::string m_function;

   /**
    * The line number from which this Exception was thrown.
    */
   int m_line;

   /**
    * The message associated with the Exception.
    */
   const std::string m_message;

   public:
      /**
       * Constructor for a generic exception.
       *
       * @param function The name of the function that threw this Exception.
       * @param line The line number from which this Exception was thrown.
       * @param message The description of the reason that this Exception was thrown.
       */
      Exception(const std::string& function,
             int line, const std::string& message);

      /**
       * @return The full description this Exception (includes message and exception location).
       */
      std::string getFullMessage() const;

      /**
       * @return The name of the function that threw this Exception.
       */
      const std::string& getFunction() const;

      /**
       * @return The description of the problem that caused this Exception.
       */
      const std::string& getMessage() const;

      /**
       * @return the line number from which this Exception was thrown.
       */
      int getLine() const;

      /**
       * Destructor.
       */
      virtual ~Exception() throw();
};

#endif
