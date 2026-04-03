

namespace perlin {

class PerlinGenerator {
	int *permutation;

	float ease(float t);

	float lerp(float t, float v1, float v2);

	float tr(int x, int y);
	float tl(int x, int y);
	float br(int x, int y);
	float bl(int x, int y);

	float val_dot(int v, float x, float y);

  public:
	int max_size;

	PerlinGenerator(int max_size = 256); // max_size should be divisible by 4

	float sample_noise(float x, float y); // get the noise at position (x, y)
	void close();						  // call this when this generator is no longer needed
};

}; // namespace perlin