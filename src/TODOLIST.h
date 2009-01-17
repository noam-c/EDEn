#error "This file (TODOLIST.h) should NOT be compiled. It is only in the source tree for use by Doxygen."

/**
 * \todo Find a better way to do the todo list and to prioritize tasks.
 * \todo Consolidate Story and Reqs wikis.
 * \todo Create new Code wiki.
 */
void ProjectOrganizationTodoList(void);

/**
 * \todo Dialogue Controller should be able to take a textbox argument to write to (instead of the usual dialogue controller)
 * \todo Give each script thread its own unique ID, and make the dialogue controller a critical section for NPCs
 * \todo Fix scripts so they can WAIT on an action to be completed
 * \todo Further integration of scripts into Tile Engine, followed by a quick test for all the script functions.
 * \todo Add NPCs to the game.
 * \todo Create an NPC controller for the Tile Engine.
 * \todo Implement semi-coroutines in the NPC controller using Lua.
 * \todo Add the player sprite to the game, and make him respond to user input.
 * \todo Integrate GraphicsUtil stuff from Solstice (synchronous fading and texture load, mainly).
 * \todo Change OpenGLTTF to not depend on gcn::Image
 * \todo Create some test harnesses/sandboxes for different pieces of the engine.
 * \todo Create unit tests for the various pieces of the game.
 * \todo Create an options menu with persistence.
 * \todo Create an About/Credits screen
 * \todo Remove Tileset dependency on SDL by abstracting texture load code into GraphicsUtil
 * \todo Move GL drawing code in Tileset to GraphicsUtil (less coupling)
 * \todo Consider changing the quad drawing code by using GL_ORTHO_2D
 */
void CodeTodoList(void);

/**
 * \todo narrateBlocking function - narrate but block until speech is flushed to screen
 * \todo narrateArrow function - narrate but show an arrow and wait until user advances
 * \todo sayBlocking function - say but block until speech is flushed to screen
 * \todo sayArrow function - say but show an arrow and wait until user advances
 * \todo playSound function - play a sound
 * \todo playSoundBlocking function - play a sound and wait until the sound is finished
 */
void ScriptFunctionsTodoList(void);

/**
 * \todo Add Academic Magic descriptions and crests from http://kainsularei.deviantart.com .
 * \todo Come up with a title for Part 2.
 * \todo Finish descriptions of Parts 1-3.
 * \todo Complete the description of the Crystal Guardian, and come up with a name for her.
 * \todo Fill in other blanks as necessary.
 */
void StoryTodoList(void);
