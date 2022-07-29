#include <math.h>
#include <glmc.h>

void glmc_assign_vec2(t_vec2 dest, const t_vec2 source) {
	dest[XX] = source[XX];
	dest[YY] = source[YY];
}
void glmc_assign_ivec2(t_ivec2 dest, const t_ivec2 source) {
	dest[XX] = source[XX];
	dest[YY] = source[YY];
}
void glmc_assign_vec3(t_vec3 dest, const t_vec3 source) {
	dest[XX] = source[XX];
	dest[YY] = source[YY];
	dest[ZZ] = source[ZZ];
}
void glmc_assign_ivec3(t_ivec3 dest, const t_ivec3 source) {
	dest[XX] = source[XX];
	dest[YY] = source[YY];
	dest[ZZ] = source[ZZ];
}
void glmc_assign_vec4(t_vec4 dest, const t_vec4 source) {
	dest[XX] = source[XX];
	dest[YY] = source[YY];
	dest[ZZ] = source[ZZ];
	dest[WW] = source[WW];
}


void glmc_assign_ivec2_from_vec2(t_ivec2 dest, const t_vec2 source) {
	dest[XX] = (int) source[XX];
	dest[YY] = (int) source[YY];
}

void glmc_assign_vec2_from_ivec2(t_vec2 dest, const t_ivec2 source) {
	dest[XX] = (float) source[XX];
	dest[YY] = (float) source[YY];
}
void glmc_assign_ivec3_from_vec3(t_ivec3 dest, const t_vec3 source) {
	dest[XX] = (int) round(source[XX]);
	dest[YY] = (int) round(source[YY]);
	dest[ZZ] = (int) round(source[ZZ]);
}
void glmc_assign_vec3_from_ivec3(t_vec3 dest, const t_ivec3 source) {
	dest[XX] = (float) source[XX];
	dest[YY] = (float) source[YY];
	dest[ZZ] = (float) source[ZZ];
}


void glmc_vec2_set(t_vec2 dest, float x, float y) {
	dest[XX] = x;
	dest[YY] = y;
}
void glmc_ivec2_set(t_ivec2 dest, int x, int y) {
	dest[XX] = x;
	dest[YY] = y;
}
void glmc_vec3_set(t_vec3 dest, float x, float y, float z) {
	dest[XX] = x;
	dest[YY] = y;
	dest[ZZ] = z;
}
void glmc_ivec3_set(t_ivec3 dest, int x, int y, int z) {
	dest[XX] = x;
	dest[YY] = y;
	dest[ZZ] = z;
}
void glmc_vec4_set(t_vec4 dest, float x, float y, float z, float a) {
	dest[XX] = x;
	dest[YY] = y;
	dest[ZZ] = z;
	dest[WW] = a;
}

int glmc_equal_ivec2(const t_ivec2 a, const t_ivec2 b) {
	return a[XX] == b[XX] && a[YY] == b[YY];
}
int glmc_equal_ivec3(const t_ivec3 a, const t_ivec3 b) {
	return a[XX] == b[XX] && a[YY] == b[YY] && a[ZZ] == b[ZZ];
}
int glmc_equal_vec2(const t_vec2 a, const t_vec2 b) {
	return a[XX] == b[XX] && a[YY] == b[YY];
}
int glmc_equal_vec3(const t_vec3 a, const t_vec3 b) {
	return a[XX] == b[XX] && a[YY] == b[YY] && a[ZZ] == b[ZZ];
}

void glmc_vec3_multi(const t_vec3 source, float multi, t_vec3 dest) {
    dest[XX] = source[XX] * multi;
    dest[YY] = source[YY] * multi;
    dest[ZZ] = source[ZZ] * multi;
}

void glmc_add_vec3(const t_vec3 v1, const t_vec3 v2, t_vec3 dest)
{
    dest[XX] = v1[XX] + v2[XX];
    dest[YY] = v1[YY] + v2[YY];
    dest[ZZ] = v1[ZZ] + v2[ZZ];
}


void		glmc_vec3(float x, float y, float z, t_vec3 dest)
{
	dest[XX] = x;
	dest[YY] = y;
	dest[ZZ] = z;
}

void		glmc_vec4(float x, float y, float z, float w, t_vec4 dest)
{
	dest[XX] = x;
	dest[YY] = y;
	dest[ZZ] = z;
	dest[WW] = w;
}

void		glmc_sub_vec3(const t_vec3 v1, const t_vec3 v2, t_vec3 dest)
{
	dest[XX] = v1[XX] - v2[XX];
	dest[YY] = v1[YY] - v2[YY];
	dest[ZZ] = v1[ZZ] - v2[ZZ];
}

void		glmc_sub_vec4(const t_vec4 v1, const t_vec4 v2, t_vec4 dest)
{
	dest[XX] = v1[XX] - v2[XX];
	dest[YY] = v1[YY] - v2[YY];
	dest[ZZ] = v1[ZZ] - v2[ZZ];
	dest[WW] = v1[WW] - v2[WW];
}

void		glmc_cross(const t_vec3 v1, const t_vec3 v2, t_vec3 dest)
{
	dest[XX] = v1[YY] * v2[ZZ] - v1[ZZ] * v2[YY];
	dest[YY] = v1[ZZ] * v2[XX] - v1[XX] * v2[ZZ];
	dest[ZZ] = v1[XX] * v2[YY] - v1[YY] * v2[XX];
}

void		glmc_normalize(t_vec3 v)
{
	float 	magnitude;

	magnitude = sqrtf(v[XX] * v[XX] + v[YY] * v[YY] + v[ZZ] * v[ZZ]);
	if (magnitude == 0.0)
	{
		v[XX] = 0.0;
        	v[YY] = 0.0;
        	v[ZZ] = 0.0;
        	return ;
	}
	v[XX] /= magnitude;
	v[YY] /= magnitude;
	v[ZZ] /= magnitude;
}
