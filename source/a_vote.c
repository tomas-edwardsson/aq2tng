//-----------------------------------------------------------------------------
// a_vote.c
//
// $Id: a_vote.c,v 1.9 2001/09/28 13:48:34 ra Exp $
//
//-----------------------------------------------------------------------------
// $Log: a_vote.c,v $
// Revision 1.9  2001/09/28 13:48:34  ra
// I ran indent over the sources. All .c and .h files reindented.
//
// Revision 1.8  2001/07/25 23:02:02  slicerdw
// Fixed the source, added the weapons and items capping to choose command
//
// Revision 1.7  2001/07/16 18:28:46  ra
// Changed a 40 second hard limit on mapvoting into a cvar.
//
// Revision 1.6  2001/06/20 07:21:21  igor_rock
// added use_warnings to enable/disable time/frags left msgs
// added use_rewards to enable/disable eimpressive, excellent and accuracy msgs
// change the configfile prefix for modes to "mode_" instead "../mode-" because
// they don't have to be in the q2 dir for doewnload protection (action dir is sufficient)
// and the "-" is bad in filenames because of linux command line parameters start with "-"
//
// Revision 1.5  2001/06/18 11:01:42  igor_rock
// added "mode-" prefix to votet configfiles, so all mode configs are close together
// when someone makes a "dir" or "ls -al" command on the server (cosmetic change)
//
// Revision 1.4  2001/06/13 09:14:23  igor_rock
// change the path for configs from "config/" to "../" because of possibility
// of exploit with the "download" command if "allow_download" is set
//
// Revision 1.3  2001/06/13 08:39:13  igor_rock
// changed "cvote" to "use_cvote" (like the other votecvars)
//
// Revision 1.2  2001/05/12 20:58:22  ra
//
//
// Adding public mapvoting and kickvoting. Its controlable via cvar's mv_public
// and vk_public (both default off)
//
// Revision 1.1.1.1  2001/05/06 17:25:12  igor_rock
// This is the PG Bund Edition V1.25 with all stuff laying around here...
//
//-----------------------------------------------------------------------------

#include "g_local.h"

#ifndef MAX_STR_LEN
#define MAX_STR_LEN 1000
#endif // MAX_STR_LEN


//=== misc functions =======================================================
//
//==========================================================================

void
_printplayerlist (edict_t * self, char *buf,
		  qboolean (*markthis) (edict_t * self, edict_t * other))
{
  int count, i;
  edict_t *other;
  char dummy, tmpbuf[128];

  count = 0;
  strcat (buf, " #  Name\n");
  strcat (buf, "------------------------------------\n");
  for (i = 1; i <= game.maxclients; i++)
    {
      other = &g_edicts[i];
      if (other->client && other != self && other->inuse)
	{
	  if (markthis (self, other) == true)
	    dummy = '*';
	  else
	    dummy = ' ';
	  sprintf (tmpbuf, "%2i %c%s", i, dummy, other->client->pers.netname);
	  count++;
	  //just to be sure:
	  tmpbuf[36] = 0;
	  strcat (tmpbuf, "\n");
	  strcat (buf, tmpbuf);
	}
    }
  if (!count)
    strcat (buf, "None\n");
  strcat (buf, "\n");
}


int
_numclients (void)
{
  int count, i;
  edict_t *other;

  count = 0;
  for (i = 1; i <= maxclients->value; i++)
    {
      other = &g_edicts[i];
      if (other->inuse)
	count++;
    }
  return count;
}

//=== map voting ===========================================================
//
// Original programed by Black Cross[NL], adapted, major changes.
//
//==========================================================================

votelist_t *map_votes;
int map_num_maps;
int map_num_votes;
int map_num_clients;
qboolean map_need_to_check_votes;

cvar_t *mapvote_min;
cvar_t *mapvote_need;
cvar_t *mapvote_pass;

//Igor[Rock] BEGIN
// moved here from the func ReadMapListFile because I need it global
char maplistpath[MAX_STR_LEN];
//Igor[Rock] END

#define MAPVOTESECTION "mapvote"

// forward declarations
votelist_t *MapWithMostVotes (float *p);
int AddVoteToMap (char *mapname, edict_t * ent);
void ReadMaplistFile (void);
qboolean _iCheckMapVotes (void);

//
void
Cmd_Votemap_f (edict_t * ent, char *t)
{
  if (!*t)
    {
      gi.cprintf (ent, PRINT_HIGH,
		  "You need an argument to the vote command (name of map).\n");
      return;
    }

  // BEGIN Igor[Rock]
  if (level.time < (float) mapvote_waittime->value)
    {
      gi.cprintf (ent, PRINT_HIGH,
		  "Mapvote currently blocked - Please vote again in %d seconds\n",
		  (int) ((float) mapvote_waittime->value + 1.0 - level.time));
    }
  else
    {
      // END Igor[Rock]
      switch (AddVoteToMap (t, ent))
	{
	case 0:
	  gi.cprintf (ent, PRINT_HIGH, "You have voted on map \"%s\"\n", t);
	  if (mv_public->value)
	    gi.bprintf (PRINT_HIGH, "%s voted for \"%s\"\n",
			ent->client->pers.netname, t);
	  break;
	case 1:
	  gi.cprintf (ent, PRINT_HIGH,
		      "You have changed your vote to map \"%s\"\n", t);
	  if (mv_public->value)
	    gi.bprintf (PRINT_HIGH,
			"%s changed his mind and voted for \"%s\"\n",
			ent->client->pers.netname, t);
	  break;
	default:
	  //error
	  gi.cprintf (ent, PRINT_HIGH, "Map \"%s\" is not in the votelist!\n",
		      t);
	  break;
	}
      // BEGIN Igor[Rock]
    }
  //END Igor[Rock]

  return;
}

//
void
Cmd_Maplist_f (edict_t * ent, char *dummy)
{
  //go through the votelist and list out all the maps and % votes
  int lines, chars_on_line, len_mr;
  float p_test, p_most;
  votelist_t *search, *most;
  char msg_buf[16384], tmp_buf[400];	//only 40 are used

  p_test = p_most = 0.0;

  most = MapWithMostVotes (&p_most);

  sprintf (msg_buf,
	   "List of maps that can be voted on:\nRequire more than %d%%%% votes (%.2f)\n\n",
	   (int) mapvote_pass->value,
	   (float) ((float) mapvote_pass->value / 100.0));

  lines = chars_on_line = 0;
  for (search = map_votes; search != NULL; search = search->next)
    {

      if (map_num_clients > 0)
	p_test =
	  (float) ((float) search->num_votes / (float) map_num_clients);

      if (p_test >= 10.0)
	len_mr = 11;
      else
	len_mr = 10;

      len_mr += strlen (search->mapname);
//Igor[Rock] begin
      //    if (num_allvotes && vrot->value)
      //       len_mr += 4;
      //Igor[Rock] end
      if ((chars_on_line + len_mr + 2) > 39)
	{
	  strcat (msg_buf, "\n");
	  lines++;
	  chars_on_line = 0;
	  if (lines > 25)
	    break;
	}
//Igor[Rock] begin
      //    if (num_allvotes && vrot->value) {
      //       sprintf(tmp_buf, "%s (%.2f,%2d%%%%)  ",
      //             search->mapname, p_test,
      //          (search->num_allvotes * 100) / num_allvotes);
      //    }
      //    else {
      sprintf (tmp_buf, "%s (%.2f)  ", search->mapname, p_test);
//    }
      //Igor[Rock] End
      strcat (msg_buf, tmp_buf);
      chars_on_line += len_mr;

    }

  if (map_votes == NULL)
    strcat (msg_buf, "None!");
  else if (most != NULL)
    {
      sprintf (tmp_buf, "\n\nMost votes: %s (%.2f)", most->mapname, p_most);
      strcat (msg_buf, tmp_buf);
    }

  strcat (msg_buf, "\n\n");
  sprintf (tmp_buf,
	   "%d/%d (%.2f%%%%) clients voted\n%d client%s minimum (%d%%%% required)",
	   map_num_votes, map_num_clients,
	   (float) ((float) map_num_votes /
		    (float) (map_num_clients > 0 ? map_num_clients : 1) * 100),	// TempFile changed to percentual display
	   (int) mapvote_min->value, (mapvote_min->value > 1 ? "s" : ""),
	   (int) mapvote_need->value);
  strcat (msg_buf, tmp_buf);

  gi.centerprintf (ent, msg_buf);

  return;
}

//
void
_MapInitClient (edict_t * ent)
{
  ent->client->resp.mapvote = NULL;
}

//
void
_RemoveVoteFromMap (edict_t * ent)
{
  votelist_t *search;

  map_need_to_check_votes = true;

  if (ent->client->resp.mapvote == NULL)
    return;

  for (search = map_votes; search != NULL; search = search->next)
    if (Q_stricmp (search->mapname, ent->client->resp.mapvote) == 0)
      {
	map_num_votes--;
	search->num_votes--;
	ent->client->resp.mapvote = NULL;
	break;
      }
  return;
}

//
void
_MapExitLevel (char *NextMap)
{
  votelist_t *votemap = NULL;
  //Igor[Rock] BEGIN
  FILE *votefile;
  char buf[MAX_STR_LEN];
  //Igor[Rock] END

  if (_iCheckMapVotes ())
    {
      votemap = MapWithMostVotes (NULL);
      strncpy (NextMap, votemap->mapname, MAX_QPATH);
      gi.bprintf (PRINT_HIGH, "Next map was voted on and is %s.\n", NextMap);
    }

  //clear stats
  for (votemap = map_votes; votemap != NULL; votemap = votemap->next)
    {
      //Igor[Rock] BEGIN
      if (votemap->num_votes)
	{
	  votemap->num_allvotes += votemap->num_votes;
	  num_allvotes += votemap->num_votes;
	}
      if (stricmp (level.mapname, votemap->mapname) == 0)
	{
	  if (map_num_clients > 1)
	    {
	      if (votemap->num_allvotes < (map_num_clients / 2))
		{
		  num_allvotes -= votemap->num_allvotes;
		  votemap->num_allvotes = 0;
		}
	      else
		{
		  num_allvotes -= (map_num_clients / 2);
		  votemap->num_allvotes -= (map_num_clients / 2);
		}
	    }
	  else
	    {
	      if (votemap->num_allvotes)
		{
		  num_allvotes--;
		  votemap->num_allvotes--;
		}
	    }
	}
      //Igor[Rock] END
      votemap->num_votes = 0;
    }

  //Igor[Rock] BEGIN
  // Save the actual votes to a file
  votefile = fopen (maplistpath, "w");
  if (votefile != NULL)
    {
      sprintf (buf, "%d\n", num_allvotes);
      fputs (buf, votefile);

      for (votemap = map_votes; votemap != NULL; votemap = votemap->next)
	{
	  sprintf (buf, "%s,%d\n", votemap->mapname, votemap->num_allvotes);
	  fputs (buf, votefile);
	}

      fclose (votefile);
    }
  //Igor[Rock] END
  map_num_votes = 0;
  map_num_clients = 0;
  map_need_to_check_votes = true;
}

//
qboolean _CheckMapVotes (void)
{
  if (_iCheckMapVotes () == true)
    {
      gi.bprintf (PRINT_HIGH, "More than %i%% map votes reached.\n",
		  (int) mapvote_pass->value);
      return true;
    }
  return false;
}

//
qboolean _MostVotesStr (char *buf)
{
  float p_most;
  votelist_t *most;

  p_most = 0.0;
  most = MapWithMostVotes (&p_most);
  if (most != NULL)
    {
      sprintf (buf, "%s (%.2f%%)", most->mapname, p_most * 100.0);
      return true;
    }
  else
    strcpy (buf, "(no map)");
  return false;
}

//
void
_MapWithMostVotes (void)
{
  char buf[1024], sbuf[512];

  if (_MostVotesStr (sbuf))
    {
      sprintf (buf, "Most wanted map: %s\n", sbuf);
      gi.bprintf (PRINT_HIGH, strtostr2 (buf));
    }

}

//
cvar_t *
_InitMapVotelist (ini_t * ini)
{
  char buf[1024];

  // note that this is done whether we have set "use_mapvote" or not!
  map_votes = NULL;
  map_num_maps = 0;
  map_num_votes = 0;
  map_num_clients = 0;
  map_need_to_check_votes = true;
  ReadMaplistFile ();

  use_mapvote = gi.cvar ("use_mapvote", "0", CVAR_SERVERINFO);
//Igor[Rock] Begin
  vrot = gi.cvar ("vrot", "0", CVAR_LATCH);
  rrot = gi.cvar ("rrot", "0", CVAR_LATCH);
//Igor[Rock] End
  mapvote_min = gi.cvar ("mapvote_min",
			 ReadIniStr (ini, MAPVOTESECTION, "mapvote_min", buf,
				     "1"), CVAR_LATCH);
  mapvote_need =
    gi.cvar ("mapvote_need",
	     ReadIniStr (ini, MAPVOTESECTION, "mapvote_need", buf, "0"),
	     CVAR_LATCH);
  mapvote_pass =
    gi.cvar ("mapvote_pass",
	     ReadIniStr (ini, MAPVOTESECTION, "mapvote_pass", buf, "50"),
	     CVAR_LATCH);
  return (use_mapvote);
}

//---------------

qboolean _iCheckMapVotes (void)
{
  static qboolean enough = false;
  float p;
  votelist_t *tmp;

  if (!map_need_to_check_votes)
    return (enough);

  tmp = MapWithMostVotes (&p);

  enough = (tmp != NULL
	    && p >= (float) ((float) mapvote_pass->value / 100.0));
  if (map_num_clients < mapvote_min->value)
    enough = false;
  if (mapvote_need->value)
    {
      if ((float) ((float) map_num_votes / (float) map_num_clients) <
	  (float) ((float) mapvote_need->value / (float) 100.0))
	enough = false;
    }

  map_need_to_check_votes = false;

  return (enough);
}


votelist_t *
MapWithMostVotes (float *p)
{
  int i;
  float p_most;
  votelist_t *search, *most;
  edict_t *e;

  p_most = 0.0;
  if (map_votes == NULL)
    return (NULL);

  //find map_num_clients
  map_num_clients = 0;
  for (i = 1; i <= maxclients->value; i++)
    {
      e = g_edicts + i;
      if (e->inuse)
	map_num_clients++;
    }

  if (map_num_clients == 0)
    return (NULL);

  most = NULL;
  for (search = map_votes; search != NULL; search = search->next)
    {
      if ((float) ((float) search->num_votes / (float) map_num_clients) >
	  p_most)
	{
	  p_most =
	    (float) ((float) search->num_votes / (float) map_num_clients);
	  most = search;
	}
    }

  if (p != NULL)
    *p = p_most;
  return (most);
}

int
AddVoteToMap (char *mapname, edict_t * ent)
{
  int changed = 0;
  votelist_t *search;

  map_need_to_check_votes = true;

  if (ent->client->resp.mapvote != NULL)
    {
      _RemoveVoteFromMap (ent);
      changed = 1;
    }

  for (search = map_votes; search != NULL; search = search->next)
    if (Q_stricmp (search->mapname, mapname) == 0)
      {
	map_num_votes++;
	search->num_votes++;
	ent->client->resp.mapvote = search->mapname;
	return changed;
      }

  // if we get here we didn't find the map!
  return -1;
}

void
MapSelected (edict_t * ent, pmenu_t * p)
{
  char *ch;

  ch = p->text;
  if (ch)
    {
      while (*ch != ' ' && *ch != '\0')
	ch++;
      *ch = '\0';
    }
  ch = p->text;
  if (ch && *ch == '*')
    ch++;
  PMenu_Close (ent);
  Cmd_Votemap_f (ent, ch);
}

void
AddMapToMenu (edict_t * ent, int fromix)
{
  int i;
  char buffer[512], spc[64];
  votelist_t *search;
  float prozent;

  i = 0;
  search = map_votes;
  while (search && i < fromix)
    {
      search = search->next;
      i++;
    }
  while (search)
    {
      prozent =
	(float) (((float) search->num_votes / (float) map_num_clients) * 100);
      i = 27 - strlen (search->mapname);
      if (prozent < 10.00)
	i -= 6;
      else if (prozent < 100.00)
	i -= 7;
      else
	i -= 8;
      if (i < 0)
	i = 0;
      spc[i--] = '\0';
      while (i >= 0)
	spc[i--] = ' ';
      //+ Marker: Hier einbauen, da� die gew�hlte Karte markiert ist
      //  problem: '*' am anfang wird nicht ber�cksichtigt. - erledigt -
      //alt: sprintf(buffer, "%s%s%.1f%%", search->mapname, spc, prozent);
      sprintf (buffer, "%s%s%s%.1f%%",
	       ent->client->resp.mapvote == search->mapname ? "*" : "",
	       search->mapname, spc, prozent);


      if (xMenu_Add (ent, buffer, MapSelected) == true)
	search = search->next;
      else
	search = NULL;
    }
}

void
MapVoteMenu (edict_t * ent, pmenu_t * p)
{
  char buf[1024], sbuf[512];

  PMenu_Close (ent);
  if (_MostVotesStr (sbuf));
  sprintf (buf, "most: %s", sbuf);
  if (xMenu_New (ent, MAPMENUTITLE, buf, AddMapToMenu) == false)
    gi.cprintf (ent, PRINT_MEDIUM, "No map to vote for.\n");
}

void
ReadMaplistFile (void)
{
  int i, bs;
  votelist_t *list = NULL, *tmp;
  FILE *maplist_file;
  char buf[MAX_STR_LEN];
//Igor[Rock] BEGIN
  // added variable maplist.ini Files with Variable "maplistname"
  // changed maplistpath to a global variable!
  cvar_t *maplistname;

  maplistname = gi.cvar ("maplistname", "maplist.ini", 0);
  if (maplistname->string && *(maplistname->string))
    sprintf (maplistpath, "%s/%s", GAMEVERSION, maplistname->string);
  else
    sprintf (maplistpath, "%s/%s", GAMEVERSION, "maplist.ini");

  maplist_file = fopen (maplistpath, "r");
//Igor[Rock] End
  if (maplist_file == NULL)
    {
      // no "maplist.ini" file so use the maps from "action.ini"
      if (num_maps <= 0)
	return;

      map_votes =
	(struct votelist_s *) gi.TagMalloc (sizeof (struct votelist_s),
					    TAG_GAME);
      map_votes->mapname = map_rotation[0];
      map_votes->num_votes = 0;
      //Igor[Rock] BEGIN
      map_votes->num_allvotes = 0;
      //Igor[Rock] END
      map_votes->next = NULL;

      list = map_votes;
      for (i = 1; i < num_maps; i++)
	{
	  tmp =
	    (struct votelist_s *) gi.TagMalloc (sizeof (struct votelist_s),
						TAG_GAME);
	  tmp->mapname = map_rotation[i];
	  tmp->num_votes = 0;
	  //Igor[Rock] BEGIN
	  tmp->num_allvotes = 0;
	  //Igor[Rock] END
	  tmp->next = NULL;
	  list->next = tmp;
	  list = tmp;
	}
    }
  else
    {
      // read the maplist.ini file
      for (i = 0; fgets (buf, MAX_STR_LEN - 10, maplist_file) != NULL;)
	{
	  //first remove trailing spaces
	  for (bs = strlen (buf);
	       bs > 0 &&
	       (buf[bs - 1] == '\r' || buf[bs - 1] == '\n'
		|| buf[bs - 1] == ' '); bs--)
	    buf[bs - 1] = '\0';
	  if (bs > 0 &&
	      strncmp (buf, "#", 1) != 0 && strncmp (buf, "//", 2) != 0)
	    {
	      if (i == 0)
		{
		  map_votes =
		    (struct votelist_s *) gi.
		    TagMalloc (sizeof (struct votelist_s), TAG_GAME);
		  map_votes->mapname = gi.TagMalloc (bs + 1, TAG_GAME);
		  strcpy (map_votes->mapname, buf);
		  map_votes->num_votes = 0;
		  //Igor[Rock] BEGIN
		  map_votes->num_allvotes = 0;
		  //Igor[Rock] END
		  map_votes->next = NULL;
		  list = map_votes;
		  i++;
		}
	      else
		{
		  tmp =
		    (struct votelist_s *) gi.
		    TagMalloc (sizeof (struct votelist_s), TAG_GAME);
		  tmp->mapname = gi.TagMalloc (bs + 1, TAG_GAME);
		  strcpy (tmp->mapname, buf);
		  tmp->num_votes = 0;
		  //Igor[Rock] BEGIN
		  tmp->num_allvotes = 0;
		  //Igor[Rock] END
		  tmp->next = NULL;
		  list->next = tmp;
		  list = tmp;
		  i++;
		}
	    }
	}
      fclose (maplist_file);
      map_num_maps = i;
    }

//Igor[Rock] BEGIN
  //load the saved values from the last run of the server
  sprintf (maplistpath, "%s-votes", maplistpath);

  maplist_file = fopen (maplistpath, "r");
  if (maplist_file != NULL)
    {
      for (i = 0; fgets (buf, MAX_STR_LEN - 10, maplist_file) != NULL;)
	{
	  //first remove trailing spaces
	  for (bs = strlen (buf);
	       bs > 0 && (buf[bs - 1] == '\r' || buf[bs - 1] == '\n'
			  || buf[bs - 1] == ' '); bs--)
	    buf[bs - 1] = '\0';
	  if (i == 0)
	    {
	      num_allvotes = atoi (buf);
	    }
	  else
	    {
	      for (tmp = map_votes; tmp->next != NULL; tmp = tmp->next)
		{
		  if (!strncmp (tmp->mapname, buf, strlen (tmp->mapname)))
		    {
		      tmp->num_allvotes =
			atoi (&buf[strlen (tmp->mapname) + 1]);
		    }
		}
	    }
	  i++;
	}
      fclose (maplist_file);
    }
//Igor[Rock] End
  return;
}

//=== kick voting ==========================================================
//
//==========================================================================

#define KICKVOTESECTION "kickvote"

cvar_t *kickvote_min;
cvar_t *kickvote_need;
cvar_t *kickvote_pass;
cvar_t *kickvote_tempban;

qboolean kickvotechanged = false;
edict_t *Mostkickvotes = NULL;
float Allkickvotes = 0.0;
float Mostkickpercent = 0.0;

void
_SetKickVote (edict_t * ent, edict_t * target)
{
  if (ent->client->resp.kickvote == target)
    {
      ent->client->resp.kickvote = NULL;
      gi.cprintf (ent, PRINT_MEDIUM, "Your kickvote on %s is removed\n",
		  target->client->pers.netname);
      if (vk_public->value)
	gi.bprintf (PRINT_HIGH, "%s doesnt want to kick %s after all\n",
		    ent->client->pers.netname, target->client->pers.netname);
    }
  else
    {
      if (ent->client->resp.kickvote)
	gi.cprintf (ent, PRINT_MEDIUM, "Kickvote was changed to %s\n",
		    target->client->pers.netname);
      else
	{
	  gi.cprintf (ent, PRINT_MEDIUM, "You voted on %s to be kicked\n",
		      target->client->pers.netname);
	  if (vk_public->value)
	    gi.bprintf (PRINT_HIGH, "%s voted to kick %s\n",
			ent->client->pers.netname,
			target->client->pers.netname);
	}
      ent->client->resp.kickvote = target;
      kickvotechanged = true;
    }

  kickvotechanged = true;
  _CheckKickVote ();
}

void
_ClrKickVotesOn (edict_t * target)
{
  edict_t *other;
  int i, j;

  j = 0;
  for (i = 1; i <= game.maxclients; i++)
    {
      other = &g_edicts[i];
      if (other->client && other->inuse)
	{
	  if (other->client->resp.kickvote == target)
	    {
	      other->client->resp.kickvote = NULL;
	      j++;
	    }
	}
    }

  if (j > 0 || target->client->resp.kickvote)
    {
      kickvotechanged = true;
      _CheckKickVote ();
    }
}

void
_DoKick (edict_t * target)
{
  char buf[128];

  sprintf (buf, "more than %i%% voted for.", (int) kickvote_pass->value);
  _ClrKickVotesOn (target);
  if (kickvote_tempban->value)
    DoBan (target, buf);
  else
    KickClient (target, buf);
}

cvar_t *
_InitKickVote (ini_t * ini)
{
  char buf[1024];

  kickvote_min = gi.cvar ("kickvote_min",
			  ReadIniStr (ini, KICKVOTESECTION, "kickvote_min",
				      buf, "4"), CVAR_LATCH);
  kickvote_need =
    gi.cvar ("kickvote_need",
	     ReadIniStr (ini, KICKVOTESECTION, "kickvote_need", buf, "0"),
	     CVAR_LATCH);
  kickvote_pass =
    gi.cvar ("kickvote_pass",
	     ReadIniStr (ini, KICKVOTESECTION, "kickvote_pass", buf, "75"),
	     CVAR_LATCH);
  kickvote_tempban =
    gi.cvar ("kickvote_tempban",
	     ReadIniStr (ini, KICKVOTESECTION, "kickvote_tempban", buf, "1"),
	     CVAR_LATCH);


  kickvotechanged = false;
  return (use_kickvote);
}

void
_InitKickClient (edict_t * ent)
{
  ent->client->resp.kickvote = NULL;
}

void
_ClientKickDisconnect (edict_t * ent)
{
  _ClrKickVotesOn (ent);
}

void
_CheckKickVote (void)
{
  int i, j, votes, maxvotes, playernum, playervoted;
  edict_t *other, *target, *mtarget;

  if (kickvotechanged == false)
    return;

  kickvotechanged = false;
  playernum = _numclients ();

  if (playernum < kickvote_need->value)
    return;

  maxvotes = 0;
  mtarget = NULL;
  playervoted = 0;
  for (i = 1; i <= game.maxclients; i++)
    {
      other = &g_edicts[i];
      if (other->client && other->inuse && other->client->resp.kickvote)
	{
	  votes = 0;
	  target = other->client->resp.kickvote;
	  playervoted++;
	  for (j = 1; j <= game.maxclients; j++)
	    {
	      other = &g_edicts[j];
	      if (other->client && other->inuse
		  && other->client->resp.kickvote == target)
		votes++;
	    }
	  if (votes > maxvotes)
	    {
	      maxvotes = votes;
	      mtarget = target;
	    }
	}
    }

  Mostkickvotes = NULL;

  if (!mtarget)
    return;

  Mostkickvotes = mtarget;
  Mostkickpercent = (float) (((float) maxvotes / (float) playernum) * 100.0);
  Allkickvotes = (float) (((float) playervoted / (float) playernum) * 100.0);

  if (Allkickvotes < kickvote_need->value)
    return;
  if (Mostkickpercent < kickvote_pass->value)
    return;

  // finally
  _DoKick (mtarget);
}

void
_KickSelected (edict_t * ent, pmenu_t * p)
{
  char *ch;

  ch = p->text;
  if (ch)
    {
      while (*ch != ':' && *ch != '\0')
	ch++;
      *ch = '\0';
    }
  ch = p->text;
  if (ch && *ch == '*')
    ch++;
  PMenu_Close (ent);
  Cmd_Votekicknum_f (ent, ch);
}

#define MostKickMarker " "

void
_AddKickuserToMenu (edict_t * ent, int fromix)
{
  int i, j;
  edict_t *other;
  qboolean erg;
  char buf[256];

  i = 1;
  j = 0;
  while (i <= game.maxclients && j < fromix)
    {
      other = &g_edicts[i];
      if (other->inuse && other != ent)
	j++;
      i++;
    }
  erg = true;
  while (i <= game.maxclients && erg)
    {
      other = &g_edicts[i];
      if (other->inuse && other != ent)
	{
	  //+ Marker: Hier gew�hlten markieren - erledigt -
	  sprintf (buf, "%s%2i: %s%s",
		   other == ent->client->resp.kickvote ? "*" : "", i,
		   other->client->pers.netname,
		   other == Mostkickvotes ? MostKickMarker : "");
	  erg = xMenu_Add (ent, buf, _KickSelected);
	}
      i++;
    }
}

void
_KickVoteSelected (edict_t * ent, pmenu_t * p)
{
  PMenu_Close (ent);
  if (xMenu_New
      (ent, KICKMENUTITLE, "vote for a player to kick",
       _AddKickuserToMenu) == false)
    gi.cprintf (ent, PRINT_MEDIUM, "No player to kick.\n");
}

void
Cmd_Votekick_f (edict_t * ent, char *argument)
{
  edict_t *target;

  if (!*argument)
    {
      gi.cprintf (ent, PRINT_HIGH, "\nUse votekick <playername>.\n");
      return;
    }
  target = FindClientByPersName (argument);
  if (target && target != ent)
    _SetKickVote (ent, target);
  else
    gi.cprintf (ent, PRINT_HIGH,
		"\nUse kicklist to see who can be kicked.\n");

}

void
Cmd_Votekicknum_f (edict_t * ent, char *argument)
{
  int i;
  edict_t *target;

  if (!*argument)
    {
      gi.cprintf (ent, PRINT_HIGH, "\nUse votekicknum <playernumber>.\n");
      return;
    }

  i = atoi (argument);

  if (i && i <= game.maxclients)
    {
      target = &g_edicts[i];
      if (target && target->client && target != ent && target->inuse)
	_SetKickVote (ent, target);
      else
	gi.cprintf (ent, PRINT_HIGH,
		    "\nUse kicklist to see who can be kicked.\n");
    }
  else
    gi.cprintf (ent, PRINT_HIGH, "\nUsed votekicknum with illegal number.\n");
}

qboolean _vkMarkThis (edict_t * self, edict_t * other)
{
  if (self->client->resp.kickvote == other)
    return true;
  return false;
}

void
Cmd_Kicklist_f (edict_t * ent, char *argument)
{
  char buf[16384], tbuf[2048];
  strcpy (buf, "\nAvailable players to kick:\n\n");
  _printplayerlist (ent, buf, _vkMarkThis);

  // adding vote settings
  sprintf (tbuf, "Vote rules: %i client%s min. (currently %i),\n" \
	   "%.1f%%%% must have voted overall (currently %.1f%%%%)\n" \
	   "and %.1f%%%% on the same (currently %.1f%%%% on %s),\n" \
	   "kicked players %s be temporarily banned.\n\n",
	   (int) (kickvote_min->value),
	   (kickvote_min->value == 1) ? " " : "s ",
	   _numclients (),
	   kickvote_need->value, Allkickvotes,
	   kickvote_pass->value, Mostkickpercent,
	   Mostkickvotes ==
	   NULL ? "nobody" : Mostkickvotes->client->pers.netname,
	   kickvote_tempban ? "will" : "won't");
  // double percent sign! cprintf will process them as format strings.

  strcat (buf, tbuf);
  gi.cprintf (ent, PRINT_MEDIUM, buf);
}

//=== config voting ========================================================
//
// Original programed by Black Cross[NL], adapted, major changes.
//
//==========================================================================

configlist_t *config_votes;
int config_num_configs;
int config_num_votes;
int config_num_clients;
qboolean config_need_to_check_votes;

cvar_t *cvote_min;
cvar_t *cvote_need;
cvar_t *cvote_pass;

char configlistpath[MAX_STR_LEN];

#define CONFIGVOTESECTION "configvote"

// forward declarations
configlist_t *ConfigWithMostVotes (float *p);
int AddVoteToConfig (char *configname, edict_t * ent);
void ReadConfiglistFile (void);
qboolean _iCheckConfigVotes (void);

//
void
Cmd_Voteconfig_f (edict_t * ent, char *t)
{
  if (!*t)
    {
      gi.cprintf (ent, PRINT_HIGH,
		  "You need an argument to the vote command (name of config).\n");
      return;
    }

  // BEGIN Igor[Rock]
  if (level.time < 40.0)
    {
      gi.cprintf (ent, PRINT_HIGH,
		  "Configvote currently blocked - Please vote again in %d seconds\n",
		  (int) (41.0 - level.time));
    }
  else
    {
      // END Igor[Rock]
      switch (AddVoteToConfig (t, ent))
	{
	case 0:
	  gi.cprintf (ent, PRINT_HIGH, "You have voted on config \"%s\"\n",
		      t);
	  break;
	case 1:
	  gi.cprintf (ent, PRINT_HIGH,
		      "You have changed your vote to config \"%s\"\n", t);
	  break;
	default:
	  //error
	  gi.cprintf (ent, PRINT_HIGH,
		      "Config \"%s\" is not in the votelist!\n", t);
	  break;
	}
      // BEGIN Igor[Rock]
    }
  //END Igor[Rock]

  return;
}

//
void
Cmd_Configlist_f (edict_t * ent, char *dummy)
{
  //go through the votelist and list out all the configs and % votes
  int lines, chars_on_line, len_mr;
  float p_test, p_most;
  configlist_t *search, *most;
  char msg_buf[16384], tmp_buf[400];	//only 40 are used

  p_test = p_most = 0.0;

  most = ConfigWithMostVotes (&p_most);

  sprintf (msg_buf,
	   "List of configs that can be voted on:\nRequire more than %d%%%% votes (%.2f)\n\n",
	   (int) cvote_pass->value,
	   (float) ((float) cvote_pass->value / 100.0));

  lines = chars_on_line = 0;
  for (search = config_votes; search != NULL; search = search->next)
    {

      if (config_num_clients > 0)
	p_test =
	  (float) ((float) search->num_votes / (float) config_num_clients);

      if (p_test >= 10.0)
	len_mr = 11;
      else
	len_mr = 10;

      len_mr += strlen (search->configname);

      if ((chars_on_line + len_mr + 2) > 39)
	{
	  strcat (msg_buf, "\n");
	  lines++;
	  chars_on_line = 0;
	  if (lines > 25)
	    break;
	}
      sprintf (tmp_buf, "%s (%.2f)  ", search->configname, p_test);
      strcat (msg_buf, tmp_buf);
      chars_on_line += len_mr;

    }

  if (config_votes == NULL)
    strcat (msg_buf, "None!");
  else if (most != NULL)
    {
      sprintf (tmp_buf, "\n\nMost votes: %s (%.2f)",
	       most->configname, p_most);
      strcat (msg_buf, tmp_buf);
    }

  strcat (msg_buf, "\n\n");
  sprintf (tmp_buf,
	   "%d/%d (%.2f%%%%) clients voted\n%d client%s minimum (%d%%%% required)",
	   config_num_votes, config_num_clients,
	   (float) ((float) config_num_votes /
		    (float) (config_num_clients >
			     0 ? config_num_clients : 1) * 100),
	   (int) cvote_min->value, (cvote_min->value > 1 ? "s" : ""),
	   (int) cvote_need->value);
  strcat (msg_buf, tmp_buf);

  gi.centerprintf (ent, msg_buf);

  return;
}

//
void
_ConfigInitClient (edict_t * ent)
{
  ent->client->resp.cvote = NULL;
}

//
void
_RemoveVoteFromConfig (edict_t * ent)
{
  configlist_t *search;

  config_need_to_check_votes = true;

  if (ent->client->resp.cvote == NULL)
    return;

  for (search = config_votes; search != NULL; search = search->next)
    if (Q_stricmp (search->configname, ent->client->resp.cvote) == 0)
      {
	config_num_votes--;
	search->num_votes--;
	ent->client->resp.cvote = NULL;
	break;
      }
  return;
}

//
void
_ConfigExitLevel (char *NextMap)
{
  configlist_t *voteconfig = NULL;
  char buf[MAX_STR_LEN];

  if (_iCheckConfigVotes ())
    {
      strncpy (NextMap, map_votes->mapname, MAX_QPATH);
      voteconfig = ConfigWithMostVotes (NULL);
      gi.bprintf (PRINT_HIGH, "A new config was voted on and is %s.\n",
		  voteconfig->configname);
      Com_sprintf (buf, sizeof (buf), "exec \"mode_%s.cfg\"\n",
		   voteconfig->configname);

      //clear stats
      for (voteconfig = config_votes; voteconfig != NULL;
	   voteconfig = voteconfig->next)
	{
	  voteconfig->num_votes = 0;
	}

      //clear stats
      config_num_votes = 0;
      config_num_clients = 0;
      config_need_to_check_votes = true;
      gi.AddCommandString (buf);
    }
  else
    {
      //clear stats
      config_num_votes = 0;
      config_num_clients = 0;
      config_need_to_check_votes = true;
    }

}

//
qboolean _CheckConfigVotes (void)
{
  if (_iCheckConfigVotes () == true)
    {
      gi.bprintf (PRINT_HIGH, "More than %i%% config votes reached.\n",
		  (int) cvote_pass->value);
      return true;
    }
  return false;
}

//
qboolean _ConfigMostVotesStr (char *buf)
{
  float p_most;
  configlist_t *most;

  p_most = 0.0;
  most = ConfigWithMostVotes (&p_most);
  if (most != NULL)
    {
      sprintf (buf, "%s (%.2f%%)", most->configname, p_most * 100.0);
      return true;
    }
  else
    strcpy (buf, "(no config)");
  return false;
}

//
void
_ConfigWithMostVotes (void)
{
  char buf[1024], sbuf[512];

  if (_ConfigMostVotesStr (sbuf))
    {
      sprintf (buf, "Most wanted config: %s\n", sbuf);
      gi.bprintf (PRINT_HIGH, strtostr2 (buf));
    }

}

//
cvar_t *
_InitConfiglist (ini_t * ini)
{
  char buf[1024];

  // note that this is done whether we have set "use_cvote" or not!
  config_votes = NULL;
  config_num_configs = 0;
  config_num_votes = 0;
  config_num_clients = 0;
  config_need_to_check_votes = true;
  ReadConfiglistFile ();

  use_cvote = gi.cvar ("use_cvote", "0", CVAR_SERVERINFO);

  cvote_min = gi.cvar ("cvote_min",
		       ReadIniStr (ini, CONFIGVOTESECTION, "cvote_min", buf,
				   "1"), CVAR_LATCH);
  cvote_need =
    gi.cvar ("cvote_need",
	     ReadIniStr (ini, CONFIGVOTESECTION, "cvote_need", buf, "0"),
	     CVAR_LATCH);
  cvote_pass =
    gi.cvar ("cvote_pass",
	     ReadIniStr (ini, CONFIGVOTESECTION, "cvote_pass", buf, "50"),
	     CVAR_LATCH);
  return (use_cvote);
}

//---------------

qboolean _iCheckConfigVotes (void)
{
  static qboolean enough = false;
  float p;
  configlist_t *tmp;

  if (!config_need_to_check_votes)
    return (enough);

  tmp = ConfigWithMostVotes (&p);

  enough = (tmp != NULL && p >= (float) ((float) cvote_pass->value / 100.0));
  if (config_num_clients < cvote_min->value)
    enough = false;
  if (cvote_need->value)
    {
      if ((float) ((float) config_num_votes / (float) config_num_clients) <
	  (float) ((float) cvote_need->value / (float) 100.0))
	enough = false;
    }

  config_need_to_check_votes = false;

  return (enough);
}


configlist_t *
ConfigWithMostVotes (float *p)
{
  int i;
  float p_most;
  configlist_t *search, *most;
  edict_t *e;

  p_most = 0.0;
  if (config_votes == NULL)
    return (NULL);

  //find config_num_clients
  config_num_clients = 0;
  for (i = 1; i <= maxclients->value; i++)
    {
      e = g_edicts + i;
      if (e->inuse)
	config_num_clients++;
    }

  if (config_num_clients == 0)
    return (NULL);

  most = NULL;
  for (search = config_votes; search != NULL; search = search->next)
    {
      if ((float) ((float) search->num_votes / (float) config_num_clients) >
	  p_most)
	{
	  p_most =
	    (float) ((float) search->num_votes / (float) config_num_clients);
	  most = search;
	}
    }

  if (p != NULL)
    *p = p_most;
  return (most);
}

int
AddVoteToConfig (char *configname, edict_t * ent)
{
  int changed = 0;
  configlist_t *search;

  config_need_to_check_votes = true;

  if (ent->client->resp.cvote != NULL)
    {
      _RemoveVoteFromConfig (ent);
      changed = 1;
    }

  for (search = config_votes; search != NULL; search = search->next)
    if (Q_stricmp (search->configname, configname) == 0)
      {
	config_num_votes++;
	search->num_votes++;
	ent->client->resp.cvote = search->configname;
	return changed;
      }

  // if we get here we didn't find the config!
  return -1;
}

void
ConfigSelected (edict_t * ent, pmenu_t * p)
{
  char *ch;

  ch = p->text;
  if (ch)
    {
      while (*ch != ' ' && *ch != '\0')
	ch++;
      *ch = '\0';
    }
  ch = p->text;
  if (ch && *ch == '*')
    ch++;
  PMenu_Close (ent);
  Cmd_Voteconfig_f (ent, ch);
}

void
AddConfigToMenu (edict_t * ent, int fromix)
{
  int i;
  char buffer[512], spc[64];
  configlist_t *search;
  float prozent;

  i = 0;
  search = config_votes;
  while (search && i < fromix)
    {
      search = search->next;
      i++;
    }
  while (search)
    {
      prozent =
	(float) (((float) search->num_votes / (float) config_num_clients) *
		 100);
      i = 27 - strlen (search->configname);
      if (prozent < 10.00)
	i -= 6;
      else if (prozent < 100.00)
	i -= 7;
      else
	i -= 8;
      if (i < 0)
	i = 0;
      spc[i--] = '\0';
      while (i >= 0)
	spc[i--] = ' ';
      sprintf (buffer, "%s%s%s%.1f%%",
	       ent->client->resp.cvote == search->configname ? "*" : "",
	       search->configname, spc, prozent);

      if (xMenu_Add (ent, buffer, ConfigSelected) == true)
	search = search->next;
      else
	search = NULL;
    }
}

void
ConfigVoteMenu (edict_t * ent, pmenu_t * p)
{
  char buf[1024], sbuf[512];

  PMenu_Close (ent);
  if (_ConfigMostVotesStr (sbuf));
  sprintf (buf, "most: %s", sbuf);
  if (xMenu_New (ent, CONFIGMENUTITLE, buf, AddConfigToMenu) == false)
    gi.cprintf (ent, PRINT_MEDIUM, "No config to vote for.\n");
}

void
ReadConfiglistFile (void)
{
  int i, bs;
  configlist_t *list = NULL, *tmp;
  FILE *configlist_file;
  char buf[MAX_STR_LEN];
  cvar_t *configlistname;

  configlistname = gi.cvar ("configlistname", "configlist.ini", 0);
  if (configlistname->string && *(configlistname->string))
    sprintf (configlistpath, "%s/%s", GAMEVERSION, configlistname->string);
  else
    sprintf (configlistpath, "%s/%s", GAMEVERSION, "configlist.ini");

  configlist_file = fopen (configlistpath, "r");
  //Igor[Rock] End
  if (configlist_file == NULL)
    {
    }
  else
    {
      // read the configlist.ini file
      for (i = 0; fgets (buf, MAX_STR_LEN - 10, configlist_file) != NULL;)
	{
	  //first remove trailing spaces
	  for (bs = strlen (buf);
	       bs > 0 && (buf[bs - 1] == '\r' || buf[bs - 1] == '\n'
			  || buf[bs - 1] == ' '); bs--)
	    buf[bs - 1] = '\0';
	  if (bs > 0 &&
	      strncmp (buf, "#", 1) != 0 && strncmp (buf, "//", 2) != 0)
	    {
	      if (i == 0)
		{
		  config_votes =
		    (struct configlist_s *) gi.
		    TagMalloc (sizeof (struct configlist_s), TAG_GAME);
		  config_votes->configname = gi.TagMalloc (bs + 1, TAG_GAME);
		  strcpy (config_votes->configname, buf);
		  config_votes->num_votes = 0;
		  config_votes->next = NULL;
		  list = config_votes;
		  i++;
		}
	      else
		{
		  tmp =
		    (struct configlist_s *) gi.
		    TagMalloc (sizeof (struct configlist_s), TAG_GAME);
		  tmp->configname = gi.TagMalloc (bs + 1, TAG_GAME);
		  strcpy (tmp->configname, buf);
		  tmp->num_votes = 0;
		  tmp->next = NULL;
		  list->next = tmp;
		  list = tmp;
		  i++;
		}
	    }
	}
      fclose (configlist_file);
      config_num_configs = i;
    }

  return;
}


//=== player ignoring ======================================================
//
// player ingoring is no voting, but since it uses the same
// functions like kickvoting and offers a menu, I put it here.
// At least you can say, that when it's no vote, it's a choice! :)
//
//==========================================================================


#define IGNORELIST client->resp.ignorelist


//Returns the next free slot in ignore list
int
_FindFreeIgnoreListEntry (edict_t * source)
{
  return (IsInIgnoreList (source, NULL));
}

//Clears a clients ignore list
void
_ClearIgnoreList (edict_t * ent)
{
  int i;

  if (!ent->client)
    return;
  for (i = 0; i < PG_MAXPLAYERS; i++)
    ent->IGNORELIST[i] = NULL;
}

//Checks if an edict is to be ignored, returns position
int
IsInIgnoreList (edict_t * source, edict_t * subject)
{
  int i;

  if (!source || !source->client)
    return 0;

  //ignorelist[0] is not used...
  for (i = 1; i < PG_MAXPLAYERS; i++)
    if (source->IGNORELIST[i] == subject)
      return i;
  return 0;
}

//Adds an edict to ignore list. If allready in, it will be removed
void
_AddOrDelIgnoreSubject (edict_t * source, edict_t * subject, qboolean silent)
{
  int i;

  if (!source->client)
    return;
  if (!subject->client || !subject->inuse)
    {
      gi.cprintf (source, PRINT_MEDIUM,
		  "\nOnly valid clients may be added to ignore list!\n");
      return;
    }

  i = IsInIgnoreList (source, subject);
  if (i)
    {
      //subject is in ignore list, so delete it
      source->IGNORELIST[i] = NULL;
      if (!silent)
	gi.cprintf (source, PRINT_MEDIUM,
		    "\n%s was removed from ignore list.\n",
		    subject->client->pers.netname);

      //Maybe this has to be taken out :)

      if (!silent)
	gi.cprintf (subject, PRINT_MEDIUM, "\n%s listen to your words.\n",
		    source->client->pers.netname);

      source->client->resp.ignore_time = level.framenum;
    }
  else
    {
      //subject has to be added
      i = _FindFreeIgnoreListEntry (source);

      if (!i)
	{
	  if (!silent)
	    gi.cprintf (source, PRINT_MEDIUM,
			"\nSorry, ignore list is full!\n");
	}
      else
	{
	  //we've found a place
	  source->IGNORELIST[i] = subject;
	  if (!silent)
	    gi.cprintf (source, PRINT_MEDIUM,
			"\n%s was added to ignore list.\n",
			subject->client->pers.netname);

	  //Maybe this has to be taken out :)

	  if (!silent)
	    gi.cprintf (subject, PRINT_MEDIUM, "\n%s ignores you.\n",
			source->client->pers.netname);
	}
    }
}

//
void
_ClrIgnoresOn (edict_t * target)
{
  edict_t *other;
  int i;

  for (i = 1; i <= game.maxclients; i++)
    {
      other = &g_edicts[i];
      if (other->client && other->inuse)
	{
	  if (IsInIgnoreList (other, target))
	    _AddOrDelIgnoreSubject (other, target, true);
	}
    }
}


//Ignores a player by name
void
Cmd_Ignore_f (edict_t * self, char *s)
{
  edict_t *target;

  if (!*s)
    {
      gi.cprintf (self, PRINT_MEDIUM, "\nUse ignore <playername>.\n");
      return;
    }
  target = FindClientByPersName (s);
  if (target && target != self)
    {
      if (level.framenum > (self->client->resp.ignore_time + 50))
	_AddOrDelIgnoreSubject (self, target, false);
      else
	{
	  gi.cprintf (self, PRINT_MEDIUM,
		      "Wait 5 seconds before ignoring again.\n");
	  return;
	}
    }
  else
    gi.cprintf (self, PRINT_MEDIUM,
		"\nUse ignorelist to see who can be ignored.\n");
}

//Ignores a player by number
void
Cmd_Ignorenum_f (edict_t * self, char *s)
{
  int i;
  edict_t *target;

  if (!*s)
    {
      gi.cprintf (self, PRINT_MEDIUM, "\nUse ignorenum <playernumber>.\n");
      return;
    }

  i = atoi (s);

  if (i && i <= game.maxclients)
    {

      target = &g_edicts[i];
      if (target && target->client && target != self && target->inuse)
	_AddOrDelIgnoreSubject (self, target, false);
      else
	gi.cprintf (self, PRINT_MEDIUM,
		    "\nUse ignorelist to see who can be ignored.\n");
    }
  else
    gi.cprintf (self, PRINT_MEDIUM,
		"\nUsed ignorenum with illegal number.\n");
}

qboolean _ilMarkThis (edict_t * self, edict_t * other)
{
  if (IsInIgnoreList (self, other))
    return true;
  return false;
}

void
Cmd_Ignorelist_f (edict_t * self, char *s)
{
  char buf[16384];
  strcpy (buf, "\nAvailable players to ignore:\n\n");
  _printplayerlist (self, buf, _ilMarkThis);
  gi.cprintf (self, PRINT_MEDIUM, buf);
}

//Clears ignore list - user interface :)
void
Cmd_Ignoreclear_f (edict_t * self, char *s)
{
  _ClearIgnoreList (self);
  gi.cprintf (self, PRINT_MEDIUM, "\nYour ignorelist is now clear.\n");
}

void
_IgnoreSelected (edict_t * ent, pmenu_t * p)
{
  char *ch;

  ch = p->text;
  if (ch)
    {
      while (*ch != ':' && *ch != '\0')
	ch++;
      *ch = '\0';
    }
  ch = p->text;
  if (ch && *ch == '*')
    ch++;
  PMenu_Close (ent);
  Cmd_Ignorenum_f (ent, ch);
}

void
_AddIgnoreuserToMenu (edict_t * ent, int fromix)
{
  int i, j;
  edict_t *other;
  qboolean erg;
  char buf[256];

  i = 1;
  j = 0;
  while (i < game.maxclients && j < fromix)
    {
      other = &g_edicts[i];
      if (other->inuse && other != ent)
	j++;
      i++;
    }
  erg = true;
  while (i < game.maxclients && erg)
    {
      other = &g_edicts[i];
      if (other->inuse && other != ent)
	{
	  //+ Marker: Hier gew�hlten markieren - erledigt -
	  sprintf (buf, "%s%2i: %s", IsInIgnoreList (ent, other) ? "*" : "",
		   i, other->client->pers.netname);
	  erg = xMenu_Add (ent, buf, _IgnoreSelected);
	}
      i++;
    }
}

void
_IgnoreVoteSelected (edict_t * ent, pmenu_t * p)
{
  PMenu_Close (ent);
  if (xMenu_New
      (ent, IGNOREMENUTITLE, "de-/select a player to ignore",
       _AddIgnoreuserToMenu) == false)
    gi.cprintf (ent, PRINT_MEDIUM, "No player to ignore.\n");
}



//=== flag voting ==========================================================
//
//==========================================================================
