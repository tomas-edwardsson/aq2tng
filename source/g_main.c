//-----------------------------------------------------------------------------
//
//
// $Id: g_main.c,v 1.62 2002/03/25 23:35:19 freud Exp $
//
//-----------------------------------------------------------------------------
// $Log: g_main.c,v $
// Revision 1.62  2002/03/25 23:35:19  freud
// Ghost code, use_ghosts and more stuff..
//
// Revision 1.61  2002/03/25 18:32:11  freud
// I'm being too productive.. New ghost command needs testing.
//
// Revision 1.60  2002/03/25 15:16:24  freud
// use_newspawns had a typo in g_main.c fixed.
//
// Revision 1.58  2002/02/19 09:32:47  freud
// Removed PING PONGs from CVS, not fit for release.
//
// Revision 1.57  2002/02/18 23:17:55  freud
// Polling tweaks for PINGs
//
// Revision 1.56  2002/02/18 20:33:48  freud
// PING PONG Change polling times
//
// Revision 1.55  2002/02/18 20:21:36  freud
// Added PING PONG mechanism for timely disconnection of clients. This is
// based on a similar scheme as the scheme used by IRC. The client has
// cvar ping_timeout seconds to reply or will be disconnected.
//
// Revision 1.54  2002/02/18 19:44:45  freud
// Fixed the teamskin/teamname bug after softmap/map_restart
//
// Revision 1.53  2002/02/17 20:10:09  freud
// Better naming of auto_items is auto_equip, requested by Deathwatch.
//
// Revision 1.52  2002/02/17 20:01:32  freud
// Fixed stat_mode overflows, finally.
// Added 2 new cvars:
// 	auto_join (0|1), enables auto joining teams from previous map.
// 	auto_items (0|1), enables weapon and items caching between maps.
//
// Revision 1.51  2002/02/17 19:04:14  freud
// Possible bugfix for overflowing clients with stat_mode set.
//
// Revision 1.50  2002/02/03 01:07:28  freud
// more fixes with stats
//
// Revision 1.49  2002/02/01 15:02:43  freud
// More stat_mode fixes, stat_mode 1 would overflow clients at EndDMLevel.
//
// Revision 1.48  2002/02/01 12:54:08  ra
// messin with stat_mode
//
// Revision 1.47  2002/01/24 11:29:34  ra
// Cleanup's in stats code
//
// Revision 1.46  2002/01/24 02:24:56  deathwatch
// Major update to Stats code (thanks to Freud)
// new cvars:
// stats_afterround - will display the stats after a round ends or map ends
// stats_endmap - if on (1) will display the stats scoreboard when the map ends
//
// Revision 1.45  2002/01/23 01:29:07  deathwatch
// rrot should be a lot more random now (hope it works under linux as well)
//
// Revision 1.44  2002/01/22 16:55:49  deathwatch
// fixed a bug with rrot which would make it override sv softmap (moved the dosoft check up in g_main.c
// fixed a bug with rrot which would let it go to the same map (added an if near the end of the rrot statement in the EndDMLevel function)
//
// Revision 1.43  2001/12/24 18:06:05  slicerdw
// changed dynamic check for darkmatch only
//
// Revision 1.41  2001/12/09 14:02:11  slicerdw
// Added gl_clear check -> video_check_glclear cvar
//
// Revision 1.40  2001/11/29 17:58:31  igor_rock
// TNG IRC Bot - First Version
//
// Revision 1.39  2001/11/25 19:09:25  slicerdw
// Fixed Matchtime
//
// Revision 1.38  2001/11/10 14:00:14  deathwatch
// Fixed resetting of teamXscores
//
// Revision 1.37  2001/11/09 23:58:12  deathwatch
// Fiixed the %me command to display '[DEAD]' properly
// Added the resetting of the teamXscore cvars at the exitlevel function where the team scores are reset as well. (not sure if that is correct)
//
// Revision 1.36  2001/11/08 10:05:09  igor_rock
// day/night changing smoothened
// changed default for day_cycle to 10 (because of more steps)
//
// Revision 1.35  2001/11/04 15:15:19  ra
// New server commands: "sv softmap" and "sv map_restart".  sv softmap
// takes one map as argument and starts is softly without restarting
// the server.   map_restart softly restarts the current map.
//
// Revision 1.34  2001/11/02 16:07:47  ra
// Changed teamplay spawn code so that teams dont spawn in the same place
// often in a row
//
// Revision 1.33  2001/09/30 03:09:34  ra
// Removed new stats at end of rounds and created a new command to
// do the same functionality.   Command is called "time"
//
// Revision 1.32  2001/09/29 19:54:04  ra
// Made a CVAR to turn off extratimingstats
//
// Revision 1.31  2001/09/28 13:48:34  ra
// I ran indent over the sources. All .c and .h files reindented.
//
// Revision 1.30  2001/09/28 13:44:23  slicerdw
// Several Changes / improvements
//
// Revision 1.29  2001/09/02 20:33:34  deathwatch
// Added use_classic and fixed an issue with ff_afterround, also updated version
// nr and cleaned up some commands.
//
// Updated the VC Project to output the release build correctly.
//
// Revision 1.28  2001/08/18 15:22:28  deathwatch
// fixed the if in CycleLights to prevent the lights from cycling in other modes
//
// Revision 1.27  2001/08/18 01:28:06  deathwatch
// Fixed some stats stuff, added darkmatch + day_cycle, cleaned up several files, restructured ClientCommand
//
// Revision 1.26  2001/08/15 14:50:48  slicerdw
// Added Flood protections to Radio & Voice, Fixed the sniper bug AGAIN
//
// Revision 1.25  2001/08/08 12:42:22  slicerdw
// Ctf Should finnaly be fixed now, lets hope so
//
// Revision 1.24  2001/08/06 14:38:45  ra
// Adding UVtime for ctf
//
// Revision 1.23  2001/08/06 03:00:49  ra
// Added FF after rounds. Please someone look at the EVIL if statments for me :)
//
// Revision 1.22  2001/07/25 23:02:02  slicerdw
// Fixed the source, added the weapons and items capping to choose command
//
// Revision 1.21  2001/07/20 11:56:04  slicerdw
// Added a check for the players spawning during countdown on ctf ( lets hope it works )
//
// Revision 1.19  2001/06/26 21:19:31  ra
// Adding timestamps to gameendings.
//
// Revision 1.18  2001/06/25 11:44:47  slicerdw
// New Video Check System - video_check and video_check_lockpvs no longer latched
//
// Revision 1.17  2001/06/23 14:09:17  slicerdw
// Small fix on the Time Reporting on matchmode
//
// Revision 1.16  2001/06/22 16:34:05  slicerdw
// Finished Matchmode Basics, now with admins, Say command tweaked...
//
// Revision 1.15  2001/06/21 00:05:30  slicerdw
// New Video Check System done -  might need some revision but works..
//
// Revision 1.12  2001/06/20 07:21:21  igor_rock
// added use_warnings to enable/disable time/frags left msgs
// added use_rewards to enable/disable eimpressive, excellent and accuracy msgs
// change the configfile prefix for modes to "mode_" instead "../mode-" because
// they don't have to be in the q2 dir for doewnload protection (action dir is sufficient)
// and the "-" is bad in filenames because of linux command line parameters start with "-"
//
// Revision 1.11  2001/06/18 12:36:40  igor_rock
// added new irvision mode (with reddish screen and alpha blend) and corresponding
// new cvar "new_irvision" to enable the new mode
//
// Revision 1.10  2001/06/13 08:39:13  igor_rock
// changed "cvote" to "use_cvote" (like the other votecvars)
//
// Revision 1.9  2001/06/01 19:18:42  slicerdw
// Added Matchmode Code
//
// Revision 1.8  2001/05/31 16:58:14  igor_rock
// conflicts resolved
//
// Revision 1.7.2.3  2001/05/25 18:59:52  igor_rock
// Added CTF Mode completly :)
// Support for .flg files is still missing, but with "real" CTF maps like
// tq2gtd1 the ctf works fine.
// (I hope that all other modes still work, just tested DM and teamplay)
//
// Revision 1.7.2.2  2001/05/20 18:54:19  igor_rock
// added original ctf code snippets from zoid. lib compilesand runs but
// doesn't function the right way.
// Jsut committing these to have a base to return to if something wents
// awfully wrong.
//
// Revision 1.7.2.1  2001/05/20 15:17:31  igor_rock
// removed the old ctf code completly
//
// Revision 1.7  2001/05/14 21:10:16  igor_rock
// added wp_flags support (and itm_flags skeleton - doesn't disturb in the moment)
//
// Revision 1.6  2001/05/13 01:23:01  deathwatch
// Added Single Barreled Handcannon mode, made the menus and scoreboards
// look nicer and made the voice command a bit less loud.
//
// Revision 1.5  2001/05/12 21:19:51  ra
//
//
// Added punishkills.
//
// Revision 1.4  2001/05/12 20:58:22  ra
//
//
// Adding public mapvoting and kickvoting. Its controlable via cvar's mv_public
// and vk_public (both default off)
//
// Revision 1.3  2001/05/07 21:18:35  slicerdw
// Added Video Checking System
//
// Revision 1.2  2001/05/07 08:32:17  mort
// Basic CTF code
// No spawns etc
// Just the cvars and flag entity
//
// Revision 1.1.1.1  2001/05/06 17:31:37  igor_rock
// This is the PG Bund Edition V1.25 with all stuff laying around here...
//
//-----------------------------------------------------------------------------

#include <time.h>
#include "g_local.h"

extern int num_used_spawns;
extern int teamplay_usedspawns[];

game_locals_t game;
level_locals_t level;
game_import_t gi;
game_export_t globals;
spawn_temp_t st;

int sm_meat_index;
int snd_fry;
int meansOfDeath;
int locOfDeath;
int stopAP;

edict_t *g_edicts;

//FIREBLADE
cvar_t *hostname;
cvar_t *teamplay;
cvar_t *radiolog;
cvar_t *motd_time;
cvar_t *actionmaps;
cvar_t *roundtimelimit;
cvar_t *maxteamkills;
cvar_t *twbanrounds;
cvar_t *tkbanrounds;
cvar_t *limchasecam;
cvar_t *roundlimit;
cvar_t *skipmotd;
cvar_t *nohud;
cvar_t *noscore;
cvar_t *actionversion;
cvar_t *needpass;
cvar_t *use_voice;
cvar_t *ppl_idletime;
cvar_t *use_tourney;
cvar_t *use_3teams;
cvar_t *use_kickvote;
cvar_t *mv_public;		// AQ:TNG - JBravo adding public voting
cvar_t *vk_public;		// AQ:TNG - JBravo adding public voting
cvar_t *punishkills;		// AQ:TNG - JBravo adding punishkills
cvar_t *mapvote_waittime;
cvar_t *ff_afterround;
cvar_t *uvtime;			// CTF Invunerability Time
cvar_t *sv_gib;
cvar_t *sv_crlf;		// Allow Control Char
cvar_t *vrot;			// Vote Rotation
cvar_t *rrot;			// Random Rotation
cvar_t *strtwpn;		// Start DM Weapon
cvar_t *llsound;
cvar_t *use_cvote;
cvar_t *new_irvision;
cvar_t *use_rewards;
cvar_t *use_warnings;
cvar_t *use_mapvote;
cvar_t *deathmatch;
cvar_t *coop;
cvar_t *dmflags;
cvar_t *skill;
cvar_t *fraglimit;
cvar_t *timelimit;
cvar_t *capturelimit;
cvar_t *password;
cvar_t *maxclients;
cvar_t *maxentities;
cvar_t *g_select_empty;
cvar_t *dedicated;
cvar_t *filterban;
cvar_t *sv_maxvelocity;
cvar_t *sv_gravity;
cvar_t *sv_rollspeed;
cvar_t *sv_rollangle;
cvar_t *gun_x;
cvar_t *gun_y;
cvar_t *gun_z;
cvar_t *run_pitch;
cvar_t *run_roll;
cvar_t *bob_up;
cvar_t *bob_pitch;
cvar_t *bob_roll;
cvar_t *sv_cheats;
cvar_t *flood_msgs;
cvar_t *flood_persecond;
cvar_t *flood_waitdelay;
cvar_t *unique_weapons;
cvar_t *unique_items;
cvar_t *ir;
cvar_t *knifelimit;
cvar_t *tgren;
cvar_t *allweapon;
cvar_t *allitem;
cvar_t *sv_shelloff;
cvar_t *bholelimit;
cvar_t *splatlimit;
cvar_t *check_time;		// Time to wait before checks start ?
cvar_t *video_check;
cvar_t *video_checktime;
cvar_t *video_max_3dfx;
cvar_t *video_max_3dfxam;
cvar_t *video_max_opengl;
cvar_t *video_force_restart;
cvar_t *video_check_lockpvs;
cvar_t *video_check_glclear;
cvar_t *hc_single;
cvar_t *wp_flags;		// Weapon Banning
cvar_t *itm_flags;		// Item Banning
cvar_t *matchmode;
cvar_t *darkmatch;		// Darkmatch
cvar_t *day_cycle;		// If darkmatch is on, this value is the nr of seconds between each interval (day, dusk, night, dawn)
cvar_t *hearall;		// used for matchmode

cvar_t *mm_forceteamtalk;
cvar_t *mm_adminpwd;

cvar_t *team1score;
cvar_t *team2score;
cvar_t *team3score;
cvar_t *stats_endmap; // If on (1) show the fpm/etc stats when the map ends
cvar_t *stats_afterround;     // Collect TNG stats between rounds

cvar_t *auto_join;
cvar_t *auto_equip;

//TNG:Freud - new spawning system
cvar_t *use_newspawns;
//TNG:Freud - ghosts
cvar_t *use_ghosts;

cvar_t *use_punch;

cvar_t *radio_max;		// max nr Radio and Voice requests
cvar_t *radio_time;		// max nr of time for the radio_max
cvar_t *radio_ban;		// silence for xx nr of secs
cvar_t *radio_repeat;		// same as radio_max, only for repeats
cvar_t *use_classic;		// Used to reset spread/gren strength to 1.52

void SpawnEntities (char *mapname, char *entities, char *spawnpoint);
void ClientThink (edict_t * ent, usercmd_t * cmd);
qboolean ClientConnect (edict_t * ent, char *userinfo);
void ClientUserinfoChanged (edict_t * ent, char *userinfo);
void ClientDisconnect (edict_t * ent);
void ClientBegin (edict_t * ent);
void ClientCommand (edict_t * ent);
void CheckNeedPass (void);
void RunEntity (edict_t * ent);
void WriteGame (char *filename, qboolean autosave);
void ReadGame (char *filename);
void WriteLevel (char *filename);
void ReadLevel (char *filename);
void InitGame (void);
void G_RunFrame (void);

int dosoft;


gghost_t ghost_players[MAX_CLIENTS];
int num_ghost_players;


//===================================================================


void
ShutdownGame (void)
{
  gi.dprintf ("==== ShutdownGame ====\n");
  //PG BUND
  vExitGame ();
  gi.FreeTags (TAG_LEVEL);
  gi.FreeTags (TAG_GAME);
}


/*
  =================
  GetGameAPI
  
  Returns a pointer to the structure with all entry points
  and global variables
  =================
*/
game_export_t *
GetGameAPI (game_import_t * import)
{
  gi = *import;

  globals.apiversion = GAME_API_VERSION;
  globals.Init = InitGame;
  globals.Shutdown = ShutdownGame;
  globals.SpawnEntities = SpawnEntities;

  globals.WriteGame = WriteGame;
  globals.ReadGame = ReadGame;
  globals.WriteLevel = WriteLevel;
  globals.ReadLevel = ReadLevel;

  globals.ClientThink = ClientThink;
  globals.ClientConnect = ClientConnect;
  globals.ClientUserinfoChanged = ClientUserinfoChanged;
  globals.ClientDisconnect = ClientDisconnect;
  globals.ClientBegin = ClientBegin;
  globals.ClientCommand = ClientCommand;

  globals.RunFrame = G_RunFrame;

  globals.ServerCommand = ServerCommand;

  globals.edict_size = sizeof (edict_t);

  return &globals;
}

#ifndef GAME_HARD_LINKED
// this is only here so the functions in q_shared.c and q_shwin.c can link
void
Sys_Error (char *error, ...)
{
  va_list argptr;
  char text[1024];

  va_start (argptr, error);
  vsprintf (text, error, argptr);
  va_end (argptr);

  gi.error (ERR_FATAL, "%s", text);
}

void
Com_Printf (char *msg, ...)
{
  va_list argptr;
  char text[1024];

  va_start (argptr, msg);
  vsprintf (text, msg, argptr);
  va_end (argptr);

  gi.dprintf ("%s", text);
}

#endif

//======================================================================


/*
  =================
  ClientEndServerFrames
  =================
*/
void
ClientEndServerFrames (void)
{
  int i;
  edict_t *ent;

  // calc the player views now that all pushing
  // and damage has been added
  for (i = 0; i < maxclients->value; i++)
    {
      ent = g_edicts + 1 + i;
      if (!ent->inuse || !ent->client)
	continue;
      ClientEndServerFrame (ent);
    }

}

/*
  =================
  EndDMLevel
  
  The timelimit or fraglimit has been exceeded
  -----------------
  =================
*/

//AZEROV
extern void UnBan_TeamKillers (void);
//AZEROV

void EndDMLevel (void)
{
  edict_t *ent = NULL, *cl_ent; // TNG Stats was: edict_t *ent = NULL;
  char *nextmapname = NULL;
  qboolean byvote = false;
  //Igor[Rock] BEGIN
  votelist_t *maptosort = NULL;
  votelist_t *tmp = NULL;
  int newmappos, i; // TNG Stats, was: int newmappos;
  //Igor[Rock] END
  //JBravo[QNI] Begin
  char ltm[64];
  struct tm *now;
  time_t tnow;

  // TNG Stats:
  // Reuse ltm for stats.
  ltm[0] = '\0';
  // TNG Stats End

  tnow = time ((time_t *) 0);
  now = localtime (&tnow);
  (void) strftime (ltm, 64, "%A %d %B %H:%M:%S", now);
  gi.bprintf (PRINT_HIGH, "Game ending at: %s\n", ltm);

  if(teamplay->value) {
	num_used_spawns = 0;
	teamplay_usedspawns[0] = 0;
  }
  //JBravo[QNI] END

  // stay on same level flag
  if ((int) dmflags->value & DF_SAME_LEVEL)
	{
		ent = G_Spawn ();
    ent->classname = "target_changelevel";
    nextmapname = ent->map = level.mapname;
	}

  //FIREBLADE
  //  else if (!actionmaps->value || num_maps < 1)
  //FIREBLADE
  //Igor[Rock] Begin
  else if (!actionmaps->value || (num_maps < 1 && (map_num_maps < 1 || !vrot->value || !rrot->value)))
	//Igor[Rock] End
  {
		if (level.nextmap[0])
		{			// go to a specific map
			ent = G_Spawn ();
			ent->classname = "target_changelevel";
			nextmapname = ent->map = level.nextmap;
		}
    else
		{			// search for a changelevel
			ent = G_Find (NULL, FOFS (classname), "target_changelevel");
			if (!ent)
	    {			// the map designer didn't include a changelevel,
				// so create a fake ent that goes back to the same level
	      ent = G_Spawn ();
	      ent->classname = "target_changelevel";
	      nextmapname = ent->map = level.mapname;
	    }
		}
	}
  //FIREBLADE
  else
  {
    //Igor[Rock] BEGIN
    if (dosoft==1)
    {
	team_round_going = 0;
	team_game_going = 0;
			dosoft=0;
      ent = G_Spawn ();
      nextmapname = ent->map = level.nextmap;
    }
    else if (vrot->value)
		{
			ent = G_Spawn ();
			ent->classname = "target_changelevel";
			Com_sprintf (level.nextmap, sizeof (level.nextmap), "%s", map_votes->mapname);
			nextmapname = ent->map = level.nextmap;
			maptosort = map_votes;
			map_votes = maptosort->next;
			for (tmp = map_votes; tmp->next != NULL; tmp = tmp->next)
	    {}
			tmp->next = maptosort;
			maptosort->next = NULL;
		}
    else if (rrot->value)
		{
			// TNG: Making sure the rotation works fine
			// If there is just one map in the rotation:
			if(num_maps == 1)
			{
				cur_map = 0;
				ent = G_Spawn ();
				ent->classname = "target_changelevel"; //Yoohoo
				Com_sprintf (level.nextmap, sizeof (level.nextmap), "%s", map_rotation[cur_map]);
				nextmapname = ent->map = level.nextmap;
			} 
			// if there are 2 or more
			else 
			{
				nextmapname = level.mapname;
				while(!Q_strcasecmp(level.mapname, nextmapname)) {
					srand(rand()); // Reinitializing the random generator
					cur_map = rand () % num_maps;
					if (cur_map >= num_maps)
						cur_map = 0;
					ent = G_Spawn ();
					ent->classname = "target_changelevel"; //Yoohoo
					Com_sprintf (level.nextmap, sizeof (level.nextmap), "%s", map_rotation[cur_map]);
					nextmapname = ent->map = level.nextmap;
				}
			}
		}
    else
		{
			//Igor[Rock] End
			cur_map++;
			if (cur_map >= num_maps)
				cur_map = 0;
			ent = G_Spawn ();
			ent->classname = "target_changelevel";
			Com_sprintf (level.nextmap, sizeof (level.nextmap), "%s", map_rotation[cur_map]);
			nextmapname = ent->map = level.nextmap;
			//Igor[Rock] BEGIN
		}
    //Igor[Rock] End
	}

  //PG BUND - BEGIN
  level.tempmap[0] = '\0';
  vExitLevel (level.tempmap);
  if (level.tempmap[0])
  {
		// change to new map...
    byvote = true;
    nextmapname = ent->map = level.tempmap;	// TempFile added ent->map to fit 1.52 EndDMLevel() conventions
    if (level.nextmap != NULL)
			level.nextmap[0] = '\0';
	}
  //PG BUND - END

  //Igor[Rock] Begin (we have to change the position in the maplist here, because now the votes are up-to-date
  if ((maptosort != NULL) && (num_allvotes > map_num_maps))
  {				// I inserted the map_num_maps here to block an one user vote rotation...
		newmappos =	(int) (((100.0 - (((float) maptosort->num_allvotes * 100.0) / (float) num_allvotes)) * ((float) map_num_maps - 1.0)) / 100.0);
    if (!(newmappos == (map_num_maps - 1)))
		{
			// Delete the map from the end of the list
			for (tmp = map_votes; tmp->next != maptosort; tmp = tmp->next)
	    {
	    }
			tmp->next = NULL;
			//insert it at the right position
			if (newmappos == 0)
	    {
	      maptosort->next = map_votes;
	      map_votes = maptosort;
	    }
			else
	    {
	      newmappos--;
	      for (tmp = map_votes; newmappos > 0; tmp = tmp->next)
				{
					newmappos--;
				}
	      maptosort->next = tmp->next;
	      tmp->next = maptosort;
	    }
		}
	}
  //Igor[Rock] End
  if (level.nextmap != NULL && !byvote)
    gi.bprintf (PRINT_HIGH, "Next map in rotation is %s.\n", level.nextmap);
  //FIREBLADE

  ReadMOTDFile ();
  BeginIntermission (ent);

  //AZEROV
  UnBan_TeamKillers ();
  //AZEROV
}

/*
  =================
  CheckDMRules
  =================
*/
void
CheckDMRules (void)
{
  int i;
  gclient_t *cl;

  if (level.intermissiontime)
    return;

  if (!deathmatch->value)
    return;

  //FIREBLADE
  if (teamplay->value)
    {
      CheckTeamRules ();
    }
  else				/* not teamplay */
    {
      if (timelimit->value)
	{
	  if (level.time >= timelimit->value * 60)
	    {
	      gi.bprintf (PRINT_HIGH, "Timelimit hit.\n");
	      EndDMLevel ();
	      return;
	    }
	}
      //FIREBLADE
      //PG BUND - BEGIN
      if (vCheckVote () == true)
	{
	  EndDMLevel ();
	  return;
	}
      //PG BUND - END
    }

  if (ctf->value)
    {
      if (CTFCheckRules ())
	{
	  ResetPlayers ();
	  EndDMLevel ();
	}
    }

  if (fraglimit->value)
    {
      for (i = 0; i < maxclients->value; i++)
	{
	  cl = game.clients + i;
	  if (!g_edicts[i + 1].inuse)
	    continue;
	  if (cl->resp.score >= fraglimit->value)
	    {
	      gi.bprintf (PRINT_HIGH, "Fraglimit hit.\n");
	      if (ctf->value)
		ResetPlayers ();
	      EndDMLevel ();
	      return;
	    }
	}
    }
}


/*
  =============
  ExitLevel
  =============
*/
void
ExitLevel (void)
{
  int i;
  edict_t *ent;
  char command[256];

  Com_sprintf (command, sizeof (command), "gamemap \"%s\"\n",
	       level.changemap);
  gi.AddCommandString (command);
  level.changemap = NULL;
  level.exitintermission = 0;
  level.intermissiontime = 0;
  ClientEndServerFrames ();

  // clear some things before going to next level
  for (i = 0; i < maxclients->value; i++)
    {
      ent = g_edicts + 1 + i;
      if (!ent->inuse)
	continue;
      if (ent->health > ent->client->pers.max_health)
	ent->health = ent->client->pers.max_health;
    }

  //FIREBLADE
  if (teamplay->value)
    {
      team1_score = 0;
      team2_score = 0;
      team3_score = 0;
			// AQ2 TNG - Reset serverinfo score cvars too
			team1score->value = 0;
			strcpy(team1score->string,"0");
			team2score->value = 0;
			strcpy(team2score->string,"0");
			team3score->value = 0;
			strcpy(team3score->string,"0");
    }
  //FIREBLADE
  if (ctf->value)
    {
      CTFInit ();
    }
}

// TNG Darkmatch
int day_cycle_at = 0;		// variable that keeps track where we are in the cycle (0 = normal, 1 = darker, 2 = dark, 3 = pitch black, 4 = dark, 5 = darker)
float day_next_cycle = 10.0;

void
CycleLights ()
{
  static char *brightness = "mmmlkjihgfedcbaaabcdefghijkl";
  char temp[2];

  if ((!(darkmatch->value == 3)) || (day_cycle->value == 0))
    return;

  if (level.time == 10.0)
    day_next_cycle = level.time + day_cycle->value;

  if (day_next_cycle == level.time)
    {
      day_cycle_at++;
      if (day_cycle_at == strlen(brightness))
	{
	  day_cycle_at = 0;
	}
      sprintf (temp, "%c", brightness[day_cycle_at]);
      gi.configstring (CS_LIGHTS + 0, temp);
      day_next_cycle = level.time + day_cycle->value;
    }
}


/*
  ================
  G_RunFrame
  
  Advances the world by 0.1 seconds
  ================
*/
void
G_RunFrame (void)
{
  int i, j;
  edict_t *ent;
  char buffer[64];

  level.framenum++;
  level.time = level.framenum * FRAMETIME;

  // choose a client for monsters to target this frame
  AI_SetSightClient ();

  //AQ2:TNG - Slicer Matchmode code
  //if (matchmode->value && (team_game_going && team_round_going))
	if(matchmode->value && (team_game_going && ingame))
    matchtime = matchtime + 0.1;
  // exit intermissions

  if (level.exitintermission)
    {
      ExitLevel ();
      return;
    }

  // TNG Darkmatch Cycle
  CycleLights ();

  //
  // treat each object in turn
  // even the world gets a chance to think
  //
  ent = &g_edicts[0];
  for (i = 0; i < globals.num_edicts; i++, ent++)
    {
      if (!ent->inuse)
	continue;

      level.current_entity = ent;

      VectorCopy (ent->s.origin, ent->s.old_origin);

      // if the ground entity moved, make sure we are still on it
      if ((ent->groundentity)
	  && (ent->groundentity->linkcount != ent->groundentity_linkcount))
	{
	  ent->groundentity = NULL;
	  if (!(ent->flags & (FL_SWIM | FL_FLY))
	      && (ent->svflags & SVF_MONSTER))
	    {
	      M_CheckGround (ent);
	    }
	}

  if (i > 0 && i <= maxclients->value)
	{
		if (!(level.framenum % 80)) 
		    stuffcmd(ent, "cmd_stat_mode $stat_mode\n");

		// TNG Stats End

	  ClientBeginServerFrame (ent);
	  continue;
	}

	if (maxclients->value != 20) // TNG Stats, Added if
		G_RunEntity (ent);
  }

  // see if it is time to end a deathmatch
  CheckDMRules ();

  //FIREBLADE
  CheckNeedPass ();
  //FIREBLADE

  // build the playerstate_t structures for all players
  ClientEndServerFrames ();
  //AQ2:TNG Slicer
  // if (ctf->value)
  //{
//        i = Ent_Count();
//        if (i > 200)
//                CleanWeapons();

  //}

}


//ADDED FROM 3.20 SOURCE -FB
//Commented out spectator_password stuff since we don't have that now.
/*
  =================
  CheckNeedPass
  =================
*/
void
CheckNeedPass (void)
{
  int need;

  // if password or spectator_password has changed, update needpass
  // as needed
  if (password->modified /*|| spectator_password->modified */ )
    {
      password->modified = /*spectator_password->modified = */ false;

      need = 0;

      if (*password->string && Q_stricmp (password->string, "none"))
	need |= 1;
      /*
         if (*spectator_password->string && Q_stricmp(spectator_password->string, "none"))
         need |= 2;
       */

      gi.cvar_set ("needpass", va ("%d", need));
    }
}

//FROM 3.20 END
