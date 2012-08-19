/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#error "This file (TODOLIST.h) should NOT be compiled. It is only in the source tree for use by Doxygen."

/**
 * \todo [Recommended] Fix Resource getSize functions to return an accurate size estimate for their resources
 * \todo [Recommended] Potential deadlock: What if a dialogue line has an embedded script that calls to the dialogue controller and yields? Is this important?
 * \todo [Recommended] Complete or remove ScriptException class, as it is currently empty and pointless (document it if its staying).
 * \todo [Recommended] Update ResourceLoader to include reference counting and LRU resource eviction.
 * \todo [Recommended] Route all debug output into the debug console instead of the terminal/console window
 * \todo [Recommended] Allow for turning debug flags on and off dynamically through the console to prevent flooding.
 * \todo [Optional] Add support for multi-level maps.
 * \todo [Optional] Consolidate common functionality between Tileset and Spritesheet.
 * \todo [Optional] Dialogue Controller should inherit from a generic "scrolling text controller" that takes a textbox argument to write to
   * To do this, a full rewrite of the Dialogue Controller might be necessary, due to some tight coupling in the class at the moment
 * \todo [Optional] Integrate GraphicsUtil stuff from Solstice (synchronous fading and texture load, mainly).
 * \todo [Optional] Change OpenGLTTF to remove dependency on gcn::Image
 * \todo [Optional] Create some test harnesses/sandboxes for different pieces of the engine.
 * \todo [Optional] Set up a unit testing framework and create unit tests for the various pieces of the game.
 * \todo [Optional] Organize the global code folder and try to create some new folders for organization
 * \todo [Optional] Tileset passibility matrix uses a bool (1 word) for each entry; it should only need to use one bit per entry.
 * \todo [Optional] Move GL drawing code in Tileset/Spritesheet to GraphicsUtil (less coupling)
 */
void DevelopmentTodoList(void);

/**
 * \todo Create a slider for volume control of music.
 * \todo Create a slider for volume control of sound.
 * \todo Create a dropdown for known screen resolutions (Width x Height x BitsPerPixel)
 * \todo Create a slider or list for controlling dialogue speed.
 * \todo Create a demo of the dialogue speed that runs some text when the speed changes.
 * \todo Create a slider for memory usage by the resource loader (requires the resource loader to be finished)
 * \todo Create a submenu for changing control schemes (key associations, gamepad calibration, etc.)
 */
void OptionsMenuTodoList(void);

/**
 * \todo freezeUserInput/startCutScene - don't allow user input (begin a cutscene)
 * \todo permitUserInput/endCutScene - allow user input (end a cutscene)
 * \todo freezeNPCs - don't allow NPCs to move/idle
 * \todo unfreezeNPCs - allow NPCs to move/idle
 * \todo freezeNPC - freeze a particular NPC
 * \todo unfreezeNPC - unfreeze a particular NPC
 * \todo fadeMusic function - fade a song out with a given time delay (possibly blocking until the song is faded)
 * \todo setLoopPoints function - set new looping points for a song (song continues playing with new points)
 * \todo updateQuest function - Add a quest to or update a quest in the player's quest log
 * \todo getQuestStatus function - Get the status in the quest log for a specific quest
 * \todo fadeToColor function - Synchronous fading of the screen into a specific colour, with a specific alpha
 */
void ScriptFunctionsTodoList(void);
