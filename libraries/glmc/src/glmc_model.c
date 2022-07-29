#include <math.h>
#include <glmc.h>

void			glmc_model_matrix(t_vec3 scale_factor, t_vec3 rot_axis, float angle, t_vec3 translation, t_mat4 dest)
{
	t_mat4		identity;
	t_mat4		rotate;
	t_mat4		scale;

	glmc_identity(identity);
	glmc_scale(identity, scale_factor, scale);
	glmc_rotate(scale, angle, rot_axis, rotate);
	glmc_translate(rotate, translation, dest);
}

void			glmc_look_at(t_vec3 eye, t_vec3 target, t_vec3 up, t_mat4 dest)
{
	t_vec3		x;
	t_vec3		y;
	t_vec3		z;

	glmc_sub_vec3(eye, target, z);
	glmc_normalize(z);
	glmc_cross(up, z, x);
	glmc_normalize(x);
	glmc_cross(z, x, y);
	glmc_normalize(y);
	dest[0] = x[XX];
	dest[1] = y[XX];
	dest[2] = z[XX];
	dest[3] = 0.0;
	dest[4] = x[YY];
	dest[5] = y[YY];
	dest[6] = z[YY];
	dest[7] = 0.0;
	dest[8] = x[ZZ];
	dest[9] = y[ZZ];
	dest[10] = z[ZZ];
	dest[11] = 0.0;
	dest[12] = -(x[XX] * eye[XX] + x[YY] * eye[YY] + x[ZZ] * eye[ZZ]);
	dest[13] = -(y[XX] * eye[XX] + y[YY] * eye[YY] + y[ZZ] * eye[ZZ]);
	dest[14] = -(z[XX] * eye[XX] + z[YY] * eye[YY] + z[ZZ] * eye[ZZ]);
	dest[15] = 1.0;
}

void			glmc_perspective(const t_vec4 frustum, t_mat4 dest)
{
    //glmc did this:
    
//	t_vec4		clip;
//	float		rl;
//	float		tb;
//	float		fn;
    
//	clip[TOP] = frustum[NEAR] * tan(frustum[FOV] * M_PI / 360.0);
//	clip[RIGHT] = clip[TOP] * frustum[ASPECT];
//	clip[LEFT] = -clip[RIGHT];
//	clip[BOTTOM] = -clip[TOP];
//	rl = clip[RIGHT] - clip[LEFT];
//	tb = clip[TOP] - clip[BOTTOM];
//	fn = frustum[FAR] - frustum[NEAR];
//	dest[0] = (frustum[NEAR] * 2.0) / rl;
//	dest[1] = 0.0;
//	dest[2] = 0.0;
//	dest[3] = 0.0;
//	dest[4] = 0.0;
//	dest[5] = (frustum[NEAR] * 2.0) / tb;
//	dest[6] = 0.0;
//	dest[7] = 0.0;
//	dest[8] = (clip[RIGHT] + clip[LEFT]) / rl;
//	dest[9] = (clip[TOP] + clip[BOTTOM]) / tb;
//	dest[10] = -(frustum[FAR] + frustum[NEAR]) / fn;
//	dest[11] = -1.0;
//	dest[12] = 0.0;
//	dest[13] = 0.0;
//	dest[14] = -(frustum[FAR] * frustum[NEAR] * 2.0) / fn;
	
    //instead, we do what GLM does:
    
	float tanHalfFovy = tanf(frustum[FOV] / 2.0f);
	
	dest[0] = 1.0f / (frustum[ASPECT] * tanHalfFovy);
	dest[1] = 0.0;
	dest[2] = 0.0;
	dest[3] = 0.0;
	dest[4] = 0.0;
	dest[5] = 1.0f / tanHalfFovy;
	dest[6] = 0.0;
	dest[7] = 0.0;
	dest[8] = 0.0;
	dest[9] = 0.0;
	dest[10] = - (frustum[FAR] + frustum[NEAR]) / (frustum[FAR] - frustum[NEAR]);
	dest[11] = - 1.0f;
	dest[12] = 0.0;
	dest[13] = 0.0;
	dest[14] = - (2.0f * frustum[FAR] * frustum[NEAR]) / (frustum[FAR] - frustum[NEAR]);
	dest[15] = 0.0f;
}
