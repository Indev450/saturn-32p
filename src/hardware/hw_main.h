// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// Copyright (C) 1998-2000 by DooM Legacy Team.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
//-----------------------------------------------------------------------------
/// \file
/// \brief 3D render mode functions

#ifndef __HWR_MAIN_H__
#define __HWR_MAIN_H__

#include "hw_glob.h"
#include "hw_data.h"
#include "hw_defs.h"

#include "../am_map.h"
#include "../d_player.h"
#include "../r_defs.h"
#include "../m_perfstats.h"

#define GLENCORE

// Startup & Shutdown the hardware mode renderer
void HWR_Startup(void);
void HWR_Shutdown(void);

extern float gr_viewwidth, gr_viewheight, gr_baseviewwindowx, gr_baseviewwindowy;

extern float gr_basewindowcenterx, gr_basewindowcentery;

extern unsigned msaa;
extern boolean a2c;

extern FTransform atransform;
extern float gr_viewsin, gr_viewcos;

// Performance stats
extern ps_metric_t ps_hw_nodesorttime;
extern ps_metric_t ps_hw_nodedrawtime;
extern ps_metric_t ps_hw_spritesorttime;
extern ps_metric_t ps_hw_spritedrawtime;

// Performance stats for batching
extern ps_metric_t ps_hw_numpolys;
extern ps_metric_t ps_hw_numverts;
extern ps_metric_t ps_hw_numcalls;
extern ps_metric_t ps_hw_numshaders;
extern ps_metric_t ps_hw_numtextures;
extern ps_metric_t ps_hw_numpolyflags;
extern ps_metric_t ps_hw_numcolors;
extern ps_metric_t ps_hw_batchsorttime;
extern ps_metric_t ps_hw_batchdrawtime;

extern boolean gr_shadersavailable;

// hw_draw.c
void HWR_DrawPatch(GLPatch_t *gpatch, INT32 x, INT32 y, INT32 option);
void HWR_DrawStretchyFixedPatch(GLPatch_t *gpatch, fixed_t x, fixed_t y, fixed_t pscale, fixed_t vscale, INT32 option, const UINT8 *colormap);
void HWR_DrawCroppedPatch(GLPatch_t *gpatch, fixed_t x, fixed_t y, fixed_t pscale, INT32 option, fixed_t sx, fixed_t sy, fixed_t w, fixed_t h);
void HWR_DrawFill(INT32 x, INT32 y, INT32 w, INT32 h, INT32 color);
void HWR_DrawConsoleFill(INT32 x, INT32 y, INT32 w, INT32 h, UINT32 color, INT32 options);	// Lat: separate flags from color since color needs to be an uint to work right.
void HWR_DrawDiag(INT32 x, INT32 y, INT32 wh, INT32 color);
void HWR_drawAMline(const fline_t *fl, INT32 color);
void HWR_FadeScreenMenuBack(UINT16 color, UINT8 strength);
void HWR_DrawConsoleBack(UINT32 color, INT32 height);
void HWR_DrawViewBorder(INT32 clearlines);
void HWR_DrawFlatFill(INT32 x, INT32 y, INT32 w, INT32 h, lumpnum_t flatlumpnum);

UINT8 *HWR_GetScreenshot(void);
boolean HWR_Screenshot(const char *lbmname);

// hw_main.c
void HWR_RenderFrame(INT32 viewnumber, player_t *player, boolean skybox);
void HWR_RenderPlayerView(INT32 viewnumber, player_t *player);
void HWR_SetViewSize(void);
void HWR_AddCommands(void);
void HWR_SetTransform(float fpov, player_t *player);
void HWR_ClearClipper(void);

boolean HWR_UseShader(void);
boolean HWR_ShouldUsePaletteRendering(void);
boolean HWR_PalRenderFlashpal(void);
void HWR_TogglePaletteRendering(void);

// My original intention was to split hw_main.c
// into files like hw_bsp.c, hw_sprites.c...

// hw_main.c: Lighting and fog
void HWR_Lighting(FSurfaceInfo *Surface, INT32 light_level, extracolormap_t *colormap);
UINT8 HWR_FogBlockAlpha(INT32 light, extracolormap_t *colormap); // Let's see if this can work

FBITFIELD HWR_TranstableToAlpha(INT32 transtablenum, FSurfaceInfo *pSurf);

// Get amount of memory used by gpu textures in bytes
INT32 HWR_GetTextureUsed(void);

// hw_main.c: Post-rendering
void HWR_DoPostProcessor(player_t *player);
void HWR_StartScreenWipe(void);
void HWR_EndScreenWipe(void);
void HWR_DrawIntermissionBG(void);
void HWR_DoWipe(UINT8 wipenum, UINT8 scrnnum);
void HWR_RenderVhsEffect(fixed_t upbary, fixed_t downbary, UINT8 updistort, UINT8 downdistort, UINT8 barsize);
void HWR_MakeScreenFinalTexture(void);
void HWR_DrawScreenFinalTexture(INT32 width, INT32 height);

// hw_main.c: Planes
void HWR_RenderPlane(subsector_t *subsector, extrasubsector_t *xsub, boolean isceiling, fixed_t fixedheight, FBITFIELD PolyFlags, INT32 lightlevel, lumpnum_t lumpnum, sector_t *FOFsector, UINT8 alpha, extracolormap_t *planecolormap);
void HWR_AddTransparentFloor(lumpnum_t lumpnum, extrasubsector_t *xsub, boolean isceiling, fixed_t fixedheight, INT32 lightlevel, INT32 alpha, sector_t *FOFSector, FBITFIELD blend, boolean fogplane, extracolormap_t *planecolormap);

void HWR_RenderPolyObjectPlane(polyobj_t *polysector, boolean isceiling, fixed_t fixedheight, FBITFIELD blendmode, UINT8 lightlevel, lumpnum_t lumpnum, sector_t *FOFsector, UINT8 alpha, extracolormap_t *planecolormap);
void HWR_AddPolyObjectPlanes(void);
void HWR_AddTransparentPolyobjectFloor(lumpnum_t lumpnum, polyobj_t *polysector, boolean isceiling, fixed_t fixedheight, INT32 lightlevel, INT32 alpha, sector_t *FOFSector, FBITFIELD blend, extracolormap_t *planecolormap);

// hw_main.c: Segs
void HWR_ProcessSeg(void); // Sort of like GLWall::Process in GZDoom
void HWR_RenderWall(FOutVector *wallVerts, FSurfaceInfo *pSurf, FBITFIELD blend, boolean fogwall, INT32 lightlevel, extracolormap_t *wallcolormap);
void HWR_ProjectWall(FOutVector *wallVerts, FSurfaceInfo *pSurf, FBITFIELD blendmode, INT32 lightlevel, extracolormap_t *wallcolormap);
void HWR_AddTransparentWall(FOutVector *wallVerts, FSurfaceInfo * pSurf, INT32 texnum, boolean noencore, FBITFIELD blend, boolean fogwall, INT32 lightlevel, extracolormap_t *wallcolormap);
// moved HWR_SplitWall to hw_main.c
void HWR_DrawSkyWall(FOutVector *wallVerts, FSurfaceInfo *Surf);
void HWR_DrawSkyBackground(float fpov);

void HWR_AddPolyObjectSegs(void);

// hw_main.c: Sprites
void HWR_AddSprites(sector_t *sec);
void HWR_ProjectSprite(mobj_t *thing);
void HWR_ProjectPrecipitationSprite(precipmobj_t *thing);
void HWR_DrawSprites(void);

// hw_bsp.c
void HWR_CreatePlanePolygons(INT32 bspnum);
extern boolean gr_maphasportals;

// hw_cache.c
void HWR_LoadTextures(size_t pnumtextures);
RGBA_t *HWR_GetTexturePalette(void);

void HWR_SetShaderState(void);

// Console variables
extern consvar_t cv_grshaders;
extern consvar_t cv_grfofcut;
extern consvar_t cv_fofzfightfix;
extern consvar_t cv_splitwallfix;
extern consvar_t cv_slopepegfix;
extern consvar_t cv_grshearing;
extern consvar_t cv_grfov;
extern consvar_t cv_grmdls;
extern consvar_t cv_grfog;
extern consvar_t cv_grfogdensity;
extern consvar_t cv_grfiltermode;
extern consvar_t cv_granisotropicmode;
extern consvar_t cv_grcorrecttricks;
extern consvar_t cv_grfovchange;
extern consvar_t cv_grsolvetjoin;
extern consvar_t cv_grspritebillboarding;
extern consvar_t cv_grfakecontrast;
extern consvar_t cv_grslopecontrast;
extern consvar_t cv_grhorizonlines;
extern consvar_t cv_grfallbackplayermodel;
extern consvar_t cv_grbatching;
extern consvar_t cv_grrenderdistance;
extern consvar_t cv_grpaletterendering;
extern consvar_t cv_grpalettedepth;
extern consvar_t cv_grflashpal;
extern consvar_t cv_grscreentextures;
extern consvar_t cv_grportals;
extern consvar_t cv_nostencil;
extern consvar_t cv_secbright;

extern CV_PossibleValue_t granisotropicmode_cons_t[];

#endif
