// SONIC ROBO BLAST 2
//-----------------------------------------------------------------------------
// Copyright (C) 1998-2000 by DooM Legacy Team.
// Copyright (C) 1999-2018 by Sonic Team Junior.
//
// This program is free software distributed under the
// terms of the GNU General Public License, version 2.
// See the 'LICENSE' file for more details.
//-----------------------------------------------------------------------------
/// \file  d_netcmd.h
/// \brief host/client network commands
///        commands are executed through the command buffer
///        like console commands

#ifndef __D_NETCMD__
#define __D_NETCMD__

#include "command.h"

// console vars
extern consvar_t cv_playername;
extern consvar_t cv_playercolor;
extern consvar_t cv_skin;
extern consvar_t cv_localskin;
// secondary splitscreen player
extern consvar_t cv_playername2;
extern consvar_t cv_playercolor2;
extern consvar_t cv_skin2;
// third splitscreen player
extern consvar_t cv_playername3;
extern consvar_t cv_playercolor3;
extern consvar_t cv_skin3;
// fourth splitscreen player
extern consvar_t cv_playername4;
extern consvar_t cv_playercolor4;
extern consvar_t cv_skin4;
// preferred number of players
extern consvar_t cv_splitplayers;

#ifdef SEENAMES
extern consvar_t cv_seenames, cv_allowseenames;
#endif
extern consvar_t cv_usemouse;
//WTF
extern consvar_t cv_mouseturn;
extern consvar_t cv_usejoystick;
extern consvar_t cv_usejoystick2;
extern consvar_t cv_usejoystick3;
extern consvar_t cv_usejoystick4;
#ifdef LJOYSTICK
extern consvar_t cv_joyport;
extern consvar_t cv_joyport2;
#endif
extern consvar_t cv_joyscale;
extern consvar_t cv_joyscale2;
extern consvar_t cv_joyscale3;
extern consvar_t cv_joyscale4;

// splitscreen with second mouse
extern consvar_t cv_mouse2port;
extern consvar_t cv_usemouse2;
#if defined (__unix__) || defined (__APPLE__) || defined (UNIXCOMMON)
extern consvar_t cv_mouse2opt;
#endif

// normally in p_mobj but the .h is not read
extern consvar_t cv_itemrespawntime;
extern consvar_t cv_itemrespawn;

extern consvar_t cv_flagtime;
extern consvar_t cv_suddendeath;

extern consvar_t cv_touchtag;
extern consvar_t cv_hidetime;

extern consvar_t cv_friendlyfire;
extern consvar_t cv_pointlimit;
extern consvar_t cv_timelimit;
extern consvar_t cv_numlaps;
extern consvar_t cv_basenumlaps;
extern UINT32 timelimitintics;
extern consvar_t cv_allowexitlevel;

extern consvar_t cv_autobalance;
extern consvar_t cv_teamscramble;
extern consvar_t cv_scrambleonchange;

extern consvar_t cv_netstat;
#ifdef WALLSPLATS
extern consvar_t cv_splats;
#endif

extern consvar_t cv_countdowntime;
extern consvar_t cv_runscripts;
extern consvar_t cv_mute;
extern consvar_t cv_killingdead;
extern consvar_t cv_pause;

extern consvar_t cv_restrictskinchange, cv_allowteamchange, cv_ingamecap, cv_respawntime;
extern consvar_t cv_spectatorreentry, cv_antigrief;

/*extern consvar_t cv_teleporters, cv_superring, cv_supersneakers, cv_invincibility;
extern consvar_t cv_jumpshield, cv_watershield, cv_ringshield, cv_forceshield, cv_bombshield;
extern consvar_t cv_1up, cv_eggmanbox;
extern consvar_t cv_recycler;*/

// SRB2kart items
extern consvar_t cv_sneaker, cv_rocketsneaker, cv_invincibility, cv_banana;
extern consvar_t cv_eggmanmonitor, cv_orbinaut, cv_jawz, cv_mine;
extern consvar_t cv_ballhog, cv_selfpropelledbomb, cv_grow, cv_shrink;
extern consvar_t cv_thundershield, cv_hyudoro, cv_pogospring, cv_kitchensink;

extern consvar_t cv_triplesneaker, cv_triplebanana, cv_decabanana;
extern consvar_t cv_tripleorbinaut, cv_quadorbinaut, cv_dualjawz;

//Lastserver
extern consvar_t cv_lastserver;

extern consvar_t cv_kartminimap;
extern consvar_t cv_kartcheck;
extern consvar_t cv_kartinvinsfx;
extern consvar_t cv_kartspeed;
extern consvar_t cv_kartbumpers;
extern consvar_t cv_kartfrantic;
extern consvar_t cv_kartcomeback;
extern consvar_t cv_kartencore;
extern consvar_t cv_kartvoterulechanges;
extern consvar_t cv_kartgametypepreference;
extern consvar_t cv_kartspeedometer;
extern consvar_t cv_kartvoices;

extern consvar_t cv_karteliminatelast;

extern consvar_t cv_votetime;
extern consvar_t cv_votemaxrows;

extern consvar_t cv_kartdebugitem, cv_kartdebugamount, cv_kartdebugshrink, cv_kartdebugdistribution, cv_kartdebughuddrop;
extern consvar_t cv_kartdebugcheckpoint, cv_kartdebugnodes, cv_kartdebugcolorize;

extern consvar_t cv_itemfinder;

extern consvar_t cv_inttime, cv_advancemap, cv_playersforexit;
extern consvar_t cv_soniccd;
extern consvar_t cv_match_scoring;
extern consvar_t cv_overtime;
extern consvar_t cv_startinglives;

// for F_finale.c
extern consvar_t cv_rollingdemos;

extern consvar_t cv_ringslinger, cv_soundtest;

extern consvar_t cv_specialrings, cv_powerstones, cv_matchboxes, cv_competitionboxes;

extern consvar_t cv_maxping;
extern consvar_t cv_pingtimeout;
extern consvar_t cv_showping;
extern consvar_t cv_pingmeasurement;
extern consvar_t cv_pingicon;
extern consvar_t cv_pingstyle;

//extern consvar_t cv_smallpos;
extern consvar_t cv_showminimapnames;
extern consvar_t cv_minihead;

extern consvar_t cv_showlapemblem; 

extern consvar_t cv_showviewpointtext;
extern consvar_t cv_luaimmersion;
extern consvar_t cv_fakelocalskin;
extern consvar_t cv_showlocalskinmenus;

extern consvar_t cv_skipmapcheck;

extern consvar_t cv_sleep;

extern consvar_t cv_nametag;
extern consvar_t cv_nametagtrans;
extern consvar_t cv_nametagfacerank;
extern consvar_t cv_nametagmaxplayers;
extern consvar_t cv_nametagmaxlenght;
//extern consvar_t cv_nametagscaling;
extern consvar_t cv_nametagdist;
extern consvar_t cv_showownnametag;
extern consvar_t cv_smallnametags;
extern consvar_t cv_nametagrestat;
extern consvar_t cv_nametaghop;
extern consvar_t cv_nametagscore;
extern consvar_t cv_shownametagfinish;
extern consvar_t cv_shownametagspectator;

#define SKINSELECTSPIN_PAIN 25
extern consvar_t cv_skinselectspin;

extern consvar_t cv_driftgauge;
extern consvar_t cv_driftgaugeofs;
extern consvar_t cv_driftgaugetrans;
extern consvar_t cv_driftgaugestyle;

extern consvar_t cv_perfstats;
extern consvar_t cv_ps_thinkframe_page;
extern consvar_t cv_ps_samplesize;
extern consvar_t cv_ps_descriptor;

extern consvar_t cv_showtrackaddon;

extern consvar_t cv_showspecstuff;

typedef enum
{
	SKINMENUTYPE_SCROLL = 0,
	SKINMENUTYPE_2D,    // 1
	SKINMENUTYPE_GRID,   // 2
	SKINMENUTYPE_EXTENDED	// 3
} skinmenutype_t;

typedef enum
{
	SKINMENUSORT_REALNAME = 0,
	SKINMENUSORT_NAME, // 1
	SKINMENUSORT_SPEED, // 2
	SKINMENUSORT_WEIGHT, // 3
	SKINMENUSORT_PREFCOLOR, // 4
	SKINMENUSORT_ID, // 5
	MAXSKINMENUSORTS // 6
} skinmenusort_t;

extern consvar_t cv_skinselectmenu;
extern consvar_t cv_skinselectgridsort;

typedef enum
{
	XD_NAMEANDCOLOR = 1,
	XD_WEAPONPREF,  // 2
	XD_KICK,        // 3
	XD_NETVAR,      // 4
	XD_SAY,         // 5
	XD_MAP,         // 6
	XD_EXITLEVEL,   // 7
	XD_ADDFILE,     // 8
	XD_PAUSE,       // 9
	XD_ADDPLAYER,   // 10
	XD_TEAMCHANGE,  // 11
	XD_CLEARSCORES, // 12
	XD_LOGIN,       // 13
	XD_VERIFIED,    // 14
	XD_RANDOMSEED,  // 15
	XD_RUNSOC,      // 16
	XD_REQADDFILE,  // 17
	XD_DELFILE,     // 18
	XD_SETMOTD,     // 19
	XD_RESPAWN,     // 20
	XD_DEMOTED,     // 21
	XD_SETUPVOTE,   // 22
	XD_MODIFYVOTE,  // 23
	XD_PICKVOTE,    // 24
	XD_REMOVEPLAYER,// 25
	XD_DISCORD,     // 26
	XD_LUACMD,      // 27
	XD_LUAVAR,      // 28
	MAXNETXCMD
} netxcmd_t;

extern const char *netxcmdnames[MAXNETXCMD - 1];

#if defined(_MSC_VER)
#pragma pack(1)
#endif

#ifdef _MSC_VER
#pragma warning(disable :  4214)
#endif

//Packet composition for Command_TeamChange_f() ServerTeamChange, etc.
//bitwise structs make packing bits a little easier, but byte alignment harder?
//todo: decide whether to make the other netcommands conform, or just get rid of this experiment.
typedef struct {
	UINT32 playernum    : 5;  // value 0 to 31
	UINT32 newteam      : 5;  // value 0 to 31
	UINT32 verification : 1;  // value 0 to 1
	UINT32 autobalance  : 1;  // value 0 to 1
	UINT32 scrambled    : 1;  // value 0 to 1
} ATTRPACK changeteam_packet_t;

#ifdef _MSC_VER
#pragma warning(default : 4214)
#endif

typedef struct {
	UINT16 l; // liitle endian
	UINT16 b; // big enian
} ATTRPACK changeteam_value_t;

//Since we do not want other files/modules to know about this data buffer we union it here with a Short Int.
//Other files/modules will hand the INT16 back to us and we will decode it here.
//We don't have to use a union, but we would then send four bytes instead of two.
typedef union {
	changeteam_packet_t packet;
	changeteam_value_t value;
} ATTRPACK changeteam_union;

#if defined(_MSC_VER)
#pragma pack()
#endif

extern tic_t driftsparkGrowTimer[MAXPLAYERS];

// add game commands, needs cleanup
void D_RegisterServerCommands(void);
void D_RegisterClientCommands(void);
void D_SendPlayerConfig(void);
void Command_ExitGame_f(void);
void Command_Retry_f(void);
void D_GameTypeChanged(INT32 lastgametype); // not a real _OnChange function anymore
void D_MapChange(INT32 pmapnum, INT32 pgametype, boolean pencoremode, boolean presetplayers, INT32 pdelay, boolean pskipprecutscene, boolean pfromlevelselect);
void D_SetupVote(void);
void D_ModifyClientVote(SINT8 voted, UINT8 splitplayer);
void D_PickVote(void);
void ObjectPlace_OnChange(void);
boolean IsPlayerAdmin(INT32 playernum);
void SetAdminPlayer(INT32 playernum);
void ClearAdminPlayers(void);
void RemoveAdminPlayer(INT32 playernum);
void ItemFinder_OnChange(void);
void D_SetPassword(const char *pw);

// used for the player setup menu
UINT8 CanChangeSkin(INT32 playernum);

#endif
