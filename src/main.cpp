#include "GraphicsUtil.h"
#include "ScriptEngine.h"
#include "ExecutionStack.h"
#include "MainMenu.h"
#include "ResourceLoader.h"
#include <guichan.hpp>

#include <iostream>
#include <fstream>

/**
 * The main function.
 * Creates the graphics utilities, pushes a title screen onto the ExecutionStack,
 * and executes it. Afterwards, destroys graphics utilities and we're done.
 */
int main (int argc, char *argv[])
{   
    try
    {  GraphicsUtil::getInstance();
       ExecutionStack* stack = ExecutionStack::getInstance();
       std::cerr << "Pushing state" << std::endl;
       stack->pushState(new MainMenu());
       std::cerr << "Executing game" << std::endl;
       stack->execute();
       ResourceLoader::freeAll();

       ExecutionStack::destroy();
       GraphicsUtil::destroy();
    }
    catch (gcn::Exception e)
  	 {  std::cerr << e.getMessage() << std::endl;
       return 1;
    }
    catch(Exception e)
    {  std::cerr << e.getMessage() << std::endl;
       return 1;
    }
    catch(std::exception e)
    {  std::cerr << e.what() << std::endl;
       return 1;
    }
    catch(...)
    {  std::cerr << "GENERAL BULLSHIT" << std::endl;
       return 1;
    }	

    return 0;
}
