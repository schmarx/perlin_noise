
#include <stdlib.h>

#include "perlin.hh"

#define rng(max) rand() % ((max) + 1)

namespace perlin {

float sample_rand(int max) {
	return rand() % (max + 1);
}

void shuffle(int *permutation, int max_size) {
	for (int i = 0; i < max_size; i++) {
		int index = sample_rand(max_size - 1);
		int tmp = permutation[index];
		permutation[index] = permutation[i];
		permutation[i] = tmp;
		permutation[i + max_size] = tmp; // wrap beyond the max
	}
}

float PerlinGenerator::ease(float t) {
	return ((6 * t - 15) * t + 10) * t * t * t;
}

float PerlinGenerator::lerp(float t, float v1, float v2) {
	return v1 + (v2 - v1) * t;
}

float PerlinGenerator::tr(int x, int y) { return permutation[permutation[x + 1] + y + 1]; }
float PerlinGenerator::tl(int x, int y) { return permutation[permutation[x] + y + 1]; }
float PerlinGenerator::br(int x, int y) { return permutation[permutation[x + 1] + y]; }
float PerlinGenerator::bl(int x, int y) { return permutation[permutation[x] + y]; }

float PerlinGenerator::val_dot(int v, float x, float y) {
	int index = v % 4;
	if (index == 0) return x + y;
	if (index == 1) return -x + y;
	if (index == 2) return x - y;
	return -x - y;
}

// max_size should be divisible by 4
PerlinGenerator::PerlinGenerator(int max_size) {
	this->max_size = max_size;

	permutation = (int *)calloc(max_size * 2, sizeof(int));

	for (int i = 0; i < max_size; i++) {
		permutation[i] = i;
	}

	shuffle(permutation, max_size);
}

// get the noise at position (x, y)
float PerlinGenerator::sample_noise(float x, float y) {
	int x_wrap = ((int)x) & (max_size - 1);
	int y_wrap = ((int)y) & (max_size - 1);

	float xr = x - ((int)x);
	float yr = y - ((int)y);

	float f1 = ease(xr);
	float f2 = ease(yr);

	float dot_tr = val_dot(tr(x_wrap, y_wrap), xr - 1, yr - 1);
	float dot_tl = val_dot(tl(x_wrap, y_wrap), xr, yr - 1);
	float dot_br = val_dot(br(x_wrap, y_wrap), xr - 1, yr);
	float dot_bl = val_dot(bl(x_wrap, y_wrap), xr, yr);

	return lerp(f1, lerp(f2, dot_bl, dot_tl), lerp(f2, dot_br, dot_tr));
}

void PerlinGenerator::close() {
	free(permutation);
}

}; // namespace perlin