#include <stdio.h>
#include <stdlib.h>
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

static void read_pcx(FILE * handle, void *buf, int buf_len, char *pal)
{
	unsigned char *buffer=buf;
	short c1;
	short a, b;
	long ofs1;
	if (buffer != 0) {
		fseek(handle, 128, SEEK_CUR);
		ofs1 = 0;
		while (ofs1 < buf_len) {
			a = fgetc(handle);
			if ((a & 0xc0) == 0xc0) {
				b = fgetc(handle);
				a &= 0x3f;
				for (c1 = 0; c1 < a && ofs1 < buf_len; c1++)
					buffer[ofs1++] = (char) b;
			} else
				buffer[ofs1++] = (char) a;
		}
		if (pal != 0) {
			fseek(handle, 1, SEEK_CUR);
			for (c1 = 0; c1 < 768; c1++)
				pal[c1] = fgetc(handle) >> 2;
		}
	}
}

static void write_pcx(FILE *pcxfile, unsigned char *data, int width, int height, unsigned char *palette)
{
	int    i;

	fputc(0x0a, pcxfile); /* manufacturer */
	fputc(5, pcxfile); /* version */
	fputc(1, pcxfile); /* encoding */
	fputc(8, pcxfile); /* bits_per_pixel */
	fputc(0, pcxfile); /* xmin */
	fputc(0, pcxfile);
	fputc(0, pcxfile); /* ymin */
	fputc(0, pcxfile);
	fputc((width - 1) & 0xff, pcxfile); /* xmax */
	fputc(((width - 1) >> 8) & 0xff, pcxfile);
	fputc((height - 1) & 0xff, pcxfile); /* ymax */
	fputc(((height - 1) >> 8) & 0xff, pcxfile);
	fputc(width & 0xff, pcxfile); /* hres */
	fputc((width >> 8) & 0xff, pcxfile);
	fputc(height & 0xff, pcxfile); /* vres */
	fputc((height >> 8) & 0xff, pcxfile);
	for (i = 0; i < 48; i++) /* palette */
		fputc(0, pcxfile);
	fputc(0, pcxfile); /* reserved */
	fputc(1, pcxfile); /* color_planes */
	fputc(width & 0xff, pcxfile); /* bytes_per_line */
	fputc((width >> 8) & 0xff, pcxfile);
	fputc(1 & 0xff, pcxfile); /* palette_type */
	fputc((1 >> 8) & 0xff, pcxfile);
	for (i = 0; i < 58; i++) /* filler */
		fputc(0, pcxfile);

	/* pack the image */

	for (i = 0 ; i < width*height ; i++)
		if ( (*data & 0xc0) != 0xc0)
			fputc(*data++, pcxfile);
		else
		{
			fputc(0xc1, pcxfile);
			fputc(*data++, pcxfile);
		}

	/* write the palette */

	fputc(0x0c, pcxfile); /* palette ID byte */
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

int write_gob(FILE *handle, gob_t *gob)
{
	int i;
	int offset;

	fputc((gob->num_images >> 0) & 0xff, handle);
	fputc((gob->num_images >> 8) & 0xff, handle);

	offset = 2 + (gob->num_images * 4);

	for (i=0; i<gob->num_images; i++) {
		fputc((offset >> 0) & 0xff, handle);
		fputc((offset >> 8) & 0xff, handle);
		fputc((offset >> 16) & 0xff, handle);
		fputc((offset >> 24) & 0xff, handle);

		offset += 8;
		offset += gob->width[i] * gob->height[i];
	}
	for (i=0; i<gob->num_images; i++) {
		fputc((gob->width[i] >> 0) & 0xff, handle);
		fputc((gob->width[i] >> 8) & 0xff, handle);

		fputc((gob->height[i] >> 0) & 0xff, handle);
		fputc((gob->height[i] >> 8) & 0xff, handle);

		fputc((gob->hs_x[i] >> 0) & 0xff, handle);
		fputc((gob->hs_x[i] >> 8) & 0xff, handle);

		fputc((gob->hs_y[i] >> 0) & 0xff, handle);
		fputc((gob->hs_y[i] >> 8) & 0xff, handle);

		fwrite(gob->data[i], 1, gob->width[i] * gob->height[i], handle);
	}
	return 0;
}

int main(int argc, char **argv)
{
	int usage = 0;
	int unpack = 0;
	FILE *f;
	int len;
	gob_t gob;
	char *filename = NULL;

	if (argc < 2)
		usage = 1;

	if (argc > 1)
		if (argv[1][0] == '-') {
			if (argv[1][1] == 'u') {
				if (argc < 3)
					usage = 1;
				unpack = 1;
			} else
				usage = 1;
		}
	
	if (usage) {
		printf("Usage: gobpack [-u] <file> [palette.pcx]\n\t-u to unpack the gob\n");
		return 1;
	}

	if (unpack) {
		int width, height;
		int x_count, y_count;
		int xi, yi;
		int i;
		unsigned char *data;
		unsigned char *dst;
		unsigned char palette[768];
		unsigned char *pal = NULL;

		if (argc > 3) {
			f = fopen(argv[3], "rb");
			if (f) {
				fseek(f, -769, SEEK_END);
				i = fgetc(f);
				if (i == 0x0c) {
					pal = palette;
					fread(pal, 1, 768, f);
				}
				fclose(f);
			}
		}

		filename = malloc(strlen(argv[2]) + 5);
		if (!filename) {
			printf("Not enough memory!\n");
			return -1;
		}

		strcpy(filename, argv[2]);
		strcat(filename, ".gob");
		f = fopen(filename, "rb");
		if (!f) {
			printf("Couldn't open file %s\n", filename);
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
			if (gob.height[i] > height)
				height = gob.height[i];
			if (gob.width[i] > width)
				width = gob.width[i];
		}
		width+=2;
		height+=2;

		data = malloc(400*256);
		if (!data) {
			printf("Not enough memory!\n");
			return -1;
		}
		memset(data, 0, 400*256);

		x_count = 400 / width;
		y_count = 256 / width;

		for (yi = 0; yi < y_count; yi++) {
			for (xi = 0; xi < x_count; xi++) {
				int x,y;
				unsigned char *src;

				i = yi * x_count + xi;
				if (i >= gob.num_images)
					continue;

				src = gob.data[i];
				dst = &data[(yi * height) * 400 + (xi * width)];
				for (y = 0; y < gob.height[i]; y++) {
					for (x = 0; x < gob.width[i]; x++) {
						dst[y * 400 + x] = src[y * gob.width[i] + x];
					}
				}
			}
		}
		
		strcpy(filename, argv[2]);
		strcat(filename, ".pcx");
		f = fopen(filename, "wb");
		if (!f) {
			printf("Couldn't open file %s\n", filename);
			return -1;
		}

		write_pcx(f, data, 400, 256, pal);

		fclose(f);

		strcpy(filename, argv[2]);
		strcat(filename, ".txt");
		f = fopen(filename, "w");
		if (!f) {
			printf("Couldn't open file %s\n", filename);
			return -1;
		}

		fprintf(f, "num_images: %i\n\n", gob.num_images);
		for (yi = 0; yi < y_count; yi++) {
			for (xi = 0; xi < x_count; xi++) {
				
				i = yi * x_count + xi;
				if (i >= gob.num_images)
					continue;

				fprintf(f, "image: %i\n", i + 1);
				fprintf(f, "x: %i\n", (xi * width));
				fprintf(f, "y: %i\n", (yi * height));
				fprintf(f, "width: %i\n", gob.width[i]);
				fprintf(f, "height: %i\n", gob.height[i]);
				fprintf(f, "hotspot_x: %i\n", gob.hs_x[i]);
				fprintf(f, "hotspot_y: %i\n", gob.hs_y[i]);
				fprintf(f, "\n");
			}
		}

		fclose(f);
	} else {
		unsigned char *data;
		int i = 0;
		int x_pos = 0, y_pos = 0;

		data = malloc(400*256);
		if (!data) {
			printf("Not enough memory!\n");
			return -1;
		}

		filename = malloc(strlen(argv[1]) + 5);
		if (!filename) {
			printf("Not enough memory!\n");
			return -1;
		}

		strcpy(filename, argv[1]);
		strcat(filename, ".pcx");
		f = fopen(filename, "rb");
		if (!f) {
			printf("Couldn't open file %s\n", filename);
			return -1;
		}

		read_pcx(f, data, 400*256, NULL);

		fclose(f);

		strcpy(filename, argv[1]);
		strcat(filename, ".txt");
		f = fopen(filename, "r");
		if (!f) {
			printf("Couldn't open file %s\n", filename);
			return -1;
		}

		gob.num_images = 0;

		while (!feof(f)) {
			char buffer[1024];
			int value;

			fscanf(f, "%s %i\n", buffer, &value);
			if (strcmp(buffer, "num_images:") == 0) {
				if (gob.num_images != 0) {
					printf("Parse error in %s\n", filename);
					return -1;
				}
				gob.num_images = value;
				gob.width = malloc(gob.num_images*sizeof(int));
				gob.height = malloc(gob.num_images*sizeof(int));
				gob.hs_x = malloc(gob.num_images*sizeof(int));
				gob.hs_y = malloc(gob.num_images*sizeof(int));
				gob.data = malloc(gob.num_images*sizeof(void *));
				gob.orig_data = malloc(gob.num_images*sizeof(void *));
			} else if (strcmp(buffer, "image:") == 0) {
				i = value - 1;
			} else if (strcmp(buffer, "x:") == 0) {
				x_pos = value;
			} else if (strcmp(buffer, "y:") == 0) {
				y_pos = value;
			} else if (strcmp(buffer, "width:") == 0) {
				gob.width[i] = value;
			} else if (strcmp(buffer, "height:") == 0) {
				gob.height[i] = value;
			} else if (strcmp(buffer, "hotspot_x:") == 0) {
				gob.hs_x[i] = value;
			} else if (strcmp(buffer, "hotspot_y:") == 0) {
				int x, y;
				unsigned char *dst;

				gob.hs_y[i] = value;
				gob.orig_data[i] = malloc(gob.width[i] * gob.height[i]);
				gob.data[i] = gob.orig_data[i];
				dst = gob.data[i];
				for (y = 0; y < gob.height[i]; y++) {
					for (x = 0; x < gob.width[i]; x++) {
						dst[y * gob.width[i] + x] = data[(y_pos + y) * 400 + (x_pos + x)];
					}
				}
			}
		}

		fclose(f);

		strcpy(filename, argv[1]);
		strcat(filename, ".gob");
		f = fopen(filename, "wb");
		if (!f) {
			printf("Couldn't open file %s\n", filename);
			return -1;
		}

		write_gob(f, &gob);

		fclose(f);

		printf("%s build\n", filename);
	}

	return 0;
}
