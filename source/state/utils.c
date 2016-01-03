#include <stdio.h>

#include <3ds.h>
#include "state.h"

s8 	stdInputField(t_stinf *state, s8 up, s8 down, s8 left, s8 right)
{
  u32 	kPressed = state->kPressed;
  if (state->inSel)
    if (kPressed & KEY_B) {state->inSel = 0; return 1;}
  if (!state->inSel)
  {
    if (kPressed & KEY_A) {state->inSel = 1; return 1;}
    if (kPressed & KEY_UP && up) state->inState += up;
    if (kPressed & KEY_DOWN && down) state->inState += down;
    if (kPressed & KEY_LEFT && left) state->inState += left;
    if (kPressed & KEY_RIGHT && right) state->inState += right;
    return 1;
  }
  return 0;
}

s8 	dirInputField(t_stinf *state, s8 up, s8 down, s8 left, s8 right)
{
  u32 	kPressed = state->kPressed;
  if (state->inSel)
    if (kPressed & KEY_B) {state->inSel = 0; return 1;}
  if (!state->inSel)
  {
    if (kPressed & KEY_A) {state->inSel = 1; return 1;}
    if (kPressed & KEY_UP && up) state->inState = up;
    if (kPressed & KEY_DOWN && down) state->inState = down;
    if (kPressed & KEY_LEFT && left) state->inState = left;
    if (kPressed & KEY_RIGHT && right) state->inState = right;
    return 1;
  }
  return 0;
}