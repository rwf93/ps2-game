/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# (c) 2005 Naomi Peori <naomi@peori.ca>
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
*/

 int points_count_cube = 36;

 int points_cube[36] = {
   0,  1,  2,
   1,  2,  3,
   4,  5,  6,
   5,  6,  7,
   8,  9, 10,
   9, 10, 11,
  12, 13, 14,
  13, 14, 15,
  16, 17, 18,
  17, 18, 19,
  20, 21, 22,
  21, 22, 23
 };

 int vertex_count_cube = 24;

 VECTOR vertices_cube[24] = {
  {  10.00f,  10.00f,  10.00f, 1.00f },
  {  10.00f,  10.00f, -10.00f, 1.00f },
  {  10.00f, -10.00f,  10.00f, 1.00f },
  {  10.00f, -10.00f, -10.00f, 1.00f },
  { -10.00f,  10.00f,  10.00f, 1.00f },
  { -10.00f,  10.00f, -10.00f, 1.00f },
  { -10.00f, -10.00f,  10.00f, 1.00f },
  { -10.00f, -10.00f, -10.00f, 1.00f },
  { -10.00f,  10.00f,  10.00f, 1.00f },
  {  10.00f,  10.00f,  10.00f, 1.00f },
  { -10.00f,  10.00f, -10.00f, 1.00f },
  {  10.00f,  10.00f, -10.00f, 1.00f },
  { -10.00f, -10.00f,  10.00f, 1.00f },
  {  10.00f, -10.00f,  10.00f, 1.00f },
  { -10.00f, -10.00f, -10.00f, 1.00f },
  {  10.00f, -10.00f, -10.00f, 1.00f },
  { -10.00f,  10.00f,  10.00f, 1.00f },
  {  10.00f,  10.00f,  10.00f, 1.00f },
  { -10.00f, -10.00f,  10.00f, 1.00f },
  {  10.00f, -10.00f,  10.00f, 1.00f },
  { -10.00f,  10.00f, -10.00f, 1.00f },
  {  10.00f,  10.00f, -10.00f, 1.00f },
  { -10.00f, -10.00f, -10.00f, 1.00f },
  {  10.00f, -10.00f, -10.00f, 1.00f }
 };

VECTOR colours_cube[24] = {
  { 1.00f, 1.00f, 1.00f, 1.00f },
  { 1.00f, 1.00f, 1.00f, 1.00f },
  { 1.00f, 1.00f, 1.00f, 1.00f },
  { 1.00f, 1.00f, 1.00f, 1.00f },
  { 1.00f, 1.00f, 1.00f, 1.00f },
  { 1.00f, 1.00f, 1.00f, 1.00f },
  { 1.00f, 1.00f, 1.00f, 1.00f },
  { 1.00f, 1.00f, 1.00f, 1.00f },
  { 1.00f, 1.00f, 1.00f, 1.00f },
  { 1.00f, 1.00f, 1.00f, 1.00f },
  { 1.00f, 1.00f, 1.00f, 1.00f },
  { 1.00f, 1.00f, 1.00f, 1.00f },
  { 1.00f, 1.00f, 1.00f, 1.00f },
  { 1.00f, 1.00f, 1.00f, 1.00f },
  { 1.00f, 1.00f, 1.00f, 1.00f },
  { 1.00f, 1.00f, 1.00f, 1.00f },
  { 1.00f, 1.00f, 1.00f, 1.00f },
  { 1.00f, 1.00f, 1.00f, 1.00f },
  { 1.00f, 1.00f, 1.00f, 1.00f },
  { 1.00f, 1.00f, 1.00f, 1.00f },
  { 1.00f, 1.00f, 1.00f, 1.00f },
  { 1.00f, 1.00f, 1.00f, 1.00f },
  { 1.00f, 1.00f, 1.00f, 1.00f },
  { 1.00f, 1.00f, 1.00f, 1.00f }
 };

VECTOR coordinates_cube[24] = {
    {0.00f, 0.00f, 1.00f, 0.00f},
    {1.00f, 0.00f, 1.00f, 0.00f},
    {0.00f, 1.00f, 1.00f, 0.00f},
    {1.00f, 1.00f, 1.00f, 0.00f},
    {0.00f, 0.00f, 1.00f, 0.00f},
    {1.00f, 0.00f, 1.00f, 0.00f},
    {0.00f, 1.00f, 1.00f, 0.00f},
    {1.00f, 1.00f, 1.00f, 0.00f},
    {0.00f, 0.00f, 1.00f, 0.00f},
    {1.00f, 0.00f, 1.00f, 0.00f},
    {0.00f, 1.00f, 1.00f, 0.00f},
    {1.00f, 1.00f, 1.00f, 0.00f},
    {0.00f, 0.00f, 1.00f, 0.00f},
    {1.00f, 0.00f, 1.00f, 0.00f},
    {0.00f, 1.00f, 1.00f, 0.00f},
    {1.00f, 1.00f, 1.00f, 0.00f},
    {0.00f, 0.00f, 1.00f, 0.00f},
    {1.00f, 0.00f, 1.00f, 0.00f},
    {0.00f, 1.00f, 1.00f, 0.00f},
    {1.00f, 1.00f, 1.00f, 0.00f},
    {0.00f, 0.00f, 1.00f, 0.00f},
    {1.00f, 0.00f, 1.00f, 0.00f},
    {0.00f, 1.00f, 1.00f, 0.00f},
    {1.00f, 1.00f, 1.00f, 0.00f}
};