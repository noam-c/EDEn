EDEn README
===========

EDEn (the Exodus Draconis Engine) is a free and open-source engine used to create top-down, 2D RPGs with 3D effects. Aside from the usual trimmings one would expect from the genre, the engine aims to also enable some gameplay elements typically found in adventure games.

The initial purpose of this engine was to facilitate the creation of a very specific game (Exodus Draconis, for which the engine is named). While development of the game continues, the engine will contain common code and utilities created along the way that may aid in the development of other RPGs.

EDEn is programmed in C++ and uses SDL for graphics and audio. By exposing C++ functionality as a Lua API, EDEn allows game developers to develop and tweak gameplay, quests and stories on the fly without requiring the source to be recompiled. The engine can also handle a variety of image and audio assets supported by SDL_image and SDL_mixer, respectively.

In general, EDEn aims to help game designers quickly turn RPG stories, gameplay and level designs into playable RPGs. 

License and Copyright
---------------------

Most of the code is Copyright 2016 Noam Chitayat. Contributors retain copyright to their original contributions.

The EDEn source is released under the terms of the Ruby License, with the following exceptions:

* The following CMake scripts (located in the CMakeModules folder) are modified from CMake source code, and are thus made available under CMake's BSD license. Please refer to CMakeModules/CMakeCopyright.md for additional information.
  - FindSDL2.cmake
  - FindSDL2_image.cmake
  - FindSDL2_mixer.cmake

* Code in the json folder or under the Json namespace is part of the JsonCpp JSON parsing library.

	JsonCpp is copryright 2007-2016 Baptiste Lepilleur. Distributed under MIT license, or public domain if desired and recognized in your jurisdiction.

* Code in the tinyxml folder is part of the TinyXML XML parsing library.

	TinyXML is copyright 2000-2016 Lee Thomason.

* Code in the LuaWrapper folder is part of the LuaWrapper library.

	LuaWrapper is copyright 2010-2016 Alexander Ameson.

Art and audio assets are copyright their creators. For specific asset credits, please refer to ASSETS.txt.

For the full text of the Ruby license in use, please refer to the LICENSE.txt file.

Dependencies
------------

EDEn makes use of the CMake build system.

EDEn depends on the following development libraries to build the source:

* SDL 2.0
* SDL_image 2.0
* SDL_mixer 2.0
* Lua 5.1
* libRocket

Windows developers will also need the Windows implementation of dirent.h.

EDEn requires the equivalent runtime libraries to execute.

Maps and tilesets in EDEn are created using the Tiled Map Editor (http://www.mapeditor.org).
For more information on the Tiled Map (TMX) and Tileset (TSX) formats, please refer to the Tiled wiki (http://github.com/bjorn/tiled/wiki/TMX-Map-Format).

Links
-----

* CMake - http://www.cmake.org
* SDL Cross-platform Library - http://www.libsdl.org
* libRocket - http://www.librocket.com
* Lua Programming Language - http://www.lua.org
* JsonCpp library - http://jsoncpp.sourceforge.net/
* TinyXml library - http://www.grinninglizard.com/tinyxml/
* dirent.h implementation for Windows - http://www.softagalleria.net/dirent.php

Running EDEn
------------

To build EDEn by command-line, navigate to the root directory and run the following commands:

	cmake .
	make
	./eden

On Windows, CMake can be used to create a Visual Studio solution that can be used to build and run the source.

On Mac OS X, CMake can be used to create an XCode project that can be used to build and run the source.

