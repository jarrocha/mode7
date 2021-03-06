#ifndef MODE7_H
#define MODE7_H

/* 3-dimensional vector structure */
typedef struct vector3 {
    double x,y,z;
} Vector3;

/* Some vector routines */
Vector3 v3(double x, double y, double z);
Vector3 v3_add(Vector3 v1, Vector3 v2);
Vector3 v3_sub(Vector3 v1, Vector3 v2);
int v3_equal(Vector3 v1, Vector3 v2);
double v3_dot(Vector3 v1, Vector3 v2);
Vector3 v3_cross(Vector3 v1, Vector3 v2);
Vector3 v3_normalize(Vector3 v);

/* Initializes the module. x,y,w,h defines the viewport
in which the mode-7 stuff will be drawn */
void m7_init(int x, int y, int w, int h);

void m7_dims(int *X, int *Y, int *W, int *H);

/* Cleans up the module. */
void m7_deinit();

/* Clears the z-buffer, needs to be called on every frame */
void m7_clear_zbuf();

/* Getters and setters for the camera parameters */
void m7_set_camera_pos(double x, double y, double z);
void m7_get_camera_pos(double *x, double *y, double *z);
void m7_set_camera_ang(double phi, double theta);
void m7_get_camera_ang(double *phi, double *theta);

/* Sets the camera angle such that it looks at (x,y,z) from its current position */
void m7_lookat(double x, double y, double z);

/* Enable fog, of the specified color */
void m7_enable_fog(unsigned int color);

/* Disables fog */
void m7_disable_fog();

enum M7_ANCHOR { M7_ANCHOR_CENTER, M7_ANCHOR_BOTTOM };
void m7_anchor_mode(enum M7_ANCHOR mode);

/* Enables/disables drawing backfaces */
void m7_backface(int enable);

/* Function that will be used to draw the floor through `m7_draw_floor()`
 * `pwx` and `pwz` are the x and z coordinates of the pixel on the floor.
 * `data` is passed unaltered through `m7_draw_floor()`.
 * It should return the color of the pixel at `pwx,pxz` on the floor.
 */
typedef unsigned int (*m7_plotfun)(void *data, double pwx, double pwz);

/* Draws the floor using a specific `plotfun`; `data` is passed to plotfun unaltered.
 */
void m7_draw_floor(Bitmap *dst, m7_plotfun plotfun, void *data);

/* Draws the bitmap `src` as a skybox on the bitmap `dst` using the current camera
 * parameters.
 * It is stretched on the x dimension to span the entire circumference of
 * the horizon. It is stretched to `height` pixels on the y dimension.
 * `background` is the color of pixels above the skybox.
 */
void m7_draw_skybox(Bitmap *dst, Bitmap *src, int height, unsigned int background);

/* Draws a sprite at position (wx, wy, wz) in the world on the bitmap dst,
 * scaled according to its distance from the camera, and taking the z-buffer
 * into account.
 * The sprite is blitted from the bitmap src in the ara defined by sx, sy, sw, sh.
 * The color of the src bitmap is used as a mask color.
 * The value of MODE7_ANCHOR_BOTTOM determines whether the sprite is anchored at the
 * bottom (useful for sprites walking on the floor) or anchored at the center
 * (useful for sprites floating in the air)
 */
void m7_draw_sprite(Bitmap *dst, double wx, double wy, double wz, Bitmap *src, int sx, int sy, int sw, int sh);

/* Draws a filled triangle with world coordinates given by the 3 vectors in `tri`.
 * It uses the color of the bitmap `bmp` (as per `bm_set_color()`).
 */
void m7_draw_tri(Bitmap *bmp, Vector3 tri[3]);

#ifdef OBJ_H
/* Draws a OBJ mesh `obj` at position `pos` in the world, rotated around the y-axis by `yrot`
 * using `color`.
 */
void m7_draw_obj(Bitmap *dst, ObjMesh *obj, Vector3 pos, double yrot, unsigned int color);
#endif

/* Draws a line in 3D space from `p0` to `p1`.
 * It takes the Z-buffer into account.
 */
void m7_line(Bitmap *bmp, Vector3 p0, Vector3 p1);

/* Finds the angle of an object facing in direction `phi_o`
 * relative to the camera.
 * It is used to choose the appropriate sprite for an object.
 */
double m7_rel_angle(double phi_o);

/* Enables/disables the stencil buffer */
void m7_stencil_enable(int enable);

/* Sets the color of the stencil buffer */
void m7_set_stencil(unsigned int color);

/* Clears the stencil buffer to zeros */
void m7_clear_stencil();

/* Gets the value of the stencil buffer at the point `x,y` */
unsigned int m7_stencil_enable_at(int x, int y);

/* Retrieves the `Bitmap` of the stencil buffer */
Bitmap *m7_get_stencil();

/* Projects a point `p` using the Mode7 parameters.
 * The projected point is stored in `*o`.
 * Returns 1 on success, 0 if the point was clipped.
 */
int m7_project(Vector3 p, Vector3 *o);

#endif  /* MODE7_H */
