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
/// \file  r_draw.c
/// \brief span / column drawer functions, for 8bpp and 16bpp
///        All drawing to the view buffer is accomplished in this file.
///        The other refresh files only know about ccordinates,
///        not the architecture of the frame buffer.
///        The frame buffer is a linear one, and we need only the base address.

#include "doomdef.h"
#include "doomstat.h"
#include "r_local.h"
#include "r_things.h"
#include "st_stuff.h" // need ST_HEIGHT
#include "i_video.h"
#include "v_video.h"
#include "m_misc.h"
#include "w_wad.h"
#include "z_zone.h"
#include "console.h" // Until buffering gets finished
#include "k_kart.h" // SRB2kart

#ifdef HWRENDER
#include "hardware/hw_main.h"
#endif

// ==========================================================================
//                     COMMON DATA FOR 8bpp AND 16bpp
// ==========================================================================

/**	\brief view info
*/
INT32 viewwidth, scaledviewwidth, viewheight, viewwindowx, viewwindowy;

/**	\brief pointer to the start of each line of the screen,
*/
UINT8 *ylookup[MAXVIDHEIGHT*4];

/**	\brief pointer to the start of each line of the screen, for view1 (splitscreen)
*/
UINT8 *ylookup1[MAXVIDHEIGHT*4];

/**	\brief pointer to the start of each line of the screen, for view2 (splitscreen)
*/
UINT8 *ylookup2[MAXVIDHEIGHT*4];

/**	\brief pointer to the start of each line of the screen, for view3 (splitscreen)
*/
UINT8 *ylookup3[MAXVIDHEIGHT*4];

/**	\brief pointer to the start of each line of the screen, for view4 (splitscreen)
*/
UINT8 *ylookup4[MAXVIDHEIGHT*4];

/**	\brief  x byte offset for columns inside the viewwindow,
	so the first column starts at (SCRWIDTH - VIEWWIDTH)/2
*/
INT32 columnofs[MAXVIDWIDTH*4];

UINT8 *topleft;

// =========================================================================
//                      COLUMN DRAWING CODE STUFF
// =========================================================================

lighttable_t *dc_colormap;
INT32 dc_x = 0, dc_yl = 0, dc_yh = 0;

fixed_t dc_iscale, dc_texturemid;
UINT8 dc_hires; // under MSVC boolean is a byte, while on other systems, it a bit,
               // soo lets make it a byte on all system for the ASM code
UINT8 *dc_source;

// -----------------------
// translucency stuff here
// -----------------------
#define NUMTRANSTABLES 9 // how many translucency tables are used

UINT8 *transtables; // translucency tables

/**	\brief R_DrawTransColumn uses this
*/
UINT8 *dc_transmap; // one of the translucency tables

// ----------------------
// translation stuff here
// ----------------------


/**	\brief R_DrawTranslatedColumn uses this
*/
UINT8 *dc_translation;

struct r_lightlist_s *dc_lightlist = NULL;
INT32 dc_numlights = 0, dc_maxlights, dc_texheight;

// =========================================================================
//                      SPAN DRAWING CODE STUFF
// =========================================================================

INT32 ds_y, ds_x1, ds_x2;
lighttable_t *ds_colormap;
fixed_t ds_xfrac, ds_yfrac, ds_xstep, ds_ystep;

UINT8 *ds_source; // points to the start of a flat
UINT8 *ds_transmap; // one of the translucency tables

// Vectors for Software's tilted slope drawers
floatv3_t *ds_su, *ds_sv, *ds_sz;
floatv3_t *ds_sup, *ds_svp, *ds_szp;
float focallengthf, zeroheight;

/**	\brief Variable flat sizes
*/

UINT32 nflatxshift, nflatyshift, nflatshiftup, nflatmask;

// ==========================================================================
//                        OLD DOOM FUZZY EFFECT
// ==========================================================================

// =========================================================================
//                   TRANSLATION COLORMAP CODE
// =========================================================================

#define DEFAULT_TT_CACHE_INDEX MAXSKINS
#define BOSS_TT_CACHE_INDEX (MAXSKINS + 1)
#define METALSONIC_TT_CACHE_INDEX (MAXSKINS + 2)
#define ALLWHITE_TT_CACHE_INDEX (MAXSKINS + 3)
#define RAINBOW_TT_CACHE_INDEX (MAXSKINS + 4)
#define BLINK_TT_CACHE_INDEX (MAXSKINS + 5)
#define TT_CACHE_SIZE (MAXSKINS + 6)
#define SKIN_RAMP_LENGTH 16
#define DEFAULT_STARTTRANSCOLOR 160
#define NUM_PALETTE_ENTRIES 256

static UINT8** translationtablecache[TT_CACHE_SIZE] = {NULL};
static UINT8** localtranslationtablecache[MAXSKINS] = {NULL};


// See also the enum skincolors_t
// TODO Callum: Can this be translated?
/*
const char *Color_Names[MAXSKINCOLORS] =
{
	"None",      // SKINCOLOR_NONE
	"White",     // SKINCOLOR_WHITE
	"Silver",    // SKINCOLOR_SILVER
	"Grey",      // SKINCOLOR_GREY
	"Black",     // SKINCOLOR_BLACK
	"Cyan",      // SKINCOLOR_CYAN
	"Teal",      // SKINCOLOR_TEAL
	"Steel_Blue",// SKINCOLOR_STEEL
	"Blue",      // SKINCOLOR_BLUE
	"Peach",     // SKINCOLOR_PEACH
	"Tan",       // SKINCOLOR_TAN
	"Pink",      // SKINCOLOR_PINK
	"Lavender",  // SKINCOLOR_LAVENDER
	"Purple",    // SKINCOLOR_PURPLE
	"Orange",    // SKINCOLOR_ORANGE
	"Rosewood",  // SKINCOLOR_ROSEWOOD
	"Beige",     // SKINCOLOR_BEIGE
	"Brown",     // SKINCOLOR_BROWN
	"Red",       // SKINCOLOR_RED
	"Dark_Red",  // SKINCOLOR_DARKRED
	"Neon_Green",// SKINCOLOR_NEONGREEN
	"Green",     // SKINCOLOR_GREEN
	"Zim",       // SKINCOLOR_ZIM
	"Olive",     // SKINCOLOR_OLIVE
	"Yellow",    // SKINCOLOR_YELLOW
	"Gold"       // SKINCOLOR_GOLD
};

const UINT8 Color_Opposite[MAXSKINCOLORS*2] =
{
	SKINCOLOR_NONE,8,   // SKINCOLOR_NONE
	SKINCOLOR_BLACK,10, // SKINCOLOR_WHITE
	SKINCOLOR_GREY,4,   // SKINCOLOR_SILVER
	SKINCOLOR_SILVER,12,// SKINCOLOR_GREY
	SKINCOLOR_WHITE,8,  // SKINCOLOR_BLACK
	SKINCOLOR_NONE,8,   // SKINCOLOR_CYAN
	SKINCOLOR_NONE,8,   // SKINCOLOR_TEAL
	SKINCOLOR_NONE,8,   // SKINCOLOR_STEEL
	SKINCOLOR_ORANGE,9, // SKINCOLOR_BLUE
	SKINCOLOR_NONE,8,   // SKINCOLOR_PEACH
	SKINCOLOR_NONE,8,   // SKINCOLOR_TAN
	SKINCOLOR_NONE,8,   // SKINCOLOR_PINK
	SKINCOLOR_NONE,8,   // SKINCOLOR_LAVENDER
	SKINCOLOR_NONE,8,   // SKINCOLOR_PURPLE
	SKINCOLOR_BLUE,12,  // SKINCOLOR_ORANGE
	SKINCOLOR_NONE,8,   // SKINCOLOR_ROSEWOOD
	SKINCOLOR_NONE,8,   // SKINCOLOR_BEIGE
	SKINCOLOR_NONE,8,   // SKINCOLOR_BROWN
	SKINCOLOR_GREEN,5,  // SKINCOLOR_RED
	SKINCOLOR_NONE,8,   // SKINCOLOR_DARKRED
	SKINCOLOR_NONE,8,   // SKINCOLOR_NEONGREEN
	SKINCOLOR_RED,11,   // SKINCOLOR_GREEN
	SKINCOLOR_PURPLE,3, // SKINCOLOR_ZIM
	SKINCOLOR_NONE,8,   // SKINCOLOR_OLIVE
	SKINCOLOR_NONE,8,   // SKINCOLOR_YELLOW
	SKINCOLOR_NONE,8    // SKINCOLOR_GOLD
};
*/

CV_PossibleValue_t Color_cons_t[MAXSKINCOLORS+1];

/**	\brief The R_InitTranslationTables

  load in color translation tables
*/
void R_InitTranslationTables(void)
{
	// Load here the transparency lookup tables 'TINTTAB'
	// NOTE: the TINTTAB resource MUST BE aligned on 64k for the asm
	// optimised code (in other words, transtables pointer low word is 0)
	transtables = Z_MallocAlign(NUMTRANSTABLES*0x10000, PU_STATIC,
		NULL, 16);

	W_ReadLump(W_GetNumForName("TRANS10"), transtables);
	W_ReadLump(W_GetNumForName("TRANS20"), transtables+0x10000);
	W_ReadLump(W_GetNumForName("TRANS30"), transtables+0x20000);
	W_ReadLump(W_GetNumForName("TRANS40"), transtables+0x30000);
	W_ReadLump(W_GetNumForName("TRANS50"), transtables+0x40000);
	W_ReadLump(W_GetNumForName("TRANS60"), transtables+0x50000);
	W_ReadLump(W_GetNumForName("TRANS70"), transtables+0x60000);
	W_ReadLump(W_GetNumForName("TRANS80"), transtables+0x70000);
	W_ReadLump(W_GetNumForName("TRANS90"), transtables+0x80000);
}

/**	\brief	Retrieves a translation colormap from the cache.

	\param	skinnum	number of skin, TC_DEFAULT or TC_BOSS
	\param	color	translation color
	\param	flags	set GTC_CACHE to use the cache

	\return	Colormap. If not cached, caller should Z_Free.
*/
static UINT8* RGetTranslationColormap(INT32 skinnum, skincolors_t color, UINT8 flags, boolean local)
{
	UINT8 ***tt;
	UINT8* ret;
	INT32 skintableindex;

	if (local)
	{
		tt = localtranslationtablecache;
		skintableindex = skinnum;
	}
	else
	{
		tt = translationtablecache;
		// Adjust if we want the default colormap
		if (skinnum == TC_DEFAULT) skintableindex = DEFAULT_TT_CACHE_INDEX;
		else if (skinnum == TC_BOSS) skintableindex = BOSS_TT_CACHE_INDEX;
		else if (skinnum == TC_METALSONIC) skintableindex = METALSONIC_TT_CACHE_INDEX;
		else if (skinnum == TC_ALLWHITE) skintableindex = ALLWHITE_TT_CACHE_INDEX;
		else if (skinnum == TC_RAINBOW) skintableindex = RAINBOW_TT_CACHE_INDEX;
		else if (skinnum == TC_BLINK) skintableindex = BLINK_TT_CACHE_INDEX;
		else skintableindex = skinnum;
	}

	if (flags & GTC_CACHE)
	{

		// Allocate table for skin if necessary
		if (!tt[skintableindex])
			tt[skintableindex] = Z_Calloc(MAXTRANSLATIONS * sizeof(UINT8**), PU_STATIC, NULL);

		// Get colormap
		ret = tt[skintableindex][color];
	}
	else ret = NULL;

	// Generate the colormap if necessary
	if (!ret)
	{
		ret = Z_MallocAlign(NUM_PALETTE_ENTRIES, (flags & GTC_CACHE) ? PU_LEVEL : PU_STATIC, NULL, 8);
		K_GenerateKartColormap(ret, skinnum, color, local); //R_GenerateTranslationColormap(ret, skinnum, color);		// SRB2kart

		// Cache the colormap if desired
		if (flags & GTC_CACHE)
			tt[skintableindex][color] = ret;
	}

	return ret;
}

UINT8* R_GetTranslationColormap(INT32 skinnum, skincolors_t color, UINT8 flags)
{
	return RGetTranslationColormap(skinnum, color, flags, false);
}

UINT8* R_GetLocalTranslationColormap(skin_t *skin, skin_t *localskin, skincolors_t color, UINT8 flags, boolean local)
{
	if (localskin)
		return RGetTranslationColormap(( localskin - ( (local) ? localskins : skins ) ), color, flags, local);
	else
		return RGetTranslationColormap(( skin - skins ), color, flags, false);
}

patch_t* R_GetSkinFaceRank(player_t* ply) 
{
	if (ply->skinlocal && ply->localskin)
		return localfacerankprefix[ply->localskin - 1];
	else if (ply->localskin)
		return facerankprefix[ply->localskin - 1];
	return facerankprefix[ply->skin];
}

patch_t* R_GetSkinFaceWant(player_t* ply) 
{
	if (ply->skinlocal && ply->localskin)
		return localfacewantprefix[ply->localskin - 1];
	else if (ply->localskin)
		return facewantprefix[ply->localskin - 1];
	return facewantprefix[ply->skin];
}

patch_t* R_GetSkinFaceMini(player_t* ply) 
{
	if (ply->skinlocal && ply->localskin)
		return localfacemmapprefix[ply->localskin - 1];
	else if (ply->localskin)
		return facemmapprefix[ply->localskin - 1];
	return facemmapprefix[ply->skin];
}

/**	\brief	Flushes cache of translation colormaps.

	Flushes cache of translation colormaps, but doesn't actually free the
	colormaps themselves. These are freed when PU_LEVEL blocks are purged,
	at or before which point, this function should be called.

	\return	void
*/
void R_FlushTranslationColormapCache(void)
{
	INT32 i;

	for (i = 0; i < (INT32)(sizeof(translationtablecache) / sizeof(translationtablecache[0])); i++)
		if (translationtablecache[i])
			memset(translationtablecache[i], 0, MAXTRANSLATIONS * sizeof(UINT8**));

	for (i = 0; i < (INT32)(sizeof(localtranslationtablecache) / sizeof(localtranslationtablecache[0])); i++)
		if (localtranslationtablecache[i])
			memset(localtranslationtablecache[i], 0, MAXTRANSLATIONS * sizeof(UINT8**));
}

/*
UINT8 R_GetColorByName(const char *name)
{
	UINT8 color = (UINT8)atoi(name);
	if (color > 0 && color < MAXSKINCOLORS)
		return color;
	for (color = 1; color < MAXSKINCOLORS; color++)
		if (!stricmp(Color_Names[color], name))
			return color;
	return 0;
}
*/

// ==========================================================================
//               COMMON DRAWER FOR 8 AND 16 BIT COLOR MODES
// ==========================================================================

// in a perfect world, all routines would be compatible for either mode,
// and optimised enough
//
// in reality, the few routines that can work for either mode, are
// put here

/**	\brief	The R_InitViewBuffer function

	Creates lookup tables for getting the framebuffer address
	of a pixel to draw.

	\param	width	witdh of buffer
	\param	height	hieght of buffer

	\return	void


*/

void R_InitViewBuffer(INT32 width, INT32 height)
{
	INT32 i, bytesperpixel = vid.bpp;

	if (width > MAXVIDWIDTH)
		width = MAXVIDWIDTH;
	if (height > MAXVIDHEIGHT)
		height = MAXVIDHEIGHT;
	if (bytesperpixel < 1 || bytesperpixel > 4)
		I_Error("R_InitViewBuffer: wrong bytesperpixel value %d\n", bytesperpixel);

	viewwindowx = 0;
	viewwindowy = 0;

	// Column offset for those columns of the view window, but relative to the entire screen
	for (i = 0; i < width; i++)
		columnofs[i] = (viewwindowx + i) * bytesperpixel;

	// Precalculate all row offsets.
	for (i = 0; i < height; i++)
	{
		ylookup[i] = ylookup1[i] = screens[0] + i*vid.width*bytesperpixel;
		if (splitscreen == 1)
			ylookup2[i] = screens[0] + (i+viewheight)*vid.width*bytesperpixel;
		else
			ylookup2[i] = screens[0] + i*vid.width*bytesperpixel + (viewwidth*bytesperpixel);
		ylookup3[i] = screens[0] + (i+viewheight)*vid.width*bytesperpixel;
		ylookup4[i] = screens[0] + (i+viewheight)*vid.width*bytesperpixel + (viewwidth*bytesperpixel);
	}
}

/**	\brief viewborder patches lump numbers
*/
lumpnum_t viewborderlump[8];

/**	\brief Store the lumpnumber of the viewborder patches
*/

void R_InitViewBorder(void)
{
	viewborderlump[BRDR_T] = W_GetNumForName("brdr_t");
	viewborderlump[BRDR_B] = W_GetNumForName("brdr_b");
	viewborderlump[BRDR_L] = W_GetNumForName("brdr_l");
	viewborderlump[BRDR_R] = W_GetNumForName("brdr_r");
	viewborderlump[BRDR_TL] = W_GetNumForName("brdr_tl");
	viewborderlump[BRDR_BL] = W_GetNumForName("brdr_bl");
	viewborderlump[BRDR_TR] = W_GetNumForName("brdr_tr");
	viewborderlump[BRDR_BR] = W_GetNumForName("brdr_br");
}

#if 0
/**	\brief R_FillBackScreen

	Fills the back screen with a pattern for variable screen sizes
	Also draws a beveled edge.
*/
void R_FillBackScreen(void)
{
	UINT8 *src, *dest;
	patch_t *patch;
	INT32 x, y, step, boff;

	// quickfix, don't cache lumps in both modes
	if (rendermode != render_soft)
		return;

	// draw pattern around the status bar too (when hires),
	// so return only when in full-screen without status bar.
	if (scaledviewwidth == vid.width && viewheight == vid.height)
		return;

	src = scr_borderpatch;
	dest = screens[1];

	for (y = 0; y < vid.height; y++)
	{
		for (x = 0; x < vid.width/128; x++)
		{
			M_Memcpy (dest, src+((y&127)<<7), 128);
			dest += 128;
		}

		if (vid.width&127)
		{
			M_Memcpy(dest, src+((y&127)<<7), vid.width&127);
			dest += (vid.width&127);
		}
	}

	// don't draw the borders when viewwidth is full vid.width.
	if (scaledviewwidth == vid.width)
		return;

	step = 8;
	boff = 8;

	patch = W_CacheLumpNum(viewborderlump[BRDR_T], PU_CACHE);
	for (x = 0; x < scaledviewwidth; x += step)
		V_DrawPatch(viewwindowx + x, viewwindowy - boff, 1, patch);

	patch = W_CacheLumpNum(viewborderlump[BRDR_B], PU_CACHE);
	for (x = 0; x < scaledviewwidth; x += step)
		V_DrawPatch(viewwindowx + x, viewwindowy + viewheight, 1, patch);

	patch = W_CacheLumpNum(viewborderlump[BRDR_L], PU_CACHE);
	for (y = 0; y < viewheight; y += step)
		V_DrawPatch(viewwindowx - boff, viewwindowy + y, 1, patch);

	patch = W_CacheLumpNum(viewborderlump[BRDR_R],PU_CACHE);
	for (y = 0; y < viewheight; y += step)
		V_DrawPatch(viewwindowx + scaledviewwidth, viewwindowy + y, 1,
			patch);

	// Draw beveled corners.
	V_DrawPatch(viewwindowx - boff, viewwindowy - boff, 1,
		W_CacheLumpNum(viewborderlump[BRDR_TL], PU_CACHE));
	V_DrawPatch(viewwindowx + scaledviewwidth, viewwindowy - boff, 1,
		W_CacheLumpNum(viewborderlump[BRDR_TR], PU_CACHE));
	V_DrawPatch(viewwindowx - boff, viewwindowy + viewheight, 1,
		W_CacheLumpNum(viewborderlump[BRDR_BL], PU_CACHE));
	V_DrawPatch(viewwindowx + scaledviewwidth, viewwindowy + viewheight, 1,
		W_CacheLumpNum(viewborderlump[BRDR_BR], PU_CACHE));
}
#endif

/**	\brief	The R_VideoErase function

	Copy a screen buffer.

	\param	ofs	offest from buffer
	\param	count	bytes to erase

	\return	void


*/
void R_VideoErase(size_t ofs, INT32 count)
{
	// LFB copy.
	// This might not be a good idea if memcpy
	//  is not optimal, e.g. byte by byte on
	//  a 32bit CPU, as GNU GCC/Linux libc did
	//  at one point.
	M_Memcpy(screens[0] + ofs, screens[1] + ofs, count);
}

#if 0
/**	\brief The R_DrawViewBorder

  Draws the border around the view
	for different size windows?
*/
void R_DrawViewBorder(void)
{
	INT32 top, side, ofs;

	if (rendermode == render_none)
		return;
#ifdef HWRENDER
	if (rendermode != render_soft)
	{
		HWR_DrawViewBorder(0);
		return;
	}
	else
#endif

#ifdef DEBUG
	fprintf(stderr,"RDVB: vidwidth %d vidheight %d scaledviewwidth %d viewheight %d\n",
		vid.width, vid.height, scaledviewwidth, viewheight);
#endif

	if (scaledviewwidth == vid.width)
		return;

	top = (vid.height - viewheight)>>1;
	side = (vid.width - scaledviewwidth)>>1;

	// copy top and one line of left side
	R_VideoErase(0, top*vid.width+side);

	// copy one line of right side and bottom
	ofs = (viewheight+top)*vid.width - side;
	R_VideoErase(ofs, top*vid.width + side);

	// copy sides using wraparound
	ofs = top*vid.width + vid.width-side;
	side <<= 1;

    // simpler using our VID_Blit routine
	VID_BlitLinearScreen(screens[1] + ofs, screens[0] + ofs, side, viewheight - 1,
		vid.width, vid.width);
}
#endif

// ==========================================================================
//                   INCLUDE 8bpp DRAWING CODE HERE
// ==========================================================================

#include "r_draw8.c"

// ==========================================================================
//                   INCLUDE 16bpp DRAWING CODE HERE
// ==========================================================================

#ifdef HIGHCOLOR
#include "r_draw16.c"
#endif
