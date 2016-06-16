
ANIM.C
  AS5_RndPrg = AS5_RndShaderLoad("a");

  AS5_RndShaderFree(AS5_RndPrg);

RENDER.C:

/* Primitive draw function.
 * ARGUMENTS:
 *   - primtive to draw:
 *       as5PRIM *Pr;
 * RETURNS: None.
 */
VOID AS5_RndPrimDraw( as5PRIM *Pr )
{
  INT loc;
  MATR M;

  /* Build transform matrix */
  M = MatrMulMatr(AS5_RndMatrWorld,
    MatrMulMatr(AS5_RndMatrView, AS5_RndMatrProj));
  glLoadMatrixf(M.A[0]);

  glUseProgram(AS5_RndPrg);

  /* Setup global variables */
  if ((loc = glGetUniformLocation(AS5_RndPrg, "MatrWorld")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AS5_RndMatrWorld.A[0]);
  if ((loc = glGetUniformLocation(AS5_RndPrg, "MatrView")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AS5_RndMatrView.A[0]);
  if ((loc = glGetUniformLocation(AS5_RndPrg, "MatrProj")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AS5_RndMatrProj.A[0]);
  if ((loc = glGetUniformLocation(AS5_RndPrg, "Time")) != -1)
    glUniform1f(loc, AS5_Anim.Time);


  /* Activete primitive vertex array */
  glBindVertexArray(Pr->VA);
  /* Activete primitive index buffer */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
  /* Draw primitive */
  glDrawElements(GL_TRIANGLES, Pr->NumOfI, GL_UNSIGNED_INT, NULL);
  glUseProgram(0);
} /* End of 'AS5_RndPrimDraw' function */

/* Primitive free function.
 * ARGUMENTS:
 *   - primtive to free:
 *       as5PRIM *Pr;
 * RETURNS: None.
 */
VOID AS5_RndPrimFree( as5PRIM *Pr )
{
  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Pr->VBuf);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Pr->VA);
  glDeleteBuffers(1, &Pr->IBuf);
  memset(Pr, 0, sizeof(as5PRIM));
} /* End of 'AS5_RndPrimFree' function */

