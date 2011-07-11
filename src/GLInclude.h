#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#endif

// Putting gl.h and glu.h in OpenGL/ isn't normal, but on Apple it is.
// Mac. Not even once.