#ifndef GLMC_LIBRARY_H
# define GLMC_LIBRARY_H

#ifdef __cplusplus
extern "C" {
#endif

enum	e_indexes
{
	XX = 0,
	YY,
	ZZ,
	WW
};

enum	e_frustum
{
	FOV = 0,
	ASPECT,
	NEAR,
	FAR
};

enum	e_clip
{
	LEFT = 0,
	RIGHT,
	BOTTOM,
	TOP
};

typedef int	t_ivec2[2];
typedef int	t_ivec3[3];
typedef int	t_ivec4[4];

typedef float	t_vec2[2];
typedef float	t_vec3[3];
typedef float	t_vec4[4];
typedef float	t_mat4[16];

/*
** glmc_model.c
*/
void			glmc_model_matrix(t_vec3 scale_factor, t_vec3 rot_axis, float angle, t_vec3 translation, t_mat4 dest);
void			glmc_look_at(t_vec3 eye, t_vec3 target, t_vec3 up, t_mat4 dest);
void			glmc_perspective(const t_vec4 frustum, t_mat4 dest);

/*
** glmc_matrix.c
*/
void			glmc_identity(t_mat4 dest);
void			glmc_rotate(const t_mat4 m, float angle, t_vec3 a, t_mat4 dest);
void			glmc_scale(const t_mat4 m, const t_vec3 scale, t_mat4 dest);
void			glmc_translate(const t_mat4 m, const t_vec3 vec, t_mat4 dest);

/*
** glmc_vector.c
*/

void glmc_assign_vec2(t_vec2 dest, const t_vec2 source);
void glmc_assign_ivec2(t_ivec2 dest, const t_ivec2 source);
void glmc_assign_vec3(t_vec3 dest, const t_vec3 source);
void glmc_assign_ivec3(t_ivec3 dest, const t_ivec3 source);
void glmc_assign_vec4(t_vec4 dest, const t_vec4 source);

void glmc_assign_ivec2_from_vec2(t_ivec2 dest, const t_vec2 source);
void glmc_assign_vec2_from_ivec2(t_vec2 dest, const t_ivec2 source);
void glmc_assign_ivec3_from_vec3(t_ivec3 dest, const t_vec3 source);
void glmc_assign_vec3_from_ivec3(t_vec3 dest, const t_ivec3 source);

void glmc_vec2_set(t_vec2 dest, float x, float y);
void glmc_ivec2_set(t_ivec2 dest, int x, int y);
void glmc_vec3_set(t_vec3 dest, float x, float y, float z);
void glmc_ivec3_set(t_ivec3 dest, int x, int y, int z);
void glmc_vec4_set(t_vec4 dest, float x, float y, float z, float a);

int glmc_equal_ivec2(const t_ivec2 a, const t_ivec2 b);
int glmc_equal_ivec3(const t_ivec3 a, const t_ivec3 b);
int glmc_equal_vec2(const t_vec2 a, const t_vec2 b);
int glmc_equal_vec3(const t_vec3 a, const t_vec3 b);

void glmc_vec3_multi(const t_vec3 source, float multi, t_vec3 dest);
void glmc_add_vec3(const t_vec3 v1, const t_vec3 v2, t_vec3 dest);

void			glmc_vec3(float x, float y, float z, t_vec3 dest);
void			glmc_vec4(float x, float y, float z, float w, t_vec4 dest);
void			glmc_sub_vec3(const t_vec3 v1, const t_vec3 v2, t_vec3 dest);
void			glmc_sub_vec4(const t_vec4 v1, const t_vec4 v2, t_vec4 dest);
void			glmc_cross(const t_vec3 v1, const t_vec3 v2, t_vec3 dest);
void			glmc_normalize(t_vec3 v);

#ifdef __cplusplus
}
#endif

#endif /* ! GLMC_LIBRARY_H */
