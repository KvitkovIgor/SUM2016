/* FILE NAME: RENDER.C
 * PROGRAMMER: VG4
 * DATE: 13.06.2016
 * PURPOSE: Render handle functions.
 */

#include <stdio.h>
#include <string.h>

#include "anim.h"

/* Global transformation matrices */
MATR
  IK3_RndMatrWorld, /* World (object) transformation matrix */
  IK3_RndMatrView,  /* Camera view transform */
  IK3_RndMatrProj;  /* Projection transform */

/* Projection parameters */
DBL
  IK3_RndProjDist, /* Near clip plane */
  IK3_RndFarClip,  /* Far clip plane */
  IK3_RndProjSize; /* Project plane size */

/* Shader support */
UINT IK3_RndPrg;

/* Materials array */
ik3MTL IK3_RndMaterials[IK3_MAX_MATERIALS];
INT IK3_RndNumOfMaterials;

/* Setup projection function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID IK3_RndSetProj( VOID )
{
  DBL ratio_x = 1, ratio_y = 1;

  if (IK3_Anim.W >= IK3_Anim.H)
    ratio_x = (DBL)IK3_Anim.W / IK3_Anim.H;
  else
    ratio_y = (DBL)IK3_Anim.H / IK3_Anim.W;

  IK3_RndMatrProj = MatrFrustum(-ratio_x * IK3_RndProjSize / 2,
                                 ratio_x * IK3_RndProjSize / 2,
                                -ratio_y * IK3_RndProjSize / 2,
                                 ratio_y * IK3_RndProjSize / 2,
                                 IK3_RndProjDist, IK3_RndFarClip);
} /* End of 'VG4_RndSetProj' function */

/* Object draw function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       vg4OBJ *Obj;
 * RETURNS: None.
 */
VOID IK3_RndObjDraw( ik3OBJ *Obj )
{
  INT i;
  INT loc, mtl;
  MATR M, MSave;

  for (i = 0; i < Obj->NumOfPrims; i++)
  {
    /* Build transform matrix */
    MSave = IK3_RndMatrWorld;
    IK3_RndMatrWorld = MatrMulMatr(IK3_RndMatrWorld, Obj->Prims[i].M);
    M = MatrMulMatr(IK3_RndMatrWorld,
      MatrMulMatr(IK3_RndMatrView, IK3_RndMatrProj));
    glLoadMatrixf(M.A[0]);
    /*
    glBegin(GL_LINES);
      glColor3d(1, 0, 0);
      glVertex3d(0, 0, 0);
      glVertex4d(1, 0, 0, 0);
      glColor3d(0, 1, 0);
      glVertex3d(0, 0, 0);
      glVertex4d(0, 1, 0, 0);
      glColor3d(0, 0, 1);
      glVertex3d(0, 0, 0);
      glVertex4d(0, 0, 1, 0);
    glEnd();
    */

    glUseProgram(IK3_RndPrg);

    mtl = Obj->Prims[i].MtlNo;
    if (mtl != -1)
    {
      if (IK3_RndMaterials[mtl].TexNo != 0)
      {
        glBindTexture(GL_TEXTURE_2D, IK3_RndMaterials[mtl].TexNo);
        if ((loc = glGetUniformLocation(IK3_RndPrg, "IsTexture")) != -1)
          glUniform1i(loc, 1);
      }
      else
      {
        if ((loc = glGetUniformLocation(IK3_RndPrg, "IsTexture")) != -1)
          glUniform1i(loc, 0);
      }
      if ((loc = glGetUniformLocation(IK3_RndPrg, "Ka")) != -1)
        glUniform3fv(loc, 1, &IK3_RndMaterials[mtl].Ka.X);
      if ((loc = glGetUniformLocation(IK3_RndPrg, "Kd")) != -1)
        glUniform3fv(loc, 1, &IK3_RndMaterials[mtl].Kd.X);
      if ((loc = glGetUniformLocation(IK3_RndPrg, "Ks")) != -1)
        glUniform3fv(loc, 1, &IK3_RndMaterials[mtl].Ks.X);
      if ((loc = glGetUniformLocation(IK3_RndPrg, "Ph")) != -1)
        glUniform1f(loc, IK3_RndMaterials[mtl].Ph);
      if ((loc = glGetUniformLocation(IK3_RndPrg, "Trans")) != -1)
        glUniform1f(loc, IK3_RndMaterials[mtl].Trans);
    }

    /* Setup global variables */
    if ((loc = glGetUniformLocation(IK3_RndPrg, "MatrWVP")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);
    if ((loc = glGetUniformLocation(IK3_RndPrg, "MatrWorld")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, IK3_RndMatrWorld.A[0]);
    if ((loc = glGetUniformLocation(IK3_RndPrg, "MatrView")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, IK3_RndMatrView.A[0]);
    if ((loc = glGetUniformLocation(IK3_RndPrg, "MatrProj")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, IK3_RndMatrProj.A[0]);
    if ((loc = glGetUniformLocation(IK3_RndPrg, "Time")) != -1)
      glUniform1f(loc, IK3_Anim.Time);
    if ((loc = glGetUniformLocation(IK3_RndPrg, "PartNo")) != -1)
      glUniform1i(loc, i);

    /* Activete primitive vertex array */
    glBindVertexArray(Obj->Prims[i].VA);
    /* Activete primitive index buffer */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Obj->Prims[i].IBuf);
    /* Draw primitive */
    glDrawElements(GL_TRIANGLES, Obj->Prims[i].NumOfI, GL_UNSIGNED_INT, NULL);
    glUseProgram(0);
    IK3_RndMatrWorld = MSave;
  }
} /* End of 'VG4_RndObjDraw' function */

/* Object free function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       vg4OBJ *Obj;
 * RETURNS: None.
 */
VOID IK3_RndObjFree( ik3OBJ *Obj )
{
  INT i;

  for (i = 0; i < Obj->NumOfPrims; i++)
  {
    glBindVertexArray(Obj->Prims[i].VA);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &Obj->Prims[i].VBuf);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &Obj->Prims[i].VA);
    glDeleteBuffers(1, &Obj->Prims[i].IBuf);
  }
  free(Obj->Prims);
  memset(Obj, 0, sizeof(ik3OBJ));
} /* End of 'VG4_RndObjFree' function */

/* Material find by name function.
 * ARGUMENTS:
 *   - material name:
 *       CHAR *Name;
 * RETURNS:
 *   (INT) number of found material or -1 if no result.
 */
INT IK3_RndFindMaterial( CHAR *Name )
{
  INT i;

  for (i = 0; i < IK3_RndNumOfMaterials; i++)
    if (strcmp(Name, IK3_RndMaterials[i].Name) == 0)  /* #include <string.h> */
      return i;
  return -1;
} /* End of 'VG4_RndFindMaterial' function */

/* Material load function.
 * ARGUMENTS:
 *   - material file name:
 *       CHAR *FileName;
 * RETURNS: None.
*/
VOID IK3_RndLoadMaterials( CHAR *FileName )
{
  INT i, NumOfMaterials;
  UINT t;
  DWORD Sign;
  FILE *F;
  ik3MTL M;
  BYTE *Image;

  if ((F = fopen(FileName, "rb")) == NULL)
    return;
  /* Read and check file signature */
  fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"GMT")
  {
    fclose(F);
    return;
  }

  /* Read all materials */
  fread(&NumOfMaterials, 4, 1, F);
  for (i = 0; i < NumOfMaterials; i++)
  {
    if (IK3_RndNumOfMaterials >= IK3_MAX_MATERIALS)
      break;

    /* Read illumination coefficients and texture parameters */
    fread(&M, sizeof(ik3MTL), 1, F);
    
    /* Read image */
    if (M.TexW != 0 && M.TexH != 0 && M.TexNo != 0)
    {
      /* Allocate memory for texture image */
      if ((Image = malloc(M.TexW * M.TexH * M.TexNo)) == NULL)
      {
        fclose(F);
        return;
      }
      fread(Image, M.TexNo, M.TexW * M.TexH, F);
      /* Flip image */
      /*
      for (y = 0; y < M.TexH / 2; y++)
        for (x = 0; x < M.TexW * M.TexNo; x++)
        {
          INT
            first = y * M.TexW * M.TexNo + x,
            last = (M.TexH - 1 - y) * M.TexW * M.TexNo + x;
          BYTE tmp = Image[first];

          Image[first] = Image[last];
          Image[last] = tmp;
        }
      */
      glGenTextures(1, &t);
      glBindTexture(GL_TEXTURE_2D, t);
      gluBuild2DMipmaps(GL_TEXTURE_2D, M.TexNo, M.TexW, M.TexH,
        M.TexNo == 3 ? GL_BGR_EXT : GL_BGRA, GL_UNSIGNED_BYTE, Image);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glBindTexture(GL_TEXTURE_2D, 0);
      M.TexNo = t;
    }
    /* Add material to animation and OpenGL */
    IK3_RndMaterials[IK3_RndNumOfMaterials++] = M;
  }
  fclose(F);
} /* End of 'VG4_RndLoadMaterials' function */

/* END OF 'RENDER.C' FILE */
