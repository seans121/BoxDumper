#ifndef BoxDumper_H
#define BoxDumper_H

#include <3ds.h>
#include "pkx.h"

struct 		s_3fs
{
  Handle  	handle;
  FS_archive 	arch;
};

struct s_BoxDumper
{
  PrintConsole 	top;
  PrintConsole 	bot;
  struct s_3fs  sav;
  struct s_3fs  sd;
  s8 		game;
  u8 		*save;
};


struct s_stateInfo;

struct s_UIState
{
  void (*initf)(struct s_stateInfo *);
  void (*dispf)(struct s_stateInfo *);
  void (*inputf)(struct s_stateInfo *);
};

//General state structure
struct s_stateInfo
{
  u32 			kPressed; //stores what keys are pressed
  struct s_BoxDumper	*pch;

  struct s_pkm 		pkm; //the currently edited pokemon
  struct s_pkm 		cpy; //the pokemon in the clipboard
  s16 			pkmSlot; //the current box slot which is edited

  struct s_UIState 	curState; //current state
  s8 			inState; //ID of what is selected
  u8			inSel; //0 if we're editing a value, 1 if we're not
  s8 			cont; //if 0, terminate the program, if 1, terminate the program and export the savefile
};

typedef struct s_stateInfo t_stinf;

extern struct s_UIState uploadBoxesState;

s32 	loadFile(char *path, void *dst, FS_archive *fsarch, Handle *fshdl, u64 maxSize, u32 *bytesRead);
s32 	filesysInit(Handle *, Handle *, FS_archive *, FS_archive *);
s32 	filesysExit(Handle *, Handle *, FS_archive *, FS_archive *);

s32 	loadSave(u8 *save, Handle *fshdl, FS_archive *fsarch);

s32	loadPokemon(t_stinf *state, u16 slot, u8 *dest);
u32 	getCHKOffset(u8 game, u8 type, u8 index);
s32 	switchState(t_stinf *state, struct s_UIState newst);
s32 	startLoop(struct s_BoxDumper *);

s16 	overlayGetpkm();
s16 	overlayGetMove();
s16 	overlayGetAbility();
s16 	overlayGetBalls();
s16 	overlayGetNature();

#endif /* end of include guard: BoxDumper_H */
