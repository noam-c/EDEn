#error "This file (TODOLIST.h) should NOT be compiled. It is only in the source tree for use by Doxygen."

/**
 * \todo Complete and populate the Exodus Draconis website (PLEASE).
 * \todo Find a better way to do the todo list and to prioritize tasks.
 * \todo Consolidate Story and Reqs wikis.
 * \todo Create new Code wiki.
 */
void OrganizationTodoList(void);

/**
 * \todo [Mandatory] Add collision detection for moving NPCs.
 * \todo [Mandatory] Recalculate an NPC's best path dynamically if there is a problem travelling a path.
 * \todo [Mandatory] Maps should specify special keys for certain tiles (such as "entrance1" to denote one of the entrance tiles) to be referred to by scripts
 * \todo [Mandatory] Dialogue Controller needs a blinking cursor to indicate finished dialogue.
 * \todo [Mandatory] Create a "Controls" class that translates pressed keys/buttons to controller-agnostic instructions based on user options.
 * \todo [Mandatory] Create an options menu with persistence.
 * \todo [Mandatory] Create an About/Credits screen
 * \todo [Recommended] System tests for all the script functions
 * \todo [Recommended] The Lua script bindings need to be more strict about parameters
   * Do type-checking, enforce strict argument counts, and write an API spec to explain the functions available
 * \todo [Recommended] Fix Resource getSize functions to return an accurate size estimate for their resources
 * \todo [Recommended] Potential deadlock: What if a dialogue line has an embedded script that calls to the dialogue controller and yields? Is this important?
 * \todo [Recommended] Complete or remove ScriptException class, as it is currently empty and pointless (document it if its staying).
 * \todo [Recommended] Update ResourceLoader to include reference counting and LRU resource eviction.
 * \todo [Recommended] Route all debug output into the debug console instead of the terminal/console window
 * \todo [Recommended] Allow for turning debug flags on and off dynamically through the console to prevent flooding.
 * \todo [Optional] Add support for multi-level maps.
 * \todo [Optional] Consolidate common functionality between Tileset and Spritesheet.
 * \todo [Optional] Perhaps make NPCs a type of Lua userdata so that we don't need C-like global functions on them? (Might want to get the Lunar library to bind the NPCs to Lua)
 * \todo [Optional] Remove references to namespace gcn outside of edwt (as many as possible if not all)
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

/**
 * \todo Add Academic Magic descriptions and crests from DA page
 * \todo Come up with a title for Part 2.
 * \todo Finish descriptions of Parts 1-3.
 * \todo Complete the description of the Crystal Guardian, and come up with a name for her.
 * \todo Fill in other blanks as necessary.
 */
void StoryTodoList(void);
