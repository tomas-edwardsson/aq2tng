//-----------------------------------------------------------------------------
// PG BUND
// a_xcmds.h
//
// header file for a_xcmd.c
//
// $Id: a_xcmds.h,v 1.1 2001/05/06 17:25:16 igor_rock Exp $
//
//-----------------------------------------------------------------------------
// $Log: a_xcmds.h,v $
// Revision 1.1  2001/05/06 17:25:16  igor_rock
// Initial revision
//
//-----------------------------------------------------------------------------

//needed for Cmd_voice_f
#define PG_SNDPATH "user/"

//needed for Cmd_Addpoint_f
#define PG_LOCEXT ".pg"
#define PG_LOCEXTEX ".adf"

// *** forward declarations

//
void Cmd_Menu_f (edict_t * self);
//
void Cmd_Punch_f (edict_t * self);
//Adds a point with name to location file - cheats must be enabled!
void Cmd_Addpoint_f (edict_t * self);
//Plays a sound file
void Cmd_Voice_f (edict_t * self);
//Shows new rules
void Cmd_Rules_f (edict_t * self);
void _Cmd_Rules_f (edict_t * self, char *argument);

//TempFile - BEGIN
// new commands for area cubes

void Cmd_BeginCube_f (edict_t *);
void Cmd_SetCubeLL_f (edict_t *);
void Cmd_SetCubeUR_f (edict_t *);
void Cmd_PrintCubeState_f (edict_t *);
void Cmd_AddCube_f (edict_t *);
void Cmd_AbortCube_f (edict_t *);

void Cmd_Lens_f (edict_t *);

//TempFile - END
