//-----------------------------------------------------------------------------
// a_xvote.c
//
// $Id: a_xvote.c,v 1.6 2006/06/18 12:52:18 igor_rock Exp $
//
//-----------------------------------------------------------------------------
// $Log: a_xvote.c,v $
// Revision 1.6  2006/06/18 12:52:18  igor_rock
// - removed extra banlist, used teamkiller ban list instead
//
// Revision 1.5  2006/06/18 09:32:23  igor_rock
// - removed menu entry "Leave team" in Deathmatch (credits to Maniac!)
//
// Revision 1.4  2003/12/09 22:06:11  igor_rock
// added "ignorepart" commadn to ignore all players with the specified part in
// their name (one shot function: if player changes his name/new palyers join,
// the list will _not_ changed!)
//
// Revision 1.3  2002/03/26 21:49:01  ra
// Bufferoverflow fixes
//
// Revision 1.2  2001/09/28 13:48:34  ra
// I ran indent over the sources. All .c and .h files reindented.
//
// Revision 1.1.1.1  2001/05/06 17:29:27  igor_rock
// This is the PG Bund Edition V1.25 with all stuff laying around here...
//
//-----------------------------------------------------------------------------

#include "g_local.h"

// Static constant used below
//#define xvlistsize (sizeof(xvotelist)/sizeof(vote_t))

cvar_t *_InitLeaveTeam (ini_t * ini)
{
  return teamplay;
}

vote_t xvotelist[] = {
  // mapvote
  {
   NULL,			// cvar
   _InitMapVotelist,		// InitGame 
   NULL,			// ExitGame 
   NULL,			// InitLevel
   _MapExitLevel,		// ExitLevel
   _MapInitClient,		// InitClient
   NULL,			// ClientConnect
   _RemoveVoteFromMap,		// ClientDisconnect
   _MapWithMostVotes,		// Newround
   _CheckMapVotes,		// CheckVote
   MAPMENUTITLE,		// Votetitle
   MapVoteMenu,			// VoteSelected

   {				// commands
    {"votemap", Cmd_Votemap_f}
    ,
    {"maplist", Cmd_Maplist_f}
    ,
    {NULL, NULL}
    ,
    {NULL, NULL}
    ,
    {NULL, NULL}
    }
   }
  ,

  // kickvote
  {
   NULL,			// cvar
   _InitKickVote,		// InitGame 
   NULL,			// ExitGame 
   NULL,			// InitLevel
   NULL,			// ExitLevel
   _InitKickClient,		// InitClient
   NULL,			// ClientConnect
   _ClientKickDisconnect,	// ClientDisconnect
   _CheckKickVote,		// Newround
   NULL,			// CheckVote
   KICKMENUTITLE,		// Votetitle
   _KickVoteSelected,		// VoteSelected

   {				// commands
    {"votekick", Cmd_Votekick_f}
    ,
    {"votekicknum", Cmd_Votekicknum_f}
    ,
    {"kicklist", Cmd_Kicklist_f}
    ,
    {NULL, NULL}
    ,
    {NULL, NULL}
    }
   }
  ,

  // ignore
  {
   NULL,			// cvar
   NULL,			// InitGame - no init, default cvar = deathmatch
   NULL,			// ExitGame 
   NULL,			// InitLevel
   NULL,			// ExitLevel
   _ClearIgnoreList,		// InitClient
   NULL,			// ClientConnect
   _ClrIgnoresOn,		// ClientDisconnect
   NULL,			// Newround
   NULL,			// CheckVote
   IGNOREMENUTITLE,		// Votetitle
   _IgnoreVoteSelected,		// VoteSelected

   {				// commands
    {"ignore", Cmd_Ignore_f}
    ,
    {"ignorenum", Cmd_Ignorenum_f}
    ,
    {"ignorelist", Cmd_Ignorelist_f}
    ,
    {"ignoreclear", Cmd_Ignoreclear_f}
    ,
    {"ignorepart", Cmd_IgnorePart_f}
    }
   }
  ,

  // configvote
  {
   NULL,			// cvar
   _InitConfiglist,		// InitGame 
   NULL,			// ExitGame 
   NULL,			// InitLevel
   _ConfigExitLevel,		// ExitLevel
   _ConfigInitClient,		// InitClient
   NULL,			// ClientConnect
   _RemoveVoteFromConfig,	// ClientDisconnect
   _ConfigWithMostVotes,	// Newround
   _CheckConfigVotes,		// CheckVote
   CONFIGMENUTITLE,		// Votetitle
   ConfigVoteMenu,		// VoteSelected

   {				// commands
    {"voteconfig", Cmd_Voteconfig_f}
    ,
    {"configlist", Cmd_Configlist_f}
    ,
    {NULL, NULL}
    ,
    {NULL, NULL}
    ,
    {NULL, NULL}
    }
   }
  ,

  // Leave Team
  {
   NULL,			// cvar
   _InitLeaveTeam,		// InitGame 
   NULL,			// ExitGame 
   NULL,			// InitLevel
   NULL,			// ExitLevel
   NULL,			// InitClient
   NULL,			// ClientConnect
   NULL,			// ClientDisconnect
   NULL,			// Newround
   NULL,			// CheckVote
   "Leave Team",		// Votetitle
   LeaveTeams,			// VoteSelected

   {				// commands
    {NULL, NULL}
    ,
    {NULL, NULL}
    ,
    {NULL, NULL}
    ,
    {NULL, NULL}
    ,
    {NULL, NULL}
    }
   }


};

static const int xvlistsize = (sizeof(xvotelist)/sizeof(vote_t));

 /**/ void
_AddVoteMenu (edict_t * ent, int fromix)
{
  int i, j;
  qboolean erg;

  i = 0;
  j = 0;
  erg = true;
  while (i < xvlistsize && erg == true)
    {
      if (xvotelist[i].VoteTitle && xvotelist[i].DependsOn->value
	  && xvotelist[i].VoteSelected)
	{
	  if (j >= fromix)
	    {
	      erg =
		xMenu_Add (ent, xvotelist[i].VoteTitle,
			   xvotelist[i].VoteSelected);
	    }
	  j++;
	}
      i++;
    }
}

void
vShowMenu (edict_t * ent, char *menu)
{
  int i;
  char fixedmenu[128];

  strncpy(fixedmenu, menu, sizeof(fixedmenu)-1);

  if (ent->client->menu)
    {
      PMenu_Close (ent);
      return;
    }
  if (!*fixedmenu)
    {
      // general menu
      if (xMenu_New (ent, "Menu", NULL, _AddVoteMenu) == false)
	{
	  gi.cprintf (ent, PRINT_MEDIUM, "Nothing to choose.\n");
	}
    }
  else
    {
      i = 0;
      while (i < xvlistsize)
	{
	  if (xvotelist[i].DependsOn->value && xvotelist[i].VoteSelected
	      && Q_stricmp (fixedmenu, xvotelist[i].VoteTitle) == 0)
	    {
	      xvotelist[i].VoteSelected (ent, NULL);
	      return;
	    }
	  i++;
	}
      gi.cprintf (ent, PRINT_MEDIUM, "No such menu: %s\n", fixedmenu);
    }
}

void
vInitGame (void)
{
  int i = 0;
  ini_t ini;

  if (OpenIniFile (IniPath (), &ini) == false)
    gi.dprintf ("Error opening ini file %s.\n", IniPath ());

  while (i < xvlistsize)
    {
      if (xvotelist[i].InitGame)
	xvotelist[i].DependsOn = xvotelist[i].InitGame (&ini);
      if (!xvotelist[i].DependsOn)
	xvotelist[i].DependsOn = deathmatch;
      i++;
    }
  CloseIniFile (&ini);
}

void
vExitGame (void)
{
  int i = 0;
  while (i < xvlistsize)
    {
      if (xvotelist[i].ExitGame)
	xvotelist[i].ExitGame ();
      i++;
    }
}

void
vInitLevel (void)
{
  int i = 0;
  while (i < xvlistsize)
    {
      if (xvotelist[i].InitLevel && xvotelist[i].DependsOn->value)
	xvotelist[i].InitLevel ();
      i++;
    }
}

void
vExitLevel (char *NextMap)
{
  int i = 0;
  while (i < xvlistsize)
    {
      // gi.dprintf("Checking %s\n", xvotelist[i].VoteTitle);    
      if (xvotelist[i].ExitLevel && xvotelist[i].DependsOn->value)
	{
	  // gi.dprintf("value ok.\n");
	  xvotelist[i].ExitLevel (NextMap);
	  /*
	     // first come, first serve..
	     if (NextMap[0])
	     return;
	   */
	}
      i++;
    }
}

void
vInitClient (edict_t * ent)
{
  int i = 0;
  while (i < xvlistsize)
    {
      if (xvotelist[i].InitClient && xvotelist[i].DependsOn->value)
	xvotelist[i].InitClient (ent);
      i++;
    }
}

qboolean vClientConnect (edict_t * ent, char *userinfo)
{
  int i = 0;
  while (i < xvlistsize)
    {
      if (xvotelist[i].ClientConnect && xvotelist[i].DependsOn->value)
	if (xvotelist[i].ClientConnect (ent, userinfo) == false)
	  return false;
      i++;
    }
  return true;
}

void
vClientDisconnect (edict_t * ent)
{
  int i = 0;
  while (i < xvlistsize)
    {
      if (xvotelist[i].ClientDisconnect && xvotelist[i].DependsOn->value)
	xvotelist[i].ClientDisconnect (ent);
      i++;
    }
}

void
vNewRound (void)
{
  int i = 0;
  while (i < xvlistsize)
    {
      if (xvotelist[i].NewRound && xvotelist[i].DependsOn->value)
	xvotelist[i].NewRound ();
      i++;
    }
}

qboolean vCheckVote (void)
{
  int i = 0;
  while (i < xvlistsize)
    {
      if (xvotelist[i].CheckVote && xvotelist[i].DependsOn->value)
	if (xvotelist[i].CheckVote () == true)
	  return true;
      i++;
    }
  return false;
}


qboolean _vCommand (edict_t * ent, char *cmd, char *arg)
{
  int i, j;

  i = 0;
  while (i < xvlistsize)
    {
      // gi.dprintf("Checking %s\n", xvotelist[i].VoteTitle);    
      if (xvotelist[i].DependsOn->value)
	{
	  // gi.dprintf("value ok.\n");     
	  j = 0;
	  while (xvotelist[i].commands[j].cmd && j < VOTE_MAX_COMMANDS)
	    {
	      if (Q_stricmp (cmd, xvotelist[i].commands[j].cmd) == 0)
		{
		  if (xvotelist[i].commands[j].cmdfunc)
		    {
		      xvotelist[i].commands[j].cmdfunc (ent, arg);
		      return true;
		    }
		}
	      j++;
	    }
	}
      i++;
    }

  return false;
}


qboolean vCommand (edict_t * ent, char *cmd)
{
  char *s;

  s = gi.args ();
  return _vCommand (ent, cmd, s);
}
