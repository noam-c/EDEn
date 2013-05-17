cmake_minimum_required(VERSION 2.8.5)

project( eden )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")

set(HEADERS
  src/Audio/Music.h
  src/Audio/Sound.h
  src/Coroutines/Coroutine.h
  src/Coroutines/Scheduler.h
  src/Coroutines/Task.h
  src/Coroutines/TaskId.h
  src/Coroutines/Timer.h
  src/DebugUtils.h
  src/DebugConsoleWindow.h
  src/Exception.h
  src/ExecutionStack.h
  src/GameContext.h
  src/GameState.h
  src/Graphics/GraphicsUtil.h
  src/Graphics/OpenGLExtensions.h
  src/Graphics/ScreenTexture.h
  src/Graphics/Texture.h
  src/json/json.h
  src/json/json-forwards.h
  src/Metadata/Item.h
  src/Metadata/ItemList.h
  src/Metadata/Metadata.h
  src/Metadata/Skill.h
  src/Metadata/SkillList.h
  src/Metadata/Usable.h
  src/Metadata/UsableId.h
  src/LuaWrapper/LuaWrapper.hpp
  src/MainMenu/MainMenu.h
  src/MainMenu/TitleScreenSettingsMenu.h
  src/Menu/CharacterDependentMenu.h
  src/Menu/DataMenu.h
  src/Menu/DataViewModel.h
  src/Menu/HomeMenu.h
  src/Menu/HomeViewModel.h
  src/Menu/ImageFormatter.h
  src/Menu/ItemMenu.h
  src/Menu/ItemViewModel.h
  src/Menu/MenuShell.h
  src/Menu/MenuShellOption.h
  src/Menu/MenuState.h
  src/Menu/SkillMenu.h
  src/Menu/SkillViewModel.h
  src/Menu/ShortcutBar.h
  src/Messaging/MessagePipe.h
  src/Messaging/Listener.h
  src/MovementDirection.h
  src/PlayerData/Character.h
  src/PlayerData/LuaCharacter.h
  src/PlayerData/CharacterRoster.h
  src/PlayerData/LuaCharacterRoster.h
  src/PlayerData/PlayerData.h
  src/PlayerData/PlayerDataSummary.h
  src/PlayerData/EquipData.h
  src/PlayerData/EquipSlot.h
  src/PlayerData/Inventory.h
  src/PlayerData/LuaInventory.h
  src/PlayerData/Shortcut.h
  src/PlayerData/Quest.h
  src/PlayerData/LuaQuest.h
  src/PlayerData/Messages/RosterUpdateMessage.h
  src/ResourceLoader/Resource.h
  src/ResourceLoader/ResourceKey.h
  src/ResourceLoader/ResourceLoader.h
  src/rocket/EdenRocketBindings.h
  src/rocket/EdenRocketRenderInterface.h
  src/rocket/EdenRocketSystemInterface.h
  src/rocket/RocketSDLInputMapping.h
  src/rocket/RocketListener.h
  src/rocket/RocketScriptHandler.h
  src/rocket/RocketScriptHandlerFactory.h
  src/ScriptEngine/FileScript.h
  src/ScriptEngine/NPCScript.h
  src/ScriptEngine/UsableScript.h
  src/ScriptEngine/Script.h
  src/ScriptEngine/ScriptEngine.h
  src/ScriptEngine/ScriptException.h
  src/ScriptEngine/ScriptFactory.h
  src/ScriptEngine/ScriptUtilities.h
  src/ScriptEngine/StringScript.h
  src/Shapes/Size.h
  src/Shapes/Point2D.h
  src/Shapes/Rectangle.h
  src/Settings/Settings.h
  src/Singleton.h
  src/Sprites/Animation.h
  src/Sprites/FrameSequence.h
  src/Sprites/Sprite.h
  src/Sprites/Spritesheet.h
  src/TileEngine/Actor.h
  src/TileEngine/Actor_Orders.h 
  src/TileEngine/LuaActor.h
  src/TileEngine/Camera.h
  src/TileEngine/DialogueController.h
  src/TileEngine/EntityGrid.h
  src/TileEngine/Layer.h
  src/TileEngine/Map.h
  src/TileEngine/MapExit.h
  src/TileEngine/NPC.h
  src/TileEngine/Pathfinder.h
  src/TileEngine/PlayerCharacter.h
  src/TileEngine/LuaPlayerCharacter.h
  src/TileEngine/Region.h
  src/TileEngine/TileEngine.h
  src/TileEngine/LuaTileEngine.h
  src/TileEngine/Tileset.h
  src/TileEngine/TileState.h
  src/TileEngine/TriggerZone.h
  src/TileEngine/Messages/ActorMoveMessage.h
  src/TileEngine/Messages/MapExitMessage.h
  src/TileEngine/Messages/MapTriggerMessage.h
  src/tinyxml/tinystr.h
  src/tinyxml/tinyxml.h
  src/Transitions/TransitionState.h
  src/Transitions/FadeState.h
  src/Transitions/SpinState.h
  src/Transitions/RandomTransitionGenerator.h
  src/Transitions/BlendState.h
  src/TODOLIST.h
)

set(SOURCES 
  src/tinyxml/tinystr.cpp
  src/tinyxml/tinyxml.cpp
  src/tinyxml/tinyxmlerror.cpp
  src/tinyxml/tinyxmlparser.cpp
  src/json/jsoncpp.cpp
  src/Audio/Music.cpp
  src/Audio/Sound.cpp
  src/Coroutines/Coroutine.cpp
  src/Coroutines/Scheduler.cpp
  src/Coroutines/Task.cpp
  src/Coroutines/Timer.cpp
  src/Metadata/Metadata.cpp
  src/Metadata/Skill.cpp
  src/Metadata/Usable.cpp
  src/MainMenu/MainMenu.cpp
  src/MainMenu/MainMenuActions.cpp
  src/MainMenu/TitleScreenSettingsMenu.cpp
  src/Menu/CharacterDependentMenu.cpp
  src/Menu/DataMenu.cpp
  src/Menu/DataViewModel.cpp
  src/Menu/HomeMenu.cpp
  src/Menu/HomeViewModel.cpp
  src/Menu/ImageFormatter.cpp
  src/Menu/ItemMenu.cpp
  src/Menu/ItemViewModel.cpp
  src/Menu/MenuShell.cpp
  src/Menu/MenuState.cpp
  src/Menu/SkillMenu.cpp
  src/Menu/SkillViewModel.cpp
  src/Menu/ShortcutBar.cpp
  src/Messaging/MessagePipe.cpp
  src/PlayerData/Character.cpp
  src/PlayerData/CharacterRoster.cpp
  src/PlayerData/LuaCharacter.cpp
  src/PlayerData/LuaCharacterRoster.cpp
  src/PlayerData/PlayerData.cpp
  src/PlayerData/PlayerDataSummary.cpp
  src/PlayerData/Quest.cpp
  src/PlayerData/LuaQuest.cpp
  src/PlayerData/EquipData.cpp
  src/PlayerData/EquipSlot.cpp
  src/PlayerData/Inventory.cpp
  src/PlayerData/LuaInventory.cpp
  src/PlayerData/Shortcut.cpp
  src/PlayerData/Messages/RosterUpdateMessage.cpp
  src/ResourceLoader/Resource.cpp
  src/ResourceLoader/ResourceLoader.cpp
  src/rocket/EdenRocketRenderInterface.cpp
  src/rocket/EdenRocketSystemInterface.cpp
  src/rocket/RocketScriptHandler.cpp
  src/rocket/RocketScriptHandlerFactory.cpp
  src/rocket/RocketSDLInputMapping.cpp
  src/Shapes/Size.cpp
  src/Shapes/Point2D.cpp
  src/Shapes/Rectangle.cpp
  src/ScriptEngine/FileScript.cpp
  src/ScriptEngine/NPCScript.cpp
  src/ScriptEngine/UsableScript.cpp
  src/ScriptEngine/Script.cpp
  src/ScriptEngine/ScriptEngine.cpp
  src/ScriptEngine/ScriptFactory.cpp
  src/ScriptEngine/ScriptFunctions.cpp
  src/ScriptEngine/ScriptUtilities.cpp
  src/ScriptEngine/StringScript.cpp
  src/Settings/Settings.cpp
  src/Sprites/Animation.cpp
  src/Sprites/Sprite.cpp
  src/Sprites/Spritesheet.cpp
  src/TileEngine/Actor.cpp
  src/TileEngine/Actor_MoveOrder.cpp
  src/TileEngine/Actor_StandOrder.cpp
  src/TileEngine/LuaActor.cpp
  src/TileEngine/Camera.cpp
  src/TileEngine/DialogueController.cpp
  src/TileEngine/EntityGrid.cpp
  src/TileEngine/Layer.cpp
  src/TileEngine/Map.cpp
  src/TileEngine/MapExit.cpp
  src/TileEngine/NPC.cpp
  src/TileEngine/PlayerCharacter.cpp
  src/TileEngine/LuaPlayerCharacter.cpp
  src/TileEngine/Pathfinder.cpp
  src/TileEngine/Region.cpp
  src/TileEngine/TileEngine.cpp
  src/TileEngine/LuaTileEngine.cpp
  src/TileEngine/Tileset.cpp
  src/TileEngine/TileState.cpp
  src/TileEngine/TriggerZone.cpp
  src/TileEngine/Messages/ActorMoveMessage.cpp
  src/TileEngine/Messages/MapExitMessage.cpp
  src/TileEngine/Messages/MapTriggerMessage.cpp
  src/Transitions/TransitionState.cpp
  src/Transitions/FadeState.cpp
  src/Transitions/SpinState.cpp
  src/Transitions/RandomTransitionGenerator.cpp
  src/Transitions/BlendState.cpp
  src/main.cpp
  src/DebugUtils.cpp
  src/DebugConsoleWindow.cpp
  src/Exception.cpp
  src/ExecutionStack.cpp
  src/GameContext.cpp
  src/GameState.cpp
  src/Graphics/GraphicsUtil.cpp
  src/Graphics/OpenGLExtensions.cpp
  src/Graphics/ScreenTexture.cpp
  src/Graphics/Texture.cpp
)

set(SOURCE_GROUP_DELIMITER "/")

IF(NOT ${CMAKE_GENERATOR} STREQUAL "Xcode")
   source_group("//" REGULAR_EXPRESSION src/[^/]*)
ENDIF()

source_group(Audio REGULAR_EXPRESSION src/Audio/.*)
source_group(Coroutines REGULAR_EXPRESSION src/Coroutines/.*)
source_group(Metadata REGULAR_EXPRESSION src/Metadata/.*)
source_group(Graphics REGULAR_EXPRESSION src/Graphics/.*)
source_group(MainMenu REGULAR_EXPRESSION src/MainMenu/.*)
source_group(Menu REGULAR_EXPRESSION src/Menu/.*)
source_group(MenuControls REGULAR_EXPRESSION src/MenuControls/.*)
source_group(Messaging REGULAR_EXPRESSION src/Messaging/.*)
source_group(PlayerData REGULAR_EXPRESSION src/PlayerData/.*)
source_group(ResourceLoader REGULAR_EXPRESSION src/ResourceLoader/.*)
source_group(rocket REGULAR_EXPRESSION src/rocket/.*)
source_group(ScriptEngine REGULAR_EXPRESSION src/ScriptEngine/.*)
source_group(Settings REGULAR_EXPRESSION src/Settings/.*)
source_group(Shapes REGULAR_EXPRESSION src/Shapes/.*)
source_group(Sprites REGULAR_EXPRESSION src/Sprites/.*)
source_group(TileEngine REGULAR_EXPRESSION src/TileEngine/.*)
source_group(Transitions REGULAR_EXPRESSION src/Transitions/.*)
source_group(json REGULAR_EXPRESSION src/json/.*)
source_group(LuaWrapper REGULAR_EXPRESSION src/LuaWrapper/.*)
source_group(tinyxml REGULAR_EXPRESSION src/tinyxml/.*)

IF(NOT ${CMAKE_GENERATOR} STREQUAL "Xcode")
   source_group("PlayerData\\Messages" REGULAR_EXPRESSION src/PlayerData/Messages/.*)
   source_group("TileEngine\\Messages" REGULAR_EXPRESSION src/TileEngine/Messages/.*)
ENDIF()

include_directories( 
  src
  src/Audio
  src/Coroutines
  src/Metadata
  src/Graphics
  src/MainMenu
  src/Menu
  src/MenuControls
  src/Messaging
  src/PlayerData
  src/PlayerData/Messages
  src/ResourceLoader
  src/rocket
  src/ScriptEngine
  src/Settings
  src/Shapes
  src/Sprites
  src/TileEngine
  src/TileEngine/Messages
  src/Transitions
  src/json
  src/LuaWrapper
  src/tinyxml
)

add_definitions( 
  -Wall
  -DDEBUG_MODE
  -D_CONSOLE
)

add_executable( eden ${SOURCES} ${HEADERS} )

IF(WIN32)
  target_link_libraries(
    eden
    SDLmain
    debug rocketCore_d
    debug rocketControls_d
    optimized rocketCore
    optimized rocketControls
    lua5.1
    SDL_ttf
    SDL_image
    SDL_mixer
    SDL
    opengl32
    glu32
  )
ELSE(WIN32)
	INCLUDE(FindOpenGL)
	INCLUDE(FindSDL)
	INCLUDE(FindSDL_image)
	INCLUDE(FindSDL_ttf)
	INCLUDE(FindSDL_mixer)
	INCLUDE(FindLua51)
	INCLUDE(FindLibRocket)
	INCLUDE(FindLibRocketControls)

	set(INCL_HEADERS
	  ${LUA_INCLUDE_DIR}
	  ${ROCKET_INCLUDE_DIR}
	  ${ROCKET_CONTROLS_INCLUDE_DIR}
	  ${SDL_INCLUDE_DIR}
	  ${SDLIMAGE_INCLUDE_DIR}
	  ${SDLTTF_INCLUDE_DIR}
	  ${SDLMIXER_INCLUDE_DIR}
	  ${OPENGL_INCLUDE_DIR}
	)

	include_directories(BEFORE SYSTEM ${INCL_HEADERS})

	target_link_libraries( eden ${LIBROCKET_LIBRARY} ${LIBROCKET_CONTROLS_LIBRARY} ${LUA_LIBRARIES} ${SDLTTF_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLMIXER_LIBRARY} ${SDL_LIBRARY} ${OPENGL_LIBRARIES} )
ENDIF(WIN32)

