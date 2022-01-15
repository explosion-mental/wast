#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

char *argv0;
#include "arg.h"
#include "stb_image.h"

#define HISTOGRAM_CAP	256

typedef struct {
	uint32_t color;
	size_t count;
} ColorFreq;

static void
usage(void)
{
	fprintf(stderr, "usage: wast [image]\n");
	exit(1);
}
size_t size = 0;

static void
addcolor(ColorFreq *hist, uint32_t color)
{
	for (size_t i = 0; i < size; i++) {
		if (hist[i].color == color) {
			hist[i].count++;
			return;
		}
	}

	hist[size].color = color;
	hist[size].count = 1;
	size++;
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

	int imgsz = x * y; //image size
	ColorFreq hist[imgsz/10]; //array of the size of the image

	for (size_t i = 0; i < imgsz; i++) { //look up pixel by pixel
		addcolor(hist, data[i]);
	}

	for (size_t i = 0; i < size; i++) { //outputs the result
		printf("#%06X: %zu\n", (hist[i].color & 0x00ffffff), hist[i].count);
	}

	stbi_image_free(data);

	return 0;
}
