// SONIC ROBO BLAST 2
//-----------------------------------------------------------------------------
// Copyright (C) 1993-1996 by id Software, Inc.
// Copyright (C) 1998-2000 by DooM Legacy Team.
// Copyright (C) 1999-2018 by Sonic Team Junior.
//
// This program is free software distributed under the
// terms of the GNU General Public License, version 2.
// See the 'LICENSE' file for more details.
//-----------------------------------------------------------------------------
/// \file  doomdef.h
/// \brief Internally used data structures for virtually everything,
///        key definitions, lots of other stuff.

#ifndef __DOOMDEF__
#define __DOOMDEF__

// Sound system select
// This should actually be in the makefile,
// but I can't stand that gibberish. D:
#define SOUND_DUMMY   0
#define SOUND_SDL     1
#define SOUND_MIXER   2
#define SOUND_FMOD    3

#ifndef SOUND
#ifdef HAVE_SDL

// Use Mixer interface?
#ifdef HAVE_MIXER
    #define SOUND SOUND_MIXER
    #ifdef HW3SOUND
    #undef HW3SOUND
    #endif
#endif

// Use generic SDL interface.
#ifndef SOUND
#define SOUND SOUND_SDL
#endif

#else // No SDL.

// Use FMOD?
#ifdef HAVE_FMOD
    #define SOUND SOUND_FMOD
    #ifdef HW3SOUND
    #undef HW3SOUND
    #endif
#else
    // No more interfaces. :(
    #define SOUND SOUND_DUMMY
#endif

#endif
#endif

#ifdef _WIN32
#define ASMCALL __cdecl
#else
#define ASMCALL
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4127 4152 4213 4514)
#ifdef _WIN64
#pragma warning(disable : 4306)
#endif
#endif
// warning level 4
// warning C4127: conditional expression is constant
// warning C4152: nonstandard extension, function/data pointer conversion in expression
// warning C4213: nonstandard extension used : cast on l-value


#include "doomtype.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _USE_MATH_DEFINES // fixes M_PI errors in r_plane.c for Visual Studio
#include <math.h>

#ifdef GETTEXT
#include <libintl.h>
#include <locale.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>

#ifdef _WIN32
#include <io.h>
#endif

//#define NOMD5

// Uncheck this to compile debugging code
//#define RANGECHECK
//#ifndef PARANOIA
//#define PARANOIA // do some tests that never fail but maybe
// turn this on by make etc.. DEBUGMODE = 1 or use the Debug profile in the VC++ projects
//#endif
#if defined (_WIN32) || defined (__unix__) || defined(__APPLE__) || defined (UNIXCOMMON) || defined (macintosh)
#define LOGMESSAGES // write message in log.txt
#endif

#ifdef LOGMESSAGES
extern FILE *logstream;
extern char  logfilename[1024];
#endif

/* A mod name to further distinguish versions. */
#define SRB2APPLICATION "SRB2Kart"

//#define DEVELOP // Disable this for release builds to remove excessive cheat commands and enable MD5 checking and stuff, all in one go. :3
#ifdef DEVELOP
#define VERSION    0 // Game version
#define SUBVERSION 0 // more precise version number
#define VERSIONSTRING "Development EXE"
#define VERSIONSTRINGW L"Development EXE"
// most interface strings are ignored in development mode.
// we use comprevision and compbranch instead.
#else
#define VERSION    1 // Game version
#define SUBVERSION 68 // more precise version number
#define VERSIONSTRING "Saturn v6.2 - 32p v1.4"
#define VERSIONSTRINGW L"Saturn v6.2 - 32p v1.4"
// Hey! If you change this, add 1 to the MODVERSION below! Otherwise we can't force updates!
// And change CMakeLists.txt (not src/, but in root), for CMake users!
// AND appveyor.yml, for the build bots!
#endif

// Maintain compatibility with 1.0.x record attack replays?
#define DEMO_COMPAT_100

// Does this version require an added patch file?
// Comment or uncomment this as necessary.
//#define USE_PATCH_DTA

// Kart has it's own, as well.
//#define USE_PATCH_KART

// Use .kart extension addons
#define USE_KART

// Modification options
// If you want to take advantage of the Master Server's ability to force clients to update
// to the latest version, fill these out.  Otherwise, just comment out UPDATE_ALERT and leave
// the other options the same.

// Comment out this line to completely disable update alerts (recommended for testing, but not for release)
#define UPDATE_ALERT

// The string used in the alert that pops up in the event of an update being available.
// Please change to apply to your modification (we don't want everyone asking where your mod is on SRB2.org!).
#define UPDATE_ALERT_STRING \
"A new update is available for SRB2Kart.\n"\
"Please visit kartkrew.org to download it.\n"\
"\n"\
"You are using version: %s\n"\
"The newest version is: %s\n"\
"\n"\
"(Press a key)\n"

// For future use, the codebase is the version of SRB2 that the modification is based on,
// and should not be changed unless you have merged changes between versions of SRB2
// (such as 2.0.4 to 2.0.5, etc) into your working copy.
// Will always resemble the versionstring, 205 = 2.0.5, 210 = 2.1, etc.
#define CODEBASE 210

// The Modification ID; must be obtained from Rob ( https://mb.srb2.org/private.php?do=newpm&u=546 ).
// DO NOT try to set this otherwise, or your modification will be unplayable through the Master Server.
// "12" is the default mod ID for version 2.1
// "17" is the 2.1 Kart's mod ID
#define MODID 17

// The Modification Version, starting from 1. Do not follow your version string for this,
// it's only for detection of the version the player is using so the MS can alert them of an update.
// Only set it higher, not lower, obviously.
// Note that we use this to help keep internal testing in check; this is why v2.1.0 is not version "1".
#define MODVERSION 10

// Filter consvars by version
// To version config.cfg, MAJOREXECVERSION is set equal to MODVERSION automatically.
// Increment MINOREXECVERSION whenever a config change is needed that does not correspond
// to an increment in MODVERSION. This might never happen in practice.
// If MODVERSION increases, set MINOREXECVERSION to 0.
#define MAJOREXECVERSION MODVERSION
#define MINOREXECVERSION 0
// (It would have been nice to use VERSION and SUBVERSION but those are zero'd out for DEVELOP builds)

// Macros
#define GETMAJOREXECVERSION(v) (v & 0xFFFF)
#define GETMINOREXECVERSION(v) (v >> 16)
#define GETEXECVERSION(major,minor) (major + (minor << 16))
#define EXECVERSION GETEXECVERSION(MAJOREXECVERSION, MINOREXECVERSION)

// =========================================================================

// The maximum number of players, multiplayer/networking.
// NOTE: it needs more than this to increase the number of players...

#define MAXPLAYERS 32
#define MAXSKINS 255
#define PLAYERSMASK (MAXPLAYERS-1)
#define MAXPLAYERNAME 21

// Master Server compatibility ONLY
#define MSCOMPAT_MAXPLAYERS (32)

typedef enum
{
	SKINCOLOR_NONE = 0,
	SKINCOLOR_WHITE,
	SKINCOLOR_SILVER,
	SKINCOLOR_GREY,
	SKINCOLOR_NICKEL,
	SKINCOLOR_BLACK,
	SKINCOLOR_SKUNK,
	SKINCOLOR_FAIRY,
	SKINCOLOR_POPCORN,
	SKINCOLOR_ARTICHOKE,
	SKINCOLOR_PIGEON,
	SKINCOLOR_SEPIA,
	SKINCOLOR_BEIGE,
	SKINCOLOR_WALNUT,
	SKINCOLOR_BROWN,
	SKINCOLOR_LEATHER,
	SKINCOLOR_SALMON,
	SKINCOLOR_PINK,
	SKINCOLOR_ROSE,
	SKINCOLOR_BRICK,
	SKINCOLOR_CINNAMON,
	SKINCOLOR_RUBY,
	SKINCOLOR_RASPBERRY,
	SKINCOLOR_CHERRY,
	SKINCOLOR_RED,
	SKINCOLOR_CRIMSON,
	SKINCOLOR_MAROON,
	SKINCOLOR_LEMONADE,
	SKINCOLOR_FLAME,
	SKINCOLOR_SCARLET,
	SKINCOLOR_KETCHUP,
	SKINCOLOR_DAWN,
	SKINCOLOR_SUNSET,
	SKINCOLOR_CREAMSICLE,
	SKINCOLOR_ORANGE,
	SKINCOLOR_PUMPKIN,
	SKINCOLOR_ROSEWOOD,
	SKINCOLOR_BURGUNDY,
	SKINCOLOR_TANGERINE,
	SKINCOLOR_PEACH,
	SKINCOLOR_CARAMEL,
	SKINCOLOR_CREAM,
	SKINCOLOR_GOLD,
	SKINCOLOR_ROYAL,
	SKINCOLOR_BRONZE,
	SKINCOLOR_COPPER,
	SKINCOLOR_QUARRY,
	SKINCOLOR_YELLOW,
	SKINCOLOR_MUSTARD,
	SKINCOLOR_CROCODILE,
	SKINCOLOR_OLIVE,
	SKINCOLOR_VOMIT,
	SKINCOLOR_GARDEN,
	SKINCOLOR_LIME,
	SKINCOLOR_HANDHELD,
	SKINCOLOR_TEA,
	SKINCOLOR_PISTACHIO,
	SKINCOLOR_MOSS,
	SKINCOLOR_CAMOUFLAGE,
	SKINCOLOR_ROBOHOOD,
	SKINCOLOR_MINT,
	SKINCOLOR_GREEN,
	SKINCOLOR_PINETREE,
	SKINCOLOR_EMERALD,
	SKINCOLOR_SWAMP,
	SKINCOLOR_DREAM,
	SKINCOLOR_PLAGUE,
	SKINCOLOR_ALGAE,
	SKINCOLOR_CARIBBEAN,
	SKINCOLOR_AZURE,
	SKINCOLOR_AQUA,
	SKINCOLOR_TEAL,
	SKINCOLOR_CYAN,
	SKINCOLOR_JAWZ, // Oni's torment
	SKINCOLOR_CERULEAN,
	SKINCOLOR_NAVY,
	SKINCOLOR_PLATINUM,
	SKINCOLOR_SLATE,
	SKINCOLOR_STEEL,
	SKINCOLOR_THUNDER,
	SKINCOLOR_RUST,
	SKINCOLOR_WRISTWATCH,
	SKINCOLOR_JET,
	SKINCOLOR_SAPPHIRE, // sweet mother, i cannot weave - slender aphrodite has overcome me with longing for a girl
	SKINCOLOR_PERIWINKLE,
	SKINCOLOR_BLUE,
	SKINCOLOR_BLUEBERRY,
	SKINCOLOR_NOVA,
	SKINCOLOR_PASTEL,
	SKINCOLOR_MOONSLAM,
	SKINCOLOR_ULTRAVIOLET,
	SKINCOLOR_DUSK,
	SKINCOLOR_BUBBLEGUM,
	SKINCOLOR_PURPLE,
	SKINCOLOR_FUCHSIA,
	SKINCOLOR_TOXIC,
	SKINCOLOR_MAUVE,
	SKINCOLOR_LAVENDER,
	SKINCOLOR_BYZANTIUM,
	SKINCOLOR_POMEGRANATE,
	SKINCOLOR_LILAC,
	SKINCOLOR_BONE, // vanilla colors - shoutouts to Sonic Team Jr. 
	SKINCOLOR_CARBON,
	SKINCOLOR_INK,
	SKINCOLOR_GHOST,
	SKINCOLOR_MARBLE,
	SKINCOLOR_BLUEBELL,
	SKINCOLOR_CHOCOLATE,
	SKINCOLOR_TAN,
	SKINCOLOR_PEACHY,
	SKINCOLOR_QUAIL,
	SKINCOLOR_LANTERN,
	SKINCOLOR_APRICOT,
	SKINCOLOR_SANDY,
	SKINCOLOR_BANANA,
	SKINCOLOR_SUNFLOWER,
	SKINCOLOR_PERIDOT,
	SKINCOLOR_APPLE,
	SKINCOLOR_SEAFOAM,
	SKINCOLOR_FOREST,
	SKINCOLOR_FROST,
	SKINCOLOR_WAVE,
	SKINCOLOR_ICY,
	SKINCOLOR_PEACOCK,
	SKINCOLOR_VAPOR,
	SKINCOLOR_GEMSTONE,
	SKINCOLOR_NEON,
	SKINCOLOR_PLUM,
	SKINCOLOR_VIOLET,
	SKINCOLOR_MAGENTA,
	SKINCOLOR_THISTLE,
	SKINCOLOR_DIAMOND, // custom color expansion begins here
	SKINCOLOR_RAVEN,
	SKINCOLOR_MUD,
	SKINCOLOR_EARTHWORM,
	SKINCOLOR_YOGURT,
	SKINCOLOR_PEARL,
	SKINCOLOR_STRAWBERRY,
	SKINCOLOR_SODA,
	SKINCOLOR_BLOODCELL,
	SKINCOLOR_MAHOGANY,
	SKINCOLOR_FIERY,
	SKINCOLOR_SPICE,
	SKINCOLOR_KING,
	SKINCOLOR_HOTDOG,
	SKINCOLOR_CARNATION,
	SKINCOLOR_CANDY,
	SKINCOLOR_NEBULA,
	SKINCOLOR_STEAMPUNK,
	SKINCOLOR_AMBER,
	SKINCOLOR_CARROT,
	SKINCOLOR_CHEESE,
	SKINCOLOR_DUNE,
	SKINCOLOR_BRASS,
	SKINCOLOR_LEMON,
	SKINCOLOR_CASKET,
	SKINCOLOR_KHAKI,
	SKINCOLOR_LIGHT,
	SKINCOLOR_PEPPERMINT,
	SKINCOLOR_LASER,
	SKINCOLOR_ASPARAGUS,
	SKINCOLOR_ARMY,
	SKINCOLOR_CROW,
	SKINCOLOR_CHARTEUSE,
	SKINCOLOR_SLIME,
	SKINCOLOR_LEAF,
	SKINCOLOR_JUNGLE,
	SKINCOLOR_EVERGREEN,
	SKINCOLOR_TROPIC,
	SKINCOLOR_IGUANA,
	SKINCOLOR_SPEARMINT,
	SKINCOLOR_PATINA,
	SKINCOLOR_LAKESIDE,
	SKINCOLOR_ELECTRIC,
	SKINCOLOR_TURQUOISE,
	SKINCOLOR_PEGASUS,
	SKINCOLOR_PLASMA,
	SKINCOLOR_COMET,
	SKINCOLOR_LIGHTNING,
	SKINCOLOR_VACATION,
	SKINCOLOR_ULTRAMARINE,
	SKINCOLOR_DEPTHS,
	SKINCOLOR_DIANNE,
	SKINCOLOR_EXOTIC,
	SKINCOLOR_SNOW,
	SKINCOLOR_MOON,
	SKINCOLOR_LUNAR,
	SKINCOLOR_ONYX,
	SKINCOLOR_LAPIS,
	SKINCOLOR_ORCA,
	SKINCOLOR_STORM,
	SKINCOLOR_MIDNIGHT,
	SKINCOLOR_COTTONCANDY, // this color was a pain to get right
	SKINCOLOR_CYBER, // this one too
	SKINCOLOR_AMETHYST,
	SKINCOLOR_IRIS,
	SKINCOLOR_GOTHIC,
	SKINCOLOR_GRAPE,
	SKINCOLOR_INDIGO,
	SKINCOLOR_SAKURA,
	SKINCOLOR_DISCO,
	SKINCOLOR_MULBERRY,
	SKINCOLOR_BOYSENBERRY,
	SKINCOLOR_MYSTIC,
	SKINCOLOR_WICKED,


	// "Careful! MAXSKINCOLORS cannot be greater than 0x40 -- Which it is now."
	// (This comment is a dirty liar! This is only limited by the integer type, so 255 for UINT8.)
	MAXSKINCOLORS,

	// Super special awesome Super flashing colors!
	// Super Sonic Yellow
	SKINCOLOR_SUPER1 = MAXSKINCOLORS,
	SKINCOLOR_SUPER2,
	SKINCOLOR_SUPER3,
	SKINCOLOR_SUPER4,
	SKINCOLOR_SUPER5,

	// Super Tails Orange
	SKINCOLOR_TSUPER1,
	SKINCOLOR_TSUPER2,
	SKINCOLOR_TSUPER3,
	SKINCOLOR_TSUPER4,
	SKINCOLOR_TSUPER5,

	// Super Knuckles Red
	SKINCOLOR_KSUPER1,
	SKINCOLOR_KSUPER2,
	SKINCOLOR_KSUPER3,
	SKINCOLOR_KSUPER4,
	SKINCOLOR_KSUPER5,

	// Hyper Sonic Pink
	SKINCOLOR_PSUPER1,
	SKINCOLOR_PSUPER2,
	SKINCOLOR_PSUPER3,
	SKINCOLOR_PSUPER4,
	SKINCOLOR_PSUPER5,

	// Hyper Sonic Blue
	SKINCOLOR_BSUPER1,
	SKINCOLOR_BSUPER2,
	SKINCOLOR_BSUPER3,
	SKINCOLOR_BSUPER4,
	SKINCOLOR_BSUPER5,

	// Aqua Super
	SKINCOLOR_ASUPER1,
	SKINCOLOR_ASUPER2,
	SKINCOLOR_ASUPER3,
	SKINCOLOR_ASUPER4,
	SKINCOLOR_ASUPER5,

	// Hyper Sonic Green
	SKINCOLOR_GSUPER1,
	SKINCOLOR_GSUPER2,
	SKINCOLOR_GSUPER3,
	SKINCOLOR_GSUPER4,
	SKINCOLOR_GSUPER5,

	// Hyper Sonic White
	SKINCOLOR_WSUPER1,
	SKINCOLOR_WSUPER2,
	SKINCOLOR_WSUPER3,
	SKINCOLOR_WSUPER4,
	SKINCOLOR_WSUPER5,

	// Creamy Super (Shadow?)
	SKINCOLOR_CSUPER1,
	SKINCOLOR_CSUPER2,
	SKINCOLOR_CSUPER3,
	SKINCOLOR_CSUPER4,
	SKINCOLOR_CSUPER5,

	MAXTRANSLATIONS
} skincolors_t;

// State updates, number of tics / second.
// NOTE: used to setup the timer rate, see I_StartupTimer().
#define TICRATE 35
#define NEWTICRATERATIO 1 // try 4 for 140 fps :)
#define NEWTICRATE (TICRATE*NEWTICRATERATIO)

#define MUSICRATE 1000 // sound timing is calculated by milliseconds

#define RING_DIST 1280*FRACUNIT // how close you need to be to a ring to attract it

#define PUSHACCEL (2*FRACUNIT) // Acceleration for MF2_SLIDEPUSH items.

// Special linedef executor tag numbers!
enum {
	LE_PINCHPHASE      = -2, // A boss entered pinch phase (and, in most cases, is preparing their pinch phase attack!)
	LE_ALLBOSSESDEAD   = -3, // All bosses in the map are dead (Egg capsule raise)
	LE_BOSSDEAD        = -4, // A boss in the map died (Chaos mode boss tally)
	LE_BOSS4DROP       = -5,  // CEZ boss dropped its cage
	LE_BRAKVILEATACK   = -6  // Brak's doing his LOS attack, oh noes
};

// Name of local directory for config files and savegames
#if (defined (__unix__) || defined (UNIXCOMMON)) && !defined (__CYGWIN__) && !defined (__APPLE__)
#define DEFAULTDIR ".srb2kart"
#else
#define DEFAULTDIR "srb2kart"
#endif

#include "g_state.h"

// commonly used routines - moved here for include convenience

/**	\brief	The I_Error function

	\param	error	the error message

	\return	void


*/
void I_Error(const char *error, ...) FUNCIERROR;

/**	\brief	write a message to stderr (use before I_Quit) for when you need to quit with a msg, but need
 the return code 0 of I_Quit();

	\param	error	message string

	\return	void
*/
void I_OutputMsg(const char *error, ...) FUNCPRINTF;

// console.h
typedef enum
{
	CONS_NOTICE,
	CONS_WARNING,
	CONS_ERROR
} alerttype_t;

void CONS_Printf(const char *fmt, ...) FUNCPRINTF;
void CONS_Alert(alerttype_t level, const char *fmt, ...) FUNCDEBUG;
void CONS_Debug(INT32 debugflags, const char *fmt, ...) FUNCDEBUG;

// For help debugging functions.
#define POTENTIALLYUNUSED CONS_Alert(CONS_WARNING, "(%s:%d) Unused code appears to be used.\n", __FILE__, __LINE__)

#include "m_swap.h"

// Things that used to be in dstrings.h
#define SAVEGAMENAME "srb2sav"
extern char savegamename[256];

// m_misc.h
#ifdef GETTEXT
#define M_GetText(String) gettext(String)
void M_StartupLocale(void);
#else
// If no translations are to be used, make a stub
// M_GetText function that just returns the string.
#define M_GetText(x) (x)
#endif
void *M_Memcpy(void* dest, const void* src, size_t n);
char *va(const char *format, ...) FUNCPRINTF;
char *M_GetToken(const char *inputString);
char *sizeu1(size_t num);
char *sizeu2(size_t num);
char *sizeu3(size_t num);
char *sizeu4(size_t num);
char *sizeu5(size_t num);

// d_main.c
extern boolean devparm; // development mode (-debug)
// d_netcmd.c
extern INT32 cv_debug;

#define DBG_BASIC       0x0001
#define DBG_DETAILED    0x0002
#define DBG_RANDOMIZER  0x0004
#define DBG_RENDER      0x0008
#define DBG_NIGHTSBASIC 0x0010
#define DBG_NIGHTS      0x0020
#define DBG_POLYOBJ     0x0040
#define DBG_GAMELOGIC   0x0080
#define DBG_NETPLAY     0x0100
#define DBG_MEMORY      0x0200
#define DBG_SETUP       0x0400
#define DBG_LUA         0x0800
#define DBG_VIEWMORPH   0x2000

// =======================
// Misc stuff for later...
// =======================

#define ANG2RAD(angle) ((float)((angle)*M_PI)/ANGLE_180)

// Modifier key variables, accessible anywhere
extern UINT8 shiftdown, ctrldown, altdown;
extern boolean capslock;

// WARNING: a should be unsigned but to add with 2048, it isn't!
#define AIMINGTODY(a) FixedDiv((FINETANGENT((2048+(((INT32)a)>>ANGLETOFINESHIFT)) & FINEMASK)*160), fovtan)

// if we ever make our alloc stuff...
#define ZZ_Alloc(x) Z_Malloc(x, PU_STATIC, NULL)
#define ZZ_Calloc(x) Z_Calloc(x, PU_STATIC, NULL)

// i_system.c, replace getchar() once the keyboard has been appropriated
INT32 I_GetKey(void);

#ifndef min // Double-Check with WATTCP-32's cdefs.h
#define min(x, y) (((x) < (y)) ? (x) : (y))
#endif
#ifndef max // Double-Check with WATTCP-32's cdefs.h
#define max(x, y) (((x) > (y)) ? (x) : (y))
#endif

#ifndef M_PIl
#define M_PIl 3.1415926535897932384626433832795029L
#endif

// Floating point comparison epsilons from float.h
#ifndef FLT_EPSILON
#define FLT_EPSILON 1.1920928955078125e-7f
#endif

#ifndef DBL_EPSILON
#define DBL_EPSILON 2.2204460492503131e-16l
#endif

// An assert-type mechanism.
#ifdef PARANOIA
#define I_Assert(e) ((e) ? (void)0 : I_Error("assert failed: %s, file %s, line %d", #e, __FILE__, __LINE__))
#else
#define I_Assert(e) ((void)0)
#endif

// The character that separates pathnames. Forward slash on
// most systems, but reverse solidus (\) on Windows.
#if defined (_WIN32)
	#define PATHSEP "\\"
#else
	#define PATHSEP "/"
#endif

// Compile date and time and revision.
extern const char *compdate, *comptime, *comprevision, *compbranch;

// Disabled code and code under testing
// None of these that are disabled in the normal build are guaranteed to work perfectly
// Compile them at your own risk!

/// Undefine to use the new method of Gamma correction see colour cube in v_video.c
#define BACKWARDSCOMPATCORRECTION

///	Allows the use of devmode in multiplayer. AKA "fishcake"
//#define NETGAME_DEVMODE

///	Allows gravity changes in netgames, no questions asked.
//#define NETGAME_GRAVITY

///	Dumps the contents of a network save game upon consistency failure for debugging.
//#define DUMPCONSISTENCY

///	See name of player in your crosshair
#define SEENAMES

///	Allow loading of savegames between different versions of the game.
///	\note	XMOD port.
///	    	Most modifications should probably enable this.
//#define SAVEGAME_OTHERVERSIONS

///	Allow the use of the SOC RESETINFO command.
///	\note	Builds that are tight on memory should disable this.
///	    	This stops the game from storing backups of the states, sprites, and mobjinfo tables.
///	    	Though this info is compressed under normal circumstances, it's still a lot of extra
///	    	memory that never gets touched.
#define ALLOW_RESETDATA

#ifndef NONET
///	Display a connection screen on join attempts.
#define CLIENT_LOADINGSCREEN
#endif

/// Backwards compatibility with musicslots.
/// \note	You should leave this enabled unless you're working with a future SRB2 version.
#define MUSICSLOT_COMPATIBILITY

/// Handle touching sector specials in P_PlayerAfterThink instead of P_PlayerThink.
/// \note   Required for proper collision with moving sloped surfaces that have sector specials on them.
#define SECTORSPECIALSAFTERTHINK

/// SRB2Kart: Camera always has noclip.
#define NOCLIPCAM

/// SRB2Kart: MIDI support is shitty and busted and we don't want it, lets throw it behind a define
#define NO_MIDI

/// Sprite rotation
#define ROTSPRITE
#define ROTANGLES 72 // Needs to be a divisor of 360 (45, 60, 90, 120...)
#define ROTANGDIFF (360 / ROTANGLES)

#if defined (HAVE_CURL) && ! defined (NONET)
#define MASTERSERVER
#define HOLEPUNCH
#else
#undef UPDATE_ALERT
#endif

#endif // __DOOMDEF__
