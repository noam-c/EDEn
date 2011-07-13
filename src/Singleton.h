#ifndef SINGLETON_H
#define SINGLETON_H

#ifndef NULL
   #define NULL 0
#endif

/**
 * The Singleton template class is a helper class that stores all the code
 * necessary to implement a Singleton class.
 *
 * To make a class a singleton, there is a simple set of instructions:
 * -# Make the class a subclass of singleton, using itself as the template class
 *    - For instance, a singleton class Car would be declared like so:<br>
 *    <code>class Car : public Singleton<Car></code>
 * -# Instead of implementing a constructor, use initialize()
 *    - Declaring a constructor for your singleton would result in a
 *      compile-time error, so use initialize() for all your parameter
 *      initialization.
 * -# Instead of implementing a destructor, use finish()
 * -# Gaze upon your new singleton class with pride. Use getInstance() to grab
 *    a singleton instance, and use destroy() to force a cleanup of an existing
 *    singleton
 *
 * \todo At the time of writing, there is no static "manager" to clean up
 *       all instantiated singletons, but there certainly should be!
 *
 * @author Noam Chitayat
 */
template<class C> class Singleton
{
   protected:
      /**
       * The singleton instance of class C.
       */
      static Singleton<C>* instance;

      /**
       * Initializes the singleton of class C.
       * This method is run after the default constructor of C,
       * and should be implemented to take care of all the initialization of
       * the new object.
       */
      virtual void initialize() = 0;
       
      /**
       * Uninitializes the singleton of class C.
       * This method is run before the destructor of C, and should be
       * implemented to take care of all the cleanup of the object before
       * it gets deleted.
       */
      virtual void finish() = 0;

   public:
      /**
       * @return the singleton instance of the class C.
       */
      static C* getInstance()
      {
         if(!Singleton<C>::instance)
         {
            Singleton<C>::instance = new C();
            Singleton<C>::instance->initialize();
         }
      
         return static_cast<C*>(instance);
      }

      /**
       * Destroy the singleton instance (if it exists)
       */
      static void destroy()
      {
         if(Singleton<C>::instance)
         {
            Singleton<C>::instance->finish();
            delete Singleton<C>::instance;
            Singleton<C>::instance = NULL;
         }
      }
};

template<class C> Singleton<C>* Singleton<C>::instance = NULL;

#endif
