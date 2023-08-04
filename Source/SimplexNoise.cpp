#include "SimplexNoise.h"

#define F 0.5f
#define G 0.25f

unsigned char perm[512] = { 151,160,137,91,90,15,
  131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
  190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
  88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
  77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
  102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
  135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
  5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
  223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
  129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
  251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
  49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
  138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
  151,160,137,91,90,15,
  131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
  190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
  88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
  77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
  102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
  135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
  5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
  223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
  129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
  251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
  49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
  138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

static float grad2lut[8][2] = {
  { -1.0f, -1.0f }, { 1.0f, 0.0f } , { -1.0f, 0.0f } , { 1.0f, 1.0f } ,
  { -1.0f, 1.0f } , { 0.0f, -1.0f } , { 0.0f, 1.0f } , { 1.0f, -1.0f }
};

static float grad3lut[16][3] = {
  { 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f },
  { -1.0f, 0.0f, 1.0f }, { 0.0f, -1.0f, 1.0f },
  { 1.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, -1.0f },
  { -1.0f, 0.0f, -1.0f }, { 0.0f, -1.0f, -1.0f },
  { 1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f },
  { -1.0f, 1.0f, 0.0f }, { -1.0f, -1.0f, 0.0f },
  { 1.0f, 0.0f, 1.0f }, { -1.0f, 0.0f, 1.0f },
  { 0.0f, 1.0f, -1.0f }, { 0.0f, -1.0f, -1.0f }
};

float grad1(int hash, float* gx) {
	int h = hash & 15;
	*gx = 1.0f + (h & 7);
	if (h & 8) *gx = -*gx;
	return 0;
}

void grad2(int hash, float* gx, float* gy) {
	int h = hash & 7;
	*gx = grad2lut[h][0];
	*gy = grad2lut[h][1];
	return;
}

void grad3(int hash, float* gx, float* gy, float* gz) {
	int h = hash & 15;
	*gx = grad3lut[h][0];
	*gy = grad3lut[h][1];
	*gz = grad3lut[h][2];
	return;
}

SimplexNoise::SimplexNoise(void)
{

}

SimplexNoise::SimplexNoise(int seed)
{
	setSeed(seed);
}


SimplexNoise::~SimplexNoise(void)
{
}

float SimplexNoise::Linear(float a, float b, float x)
{
	return  a * (1.0f - x) + b * x;
}

void SimplexNoise::setSeed(int seed)
{
	srand(seed);
	unsigned char temp[256];

	int i = 0;
	for (i = 0; i < 256; i++)
	{
		temp[i] = rand() % 256;
	}

	i = 0;
	for (i = 0; i < 512; i++)
	{
		perm[i] = temp[i % 256];
	}
}

float SimplexNoise::sdnoise1(float x, float* dnoise_dx)
{
	int i0 = floorf(x);
	int i1 = i0 + 1;
	float x0 = x - i0;
	float x1 = x0 - 1.0f;

	float gx0, gx1;
	float n0, n1;
	float t20, t40, t21, t41;

	float x20 = x0 * x0;
	float t0 = 1.0f - x20;
	t20 = t0 * t0;
	t40 = t20 * t20;
	grad1(perm[i0 & 0xff], &gx0);
	n0 = t40 * gx0 * x0;

	float x21 = x1 * x1;
	float t1 = 1.0f - x21;
	t21 = t1 * t1;
	t41 = t21 * t21;
	grad1(perm[i1 & 0xff], &gx1);
	n1 = t41 * gx1 * x1;
	*dnoise_dx = t20 * t0 * gx0 * x20;
	*dnoise_dx += t21 * t1 * gx1 * x21;
	*dnoise_dx *= -8.0f;
	*dnoise_dx += t40 * gx0 + t41 * gx1;
	*dnoise_dx *= 0.25f;
	return 0.25f * (n0 + n1);
}

float SimplexNoise::sdnoise2(float x, float y, float* dnoise_dx, float* dnoise_dy)
{
	float n0, n1, n2;
	float gx0, gy0, gx1, gy1, gx2, gy2;

	float s = (x + y) * F;
	float xs = x + s;
	float ys = y + s;
	int i = floorf(xs);
	int j = floorf(ys);

	float t = (float)(i + j) * G;
	float X0 = i - t;
	float Y0 = j - t;
	float x0 = x - X0;
	float y0 = y - Y0;
	int i1, j1;
	if (x0 > y0) { i1 = 1; j1 = 0; }
	else { i1 = 0; j1 = 1; }
	float x1 = x0 - i1 + G;
	float y1 = y0 - j1 + G;
	float x2 = x0 - 1.0f + 2.0f * G;
	float y2 = y0 - 1.0f + 2.0f * G;
	int ii = i % 256;
	int jj = j % 256;
	float t0 = 0.5f - x0 * x0 - y0 * y0;
	float t20, t40;
	if (t0 < 0.0f) t40 = t20 = t0 = n0 = gx0 = gy0 = 0.0f;
	else
	{
		grad2(perm[ii + perm[jj]], &gx0, &gy0);
		t20 = t0 * t0;
		t40 = t20 * t20;
		n0 = t40 * (gx0 * x0 + gy0 * y0);
	}

	float t1 = 0.5f - x1 * x1 - y1 * y1;
	float t21, t41;
	if (t1 < 0.0f) t21 = t41 = t1 = n1 = gx1 = gy1 = 0.0f;
	else {
		grad2(perm[ii + i1 + perm[jj + j1]], &gx1, &gy1);
		t21 = t1 * t1;
		t41 = t21 * t21;
		n1 = t41 * (gx1 * x1 + gy1 * y1);
	}

	float t2 = 0.5f - x2 * x2 - y2 * y2;
	float t22, t42;
	if (t2 < 0.0f) t42 = t22 = t2 = n2 = gx2 = gy2 = 0.0f;
	else {
		grad2(perm[ii + 1 + perm[jj + 1]], &gx2, &gy2);
		t22 = t2 * t2;
		t42 = t22 * t22;
		n2 = t42 * (gx2 * x2 + gy2 * y2);
	}
	float noise = 40.0f * (n0 + n1 + n2);
	if ((dnoise_dx != 0) && (dnoise_dy != 0))
	{
		float temp0 = t20 * t0 * (gx0 * x0 + gy0 * y0);
		*dnoise_dx = temp0 * x0;
		*dnoise_dy = temp0 * y0;
		float temp1 = t21 * t1 * (gx1 * x1 + gy1 * y1);
		*dnoise_dx += temp1 * x1;
		*dnoise_dy += temp1 * y1;
		float temp2 = t22 * t2 * (gx2 * x2 + gy2 * y2);
		*dnoise_dx += temp2 * x2;
		*dnoise_dy += temp2 * y2;
		*dnoise_dx *= -8.0f;
		*dnoise_dy *= -8.0f;
		*dnoise_dx += t40 * gx0 + t41 * gx1 + t42 * gx2;
		*dnoise_dy += t40 * gy0 + t41 * gy1 + t42 * gy2;
		*dnoise_dx *= 40.0f;
		*dnoise_dy *= 40.0f;
	}
	return noise;
}

float SimplexNoise::sdnoise3(float x, float y, float z, float* dnoise_dx, float* dnoise_dy, float* dnoise_dz)
{
	float n0, n1, n2, n3;
	float noise;
	float gx0, gy0, gz0, gx1, gy1, gz1;
	float gx2, gy2, gz2, gx3, gy3, gz3;

	float s = (x + y + z) * F;
	float xs = x + s;
	float ys = y + s;
	float zs = z + s;
	int i = floorf(xs);
	int j = floorf(ys);
	int k = floorf(zs);

	float t = (float)(i + j + k) * G;
	float X0 = i - t;
	float Y0 = j - t;
	float Z0 = k - t;
	float x0 = x - X0;
	float y0 = y - Y0;
	float z0 = z - Z0;

	int i1, j1, k1;
	int i2, j2, k2;
	if (x0 >= y0) {
		if (y0 >= z0)
		{
			i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
		}
		else if (x0 >= z0) { i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1; }
		else { i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1; }
	}
	else {
		if (y0 < z0) { i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1; }
		else if (x0 < z0) { i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1; }
		else { i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0; }
	}

	float x1 = x0 - i1 + G;
	float y1 = y0 - j1 + G;
	float z1 = z0 - k1 + G;
	float x2 = x0 - i2 + 2.0f * G;
	float y2 = y0 - j2 + 2.0f * G;
	float z2 = z0 - k2 + 2.0f * G;
	float x3 = x0 - 1.0f + 3.0f * G;
	float y3 = y0 - 1.0f + 3.0f * G;
	float z3 = z0 - 1.0f + 3.0f * G;

	int ii = i % 256;
	int jj = j % 256;
	int kk = k % 256;

	float t0 = 0.6f - x0 * x0 - y0 * y0 - z0 * z0;
	float t20, t40;
	if (t0 < 0.0f) n0 = t0 = t20 = t40 = gx0 = gy0 = gz0 = 0.0f;
	else {
		grad3(perm[ii + perm[jj + perm[kk]]], & gx0, & gy0, & gz0);
		t20 = t0 * t0;
		t40 = t20 * t20;
		n0 = t40 * (gx0 * x0 + gy0 * y0 + gz0 * z0);
	}

	float t1 = 0.6f - x1 * x1 - y1 * y1 - z1 * z1;
	float t21, t41;
	if (t1 < 0.0f) n1 = t1 = t21 = t41 = gx1 = gy1 = gz1 = 0.0f;
	else {
		grad3(perm[ii + i1 + perm[jj + j1 + perm[kk + k1]]], & gx1, & gy1, & gz1);
		t21 = t1 * t1;
		t41 = t21 * t21;
		n1 = t41 * (gx1 * x1 + gy1 * y1 + gz1 * z1);
	}

	float t2 = 0.6f - x2 * x2 - y2 * y2 - z2 * z2;
	float t22, t42;
	if (t2 < 0.0f) n2 = t2 = t22 = t42 = gx2 = gy2 = gz2 = 0.0f;
	else {
		grad3(perm[ii + i2 + perm[jj + j2 + perm[kk + k2]]], & gx2, & gy2, & gz2);
		t22 = t2 * t2;
		t42 = t22 * t22;
		n2 = t42 * (gx2 * x2 + gy2 * y2 + gz2 * z2);
	}

	float t3 = 0.6f - x3 * x3 - y3 * y3 - z3 * z3;
	float t23, t43;
	if (t3 < 0.0f) n3 = t3 = t23 = t43 = gx3 = gy3 = gz3 = 0.0f;
	else {
		grad3(perm[ii + 1 + perm[jj + 1 + perm[kk + 1]]], & gx3, & gy3, & gz3);
		t23 = t3 * t3;
		t43 = t23 * t23;
		n3 = t43 * (gx3 * x3 + gy3 * y3 + gz3 * z3);
	}

	noise = 28.0f * (n0 + n1 + n2 + n3);
	if ((dnoise_dx != 0) && (dnoise_dy != 0) && (dnoise_dz != 0))
	{
		float temp0 = t20 * t0 * (gx0 * x0 + gy0 * y0 + gz0 * z0);
		*dnoise_dx = temp0 * x0;
		*dnoise_dy = temp0 * y0;
		*dnoise_dz = temp0 * z0;
		float temp1 = t21 * t1 * (gx1 * x1 + gy1 * y1 + gz1 * z1);
		*dnoise_dx += temp1 * x1;
		*dnoise_dy += temp1 * y1;
		*dnoise_dz += temp1 * z1;
		float temp2 = t22 * t2 * (gx2 * x2 + gy2 * y2 + gz2 * z2);
		*dnoise_dx += temp2 * x2;
		*dnoise_dy += temp2 * y2;
		*dnoise_dz += temp2 * z2;
		float temp3 = t23 * t3 * (gx3 * x3 + gy3 * y3 + gz3 * z3);
		*dnoise_dx += temp3 * x3;
		*dnoise_dy += temp3 * y3;
		*dnoise_dz += temp3 * z3;
		*dnoise_dx *= -8.0f;
		*dnoise_dy *= -8.0f;
		*dnoise_dz *= -8.0f;
		*dnoise_dx += t40 * gx0 + t41 * gx1 + t42 * gx2 + t43 * gx3;
		*dnoise_dy += t40 * gy0 + t41 * gy1 + t42 * gy2 + t43 * gy3;
		*dnoise_dz += t40 * gz0 + t41 * gz1 + t42 * gz2 + t43 * gz3;
		*dnoise_dx *= 28.0f;
		*dnoise_dy *= 28.0f;
		*dnoise_dz *= 28.0f;
	}
	return noise;
}