#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/perlin.hh"

#define y_count 100
#define x_count 100

void single_freq(perlin::PerlinGenerator &generator, FILE *file) {
	for (size_t y = 0; y < y_count; y++) {
		for (size_t x = 0; x < x_count; x++) {
			if (x != 0) fprintf(file, ",");

			float value = generator.sample_noise(x * 0.04, y * 0.04);
			fprintf(file, "%f", value);
		}
		fprintf(file, "\n");
	}
}

void multi_freq(perlin::PerlinGenerator &generator, float **data, float amp, float freq) {
	for (size_t y = 0; y < y_count; y++) {
		for (size_t x = 0; x < x_count; x++) {
			float value = amp * generator.sample_noise(x * freq, y * freq);
			data[y][x] += value;
		}
	}
}

void write_data(float **data, FILE *file) {
	for (size_t y = 0; y < y_count; y++) {
		for (size_t x = 0; x < x_count; x++) {
			if (x != 0) fprintf(file, ",");
			fprintf(file, "%f", data[y][x]);
		}
		fprintf(file, "\n");
	}
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
	FILE *file = fopen("./artifacts/output.txt", "w");

	perlin::PerlinGenerator generator = perlin::PerlinGenerator(256);

	if (argc == 1) {
		// use a preset frequency
		single_freq(generator, file);
	} else {
		// use pairs of input arguments as (amplitude, frequency)

		float **data = (float **)calloc(y_count, sizeof(float *));
		for (int i = 0; i < 100; i++) {
			data[i] = (float *)calloc(x_count, sizeof(float));
		}

		for (int i = 1; i < argc - 1; i += 2) {
			float amp = atof(argv[i]);
			float freq = atof(argv[i + 1]);
			multi_freq(generator, data, amp, freq);
		}

		write_data(data, file);
	}

	generator.close();
	fclose(file);

	return EXIT_SUCCESS;
}