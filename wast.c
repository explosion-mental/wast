#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
char *argv0;
#include "arg.h"
#include "stb_image.h"
#include "stb_image_resize.h"

#define IMGRZ	512

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
order(ColorFreq *hist, int imgsz)
{
	size_t a; //count
	uint32_t b; //col

	for (int i = 0; i < imgsz; i++)  {
		for (int j = i + 1; j < imgsz; j++) {
			if (hist[i].count < hist[j].count) {
				a = hist[i].count;
				b = hist[i].color;
				hist[i].count = hist[j].count;
				hist[i].color = hist[j].color;
				hist[j].count = a;
				hist[j].color = b;
			}
		}
	}
}

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

static uint32_t *
load(const char *fname, int *width, int *height)
{
	uint32_t *image;
	const int channels = 4;	//always force 4 channels (RGBA)
	int w, h;

	if ((image = (uint32_t *)stbi_load(fname, &w, &h, NULL, channels)) == NULL) {
		fprintf(stderr, "Could not read file %s\n", fname);
		exit(1);
	}

	*width = w;
	*height = h;

	if (w <= IMGRZ || h <= IMGRZ) /* if the image is small enough */
		return image;

	float max;

	/* resize align to the longest side */
	if (w == h)
		max = w;
	else if (w > h)
		max = w;
	else if (h > w)
		max = h;

	int x = w * (512 / max);
	int y = h * (512 / max);

	unsigned char *resized = (unsigned char *)malloc(x * y * channels);

	stbir_resize_uint8((unsigned char *)image, w, h, 0, resized, x, y, 0, 4);

	*width = x;
	*height = y;

	return (uint32_t *)resized;
}

int
main(int argc, char *argv[])
{
	int w, h;

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

	uint32_t *image = load(argv[0], &w, &h);
	int imgsz = w * h; //image size
	ColorFreq hist[imgsz]; //array of the size of the image

	//descending sort, firsts on the array will be the most prominent
	//colors (the ones with the most counts)
	order(hist, imgsz);

	for (size_t i = 0; i < imgsz; i++) { //look up pixel by pixel
		addcolor(hist, image[i]);
	}

	for (size_t i = 0; i < 10; i++) { //outputs the result
		printf("#%06X: %zu\n", hist[i].color & 0x00ffffff, hist[i].count);
	}

	return 0;
}
