#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int num_images;
	int *width;
	int *height;
	int *hs_x;
	int *hs_y;
	void **data;
	void **orig_data;
} gob_t;

//
// WritePCXfile
//

static void write_pcx(FILE *pcxfile, unsigned char *data, int width, int height, unsigned char *palette)
{
	int    i;

	fputc(0x0a, pcxfile); // manufacturer
	fputc(5, pcxfile); // version
	fputc(1, pcxfile); // encoding
	fputc(8, pcxfile); // bits_per_pixel
	fputc(0, pcxfile); // xmin
	fputc(0, pcxfile);
	fputc(0, pcxfile); // ymin
	fputc(0, pcxfile);
	fputc((width - 1) & 0xff, pcxfile); // xmax
	fputc(((width - 1) >> 8) & 0xff, pcxfile);
	fputc((height - 1) & 0xff, pcxfile); // ymax
	fputc(((height - 1) >> 8) & 0xff, pcxfile);
	fputc(width & 0xff, pcxfile); // hres
	fputc((width >> 8) & 0xff, pcxfile);
	fputc(height & 0xff, pcxfile); // vres
	fputc((height >> 8) & 0xff, pcxfile);
	for (i = 0; i < 48; i++) // palette
		fputc(0, pcxfile);
	fputc(0, pcxfile); // reserved
	fputc(1, pcxfile); // color_planes
	fputc(width & 0xff, pcxfile); // bytes_per_line
	fputc((width >> 8) & 0xff, pcxfile);
	fputc(1 & 0xff, pcxfile); // palette_type
	fputc((1 >> 8) & 0xff, pcxfile);
	for (i = 0; i < 58; i++) // filler
		fputc(0, pcxfile);

	// pack the image

	for (i = 0 ; i < width*height ; i++)
		if ( (*data & 0xc0) != 0xc0)
			fputc(*data++, pcxfile);
		else
		{
			fputc(0xc1, pcxfile);
			fputc(*data++, pcxfile);
		}

	// write the palette

	fputc(0x0c, pcxfile); // palette ID byte
	if (palette)
		for (i = 0 ; i < 768 ; i++)
			fputc(*palette++, pcxfile);
	else
		for (i = 0 ; i < 768 ; i++)
			fputc(i / 3, pcxfile);
}

int read_gob(FILE *handle, gob_t *gob, int len)
{
	unsigned char *gob_data;
	int i;

	gob_data = malloc(len);
	fread(gob_data, 1, len, handle);

	gob->num_images = (short)((gob_data[0]) + (gob_data[1] << 8));

	gob->width = malloc(gob->num_images*sizeof(int));
	gob->height = malloc(gob->num_images*sizeof(int));
	gob->hs_x = malloc(gob->num_images*sizeof(int));
	gob->hs_y = malloc(gob->num_images*sizeof(int));
	gob->data = malloc(gob->num_images*sizeof(void *));
	gob->orig_data = malloc(gob->num_images*sizeof(void *));
	for (i=0; i<gob->num_images; i++) {
		int image_size;
		int offset;

		offset = (gob_data[i*4+2]) + (gob_data[i*4+3] << 8) + (gob_data[i*4+4] << 16) + (gob_data[i*4+5] << 24);

		gob->width[i]  = (short)((gob_data[offset]) + (gob_data[offset+1] << 8)); offset += 2;
		gob->height[i] = (short)((gob_data[offset]) + (gob_data[offset+1] << 8)); offset += 2;
		gob->hs_x[i]   = (short)((gob_data[offset]) + (gob_data[offset+1] << 8)); offset += 2;
		gob->hs_y[i]   = (short)((gob_data[offset]) + (gob_data[offset+1] << 8)); offset += 2;

		image_size = gob->width[i] * gob->height[i];
		gob->orig_data[i] = malloc(image_size);
		memcpy(gob->orig_data[i], &gob_data[offset], image_size);
		gob->data[i] = (unsigned short *)gob->orig_data[i];
	}
	free(gob_data);
	return 0;
}

int main(int argc, char **argv)
{
	int usage = 0;
	int unpack = 0;
	FILE *f;
	int len;
	gob_t gob;

	if (argc < 4) {
		usage = 1;
	} else if (argc == 5) {
		unpack = 1;
		if ((argv[1][0] != '-') || (argv[1][1] != 'u'))
			usage = 1;
	}
	
	if (usage) {
		printf("Usage: gobpack [-u] <file.gob> <file.pcx> <filedefs.txt>\n\t-u to unpack the gob\n");
		return 1;
	}

	if (unpack) {
		int width, height;
		int i;
		unsigned char *data;
		unsigned char *dst;

		f = fopen(argv[2], "rb");
		if (!f) {
			printf("Couldn't open file %s\n", argv[2]);
			return -1;
		}
		fseek(f, 0, SEEK_END);
		len = ftell(f);
		fseek(f, 0, SEEK_SET);

		read_gob(f, &gob, len);

		fclose(f);

		width = 0;
		height = 0;
		for (i = 0; i < gob.num_images; i++) {
			height += gob.height[i];
			if (gob.width[i] > width)
				width = gob.width[i];
		}

		data = malloc(width*height);
		memset(data, 0, width*height);
		dst = data;

		for (i = 0; i < gob.num_images; i++) {
			int x,y;
			unsigned char *src;

			src = gob.data[i];
			for (y = 0; y < gob.height[i]; y++) {
				for (x = 0; x < gob.width[i]; x++) {
					dst[y * width + x] = src[y * gob.width[i] + x];
				}
			}
			dst += gob.height[i] * width;
		}
		
		f = fopen(argv[3], "wb");
		if (!f) {
			printf("Couldn't open file %s\n", argv[3]);
			return -1;
		}

		write_pcx(f, data, width, height, NULL);

		fclose(f);

		f = fopen(argv[4], "w");
		if (!f) {
			printf("Couldn't open file %s\n", argv[4]);
			return -1;
		}

		fprintf(f, "num_images: %i\n\n", gob.num_images);
		for (i = 0; i < gob.num_images; i++) {
			fprintf(f, "width: %i\n", gob.width[i]);
			fprintf(f, "height: %i\n", gob.height[i]);
			fprintf(f, "hotspot_x: %i\n", -gob.hs_x[i]);
			fprintf(f, "hotspot_y: %i\n", -gob.hs_y[i]);
			fprintf(f, "\n");
		}

		fclose(f);
	} else {
	}

	return 0;
}