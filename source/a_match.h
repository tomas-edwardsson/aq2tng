void Cmd_Captain_f(edict_t *ent);
void Cmd_Ready_f(edict_t *ent);
void Cmd_Sub_f(edict_t *ent);
void Cmd_Teamname_f(edict_t *ent);
void Cmd_Teamskin_f(edict_t *ent);
void SendWorldMsg(char *s,int sound,int center);
int CheckForCaptains(int cteam);
extern int team1ready;
extern int team2ready;
extern float matchtime;