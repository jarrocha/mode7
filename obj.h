#ifndef OBJ_H
#define OBJ_H

/*
 * OBJ
 * ===
 *
 * Reads [Wavefront .OBJ][wiki] 3D models.
 *
 * ## Notes:
 *
 *  * *TODO:* Materials: `mtllib`, `usemtl` etc.
 *  * Faces containing more than 3 vertices are converted
 *    to triangle fans internally.
 *  * Faces can't belong to multiple groups.
 *
 * ## Links:
 *
 *  * [Wavefront .obj file][wiki]
 *  * [Object Files (.obj)][bourke]
 *  * <https://www.cs.cmu.edu/~mbz/personal/graphics/obj.html>
 *
 * [wiki]: https://en.wikipedia.org/wiki/Wavefront_.obj_file
 * [bourke]: http://paulbourke.net/dataformats/obj/
 */

typedef struct {
    double x, y, z, w;
} ObjVert;

typedef struct {
    double x, y, z;
} ObjNorm;

typedef struct {
    double u, v, w;
} ObjUVW;

typedef struct {
    int v, vt, vn;
} ObjFaceVert;

typedef struct {
    /* OBJ supports more than 3 vertices per face;
    I cater for that by just adding more triangles as a triangle strip.*/
    ObjFaceVert verts[3];
    int g, s;
} ObjFace;

typedef struct {
    char *name;
} ObjGroup;

typedef struct {
    unsigned int n, a;
    int *idx;
} ObjLine;

typedef struct ObjMesh {

    unsigned int nverts, averts;
    ObjVert *verts; /* Vertices */

    unsigned int nnorms, anorms;
    ObjNorm *norms; /* Normals */

    unsigned int ntexs, atexs;
    ObjUVW *texs;  /* Texture coordinates */

    unsigned int npspaces, apspaces;
    ObjUVW *pspaces;  /* Parameter space vertices */

    unsigned int nfaces, afaces;
    ObjFace *faces; /* Faces */

    unsigned int ngroups, agroups;
    ObjGroup *groups; /* Groups */

    unsigned int nlines, alines;
    ObjLine *lines; /* Lines */

    double xmin, xmax;
    double ymin, ymax;
    double zmin, zmax;
} ObjMesh;

typedef struct {
    enum {mtl_none, mtl_rgb, mtl_spectral, mtl_xyz} type;
    union {
        struct { double r, g, b; } rgb;
        struct { char *name; double factor; } spec;
        struct { double x, y, z; } xyz;
    };
} MtlColor;
typedef struct {
    char *name;
    MtlColor Ka;
    MtlColor Kd;
    MtlColor Ks;
    MtlColor Tf;
    double Tr;
    int illum;
    double Ns;
} Material;

typedef struct {
    unsigned int n, a;
    Material *mtls;
} MtlLibrary;

ObjMesh *obj_create();
ObjMesh *obj_load(const char *fname);
void obj_free(ObjMesh *obj);
int obj_save(ObjMesh *obj, const char *fname);
const char *obj_last_error();

/* Seek VEC_ADD_FUNCTION for the definition of these: */
ObjVert *obj_add_vert(ObjMesh *obj);
ObjNorm *obj_add_norm(ObjMesh *obj);
ObjUVW *obj_add_tex(ObjMesh *obj);
ObjUVW *obj_add_pspace(ObjMesh *obj);
ObjFace *obj_add_face(ObjMesh *obj);
ObjGroup *obj_add_group(ObjMesh *obj);
ObjLine *obj_add_line(ObjMesh *obj);
void obj_line_add_vtx(ObjLine *l, int i);

MtlLibrary *mtl_create();
void mtl_free(MtlLibrary *lib);
Material *mtl_add(MtlLibrary *lib, const char *name);
MtlLibrary *mtl_load(const char *fname);
int mtl_save(MtlLibrary *lib, const char *fname);

#endif
