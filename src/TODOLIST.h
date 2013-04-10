/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#error "This file (TODOLIST.h) should NOT be compiled. It is only in the source tree for use by Doxygen."

/**
 * \todo [Recommended] Potential deadlock: What if a dialogue line has an embedded script that calls to the dialogue controller and yields? Is this important?
 * \todo [Recommended] Complete or remove ScriptException class, as it is currently empty and pointless (document it if its staying).
 * \todo [Recommended] Route all debug output into the debug console instead of the terminal/console window
 * \todo [Recommended] Allow for turning debug flags on and off dynamically through the console to prevent flooding.
 * \todo [Optional] Add support for multi-level maps.
 * \todo [Optional] Dialogue Controller should inherit from a generic "scrolling text controller" that takes a textbox argument to write to
   * To do this, a full rewrite of the Dialogue Controller might be necessary, due to some tight coupling in the class at the moment
 */
void DevelopmentTodoList(void);

/**
 * \todo setLoopPoints function - set new looping points for a song (song continues playing with new points)
 * \todo fadeToColor function - Synchronous fading of the screen into a specific colour, with a specific alpha
 */
void ScriptFunctionsTodoList(void);
