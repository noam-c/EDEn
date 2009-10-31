#ifndef __FRAME_H_
#define __FRAME_H_

struct Frame
{
   const int left, top, right, bottom;

   Frame(int left, int top, int right, int bottom);
};

#endif
