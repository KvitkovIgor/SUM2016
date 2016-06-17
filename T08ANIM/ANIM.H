/* FILE NAME: AMIN.H
 * PROGRAMMER: IK3
 * DATE: 15.06.2016
 * PURPOSE: Ball unit sample.
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#pragma warning(disable: 4244)


#define MAX_UNITS 10000

#include "def.h"
#include "render.h"

typedef struct tagik3UNIT ik3UNIT;
typedef struct tagik3Anim
{
  HWND hWnd;
  HDC hDC;
  INT W, H;
  HGLRC hGLRC;
  HBITMAP hFrame;
  ik3UNIT *Units[MAX_UNITS];
  INT NumOfUnits;
  DBL
    GlobalTime, GlobalDeltaTime,
    Time, DeltaTime,
    FPS;
  BOOL IsPause;
  INT 
    Mx, My,
    Mz, 
    Mdx, Mdy, Mdz;
  BYTE 
    Keys[256],
    OldKeys[256],
    KeysClick[256];
  BYTE
  JBut[32];                      /* Joystick button state */
  INT JPov;                      /* Joystick point-of-view control [0,1..8] */
  DBL JX, JY, JZ, JR; 
} ik3Anim;
struct tagik3UNIT
{
  VOID (*Init)( ik3UNIT *Uni, ik3Anim *Ani );
  VOID (*Close)( ik3UNIT *Uni, ik3Anim *Ani );
  VOID (*Response)( ik3UNIT *Uni, ik3Anim *Ani );
  VOID (*Render)( ik3UNIT *Uni, ik3Anim *Ani );
};
typedef struct
{
  ik3PRIM *Prims;
  INT NumOfPrims;
} ik3OBJ;



/* Material representation type */
typedef struct
{
  CHAR Name[300]; /* Material name */
  VEC Ka, Kd, Ks; /* Illumination coefficients */
  FLT Ph, Trans;  /* Shininess and Phong, transparency */
  INT TexW, TexH; /* Textiure image size */
  INT TexNo;      /* For load: bytes per pixel, in use: OpenGL texture no */
} ik3MTL;
extern UINT IK3_RndPrg;

/* Materials array */
#define IK3_MAX_MATERIALS 10000
extern ik3MTL IK3_RndMaterials[IK3_MAX_MATERIALS];
extern INT IK3_RndNumOfMaterials, IK3_MouseWheel;

/* Setup projection function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID IK3_RndSetProj( VOID );

/* Object draw function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       vg4OBJ *Obj;
 * RETURNS: None.
 */
VOID IK3_RndObjDraw( ik3OBJ *Obj );

/* Load object from '*.g3d' file function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       vg4OBJ *Obj;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE is success, FALSE otherwise.
 */
BOOL IK3_RndObjLoad( ik3OBJ *Obj, CHAR *FileName );

/* Object free function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       vg4OBJ *Obj;
 * RETURNS: None.
 */
VOID IK3_RndObjFree( ik3OBJ *Obj );

/* Material find by name function.
 * ARGUMENTS:
 *   - material name:
 *       CHAR *Name;
 * RETURNS:
 *   (INT) number of found material or -1 if no result.
 */
INT IK3_RndFindMaterial( CHAR *Name );

/* Material load function.
 * ARGUMENTS:
 *   - material file name:
 *       CHAR *FileName;
 * RETURNS: None.
*/
VOID IK3_RndLoadMaterials( CHAR *FileName );




extern ik3Anim IK3_Anim;


VOID IK3_AnimCopyFrame( HDC hDC );
VOID IK3_AnimUnit( HWND hWnd );
VOID IK3_AnimAddUnit( ik3UNIT *Uni );
VOID IK3_AnimClose( VOID );
VOID IK3_Reasize( INT W, INT H );
VOID IK3_AnimRender( VOID );
VOID IK3_AnimDoExit( VOID );
ik3UNIT * IK3_AnimUnitCreate( INT Size );
VOID IK3_RndShaderFree( UINT Prg );
UINT IK3_RndShaderLoad( CHAR *FileNamePrefix );

#endif
