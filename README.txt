EDEn README
====================

EDEn (the Exodus Draconis Engine) is a free and open-source engine used to create top-down, 2D RPGs with 3D effects. Aside from the usual trimmings one would expect from the genre, the engine aims to also enable some gameplay elements typically found in adventure games.

The initial purpose of this engine was to facilitate the creation of a very specific game (Exodus Draconis, for which the engine is named). While development of the game continues, the engine will contain common code and utilities created along the way that may aid in the development of other RPGs.

EDEn is programmed in C++ and uses SDL for graphics and audio. By exposing C++ functionality as a Lua API, EDEn allows game developers to develop and tweak gameplay, quests and stories on the fly without requiring the source to be recompiled. The engine can also handle a variety of image and audio assets supported by SDL_image and SDL_mixer, respectively.

In general, EDEn aims to help game designers quickly turn RPG stories, gameplay and level designs into playable RPGs. 

LICENSE AND COPYRIGHT
=====================

Most of the code is Copyright 2011 Noam Chitayat. Contributors retain copyright to their original contributions.

The EDEn source is released under the terms of the Ruby License, with the following exceptions:

	- Code in the guichan folder or under the gcn namespace is part of the Guichan GUI library. Guichan is copyright 2004-2011 Olof Naessén and Per Larsson. All rights reserved. The Guichan source code is made available by the developers under the terms of the 3-clause BSD license.

Art and audio assets are copyright their creators. Specifically:
	- Schroeder's Failure, by Trash80, is used as a title theme
	- Sprite artwork is from Chrono Trigger, copyright Square Enix. All rights reserved.

For the full text of the Ruby license in use, please refer to the LICENSE.txt file.

DEPENDENCIES
============

EDEn depends on the following development libraries to build the source:

SDL 1.2
SDL_image 1.2
SDL_ttf 2.0
SDL_mixer 1.2
Lua 5.1

EDEn requires the equivalent runtime libraries to execute.

LINKS
=====

SDL Cross-platform Library - http://www.libsdl.org
Guichan GUI library - 
Lua Programming Language - http://www.lua.org

RUNNING EDEN
============

To build EDEn by command-line, navigate to the root directory and run the following commands:

	cmake .
	make
	./eden

On Windows, CMake can be used to create a Visual Studio solution that can be used to build and run the source.

On Mac OS X, CMake can be used to create an XCode project that can be used to build and run the source.

