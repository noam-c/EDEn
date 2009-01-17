#ifndef __GRAPHICS_UTIL_H_
#define __GRAPHICS_UTIL_H_

#include "Singleton.h"

struct SDL_Surface;
union SDL_Event;

namespace gcn
{   class SDLInput;
    class OpenGLGraphics;
    class OpenGLSDLImageLoader;
    class Gui;
    class Container;
};
    
namespace edwt
{   class OpenGLTrueTypeFont;
};

class GraphicsUtil;

/**
 * All cross-class utilities for graphic functionality, such as initialization, effects, and drawing
 * are encapsulated in this singleton class.
 *
 * Note: This class is a singleton.
 *
 * @author Noam Chitayat
 */
class GraphicsUtil : public Singleton<GraphicsUtil>
{
    /** The screen surface */    
    static SDL_Surface* screen;

    /** The Guichan SDL input driver */
    gcn::SDLInput* input;

    /** The Guichan OpenGL Graphics driver */
    gcn::OpenGLGraphics* graphics;

    /** The Guichan OpenGL image loader (for loading images via SDL) */
    gcn::OpenGLSDLImageLoader* imageLoader;

    /** A Gui object - binds all the drivers together */
    gcn::Gui* gui;

    /** The global default font */
    edwt::OpenGLTrueTypeFont* font;

    /**
     * Initializes SDL audio and video bindings
     * Initializes SDL mixer and TTF libraries
     * Initializes an OpenGL viewport and projection
     */
    void initSDL();

    /**
     * Initializes the Guichan library for use with OpenGL via SDL.
     */
    void initGuichan();

protected:
    /** 
     * Constructor.
     * Initializes SDL and OpenGL.
     * Initialize Guichan GUI drivers for OpenGL.
     */
    virtual void initialize();

    /**
     * Singleton destructor.
     * Cleans up the following:
     * - drivers
     * - global font
     * - the SDL TrueTypeFont library
     * - the SDL layer
     */
    virtual void finish();


public:

    /**
     * Set the widget container to draw to screen.
     *
     * @param top The widget container to be used as the top-level container
     */
    void setInterface(gcn::Container* top);

    /**
     * Draw an image from an SDL Surface to a specified xy-coordinate on screen
     *
     * @param img The image surface to draw
     * @param x The x location of the draw destination
     * @param y The y location of the draw destination
     */
    void DrawIMG(SDL_Surface *img, int x, int y);        

    /**
     * Load an image from file using SDL.
     * 
     * @param file the filename of the image to load
     * @return An SDL surface with the loaded image
     */
    SDL_Surface* ImageLoad(char *file);

    /**
     * Flush any enqueued GL commands and then flip the screen buffer
     */
    void flipScreen();

    /**
     * Run GUI widget logic
     */
    void stepGUI();

    /**
     * Draw GUI widgets to the screen
     */
    void drawGUI();

    /**
     * Push an SDL input event to the widgets
     *
     * @param event the input event to send to the widgets
     */
    void pushInput(SDL_Event event);

    /** 
     *  Graphical effect: Fade to a specific colour in a specified time period
     *
     *  @param red   The amount of red   (0.0f <= red <= 1.0f)
     *  @param green The amount of green (0.0f <= green <= 1.0f)
     *  @param blue  The amount of blue  (0.0f <= blue <= 1.0f)
     *  @param delay The amount of time taken for the fade (in milliseconds)
     */
    void FadeToColor(float red, float green, float blue, int delay);

    /**
     * Clear the color buffers and reset the model matrix
     * \todo Come up with a better name for this (though that suggestion indicates that this method may not be good design).
     */
    void clearBuffer();
};

#endif

