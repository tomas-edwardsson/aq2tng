//-----------------------------------------------------------------------------
// PG BUND
// a_xgame.h
//
// header file for a_xgame.c
//
// $Id: a_xgame.h,v 1.1 2001/05/06 17:25:24 igor_rock Exp $
//
//-----------------------------------------------------------------------------
// $Log: a_xgame.h,v $
// Revision 1.1  2001/05/06 17:25:24  igor_rock
// Initial revision
//
//-----------------------------------------------------------------------------


//relative position directions
#define RP_NORTH 1
#define RP_SOUTH 2
#define RP_EAST  4
#define RP_WEST  8

//TempFile punch delay
#define PUNCH_DELAY	5	// 5 frames, that's .5 seconds

//maximum size for location description
#define LOC_STR_LEN 128
//maximum amount of location points on a map
#define LOC_MAX_POINTS 300

//TempFile BEGIN
/*
   at first, i wanted to keep support for classic .pg files,
   but that would be inconvenient to code. So I sent all the .pg
   support to hell and completely rebuilt the location detection.
   I used different variable and type names so that it doesn't get confusing. -TF

   //structure of one point
   typedef struct
   {
   vec3_t pos;
   char desc[LOC_STR_LEN];
   } locpoint_t;

   //type declaration of point list
   typedef locpoint_t mapdesc_t[LOC_MAX_POINTS];

   //the whole map description
   extern mapdesc_t mapdesc;

 */

// TempFile cube support
typedef struct
  {
    vec3_t lowerleft;
    vec3_t upperright;
    char desc[LOC_STR_LEN];
  }
loccube_t;

typedef loccube_t mapdescex_t[LOC_MAX_POINTS];
extern mapdescex_t mapdescex;
extern loccube_t *setcube;	// this one is used for creating new cubes

extern int num_loccubes;

void FixCubeData (loccube_t *);

// TempFile END

// --- forward declarations

//initializes mapdesc, read a location file if available
void DescListInit (char *mapname);
void ParseSayText (edict_t * ent, char *text);
qboolean GetPositionText (vec3_t, char *);	//TempFile we need this public (HUD!)
