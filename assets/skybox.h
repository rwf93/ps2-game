/*
    SKYBOX.H && SKYBOX.O && SKYBOX.RAW && SKYBOX0-3.PNG
    testup skybox - using large draw for sky in ultra HD 256x256 lmao
    ignore this for now since koda is lazy and doesn't know how to skybox right

*/
#define VIEW_SIZE 2047 
#define NEG_VIEw_SIZE -2047
//we define view size as WORLD_SIZE-1, this variable is NOT dynamic...yet.
//i want to integrate this into the size of this cube so we can enable dynamic world sizes.
//or just grab from RENDER.C/H and use 
/*draw_convert_xyz(game->context.xyz, 2048, 2048, 32, model->vertex_count, (vertex_f_t*)game->context.shared_verticies);
                                       ^~~~~~~^~~~~~~~~~~~~~~~~these numbers if that makes sense.
*/
int collision_type = 0;
//this is also unused for now. It is to determine later if we want the skybox to have collisions on its faces. Koda wants cool fake colissions. 

//this is also intended to be used with the assosiated texture file

 int points_count = 36;

 int points[36] = {
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

 int vertex_count = 24;




 VECTOR vertices[24] = {
  {  2047.00f,  2047.00f,  2047.00f, 1.00f },
  {  2047.00f,  2047.00f, -2047.00f, 1.00f },
  {  2047.00f, -2047.00f,  2047.00f, 1.00f },
  {  2047.00f, -2047.00f, -2047.00f, 1.00f },
  { -2047.00f,  2047.00f,  2047.00f, 1.00f },
  { -2047.00f,  2047.00f, -2047.00f, 1.00f },
  { -2047.00f, -2047.00f,  2047.00f, 1.00f },
  { -2047.00f, -2047.00f, -2047.00f, 1.00f },
  { -2047.00f,  2047.00f,  2047.00f, 1.00f },
  {  2047.00f,  2047.00f,  2047.00f, 1.00f },
  { -2047.00f,  2047.00f, -2047.00f, 1.00f },
  {  2047.00f,  2047.00f, -2047.00f, 1.00f },
  { -2047.00f, -2047.00f,  2047.00f, 1.00f },
  {  2047.00f, -2047.00f,  2047.00f, 1.00f },
  { -2047.00f, -2047.00f, -2047.00f, 1.00f },
  {  2047.00f, -2047.00f, -2047.00f, 1.00f },
  { -2047.00f,  2047.00f,  2047.00f, 1.00f },
  {  2047.00f,  2047.00f,  2047.00f, 1.00f },
  { -2047.00f, -2047.00f,  2047.00f, 1.00f },
  {  2047.00f, -2047.00f,  2047.00f, 1.00f },
  { -2047.00f,  2047.00f, -2047.00f, 1.00f },
  {  2047.00f,  2047.00f, -2047.00f, 1.00f },
  { -2047.00f, -2047.00f, -2047.00f, 1.00f },
  {  2047.00f, -2047.00f, -2047.00f, 1.00f }
 };

 VECTOR colours[24] = {
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

 VECTOR coordinates[24] = { //works like indicies and also is technically how we map textures
  { 0.00f, 0.00f, 0.00f, 0.00f },
  { 1.00f, 0.00f, 0.00f, 0.00f },
  { 0.00f, 1.00f, 0.00f, 0.00f },
  { 1.00f, 1.00f, 0.00f, 0.00f },
  { 0.00f, 0.00f, 0.00f, 0.00f },
  { 1.00f, 0.00f, 0.00f, 0.00f },
  { 0.00f, 1.00f, 0.00f, 0.00f },
  { 1.00f, 1.00f, 0.00f, 0.00f },
  { 0.00f, 0.00f, 0.00f, 0.00f },
  { 1.00f, 0.00f, 0.00f, 0.00f },
  { 0.00f, 1.00f, 0.00f, 0.00f },
  { 1.00f, 1.00f, 0.00f, 0.00f },
  { 0.00f, 0.00f, 0.00f, 0.00f },
  { 1.00f, 0.00f, 0.00f, 0.00f },
  { 0.00f, 1.00f, 0.00f, 0.00f },
  { 1.00f, 1.00f, 0.00f, 0.00f },
  { 0.00f, 0.00f, 0.00f, 0.00f },
  { 1.00f, 0.00f, 0.00f, 0.00f },
  { 0.00f, 1.00f, 0.00f, 0.00f },
  { 1.00f, 1.00f, 0.00f, 0.00f },
  { 0.00f, 0.00f, 0.00f, 0.00f },
  { 1.00f, 0.00f, 0.00f, 0.00f },
  { 0.00f, 1.00f, 0.00f, 0.00f },
  { 1.00f, 1.00f, 0.00f, 0.00f }
 };
