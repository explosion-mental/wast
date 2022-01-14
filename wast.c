#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

char *argv0;
#include "arg.h"
#include "stb_image.h"

#define HISTOGRAM_CAP	16

typedef struct {
	uint32_t color;
	size_t count;
} ColorFreq;

ColorFreq histogram[HISTOGRAM_CAP];
size_t histogram_sz = 0;

static void
histogram_add(uint32_t color)
{
	for (size_t i = 0; i < histogram_sz; i++) {
		if (histogram[i].color == color) {
			histogram[i].count++;
			return;
		}
	}

	assert(histogram_sz < HISTOGRAM_CAP);
	histogram[histogram_sz].color = color;
	histogram[histogram_sz].count = 1;
	histogram_sz += 1;
}

static void
usage(void)
{
	fprintf(stderr, "usage: wast [image]\n");
	exit(1);
}

int
main(int argc, char *argv[])
{
	int x, y;
	uint32_t *data;

	ARGBEGIN {
	case 'v':
		puts("wast-"VERSION);
		exit(0);
		break;
	default:
		usage();
	} ARGEND

	if (!argv[0])
		usage();

	if ((data = ((uint32_t *)stbi_load(argv[0], &x, &y, NULL, 4))) == NULL) {
		fprintf(stderr, "Could not read file %s\n", argv[0]);
		exit(1);
	}

	//printf("Image: %s\n", argv[0]);
	//printf("Size: %dx%d\n", x, y);


	//for (int i = 0; i < x * y; i++) {
	for (int i = 0; i < 100; i++) {
		histogram_add(data[i]);
	}

	for (size_t i = 0; i < histogram_sz; i++) {
		printf("%08X: %zu\n", histogram[i].color, histogram[i].count);
	}

	stbi_image_free(data);

	return 0;
}
