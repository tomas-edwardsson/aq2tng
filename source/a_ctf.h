//-----------------------------------------------------------------------------
// CTF related definitions
//
// $Id: a_ctf.h,v 1.10 2001/09/28 13:48:34 ra Exp $
//
//-----------------------------------------------------------------------------
// $Log: a_ctf.h,v $
// Revision 1.10  2001/09/28 13:48:34  ra
// I ran indent over the sources. All .c and .h files reindented.
//
// Revision 1.9  2001/08/08 12:42:22  slicerdw
// Ctf Should finnaly be fixed now, lets hope so
//
// Revision 1.8  2001/06/26 18:47:30  igor_rock
// added ctf_respawn cvar
//
// Revision 1.7  2001/06/15 14:18:07  igor_rock
// corrected bug with destroyed flags (won't be destroyed anymore, instead they
// return to the base).
//
// Revision 1.6  2001/06/13 07:55:17  igor_rock
// Re-Added a_match.h and a_match.c
// Added CTF Header for a_ctf.h and a_ctf.c
//
//-----------------------------------------------------------------------------

typedef enum
{
  CTF_STATE_START,
  CTF_STATE_PLAYING
}
ctfstate_t;

extern cvar_t *ctf;
extern cvar_t *ctf_forcejoin;
extern cvar_t *ctf_mode;
extern cvar_t *ctf_dropflag;
extern cvar_t *ctf_respawn;

#define CTF_TEAM1_SKIN "ctf_r"
#define CTF_TEAM2_SKIN "ctf_b"

#define DF_CTF_FORCEJOIN	131072

#define CTF_CAPTURE_BONUS		15	// what you get for capture
#define CTF_TEAM_BONUS			10	// what your team gets for capture
#define CTF_RECOVERY_BONUS		1	// what you get for recovery
#define CTF_FLAG_BONUS			0	// what you get for picking up enemy flag
#define CTF_FRAG_CARRIER_BONUS	2	// what you get for fragging enemy flag carrier
#define CTF_FLAG_RETURN_TIME	40	// seconds until auto return

#define CTF_CARRIER_DANGER_PROTECT_BONUS	2	// bonus for fraggin someone who has recently hurt your flag carrier
#define CTF_CARRIER_PROTECT_BONUS		1	// bonus for fraggin someone while either you or your target are near your flag carrier
#define CTF_FLAG_DEFENSE_BONUS			1	// bonus for fraggin someone while either you or your target are near your flag
#define CTF_RETURN_FLAG_ASSIST_BONUS		1	// awarded for returning a flag that causes a capture to happen almost immediately
#define CTF_FRAG_CARRIER_ASSIST_BONUS		2	// award for fragging a flag carrier if a capture happens almost immediately

#define CTF_TARGET_PROTECT_RADIUS		400	// the radius around an object being defended where a target will be worth extra frags
#define CTF_ATTACKER_PROTECT_RADIUS		400	// the radius around an object being defended where an attacker will get extra frags when making kills

#define CTF_CARRIER_DANGER_PROTECT_TIMEOUT	8
#define CTF_FRAG_CARRIER_ASSIST_TIMEOUT		10
#define CTF_RETURN_FLAG_ASSIST_TIMEOUT		10

#define CTF_AUTO_FLAG_RETURN_TIMEOUT		30	// number of seconds before dropped flag auto-returns

void CTFInit (void);

void SP_info_player_team1 (edict_t * self);
void SP_info_player_team2 (edict_t * self);

char *CTFTeamName (int team);
char *CTFOtherTeamName (int team);
void CTFAssignTeam (gclient_t * who);
edict_t *SelectCTFSpawnPoint (edict_t * ent);
qboolean CTFPickup_Flag (edict_t * ent, edict_t * other);
void CTFDrop_Flag (edict_t * ent, gitem_t * item);
void CTFEffects (edict_t * player);
void CTFCalcScores (void);
void SetCTFStats (edict_t * ent);
void CTFDeadDropFlag (edict_t * self);
void CTFScoreboardMessage (edict_t * ent, edict_t * killer);
void CTFFlagSetup (edict_t * ent);
void CTFResetFlag (int team);
void CTFFragBonuses (edict_t * targ, edict_t * inflictor, edict_t * attacker);
void CTFCheckHurtCarrier (edict_t * targ, edict_t * attacker);
void CTFDestroyFlag (edict_t * self);

void CTFOpenJoinMenu (edict_t * ent);
qboolean CTFStartClient (edict_t * ent);

qboolean CTFCheckRules (void);

void SP_misc_ctf_banner (edict_t * ent);
void SP_misc_ctf_small_banner (edict_t * ent);

extern char *ctf_statusbar;

void SP_trigger_teleport (edict_t * ent);
void SP_info_teleport_destination (edict_t * ent);

void ResetPlayers ();
