//-----------------------------------------------------------------------------
// g_cmds.c
//
// $Id: g_cmds.c,v 1.62 2006/06/18 09:09:39 igor_rock Exp $
//
//-----------------------------------------------------------------------------
// $Log: g_cmds.c,v $
// Revision 1.62  2006/06/18 09:09:39  igor_rock
// - corrected some indexes from [1] to [TEAM1] and so on
//
// Revision 1.61  2006/06/17 11:35:40  igor_rock
// Some code cleanup:
// - moved team related variables to a single struct variable
//
// Revision 1.60  2004/04/08 23:19:51  slicerdw
// Optimized some code, added a couple of features and fixed minor bugs
//
// Revision 1.59  2003/06/15 21:43:53  igor
// added IRC client
//
// Revision 1.58  2003/06/15 15:34:32  igor
// - removed the zcam code from this branch (see other branch)
// - added fixes from 2.72 (source only) version
// - resetted version number to 2.72
// - This version should be exactly like the release 2.72 - just with a few
//   more fixes (which whoever did the source only variant didn't get because
//   he didn't use the CVS as he should. Shame on him.
//
// Revision 1.57  2002/09/04 11:23:09  ra
// Added zcam to TNG and bumped version to 3.0
//
// Revision 1.56  2002/03/28 11:46:03  freud
// stat_mode 2 and timelimit 0 did not show stats at end of round.
// Added lock/unlock.
// A fix for use_oldspawns 1, crash bug.
//
// Revision 1.55  2002/03/26 21:49:01  ra
// Bufferoverflow fixes
//
// Revision 1.54  2002/03/25 18:32:11  freud
// I'm being too productive.. New ghost command needs testing.
//
// Revision 1.53  2002/02/19 09:32:47  freud
// Removed PING PONGs from CVS, not fit for release.
//
// Revision 1.52  2002/02/18 20:21:36  freud
// Added PING PONG mechanism for timely disconnection of clients. This is
// based on a similar scheme as the scheme used by IRC. The client has
// cvar ping_timeout seconds to reply or will be disconnected.
//
// Revision 1.51  2002/02/13 12:13:00  deathwatch
// INVDROP Weaponfarming fix
//
// Revision 1.50  2002/02/01 12:54:08  ra
// messin with stat_mode
//
// Revision 1.49  2002/01/24 02:55:58  ra
// Fixed the mm_forceteamtalk 2 bug.
//
// Revision 1.48  2002/01/24 02:24:56  deathwatch
// Major update to Stats code (thanks to Freud)
// new cvars:
// stats_afterround - will display the stats after a round ends or map ends
// stats_endmap - if on (1) will display the stats scoreboard when the map ends
//
// Revision 1.47  2002/01/24 01:40:40  deathwatch
// Freud's AutoRecord
//
// Revision 1.46  2001/12/24 18:06:05  slicerdw
// changed dynamic check for darkmatch only
//
// Revision 1.44  2001/12/09 14:02:11  slicerdw
// Added gl_clear check -> video_check_glclear cvar
//
// Revision 1.43  2001/11/29 16:04:29  deathwatch
// Fixed the playerlist command
//
// Revision 1.42  2001/11/16 22:45:58  deathwatch
// Fixed %me again
//
// Revision 1.41  2001/11/09 23:58:12  deathwatch
// Fiixed the %me command to display '[DEAD]' properly
// Added the resetting of the teamXscore cvars at the exitlevel function where the team scores are reset as well. (not sure if that is correct)
//
// Revision 1.40  2001/11/03 20:10:18  slicerdw
// Removed the if that prevented people from talking at the end
//
// Revision 1.39  2001/11/03 17:43:20  deathwatch
// Fixed matchadmin - it should only work when in matchmode and not use say when typing it normally in any other mode (security issue for dumb ppl typing matchadmin password on a server without matchmode)
//
// Revision 1.38  2001/11/03 17:21:57  deathwatch
// Fixed something in the time command, removed the .. message from the voice command, fixed the vote spamming with mapvote, removed addpoint command (old pb command that wasnt being used). Some cleaning up of the source at a few points.
//
// Revision 1.37  2001/10/18 12:55:35  deathwatch
// Added roundtimeleft
//
// Revision 1.36  2001/09/30 03:09:34  ra
// Removed new stats at end of rounds and created a new command to
// do the same functionality.   Command is called "time"
//
// Revision 1.35  2001/09/28 22:00:46  deathwatch
// changed pheer to ph34rs in the kill denying statement
//
// Revision 1.34  2001/09/28 21:43:21  deathwatch
// Fixed a but caused due to the reformatting of the source making the say stuff not working in non-matchmode modes
//
// Revision 1.33  2001/09/28 14:20:25  slicerdw
// Few tweaks..
//
// Revision 1.32  2001/09/28 13:48:34  ra
// I ran indent over the sources. All .c and .h files reindented.
//
// Revision 1.31  2001/09/28 13:44:23  slicerdw
// Several Changes / improvements
//
// Revision 1.30  2001/08/20 00:41:15  slicerdw
// Added a new scoreboard for Teamplay with stats ( when map ends )
//
// Revision 1.29  2001/08/18 18:45:19  deathwatch
// Edited the Flashlight movement code to the Lasersight's movement code, its probably better
// and I added checks for darkmatch/being dead/being a spectator for its use
//
// Revision 1.28  2001/08/18 17:14:04  deathwatch
// Flashlight Added (not done yet, needs to prevent DEAD ppl from using it,
// the glow should be white and a bit smaller if possible and the daiper needs
// to be gone. Also, it should only work in 'darkmatch' I guess and it should
// make a sound when you turn it on/off.
//
// Revision 1.27  2001/08/17 21:31:37  deathwatch
// Added support for stats
//
// Revision 1.26  2001/08/08 12:42:22  slicerdw
// Ctf Should finnaly be fixed now, lets hope so
//
// Revision 1.25  2001/07/30 16:07:25  igor_rock
// added correct gender to "pheer" message
//
// Revision 1.24  2001/07/28 19:30:05  deathwatch
// Fixed the choose command (replaced weapon for item when it was working with items)
// and fixed some tabs on other documents to make it more readable
//
// Revision 1.23  2001/07/20 11:56:04  slicerdw
// Added a check for the players spawning during countdown on ctf ( lets hope it works )
//
// Revision 1.21  2001/06/25 12:39:38  slicerdw
// Cleaning up something i left behind..
//
// Revision 1.20  2001/06/25 11:44:47  slicerdw
// New Video Check System - video_check and video_check_lockpvs no longer latched
//
// Revision 1.19  2001/06/22 16:34:05  slicerdw
// Finished Matchmode Basics, now with admins, Say command tweaked...
//
// Revision 1.18  2001/06/21 00:05:30  slicerdw
// New Video Check System done -  might need some revision but works..
//
// Revision 1.15  2001/06/06 18:57:14  slicerdw
// Some tweaks on Ctf and related things
//
// Revision 1.12  2001/06/01 19:18:42  slicerdw
// Added Matchmode Code
//
// Revision 1.11  2001/06/01 08:25:42  igor_rock
// Merged New_Ctf-1_0 into main branch - this time hopefuly the whole one...
//
// Revision 1.10  2001/05/31 16:58:14  igor_rock
// conflicts resolved
//
// Revision 1.9  2001/05/20 15:00:19  slicerdw
// Some minor fixes and changings on Video Checking system
//
// Revision 1.8.2.4  2001/05/31 06:47:51  igor_rock
// - removed crash bug with non exisitng flag files
// - added new commands "setflag1", "setflag2" and "saveflags" to create
//   .flg files
//
// Revision 1.8.2.3  2001/05/27 13:33:37  igor_rock
// added flag drop command ("drop flag")
//
// Revision 1.8.2.2  2001/05/25 18:59:52  igor_rock
// Added CTF Mode completly :)
// Support for .flg files is still missing, but with "real" CTF maps like
// tq2gtd1 the ctf works fine.
// (I hope that all other modes still work, just tested DM and teamplay)
//
// Revision 1.8.2.1  2001/05/20 15:17:31  igor_rock
// removed the old ctf code completly
//
// Revision 1.8  2001/05/13 14:55:11  igor_rock
// corrected the lens command which was commented out in error
//
// Revision 1.7  2001/05/12 21:19:51  ra
//
//
// Added punishkills.
//
// Revision 1.6  2001/05/11 16:07:25  mort
// Various CTF bits and pieces...
//
// Revision 1.5  2001/05/11 12:21:19  slicerdw
// Commented old Location support ( ADF ) With the ML/ETE Compatible one
//
// Revision 1.4  2001/05/07 21:18:34  slicerdw
// Added Video Checking System
//
// Revision 1.3  2001/05/07 02:05:36  ra
//
//
// Added tkok command to forgive teamkills.
//
// Revision 1.2  2001/05/07 01:38:51  ra
//
//
// Added fixes for Ammo and Weaponsfarming.
//
// Revision 1.1.1.1  2001/05/06 17:30:36  igor_rock
// This is the PG Bund Edition V1.25 with all stuff laying around here...
//
//-----------------------------------------------------------------------------

#include "g_local.h"
#include "m_player.h"


char *
ClientTeam (edict_t * ent)
{
  char *p;
  static char value[512];

  value[0] = 0;

  if (!ent->client)
    return value;

  strcpy (value, Info_ValueForKey (ent->client->pers.userinfo, "skin"));
  p = strchr (value, '/');
  if (!p)
    return value;

  if ((int) (dmflags->value) & DF_MODELTEAMS)
    {
      *p = 0;
      return value;
    }

  // if ((int)(dmflags->value) & DF_SKINTEAMS)
  return ++p;
}

qboolean
OnSameTeam (edict_t * ent1, edict_t * ent2)
{
  char ent1Team[512];
  char ent2Team[512];

//FIREBLADE
  if (!ent1->client || !ent2->client)
    return false;

  if (teamplay->value)
    {
      return ent1->client->resp.team == ent2->client->resp.team;
    }
//FIREBLADE

  if (!((int) (dmflags->value) & (DF_MODELTEAMS | DF_SKINTEAMS)))
    return false;

  strcpy (ent1Team, ClientTeam (ent1));
  strcpy (ent2Team, ClientTeam (ent2));

  if (strcmp (ent1Team, ent2Team) == 0)
    return true;
  return false;
}


void
SelectNextItem (edict_t * ent, int itflags)
{
  gclient_t *cl;
  int i, index;
  gitem_t *it;

  cl = ent->client;

//FIREBLADE
  if (cl->menu)
    {
      PMenu_Next (ent);
      return;
    }
//FIREBLADE

  if (ent->solid == SOLID_NOT && ent->deadflag != DEAD_DEAD)
    return;

  // scan  for the next valid one
  for (i = 1; i <= MAX_ITEMS; i++)
    {
      index = (cl->pers.selected_item + i) % MAX_ITEMS;
      if (!cl->pers.inventory[index])
	continue;
      it = &itemlist[index];
      if (!it->use)
	continue;
      if (!(it->flags & itflags))
	continue;

      cl->pers.selected_item = index;
      return;
    }

  cl->pers.selected_item = -1;
}

void
SelectPrevItem (edict_t * ent, int itflags)
{
  gclient_t *cl;
  int i, index;
  gitem_t *it;

  cl = ent->client;

//FIREBLADE
  if (cl->menu)
    {
      PMenu_Prev (ent);
      return;
    }
//FIREBLADE

  if (ent->solid == SOLID_NOT && ent->deadflag != DEAD_DEAD)
    return;

  // scan  for the next valid one
  for (i = 1; i <= MAX_ITEMS; i++)
    {
      index = (cl->pers.selected_item + MAX_ITEMS - i) % MAX_ITEMS;
      if (!cl->pers.inventory[index])
	continue;
      it = &itemlist[index];
      if (!it->use)
	continue;
      if (!(it->flags & itflags))
	continue;

      cl->pers.selected_item = index;
      return;
    }

  cl->pers.selected_item = -1;
}

void
ValidateSelectedItem (edict_t * ent)
{
  gclient_t *cl;

  cl = ent->client;

  if (cl->pers.inventory[cl->pers.selected_item])
    return;			// valid

  SelectNextItem (ent, -1);
}


//=================================================================================

/*
==================
Cmd_Give_f

Give items to a client
==================
*/
void
Cmd_Give_f (edict_t * ent)
{
  char *name;
  char fixedname[32];
  gitem_t *it;
  int index;
  int i;
  qboolean give_all;
  edict_t *it_ent;
  edict_t etemp;

  if (deathmatch->value && !sv_cheats->value)
    {
      gi.cprintf (ent, PRINT_HIGH,
		  "You must run the server with '+set cheats 1' to enable this command.\n");
      return;
    }

  if (ent->solid == SOLID_NOT)
    {
      gi.cprintf (ent, PRINT_HIGH,
		  "This command can't be used by spectators.\n");
      return;
    }

  strncpy(fixedname, gi.args (), sizeof(fixedname)-1);
  name = fixedname;
//  name = gi.args ();

  if (Q_stricmp (name, "all") == 0)
    give_all = true;
  else
    give_all = false;

  if (Q_stricmp (gi.argv (1), "health") == 0)
    {
      /*    if (gi.argc() == 3)
         ent->health = atoi(gi.argv(2));
         else
         ent->health = ent->max_health;
         if (!give_all) */
      return;
    }

  if (give_all || Q_stricmp (name, "weapons") == 0)
    {
      for (i = 0; i < game.num_items; i++)
	{
	  it = itemlist + i;
	  if (!it->pickup)
	    continue;
	  if (!(it->flags & IT_WEAPON))
	    continue;
	  ent->client->pers.inventory[i] += 1;
	}
      if (!give_all)
	return;
    }

  if (give_all || Q_stricmp (name, "items") == 0)
    {
      for (i = 0; i < game.num_items; i++)
	{
	  it = itemlist + i;
	  if (!it->pickup)
	    continue;
	  if (!(it->flags & IT_ITEM))
	    continue;
	  etemp.item = it;

	  if (ent->client->unique_item_total >= unique_items->value)
	    ent->client->unique_item_total = unique_items->value - 1;

	  Pickup_Special (&etemp, ent);
	}
      if (!give_all)
	return;
    }


  if (give_all || Q_stricmp (name, "ammo") == 0)
    {
      ent->client->mk23_rds = ent->client->mk23_max;
      ent->client->dual_rds = ent->client->dual_max;
      ent->client->mp5_rds = ent->client->mp5_max;
      ent->client->m4_rds = ent->client->m4_max;
      ent->client->shot_rds = ent->client->shot_max;
      ent->client->sniper_rds = ent->client->sniper_max;
      ent->client->cannon_rds = ent->client->cannon_max;

      for (i = 0; i < game.num_items; i++)
	{
	  it = itemlist + i;
	  if (!it->pickup)
	    continue;
	  if (!(it->flags & IT_AMMO))
	    continue;
	  Add_Ammo (ent, it, 1000);
	}
      if (!give_all)
	return;
    }

  if (Q_stricmp (name, "armor") == 0)
    {
      /*
         gitem_armor_t   *info;

         it = FindItem("Jacket Armor");
         ent->client->pers.inventory[ITEM_INDEX(it)] = 0;

         it = FindItem("Combat Armor");
         ent->client->pers.inventory[ITEM_INDEX(it)] = 0;

         it = FindItem("Body Armor");
         info = (gitem_armor_t *)it->info;
         ent->client->pers.inventory[ITEM_INDEX(it)] = info->max_count;

         if (!give_all)
       */
      return;
    }

  if (Q_stricmp (name, "Power Shield") == 0)
    {
      /*it = FindItem("Power Shield");
         it_ent = G_Spawn();
         it_ent->classname = it->classname;
         SpawnItem (it_ent, it);
         Touch_Item (it_ent, ent, NULL, NULL);
         if (it_ent->inuse)
         G_FreeEdict(it_ent);

         if (!give_all)
       */
      return;
    }

  /*if (give_all)
     {
     for (i=0 ; i<game.num_items ; i++)
     {
     it = itemlist + i;
     if (!it->pickup)
     continue;
     if (it->flags & (IT_ARMOR|IT_WEAPON|IT_AMMO))
     continue;
     ent->client->pers.inventory[i] = 1;
     }
     return;
     } */

  if (give_all)
    {
      return;
    }


  it = FindItem (name);
  if (!it)
    {
      strncpy(fixedname, gi.argv (1), sizeof(fixedname)-1);
      name = fixedname;
//      name = gi.argv (1);
      it = FindItem (name);
      if (!it)
	{
	  gi.dprintf ("unknown item\n");
	  return;
	}
      if (!
	  (it->flags & IT_AMMO || it->flags & IT_WEAPON
	   || it->flags & IT_ITEM))
	return;
    }

  if (!(it->flags & IT_AMMO || it->flags & IT_WEAPON || it->flags & IT_ITEM))
    return;


  if (!it->pickup)
    {
      gi.dprintf ("non-pickup item\n");
      return;
    }


  index = ITEM_INDEX (it);

  if (it->flags & IT_AMMO)
    {
      /*  if (gi.argc() == 5)
         ent->client->pers.inventory[index] = atoi(gi.argv(4));
         else if ( (gi.argc() == 4)  && !(stricmp(it->pickup_name, "12 Gauge Shells")) )
         ent->client->pers.inventory[index] = atoi(gi.argv(3));
         else */
      ent->client->pers.inventory[index] += it->quantity;
    }
  else if (it->flags & IT_ITEM)
    {

      etemp.item = it;
      if (ent->client->unique_item_total >= unique_items->value)
	ent->client->unique_item_total = unique_items->value - 1;

      Pickup_Special (&etemp, ent);
    }

  else
    {
      it_ent = G_Spawn ();
      it_ent->classname = it->classname;
      SpawnItem (it_ent, it);
      Touch_Item (it_ent, ent, NULL, NULL);
      if (it_ent->inuse)
	G_FreeEdict (it_ent);
    }
}


/*
==================
Cmd_God_f

Sets client to godmode

argv(0) god
==================
*/
void
Cmd_God_f (edict_t * ent)
{
  char *msg;

  if (deathmatch->value && !sv_cheats->value)
    {
      gi.cprintf (ent, PRINT_HIGH,
		  "You must run the server with '+set cheats 1' to enable this command.\n");
      return;
    }

  ent->flags ^= FL_GODMODE;
  if (!(ent->flags & FL_GODMODE))
    msg = "godmode OFF\n";
  else
    msg = "godmode ON\n";

  gi.cprintf (ent, PRINT_HIGH, msg);
}


/*
==================
Cmd_Notarget_f

Sets client to notarget

argv(0) notarget
==================
*/
void
Cmd_Notarget_f (edict_t * ent)
{
  char *msg;

  if (deathmatch->value && !sv_cheats->value)
    {
      gi.cprintf (ent, PRINT_HIGH,
		  "You must run the server with '+set cheats 1' to enable this command.\n");
      return;
    }

  ent->flags ^= FL_NOTARGET;
  if (!(ent->flags & FL_NOTARGET))
    msg = "notarget OFF\n";
  else
    msg = "notarget ON\n";

  gi.cprintf (ent, PRINT_HIGH, msg);
}


/*
==================
Cmd_Noclip_f

argv(0) noclip
==================
*/
void
Cmd_Noclip_f (edict_t * ent)
{
  char *msg;

  if (deathmatch->value && !sv_cheats->value)
    {
      gi.cprintf (ent, PRINT_HIGH,
		  "You must run the server with '+set cheats 1' to enable this command.\n");
      return;
    }

  if (ent->movetype == MOVETYPE_NOCLIP)
    {
      ent->movetype = MOVETYPE_WALK;
      msg = "noclip OFF\n";
    }
  else
    {
      ent->movetype = MOVETYPE_NOCLIP;
      msg = "noclip ON\n";
    }

  gi.cprintf (ent, PRINT_HIGH, msg);
}


/*
==================
Cmd_Use_f

Use an inventory item
==================
*/
void
Cmd_Use_f (edict_t * ent)
{
  int index;
  gitem_t *it;
  char *s;

  s = gi.args ();

  //zucc - check for "special"
  if (stricmp (s, "special") == 0)
    {
      ReadySpecialWeapon (ent);
      return;
    }

  //zucc - alias names
  if (!stricmp (s, "blaster") || !stricmp (s, "mark 23 pistol"))
    s = MK23_NAME;
  if (!stricmp (s, "A 2nd pistol") || !stricmp (s, "railgun"))
    s = DUAL_NAME;
  if (!stricmp (s, "shotgun"))
    s = M3_NAME;
  if (!stricmp (s, "machinegun"))
    s = HC_NAME;
  if (!stricmp (s, "super shotgun"))
    s = MP5_NAME;
  if (!stricmp (s, "chaingun"))
    s = SNIPER_NAME;
  if (!stricmp (s, "bfg10k"))
    s = KNIFE_NAME;
  // zucc - let people pull up a knife ready to be thrown
  if (!stricmp (s, "throwing combat knife"))
    {
      if (ent->client->curr_weap != KNIFE_NUM)
	{
	  ent->client->resp.knife_mode = 1;
	}
      // switch to throwing mode if a knife is already out
      else
	{
	  Cmd_New_Weapon_f (ent);
	}
      s = KNIFE_NAME;
    }
  if (!stricmp (s, "slashing combat knife"))
    {
      if (ent->client->curr_weap != KNIFE_NUM)
	{
	  ent->client->resp.knife_mode = 0;
	}
      // switch to slashing mode if a knife is already out
      else
	{
	  Cmd_New_Weapon_f (ent);
	}
      s = KNIFE_NAME;
    }
  if (!stricmp (s, "grenade launcher"))
    s = M4_NAME;
  if (!stricmp (s, "grenades"))
    s = GRENADE_NAME;

  it = FindItem (s);

//FIREBLADE
  if (!it || (ent->solid == SOLID_NOT && ent->deadflag != DEAD_DEAD))
//FIREBLADE
    {
      gi.cprintf (ent, PRINT_HIGH, "Unknown item: %s\n", s);	// fixed capitalization -FB
      return;
    }

  if (!it->use)
    {
      gi.cprintf (ent, PRINT_HIGH, "Item is not usable.\n");
      return;
    }

  index = ITEM_INDEX (it);
  if (!ent->client->pers.inventory[index])
    {
      gi.cprintf (ent, PRINT_HIGH, "Out of item: %s\n", s);
      return;
    }

//TempFile
  ent->client->autoreloading = false;
//TempFile

  it->use (ent, it);
}


/*
==================
Cmd_Drop_f

Drop an inventory item
==================
*/
void
Cmd_Drop_f (edict_t * ent)
{
  int index;
  gitem_t *it;
  char *s;

  s = gi.args ();

  //zucc check to see if the string is weapon
  if (stricmp (s, "weapon") == 0)
    {
      DropSpecialWeapon (ent);
      return;
    }

  //zucc now for item
  if (stricmp (s, "item") == 0)
    {
      DropSpecialItem (ent);
      return;
    }

  if (stricmp (s, "flag") == 0)
    {
      CTFDrop_Flag (ent, NULL);
      return;
    }

// AQ:TNG - JBravo fixing ammo clip farming
  if (ent->client->weaponstate == WEAPON_RELOADING)
    return;
// Ammo clip farming fix end

  it = FindItem (s);
  if (!it)
    {
      gi.cprintf (ent, PRINT_HIGH, "unknown item: %s\n", s);
      return;
    }
  if (!it->drop)
    {
      gi.cprintf (ent, PRINT_HIGH, "Item is not dropable.\n");
      return;
    }
  index = ITEM_INDEX (it);
  if (!ent->client->pers.inventory[index])
    {
      gi.cprintf (ent, PRINT_HIGH, "Out of item: %s\n", s);
      return;
    }

  it->drop (ent, it);
}


/*
=================
Cmd_Inven_f
=================
*/
void
Cmd_Inven_f (edict_t * ent)
{
  int i;
  gclient_t *cl;

  cl = ent->client;

  cl->showscores = false;
  cl->showhelp = false;

//FIREBLADE
  if (ent->client->menu)
    {
      PMenu_Close (ent);
      return;
    }
//FIREBLADE

  if (cl->showinventory)
    {
      cl->showinventory = false;
      return;
    }

//FIREBLADE
  if (teamplay->value)
    {
      if (ent->client->resp.team == NOTEAM)
	OpenJoinMenu (ent);
      else
	OpenWeaponMenu (ent);
      return;
    }
  //FIREBLADE

  cl->showinventory = true;

  gi.WriteByte (svc_inventory);
  for (i = 0; i < MAX_ITEMS; i++)
    {
      gi.WriteShort (cl->pers.inventory[i]);
    }
  gi.unicast (ent, true);
}

/*
=================
Cmd_InvUse_f
=================
*/
void
Cmd_InvUse_f (edict_t * ent)
{
  gitem_t *it;

//FIREBLADE
  if (ent->client->menu)
    {
      PMenu_Select (ent);
      return;
    }
//FIREBLADE

  if (ent->solid == SOLID_NOT && ent->deadflag != DEAD_DEAD)
    return;

  ValidateSelectedItem (ent);

  if (ent->client->pers.selected_item == -1)
    {
      gi.cprintf (ent, PRINT_HIGH, "No item to use.\n");
      return;
    }

  it = &itemlist[ent->client->pers.selected_item];
  if (!it->use)
    {
      gi.cprintf (ent, PRINT_HIGH, "Item is not usable.\n");
      return;
    }
  it->use (ent, it);
}

/*
=================
Cmd_WeapPrev_f
=================
*/
void
Cmd_WeapPrev_f (edict_t * ent)
{
  gclient_t *cl;
  int i, index;
  gitem_t *it;
  int selected_weapon;

  if (ent->solid == SOLID_NOT && ent->deadflag != DEAD_DEAD)
    return;

  cl = ent->client;

  if (!cl->pers.weapon)
    return;

  selected_weapon = ITEM_INDEX (cl->pers.weapon);

  // scan  for the next valid one
  for (i = 1; i <= MAX_ITEMS; i++)
    {
      index = (selected_weapon + i) % MAX_ITEMS;
      if (!cl->pers.inventory[index])
	continue;
      it = &itemlist[index];
      if (!it->use)
	continue;
      if (!(it->flags & IT_WEAPON))
	continue;
      it->use (ent, it);
      if (cl->pers.weapon == it)
	return;			// successful
    }
}

/*
=================
Cmd_WeapNext_f
=================
*/
void
Cmd_WeapNext_f (edict_t * ent)
{
  gclient_t *cl;
  int i, index;
  gitem_t *it;
  int selected_weapon;

  if (ent->solid == SOLID_NOT && ent->deadflag != DEAD_DEAD)
    return;

  cl = ent->client;

  if (!cl->pers.weapon)
    return;

  selected_weapon = ITEM_INDEX (cl->pers.weapon);

  // scan  for the next valid one
  for (i = 1; i <= MAX_ITEMS; i++)
    {
      index = (selected_weapon + MAX_ITEMS - i) % MAX_ITEMS;
      if (!cl->pers.inventory[index])
	continue;
      it = &itemlist[index];
      if (!it->use)
	continue;
      if (!(it->flags & IT_WEAPON))
	continue;
      it->use (ent, it);
      if (cl->pers.weapon == it)
	return;			// successful
    }
}

/*
=================
Cmd_WeapLast_f
=================
*/
void
Cmd_WeapLast_f (edict_t * ent)
{
  gclient_t *cl;
  int index;
  gitem_t *it;

  if (ent->solid == SOLID_NOT && ent->deadflag != DEAD_DEAD)
    return;

  cl = ent->client;

  if (!cl->pers.weapon || !cl->pers.lastweapon)
    return;

  index = ITEM_INDEX (cl->pers.lastweapon);
  if (!cl->pers.inventory[index])
    return;
  it = &itemlist[index];
  if (!it->use)
    return;
  if (!(it->flags & IT_WEAPON))
    return;
  it->use (ent, it);
}

/*
=================
Cmd_InvDrop_f
=================
*/
void
Cmd_InvDrop_f (edict_t * ent)
{
  gitem_t *it;

	// TNG: Temp fix for INVDROP weapon farming
	if(teamplay->value)
		return;
	// TNG: End

  if (ent->solid == SOLID_NOT && ent->deadflag != DEAD_DEAD)
    return;

  ValidateSelectedItem (ent);

  if (ent->client->pers.selected_item == -1)
  {
    gi.cprintf (ent, PRINT_HIGH, "No item to drop.\n");
    return;
  }

  it = &itemlist[ent->client->pers.selected_item];
  if (!it->drop)
  {
    gi.cprintf (ent, PRINT_HIGH, "Item is not dropable.\n");
    return;
  }
  it->drop (ent, it);

}

/*
=================
Cmd_Kill_f
=================
*/
void
Cmd_Kill_f (edict_t * ent)
{
//FIREBLADE
  if (ent->solid == SOLID_NOT || ent->deadflag == DEAD_DEAD)
    return;
//FIREBLADE
// AQ:TNG - JBravo adding punishkills
  if (punishkills->value)
    {
      if (ent->client->attacker &&
	  ent->client->attacker->client &&
	  (ent->client->attacker->client != ent->client))
	{
	  gi.bprintf (PRINT_HIGH,
		      "%s ph34rs %s so much %s committed suicide! :)\n",
		      ent->client->pers.netname,
		      ent->client->attacker->client->pers.netname,
		      ent->client->resp.radio_gender ? "she" : "he");
	  Add_Frag (ent->client->attacker);
	  Subtract_Frag (ent);
	}
    }
// End punishkills

  if ((level.time - ent->client->respawn_time) < 5)
    return;
  ent->flags &= ~FL_GODMODE;
  ent->health = 0;
  meansOfDeath = MOD_SUICIDE;
  player_die (ent, ent, ent, 100000, vec3_origin);
// Forget all this... -FB
//        // don't even bother waiting for death frames
//        ent->deadflag = DEAD_DEAD;
////FIREBLADE
//        if (!teamplay->value)
////FIREBLADE
//                respawn (ent);
}

/*
=================
Cmd_PutAway_f
=================
*/
void
Cmd_PutAway_f (edict_t * ent)
{
  ent->client->showscores = false;
  ent->client->showhelp = false;
  ent->client->showinventory = false;
//FIREBLADE
  if (ent->client->menu)
    PMenu_Close (ent);
//FIREBLADE
}


int
PlayerSort (void const *a, void const *b)
{
  int anum, bnum;

  anum = *(int *) a;
  bnum = *(int *) b;

  anum = game.clients[anum].ps.stats[STAT_FRAGS];
  bnum = game.clients[bnum].ps.stats[STAT_FRAGS];

  if (anum < bnum)
    return -1;
  if (anum > bnum)
    return 1;
  return 0;
}

/*
=================
Cmd_Players_f
=================
*/
void
Cmd_Players_f (edict_t * ent)
{
  int i;
  int count;
  char small[64];
  char large[1280];
  int index[256];

  count = 0;
  for (i = 0; i < maxclients->value; i++)
    {
      if (game.clients[i].pers.connected)
	{
	  index[count] = i;
	  count++;
	}
    }

  if (!teamplay->value || !noscore->value)
    {
      // sort by frags
      qsort (index, count, sizeof (index[0]), PlayerSort);
    }

  // print information
  large[0] = 0;

  for (i = 0; i < count; i++)
    {
      if (!teamplay->value || !noscore->value)
	Com_sprintf (small, sizeof (small), "%3i %s\n",
		     game.clients[index[i]].ps.stats[STAT_FRAGS],
		     game.clients[index[i]].pers.netname);
      else
	Com_sprintf (small, sizeof (small), "%s\n",
		     game.clients[index[i]].pers.netname);

      if (strlen (small) + strlen (large) > sizeof (large) - 100)
	{			// can't print all of them in one packet
	  strcat (large, "...\n");
	  break;
	}
      strcat (large, small);
    }

  gi.cprintf (ent, PRINT_HIGH, "%s\n%i players\n", large, count);
}

/*
=================
Cmd_Wave_f
=================
*/
void
Cmd_Wave_f (edict_t * ent)
{
  int i;

  i = atoi (gi.argv (1));

  // can't wave when ducked
  if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
    return;

  if (ent->client->anim_priority > ANIM_WAVE)
    return;

  ent->client->anim_priority = ANIM_WAVE;

  switch (i)
    {
    case 0:
      gi.cprintf (ent, PRINT_HIGH, "flipoff\n");
      ent->s.frame = FRAME_flip01 - 1;
      ent->client->anim_end = FRAME_flip12;
      break;
    case 1:
      gi.cprintf (ent, PRINT_HIGH, "salute\n");
      ent->s.frame = FRAME_salute01 - 1;
      ent->client->anim_end = FRAME_salute11;
      break;
    case 2:
      gi.cprintf (ent, PRINT_HIGH, "taunt\n");
      ent->s.frame = FRAME_taunt01 - 1;
      ent->client->anim_end = FRAME_taunt17;
      break;
    case 3:
      gi.cprintf (ent, PRINT_HIGH, "wave\n");
      ent->s.frame = FRAME_wave01 - 1;
      ent->client->anim_end = FRAME_wave11;
      break;
    case 4:
    default:
      gi.cprintf (ent, PRINT_HIGH, "point\n");
      ent->s.frame = FRAME_point01 - 1;
      ent->client->anim_end = FRAME_point12;
      break;
    }
}

/*
==================
Cmd_Say_f
==================
*/
void
Cmd_Say_f (edict_t * ent, qboolean team, qboolean arg0, qboolean partner_msg)
{
  int j, i, offset_of_text;
  edict_t *other;
  char *p;
  char text[2048];
  gclient_t *cl;
  int meing = 0;
  char firstword[1024];
  int isadmin = 0;
	
  if (gi.argc () < 2 && !arg0)
    return;
	
  //FIREBLADE
  if (!teamplay->value)
	{
		//FIREBLADE
		if (!((int) (dmflags->value) & (DF_MODELTEAMS | DF_SKINTEAMS)))
			team = false;
	}
	
  if (!sv_crlf->value)
	{
		if (strchr (gi.args (), '\r') || strchr (gi.args (), '\n'))
		{
			gi.cprintf (ent, PRINT_HIGH,
		      "No control characters in chat messages!\n");
			return;
		}
	}
  if (matchmode->value)
	{
		//TempFile - BEGIN
		if (arg0)
			strcpy (firstword, gi.argv (0));
		else
			sscanf (gi.args (), "%s", firstword);
		
		if (!Q_stricmp ("%me", firstword))
			meing = 4;
		else if (!Q_stricmp ("%me", firstword + 1))
			meing = 5;
		//TempFile - END
		
		if (ent->client->resp.admin)
			isadmin = 1;
		if (mm_forceteamtalk->value == 1)
		{
			if (!ent->client->resp.captain && !partner_msg && !isadmin)
				team = 1;
		}
		else if (mm_forceteamtalk->value == 2)
		{
			if (!ent->client->resp.captain && !partner_msg && !isadmin &&
				((team_data[TEAM1].ready && team_data[TEAM2].ready) || team_round_going))
				team = 1;
		}
		if (team)
		{
			if (ent->client->resp.team == NOTEAM)
			{
				gi.cprintf (ent, PRINT_HIGH, "You're not on a team.\n");
				return;
			}
			if (!meing)		// TempFile
				Com_sprintf (text, sizeof (text), "%s(%s): ",
				(teamplay->value
				&& (ent->solid == SOLID_NOT
				|| ent->deadflag ==
				DEAD_DEAD)) ? "[DEAD] " : "",
				ent->client->pers.netname);
			//TempFile - BEGIN
			else
				Com_sprintf (text, sizeof (text), "(%s%s ",
				(teamplay->value
				&& (ent->solid == SOLID_NOT
				|| ent->deadflag == DEAD_DEAD)) ? "[DEAD] " : "",
				ent->client->pers.netname);
			//TempFile - END
		}
		else if (partner_msg)
		{
			if (ent->client->resp.radio_partner == NULL)
			{
				gi.cprintf (ent, PRINT_HIGH, "You don't have a partner.\n");
				return;
			}
			if (!meing)		//TempFile
				Com_sprintf (text, sizeof (text), "[%sPARTNER] %s: ",
				(teamplay->value
				&& (ent->solid == SOLID_NOT
				|| ent->deadflag == DEAD_DEAD)) ? "[DEAD] " : "",
				ent->client->pers.netname);
			//TempFile - BEGIN
			else
				Com_sprintf (text, sizeof (text), "%s partner %s ",
				(teamplay->value
				&& (ent->solid == SOLID_NOT
				|| ent->deadflag == DEAD_DEAD)) ? "[DEAD] " : "",
				ent->client->pers.netname);
			//TempFile - END
		}
		else
		{
			if (!meing)		//TempFile
			{
				if (isadmin)
					Com_sprintf (text, sizeof (text), "[ADMIN] %s: ",
					ent->client->pers.netname);
				else
					Com_sprintf (text, sizeof (text), "%s%s: ",
					(teamplay->value
					&& (ent->solid == SOLID_NOT
					|| ent->deadflag ==
					DEAD_DEAD)) ? "[DEAD] " : "",
					ent->client->pers.netname);
				//TempFile - BEGIN
			}
			else
				Com_sprintf (text, sizeof (text), "%s%s ",
				(teamplay->value
				&& (ent->solid == SOLID_NOT
				|| ent->deadflag == DEAD_DEAD)) ? "[DEAD] " : "",
				ent->client->pers.netname);
		}
		//TempFile - END
		
		offset_of_text = strlen (text);	//FB 5/31/99
		if (!meing)		//TempFile
		{
			if (arg0)
			{
				strcat (text, gi.argv (0));
				strcat (text, " ");
				strcat (text, gi.args ());
			}
			else
			{
				p = gi.args ();
				
				if (*p == '"')
				{
					p++;
					p[strlen (p) - 1] = 0;
				}
				strcat (text, p);
			}
		}
		else			// if meing
		{
			if (arg0)
			{
				//this one is easy: gi.args() cuts /me off for us!
				strcat (text, gi.args ());
			}
			else
			{
				// we have to cut off "%me ".
				p = gi.args () + meing;
				if (p[strlen (p) - 1] == '"')
					p[strlen (p) - 1] = 0;
				strcat (text, p);
			}
			
			if (team)
				strcat (text, ")");
		}
		//TempFile - END
		// don't let text be too long for malicious reasons
		// ...doubled this limit for Axshun -FB
		// down a bit, crashed sometimes - TempFile
		if (strlen (text) > 225)
			text[225] = 0;
		
		if (ent->solid != SOLID_NOT && ent->deadflag != DEAD_DEAD)
			ParseSayText (ent, text + offset_of_text);	//FB 5/31/99 - offset change
		// this will parse the % variables,
		// and again check 300 limit afterwards -FB
		// (although it checks it without the name in front, oh well)
		
		strcat (text, "\n");
		
		if (flood_msgs->value)
		{
			cl = ent->client;
			
			if (level.time < cl->flood_locktill)
			{
				gi.cprintf (ent, PRINT_HIGH,
					"You can't talk for %d more seconds.\n",
					(int) (cl->flood_locktill - level.time));
				return;
			}
			i = cl->flood_whenhead - flood_msgs->value + 1;
			if (i < 0)
				i = (sizeof (cl->flood_when) / sizeof (cl->flood_when[0])) + i;
			if (cl->flood_when[i] &&
				level.time - cl->flood_when[i] < flood_persecond->value)
			{
				cl->flood_locktill = level.time + flood_waitdelay->value;
				gi.cprintf (ent, PRINT_HIGH, "You can't talk for %d seconds.\n",
					(int) flood_waitdelay->value);
				return;
			}
			cl->flood_whenhead = (cl->flood_whenhead + 1) %
				(sizeof (cl->flood_when) / sizeof (cl->flood_when[0]));
			cl->flood_when[cl->flood_whenhead] = level.time;
		}
		
		if (dedicated->value) {
		  gi.cprintf (NULL, PRINT_CHAT, "%s", text);
		  if ((!team) && (!partner_msg)) {
		    IRC_printf (IRC_T_TALK, "%s", text);
		  }
		}
		
		for (j = 1; j <= game.maxclients; j++)
		{
			other = &g_edicts[j];
			if (!other->inuse)
				continue;
			if (!other->client)
				continue;
			if (team)
			{
				// if we are the adminent... we might want to hear (if hearall is set)
				if (!hearall->value || !other->client->resp.admin)	// hearall isn't set and we aren't adminent
					if (!OnSameTeam (ent, other))
						continue;
			}
			if (partner_msg)
			{
				if (other != ent->client->resp.radio_partner && other != ent)
					continue;
			}
			//FIREBLADE
			if (teamplay->value && team_round_going)
			{
				if ((ent->solid == SOLID_NOT || ent->deadflag == DEAD_DEAD) &&
					(other->solid != SOLID_NOT && other->deadflag != DEAD_DEAD)
					&& !ctf->value)	//AQ2:TNG Slicer
					continue;
			}
			//FIREBLADE
			//PG BUND - BEGIN
			if (IsInIgnoreList (other, ent))
				continue;
			//PG BUND - END
			gi.cprintf (other, PRINT_CHAT, "%s", text);
		}
    }
		else
    {
			
      //TempFile - BEGIN
      if (arg0)
				strcpy (firstword, gi.argv (0));
      else
				sscanf (gi.args (), "%s", firstword);
			
      if (!Q_stricmp ("%me", firstword))
				meing = 4;
      else if (!Q_stricmp ("%me", firstword + 1))
				meing = 5;
      //TempFile - END
			
      if (team)
			{
				if (ent->client->resp.team == NOTEAM)
				{
					gi.cprintf (ent, PRINT_HIGH, "You're not on a team.\n");
					return;
				}
				if (!meing)		// TempFile
					Com_sprintf (text, sizeof (text), "%s(%s): ",
					(teamplay->value
					&& (ent->solid == SOLID_NOT
					|| ent->deadflag ==
					DEAD_DEAD)) ? "[DEAD] " : "",
					ent->client->pers.netname);
				//TempFile - BEGIN
				else
					Com_sprintf (text, sizeof (text), "(%s%s ",
					(teamplay->value
					&& (ent->solid == SOLID_NOT
					|| ent->deadflag == DEAD_DEAD)) ? "[DEAD] " : "",
					ent->client->pers.netname);
				//TempFile - END
			}
      else if (partner_msg)
			{
				if (ent->client->resp.radio_partner == NULL)
				{
					gi.cprintf (ent, PRINT_HIGH, "You don't have a partner.\n");
					return;
				}
				if (!meing)		//TempFile
					Com_sprintf (text, sizeof (text), "[%sPARTNER] %s: ",
					(teamplay->value
					&& (ent->solid == SOLID_NOT
					|| ent->deadflag == DEAD_DEAD)) ? "[DEAD] " : "",
					ent->client->pers.netname);
				//TempFile - BEGIN
				else
					Com_sprintf (text, sizeof (text), "%s partner %s ",
					(teamplay->value
					&& (ent->solid == SOLID_NOT
					|| ent->deadflag == DEAD_DEAD)) ? "[DEAD] " : "",
					ent->client->pers.netname);
				//TempFile - END
			}
      else
			{
				if (!meing)		//TempFile
					Com_sprintf (text, sizeof (text), "%s%s: ",
					(teamplay->value
					&& (ent->solid == SOLID_NOT
					|| ent->deadflag ==
					DEAD_DEAD)) ? "[DEAD] " : "",
					ent->client->pers.netname);
				//TempFile - BEGIN
				else
					Com_sprintf (text, sizeof (text), "%s%s ",
					(teamplay->value
					&& (ent->solid == SOLID_NOT
					|| ent->deadflag == DEAD_DEAD)) ? "[DEAD] " : "",
					ent->client->pers.netname);
			}
      //TempFile - END
			
      offset_of_text = strlen (text);	//FB 5/31/99
      if (!meing)		//TempFile
			{
				if (arg0)
				{
					strcat (text, gi.argv (0));
					strcat (text, " ");
					strcat (text, gi.args ());
				}
				else
				{
					p = gi.args ();
					
					if (*p == '"')
					{
						p++;
						p[strlen (p) - 1] = 0;
					}
					strcat (text, p);
				}
			}
      else			// if meing
			{
				if (arg0)
				{
					//this one is easy: gi.args() cuts /me off for us!
					strcat (text, gi.args ());
				}
				else
				{
					// we have to cut off "%me ".
					p = gi.args () + meing;
					if (p[strlen (p) - 1] == '"')
						p[strlen (p) - 1] = 0;
					strcat (text, p);
				}
				
				if (team)
					strcat (text, ")");
			}
      //TempFile - END
      // don't let text be too long for malicious reasons
      // ...doubled this limit for Axshun -FB
      // down a bit, crashed sometimes - TempFile
      if (strlen (text) > 225)
				text[225] = 0;
			
      if (ent->solid != SOLID_NOT && ent->deadflag != DEAD_DEAD)
				ParseSayText (ent, text + offset_of_text);	//FB 5/31/99 - offset change
      // this will parse the % variables,
      // and again check 300 limit afterwards -FB
      // (although it checks it without the name in front, oh well)
			
      strcat (text, "\n");
			
      if (flood_msgs->value)
			{
				cl = ent->client;
				
				if (level.time < cl->flood_locktill)
				{
					gi.cprintf (ent, PRINT_HIGH,
						"You can't talk for %d more seconds.\n",
						(int) (cl->flood_locktill - level.time));
					return;
				}
				i = cl->flood_whenhead - flood_msgs->value + 1;
				if (i < 0)
					i = (sizeof (cl->flood_when) / sizeof (cl->flood_when[0])) + i;
				if (cl->flood_when[i] &&
					level.time - cl->flood_when[i] < flood_persecond->value)
				{
					cl->flood_locktill = level.time + flood_waitdelay->value;
					gi.cprintf (ent, PRINT_HIGH, "You can't talk for %d seconds.\n",
						(int) flood_waitdelay->value);
					return;
				}
				cl->flood_whenhead = (cl->flood_whenhead + 1) %
					(sizeof (cl->flood_when) / sizeof (cl->flood_when[0]));
				cl->flood_when[cl->flood_whenhead] = level.time;
			}
			
      if (dedicated->value) {
	gi.cprintf (NULL, PRINT_CHAT, "%s", text);
	IRC_printf (IRC_T_TALK, "%s", text);
       }

			
      for (j = 1; j <= game.maxclients; j++)
			{
				other = &g_edicts[j];
				if (!other->inuse)
					continue;
				if (!other->client)
					continue;
				if (team)
				{
					if (!OnSameTeam (ent, other))
						continue;
				}
				if (partner_msg)
				{
					if (other != ent->client->resp.radio_partner && other != ent)
						continue;
				}
				//FIREBLADE
				if (teamplay->value && team_round_going)
				{
					if ((ent->solid == SOLID_NOT || ent->deadflag == DEAD_DEAD) &&
						(other->solid != SOLID_NOT && other->deadflag != DEAD_DEAD)
						&& !ctf->value)	//AQ2:TNG Slicer
						continue;
				}
				//FIREBLADE
				//PG BUND - BEGIN
				if (IsInIgnoreList (other, ent))
					continue;
				//PG BUND - END                 
				gi.cprintf(other,PRINT_CHAT,"%s",text);
			}
    }
}

void Cmd_PlayerList_f (edict_t * ent)
{
  int i;
  char st[80];
  char text[1280];
  edict_t *e2;

  // connect time, ping, score, name
  *text = 0;

	// Set the lines:
  for (i = 0, e2 = g_edicts + 1; i < maxclients->value; i++, e2++)
  {
		if (!e2->inuse)
			continue;

		if(limchasecam->value)
			Com_sprintf (st, sizeof (st), "%02d:%02d %4d %3d %s\n",(level.framenum - e2->client->resp.enterframe) / 600,((level.framenum - e2->client->resp.enterframe) % 600) / 10, e2->client->ping, e2->client->resp.team, e2->client->pers.netname); // This shouldn't show player's being 'spectators' during games with limchasecam set and/or during matchmode
    else if (!teamplay->value || !noscore->value)
			Com_sprintf (st, sizeof (st), "%02d:%02d %4d %3d %s%s\n",(level.framenum - e2->client->resp.enterframe) / 600,((level.framenum - e2->client->resp.enterframe) % 600) / 10, e2->client->ping, e2->client->resp.score, e2->client->pers.netname, (e2->solid == SOLID_NOT && e2->deadflag != DEAD_DEAD) ? " (dead)" : ""); // replaced 'spectator' with 'dead'
		else
			Com_sprintf (st, sizeof (st), "%02d:%02d %4d %s%s\n",(level.framenum - e2->client->resp.enterframe) / 600, ((level.framenum - e2->client->resp.enterframe) % 600) / 10, e2->client->ping, e2->client->pers.netname, (e2->solid == SOLID_NOT && e2->deadflag != DEAD_DEAD) ? " (dead)" : ""); // replaced 'spectator' with 'dead'

    if (strlen (text) + strlen (st) > sizeof (text) - 100)
		{
			sprintf (text + strlen (text), "...\n");
			gi.cprintf (ent, PRINT_HIGH, "%s", text);
			return;
		}
    strcat (text, st);
	}
  gi.cprintf (ent, PRINT_HIGH, "%s", text);
}

//SLICER
void
Cmd_Ent_Count_f (edict_t * ent)
{
  int x;
  edict_t *e;

  x = 0;

  for (e = g_edicts; e < &g_edicts[globals.num_edicts]; e++)
    {
      if (e->inuse)
	x++;
    }

  gi.cprintf (ent, PRINT_HIGH, "%d entities counted\n", x);
}

//SLICER END

void
Cmd_SetAdmin_f (edict_t * ent)
{
	if(!matchmode->value) {
		gi.cprintf(ent, PRINT_HIGH, "Matchmode is not enabled on this server.\n");
		return;
	}

  if (gi.argc () < 2)
    {
      gi.cprintf (ent, PRINT_HIGH, "Usage:  matchadmin <password>\n");
      return;
    }
  if(strcmp (mm_adminpwd->string, "0") == 0)
  {
	  gi.cprintf (ent, PRINT_HIGH, "Admin Mode is not enabled on this server..\n");
      return;

  }
  if (strcmp (mm_adminpwd->string, gi.argv (1)) == 0)
    {
      if (ent->client->resp.admin)
	{
	  gi.cprintf (ent, PRINT_HIGH, "You are no longer a match admin.\n");
	  gi.dprintf ("%s is no longer a Match Admin\n",
		      ent->client->pers.netname);
	  IRC_printf (IRC_T_GAME, "%n is no longer a Match Admin",
		      ent->client->pers.netname);
	  ent->client->resp.admin = 0;

	}
      else
	{
	  gi.cprintf (ent, PRINT_HIGH, "You are now a match admin.\n");
	  gi.dprintf ("%s is now a Match Admin\n", ent->client->pers.netname);
	  IRC_printf (IRC_T_GAME, "%n is now a Match Admin", ent->client->pers.netname);
	  ent->client->resp.admin = 1;
	}

    }
  else
    gi.cprintf (ent, PRINT_HIGH, "Wrong password\n");



}

/*
=================
ClientCommand
=================
*/
void
ClientCommand (edict_t * ent)
{
  char *cmd;

  if (!ent->client)
    return;			// not fully in game yet
 // if (level.intermissiontime)
   // return;

  cmd = gi.argv (0);

  if (Q_stricmp (cmd, "players") == 0)
    {
      Cmd_Players_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "say") == 0)
    {
      Cmd_Say_f (ent, false, false, false);
      return;
    }
  else if (Q_stricmp (cmd, "say_team") == 0)
    {
      Cmd_Say_f (ent, true, false, false);
      return;
    }
  else if (Q_stricmp (cmd, "score") == 0)
    {
      Cmd_Score_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "help") == 0)
    {
      Cmd_Help_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "use") == 0)
    {
      Cmd_Use_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "drop") == 0)
    {
      Cmd_Drop_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "give") == 0)
    {
      Cmd_Give_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "god") == 0)
    {
      Cmd_God_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "notarget") == 0)
    {
      Cmd_Notarget_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "noclip") == 0)
    {
      Cmd_Noclip_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "inven") == 0)
    {
      Cmd_Inven_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "invnext") == 0)
    {
      SelectNextItem (ent, -1);
      return;
    }
  else if (Q_stricmp (cmd, "invprev") == 0)
    {
      SelectPrevItem (ent, -1);
      return;
    }
  else if (Q_stricmp (cmd, "invnextw") == 0)
    {
      SelectNextItem (ent, IT_WEAPON);
      return;
    }
  else if (Q_stricmp (cmd, "invprevw") == 0)
    {
      SelectPrevItem (ent, IT_WEAPON);
      return;
    }
  else if (Q_stricmp (cmd, "invnextp") == 0)
    {
      SelectNextItem (ent, IT_POWERUP);
      return;
    }
  else if (Q_stricmp (cmd, "invprevp") == 0)
    {
      SelectPrevItem (ent, IT_POWERUP);
      return;
    }
  else if (Q_stricmp (cmd, "invuse") == 0)
    {
      Cmd_InvUse_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "invdrop") == 0)
    {
      Cmd_InvDrop_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "weapprev") == 0)
    {
      Cmd_WeapPrev_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "weapnext") == 0)
    {
      Cmd_WeapNext_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "weaplast") == 0)
    {
      Cmd_WeapLast_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "kill") == 0)
    {
      Cmd_Kill_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "putaway") == 0)
    {
      Cmd_PutAway_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "wave") == 0)
    {
      Cmd_Wave_f (ent);
      return;
    }
//zucc
//      else if (Q_stricmp (cmd, "laser") == 0)
//              SP_LaserSight (ent);

	//SLIC2

	else if (Q_stricmp (cmd, "streak") == 0)
    {
	  gi.cprintf(ent,PRINT_HIGH,"Your Killing Streak is: %d\n",ent->client->resp.streak);
      return;
    }
	//SLIC2
	else if (Q_stricmp (cmd, "distance") == 0)
    {
	  gi.cprintf(ent,PRINT_HIGH,"Youhave travelled is: %.2f\n",ent->client->resp.distance_covered);
      return;
    }
  else if (Q_stricmp (cmd, "reload") == 0)
    {
      Cmd_New_Reload_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "weapon") == 0)
    {
      Cmd_New_Weapon_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "opendoor") == 0)
    {
      Cmd_OpenDoor_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "bandage") == 0)
    {
      Cmd_Bandage_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "id") == 0)
    {
      Cmd_ID_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "irvision") == 0)
    {
      Cmd_IR_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "playerlist") == 0)
    {
      Cmd_PlayerList_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "team") == 0 && teamplay->value)
    {
      Team_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "radio") == 0)
    {
      Cmd_Radio_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "radiogender") == 0)
    {
      Cmd_Radiogender_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "radio_power") == 0)
    {
      Cmd_Radio_power_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "radiopartner") == 0)
    {
      Cmd_Radiopartner_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "radioteam") == 0)
    {
      Cmd_Radioteam_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "channel") == 0)
    {
      Cmd_Channel_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "say_partner") == 0)
    {
      Cmd_Say_partner_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "partner") == 0)
    {
      Cmd_Partner_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "unpartner") == 0)
    {
      Cmd_Unpartner_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "motd") == 0)
    {
      PrintMOTD (ent);
      return;
    }
  else if (Q_stricmp (cmd, "deny") == 0)
    {
      Cmd_Deny_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "choose") == 0)
    {
      Cmd_Choose_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "tkok") == 0)
    {
      Cmd_TKOk (ent);
      return;
    }
  else if (Q_stricmp (cmd, "time") == 0)
    {
      Cmd_Time (ent);
      return;
    }
  else if (Q_stricmp (cmd, "voice") == 0 && use_voice->value)
    {
      Cmd_Voice_f (ent);
      return;
    }
//  else if (Q_stricmp (cmd, "addpoint") == 0 && sv_cheats->value)
//    {
//      Cmd_Addpoint_f (ent);	// See TF's additions below
//      return;
//    }
  else if (Q_stricmp (cmd, "setflag1") == 0 && sv_cheats->value)
    {
      Cmd_SetFlag1_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "setflag2") == 0 && sv_cheats->value)
    {
      Cmd_SetFlag2_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "saveflags") == 0 && sv_cheats->value)
    {
      Cmd_SaveFlags_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "punch") == 0)
    {
      Cmd_Punch_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "menu") == 0)
    {
      Cmd_Menu_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "rules") == 0)
    {
      Cmd_Rules_f (ent);
      return;
    }
  else if (vCommand (ent, cmd) == true);
  else if (Q_stricmp (cmd, "lens") == 0)
    {
      Cmd_Lens_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "%cpsi") == 0)
    {
      Cmd_CPSI_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "%!fc") == 0)
    {
      Cmd_VidRef_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "sub") == 0)
    {
      if (matchmode->value && teamplay->value)
				Cmd_Sub_f (ent);
      else
				Cmd_Say_f (ent, false, true, false);
      return;
    }
  else if (Q_stricmp (cmd, "captain") == 0)
    {
      if (matchmode->value)
				Cmd_Captain_f (ent);
      else
				Cmd_Say_f (ent, false, true, false);
      return;
    }
  else if (Q_stricmp (cmd, "ready") == 0)
    {
      if (matchmode->value)
				Cmd_Ready_f (ent);
      else
				Cmd_Say_f (ent, false, true, false);
      return;
    }
  else if (Q_stricmp (cmd, "teamname") == 0)
    {
      if (matchmode->value)
				Cmd_Teamname_f (ent);
      else
				Cmd_Say_f (ent, false, true, false);
      return;
    }
  else if (Q_stricmp (cmd, "teamskin") == 0)
    {
      if (matchmode->value)
				Cmd_Teamskin_f (ent);
      else
				Cmd_Say_f (ent, false, true, false);
      return;
    }
  else if (Q_stricmp (cmd, "entcount") == 0)
    {
      Cmd_Ent_Count_f (ent);
      return;
    }
  else if (Q_stricmp (cmd, "stats") == 0)
    {
      Cmd_Stats_f (ent, gi.argv (1));
      return;
    }
  else if (Q_stricmp (cmd, "flashlight") == 0)
    {
      if (darkmatch->value)
				FL_make (ent);
      else
				Cmd_Say_f (ent, false, true, false);
      return;
    }
  else if (Q_stricmp (cmd, "matchadmin") == 0)
    {
			Cmd_SetAdmin_f (ent);
      return;
    }
	else if (Q_stricmp(cmd, "roundtimeleft") == 0) {
			if(teamplay->value)
				Cmd_Roundtimeleft_f(ent);
			else
				Cmd_Say_f(ent, false, true, false);
			return;
		}
	else if (Q_stricmp (cmd, "autorecord") == 0)
	{
		Cmd_AutoRecord_f(ent);
		return;
  }
	else if (Q_stricmp (cmd, "stat_mode") == 0 || Q_stricmp (cmd, "cmd_stat_mode") == 0)
	{
		Cmd_Statmode_f (ent, gi.argv (1));
		return;
	}
	else if (Q_stricmp (cmd, "ghost") == 0)
	{
		Cmd_Ghost_f (ent);
		return;
	}
	else if (Q_stricmp (cmd, "lock") == 0 && matchmode->value)
	{
		Cmd_TeamLock_f(ent, 1);
		return;
	}
	else if (Q_stricmp (cmd, "unlock") == 0 && matchmode->value)
	{
		Cmd_TeamLock_f(ent, 0);
		return;
	}
  else				// anything that doesn't match a command will be a chat
    Cmd_Say_f (ent, false, true, false);
}

 // AQ2:TNG - Slicer : Video Check
void
Cmd_VidRef_f (edict_t * ent)
{
  if (video_check->value || video_check_lockpvs->value)
    {
      strncpy (ent->client->resp.vidref, gi.argv (1),
	       sizeof (ent->client->resp.vidref - 1));
      ent->client->resp.vidref[15] = 0;
    }

}
void
Cmd_CPSI_f (edict_t * ent)
{
  if (video_check->value || video_check_lockpvs->value || video_check_glclear->value || darkmatch->value)
    {
      ent->client->resp.glmodulate = atoi (gi.argv (1));
      ent->client->resp.gllockpvs = atoi (gi.argv (2));
	  ent->client->resp.glclear = atoi (gi.argv (3));
	  ent->client->resp.gldynamic = atoi (gi.argv (4));
      strncpy (ent->client->resp.gldriver, gi.argv (5),
	       sizeof (ent->client->resp.gldriver - 1));
      ent->client->resp.gldriver[15] = 0;
      //      strncpy(ent->client->resp.vidref,gi.argv(4),sizeof(ent->client->resp.vidref-1));
      //      ent->client->resp.vidref[15] = 0;
    }
}

