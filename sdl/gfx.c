#include "globals.h"

static int current_pal[256];
static SDL_Surface *jnb_surface;
static int fullscreen = 0;
static int vinited = 0;
static pixel_t screen_buffer[2][JNB_SURFACE_WIDTH*JNB_SURFACE_HEIGHT];
static int drawing_enable = 0;
static pixel_t *background = NULL;
static int background_drawn;
static pixel_t *mask = NULL;

pixel_t *get_vgaptr(int page, int x, int y)
{
	assert(drawing_enable==1);

	return &screen_buffer[page][y*JNB_SURFACE_WIDTH+x];
}


void open_screen(void)
{
	int lval = 0;

	lval = SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_AUDIO);
	if (lval < 0) {
		fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	if (fullscreen)
		jnb_surface = SDL_SetVideoMode(JNB_SURFACE_WIDTH, JNB_SURFACE_HEIGHT, JNB_BPP, SDL_SWSURFACE | SDL_FULLSCREEN);
	else
		jnb_surface = SDL_SetVideoMode(JNB_SURFACE_WIDTH, JNB_SURFACE_HEIGHT, JNB_BPP, SDL_SWSURFACE);

	if (!jnb_surface) {
		fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_ShowCursor(0);

	vinited = 1;

	memset(current_pal, 0, sizeof(current_pal));

	return;
}


void fs_toggle()
{
	if (!vinited) {
		fullscreen ^= 1;
		return;
	}
	if (SDL_WM_ToggleFullScreen(jnb_surface))
		fullscreen ^= 1;
}


void wait_vrt(int mix)
{
	return;
}


void clear_page(int page, int color)
{
	int i,j;
	pixel_t *buf = get_vgaptr(page, 0, 0);

	assert(drawing_enable==1);

	for (i=0; i<JNB_SURFACE_HEIGHT; i++)
		for (j=0; j<JNB_SURFACE_WIDTH; j++)
			*buf++ = color;
}


void clear_lines(int page, int y, int count, pixel_t color)
{
	int i,j;

	assert(drawing_enable==1);
#ifdef SCALE_UP2
	count *= 2;
	y *= 2;
#endif

	for (i=0; i<count; i++)
		if ((i+y)<JNB_SURFACE_HEIGHT) {
			pixel_t *buf = get_vgaptr(page, 0, i+y);
			for (j=0; j<JNB_SURFACE_WIDTH; j++)
				*buf++ = color;
		}
}


pixel_t get_color(int color, char pal[768])
{
	assert(color<256);
	assert(pal);
	return SDL_MapRGB(jnb_surface->format, (Uint8)(pal[color*3+0]<<2), (Uint8)(pal[color*3+1]<<2), (Uint8)(pal[color*3+2]<<2));
}


pixel_t get_pixel(int page, int x, int y)
{
	assert(drawing_enable==1);
#ifdef SCALE_UP2
	x *= 2;
	y *= 2;
#endif
	assert(x<JNB_SURFACE_WIDTH);
	assert(y<JNB_SURFACE_HEIGHT);

	return *get_vgaptr(page, x, y);
}


void set_pixel(int page, int x, int y, pixel_t color)
{
	assert(drawing_enable==1);
#ifdef SCALE_UP2
	x *= 2;
	y *= 2;
#endif
	assert(x<JNB_SURFACE_WIDTH);
	assert(y<JNB_SURFACE_HEIGHT);

	*get_vgaptr(page, x, y) = color;
}


static unsigned int colorMask = 0xF7DEF7DE;
static unsigned int lowPixelMask = 0x08210821;
static unsigned int qcolorMask = 0xE79CE79C;
static unsigned int qlowpixelMask = 0x18631863;
static unsigned int redblueMask = 0xF81F;
static unsigned int greenMask = 0x7E0;

int Init_2xSaI (unsigned int BitFormat)
{
    if (BitFormat == 565)
    {
	colorMask = 0xF7DEF7DE;
	lowPixelMask = 0x08210821;
	qcolorMask = 0xE79CE79C;
	qlowpixelMask = 0x18631863;
	redblueMask = 0xF81F;
	greenMask = 0x7E0;
    }
    else if (BitFormat == 555)
    {
	colorMask = 0x7BDE7BDE;
	lowPixelMask = 0x04210421;
	qcolorMask = 0x739C739C;
	qlowpixelMask = 0x0C630C63;
	redblueMask = 0x7C1F;
	greenMask = 0x3E0;
    }
    else
    {
	return 0;
    }

#ifdef MMX
    Init_2xSaIMMX (BitFormat);
#endif

    return 1;
}


void Super2xSaI (unsigned char *src, unsigned int src_pitch, int src_bytes_per_pixel,
		 unsigned char *dst, unsigned int dst_pitch, int dst_bytes_per_pixel,
		 int width, int height, int pal[256])
{
#define GET_RESULT(A, B, C, D) ((A != C || A != D) - (B != C || B != D))

#define INTERPOLATE(A, B) (((A & colorMask) >> 1) + ((B & colorMask) >> 1) + (A & B & lowPixelMask))

#define Q_INTERPOLATE(A, B, C, D) ((A & qcolorMask) >> 2) + ((B & qcolorMask) >> 2) + ((C & qcolorMask) >> 2) + ((D & qcolorMask) >> 2) \
	+ ((((A & qlowpixelMask) + (B & qlowpixelMask) + (C & qlowpixelMask) + (D & qlowpixelMask)) >> 2) & qlowpixelMask)

#define GET_COLOR(x) (pal[(x)])

	unsigned char *src_line[4];
	unsigned char *dst_line[2];
	int x, y;
	unsigned long color[16];

	/* Point to the first 3 lines. */
	src_line[0] = src;
	src_line[1] = src;
	src_line[2] = src + src_pitch;
	src_line[3] = src + (src_pitch * 2);
	
	dst_line[0] = dst;
	dst_line[1] = dst + dst_pitch;
	
	x = 0, y = 0;
	
	if (src_bytes_per_pixel == 1) {
		unsigned char *sbp;
		sbp = src_line[0];
		color[0] = GET_COLOR(*sbp);       color[1] = color[0];   color[2] = color[0];    color[3] = color[0];
		color[4] = color[0];   color[5] = color[0];   color[6] = GET_COLOR(*(sbp + 1));  color[7] = GET_COLOR(*(sbp + 2));
		sbp = src_line[2];
		color[8] = GET_COLOR(*sbp);     color[9] = color[8];     color[10] = GET_COLOR(*(sbp + 1)); color[11] = GET_COLOR(*(sbp + 2));
		sbp = src_line[3];
		color[12] = GET_COLOR(*sbp);    color[13] = color[12];   color[14] = GET_COLOR(*(sbp + 1)); color[15] = GET_COLOR(*(sbp + 2));
	} else if (src_bytes_per_pixel == 2) {
		unsigned short *sbp;
		sbp = (unsigned short*)src_line[0];
		color[0] = *sbp;       color[1] = color[0];   color[2] = color[0];    color[3] = color[0];
		color[4] = color[0];   color[5] = color[0];   color[6] = *(sbp + 1);  color[7] = *(sbp + 2);
		sbp = (unsigned short*)src_line[2];
		color[8] = *sbp;     color[9] = color[8];     color[10] = *(sbp + 1); color[11] = *(sbp + 2);
		sbp = (unsigned short*)src_line[3];
		color[12] = *sbp;    color[13] = color[12];   color[14] = *(sbp + 1); color[15] = *(sbp + 2);
	} else {
		unsigned long *lbp;
		lbp = (unsigned long*)src_line[0];
		color[0] = *lbp;       color[1] = color[0];   color[2] = color[0];    color[3] = color[0];
		color[4] = color[0];   color[5] = color[0];   color[6] = *(lbp + 1);  color[7] = *(lbp + 2);
		lbp = (unsigned long*)src_line[2];
		color[8] = *lbp;     color[9] = color[8];     color[10] = *(lbp + 1); color[11] = *(lbp + 2);
		lbp = (unsigned long*)src_line[3];
		color[12] = *lbp;    color[13] = color[12];   color[14] = *(lbp + 1); color[15] = *(lbp + 2);
	}

	for (y = 0; y < height; y++) {
	
		/* Todo: x = width - 2, x = width - 1 */
		
		for (x = 0; x < width; x++) {
			unsigned long product1a, product1b, product2a, product2b;

//---------------------------------------  B0 B1 B2 B3    0  1  2  3
//                                         4  5* 6  S2 -> 4  5* 6  7
//                                         1  2  3  S1    8  9 10 11
//                                         A0 A1 A2 A3   12 13 14 15
//--------------------------------------
			if (color[9] == color[6] && color[5] != color[10]) {
				product2b = color[9];
				product1b = product2b;
			}
			else if (color[5] == color[10] && color[9] != color[6]) {
				product2b = color[5];
				product1b = product2b;
			}
			else if (color[5] == color[10] && color[9] == color[6]) {
				int r = 0;

				r += GET_RESULT(color[6], color[5], color[8], color[13]);
				r += GET_RESULT(color[6], color[5], color[4], color[1]);
				r += GET_RESULT(color[6], color[5], color[14], color[11]);
				r += GET_RESULT(color[6], color[5], color[2], color[7]);

				if (r > 0)
					product1b = color[6];
				else if (r < 0)
					product1b = color[5];
				else
					product1b = INTERPOLATE(color[5], color[6]);
					
				product2b = product1b;

			}
			else {
				if (color[6] == color[10] && color[10] == color[13] && color[9] != color[14] && color[10] != color[12])
					product2b = Q_INTERPOLATE(color[10], color[10], color[10], color[9]);
				else if (color[5] == color[9] && color[9] == color[14] && color[13] != color[10] && color[9] != color[15])
					product2b = Q_INTERPOLATE(color[9], color[9], color[9], color[10]);
				else
					product2b = INTERPOLATE(color[9], color[10]);

				if (color[6] == color[10] && color[6] == color[1] && color[5] != color[2] && color[6] != color[0])
					product1b = Q_INTERPOLATE(color[6], color[6], color[6], color[5]);
				else if (color[5] == color[9] && color[5] == color[2] && color[1] != color[6] && color[5] != color[3])
					product1b = Q_INTERPOLATE(color[6], color[5], color[5], color[5]);
				else
					product1b = INTERPOLATE(color[5], color[6]);
			}

			if (color[5] == color[10] && color[9] != color[6] && color[4] == color[5] && color[5] != color[14])
				product2a = INTERPOLATE(color[9], color[5]);
			else if (color[5] == color[8] && color[6] == color[5] && color[4] != color[9] && color[5] != color[12])
				product2a = INTERPOLATE(color[9], color[5]);
			else
				product2a = color[9];

			if (color[9] == color[6] && color[5] != color[10] && color[8] == color[9] && color[9] != color[2])
				product1a = INTERPOLATE(color[9], color[5]);
			else if (color[4] == color[9] && color[10] == color[9] && color[8] != color[5] && color[9] != color[0])
				product1a = INTERPOLATE(color[9], color[5]);
			else
				product1a = color[5];
	
			if (dst_bytes_per_pixel == 2) {
				*((unsigned long *) (&dst_line[0][x * 4])) = product1a | (product1b << 16);
				*((unsigned long *) (&dst_line[1][x * 4])) = product2a | (product2b << 16);
			} else {
				*((unsigned long *) (&dst_line[0][x * 8])) = product1a;
				*((unsigned long *) (&dst_line[0][x * 8 + 4])) = product1b;
				*((unsigned long *) (&dst_line[1][x * 8])) = product2a;
				*((unsigned long *) (&dst_line[1][x * 8 + 4])) = product2b;
			}
			
			/* Move color matrix forward */
			color[0] = color[1]; color[4] = color[5]; color[8] = color[9];   color[12] = color[13];
			color[1] = color[2]; color[5] = color[6]; color[9] = color[10];  color[13] = color[14];
			color[2] = color[3]; color[6] = color[7]; color[10] = color[11]; color[14] = color[15];
			
			if (x < width - 3) {
				x+=3;
				if (src_bytes_per_pixel == 1) {
					color[3] = GET_COLOR(*(((unsigned char*)src_line[0]) + x));
					color[7] = GET_COLOR(*(((unsigned char*)src_line[1]) + x));
					color[11] = GET_COLOR(*(((unsigned char*)src_line[2]) + x));
					color[15] = GET_COLOR(*(((unsigned char*)src_line[3]) + x));
				} else if (src_bytes_per_pixel == 2) {
					color[3] = *(((unsigned short*)src_line[0]) + x);					
					color[7] = *(((unsigned short*)src_line[1]) + x);
					color[11] = *(((unsigned short*)src_line[2]) + x);
					color[15] = *(((unsigned short*)src_line[3]) + x);
				} else {
					color[3] = *(((unsigned long*)src_line[0]) + x);
					color[7] = *(((unsigned long*)src_line[1]) + x);
					color[11] = *(((unsigned long*)src_line[2]) + x);
					color[15] = *(((unsigned long*)src_line[3]) + x);
				}
				x-=3;
			}
		}

		/* We're done with one line, so we shift the source lines up */
		src_line[0] = src_line[1];
		src_line[1] = src_line[2];
		src_line[2] = src_line[3];		

		/* Read next line */
		if (y + 3 >= height)
			src_line[3] = src_line[2];
		else
			src_line[3] = src_line[2] + src_pitch;
			
		/* Then shift the color matrix up */
		if (src_bytes_per_pixel == 1) {
			unsigned char *sbp;
			sbp = src_line[0];
			color[0] = GET_COLOR(*sbp);     color[1] = color[0];    color[2] = GET_COLOR(*(sbp + 1));  color[3] = GET_COLOR(*(sbp + 2));
			sbp = src_line[1];
			color[4] = GET_COLOR(*sbp);     color[5] = color[4];    color[6] = GET_COLOR(*(sbp + 1));  color[7] = GET_COLOR(*(sbp + 2));
			sbp = src_line[2];
			color[8] = GET_COLOR(*sbp);     color[9] = color[8];    color[10] = GET_COLOR(*(sbp + 1)); color[11] = GET_COLOR(*(sbp + 2));
			sbp = src_line[3];
			color[12] = GET_COLOR(*sbp);    color[13] = color[12];  color[14] = GET_COLOR(*(sbp + 1)); color[15] = GET_COLOR(*(sbp + 2));
		} else if (src_bytes_per_pixel == 2) {
			unsigned short *sbp;
			sbp = (unsigned short*)src_line[0];
			color[0] = *sbp;     color[1] = color[0];    color[2] = *(sbp + 1);  color[3] = *(sbp + 2);
			sbp = (unsigned short*)src_line[1];
			color[4] = *sbp;     color[5] = color[4];    color[6] = *(sbp + 1);  color[7] = *(sbp + 2);
			sbp = (unsigned short*)src_line[2];
			color[8] = *sbp;     color[9] = color[9];    color[10] = *(sbp + 1); color[11] = *(sbp + 2);
			sbp = (unsigned short*)src_line[3];
			color[12] = *sbp;    color[13] = color[12];  color[14] = *(sbp + 1); color[15] = *(sbp + 2);
		} else {
			unsigned long *lbp;
			lbp = (unsigned long*)src_line[0];
			color[0] = *lbp;     color[1] = color[0];    color[2] = *(lbp + 1);  color[3] = *(lbp + 2);
			lbp = (unsigned long*)src_line[1];
			color[4] = *lbp;     color[5] = color[4];    color[6] = *(lbp + 1);  color[7] = *(lbp + 2);
			lbp = (unsigned long*)src_line[2];
			color[8] = *lbp;     color[9] = color[9];    color[10] = *(lbp + 1); color[11] = *(lbp + 2);
			lbp = (unsigned long*)src_line[3];
			color[12] = *lbp;    color[13] = color[12];  color[14] = *(lbp + 1); color[15] = *(lbp + 2);
		}
		
		if (y < height - 1) {
			dst_line[0] += dst_pitch * 2;
			dst_line[1] += dst_pitch * 2;
		}
	}
}


void Scale2x (unsigned char *src, unsigned int src_pitch, int src_bytes_per_pixel,
		 unsigned char *dst, unsigned int dst_pitch, int dst_bytes_per_pixel,
		 int width, int height, int pal[256])
{
#define GET_COLOR(x) (pal[(x)])

	int x,y;
	unsigned char *src_line;
	unsigned char *dst_line[2];

	src_line = src;
	dst_line[0] = dst;
	dst_line[1] = dst + dst_pitch;
	for (y=0; y<height; y++) {
		for (x=0; x<width; x++) {
			int color;

			if (src_bytes_per_pixel == 1) {
				color = GET_COLOR(*(((unsigned char*)src_line) + x));
			} else if (src_bytes_per_pixel == 2) {
				color = *(((unsigned short*)src_line) + x);
			} else {
				color = *(((unsigned int*)src_line) + x);
			}

			if (dst_bytes_per_pixel == 2) {
				*((unsigned long *) (&dst_line[0][x * 4])) = color | (color << 16);
				*((unsigned long *) (&dst_line[1][x * 4])) = color | (color << 16);
			} else {
				*((unsigned long *) (&dst_line[0][x * 8])) = color;
				*((unsigned long *) (&dst_line[0][x * 8 + 4])) = color;
				*((unsigned long *) (&dst_line[1][x * 8])) = color;
				*((unsigned long *) (&dst_line[1][x * 8 + 4])) = color;
			}
		}

		src_line += src_pitch;

		if (y < height - 1) {
			dst_line[0] += dst_pitch * 2;
			dst_line[1] += dst_pitch * 2;
		}
	}
}

void flippage(int page)
{
	int h;
	int w;
	pixel_t *src;
	unsigned char *dest;

	assert(drawing_enable==0);

	SDL_LockSurface(jnb_surface);
        dest=(unsigned char *)jnb_surface->pixels;
	src=screen_buffer[page];
#ifdef SCALE_UP
	Super2xSaI(src, JNB_WIDTH, 1, dest, jnb_surface->pitch, 2, JNB_WIDTH, JNB_HEIGHT, current_pal);
#else
        w=(jnb_surface->clip_rect.w>JNB_SURFACE_WIDTH)?(JNB_SURFACE_WIDTH):(jnb_surface->clip_rect.w);
        h=(jnb_surface->clip_rect.h>JNB_SURFACE_HEIGHT)?(JNB_SURFACE_HEIGHT):(jnb_surface->clip_rect.h);
        for (; h>0; h--) {
		memcpy(dest,src,w*JNB_BYTESPP);
		dest+=jnb_surface->pitch;
		src+=JNB_SURFACE_WIDTH;
        }
#endif
        SDL_UnlockSurface(jnb_surface);
	SDL_Flip(jnb_surface);
}


void draw_begin(void)
{
	assert(drawing_enable==0);

	drawing_enable = 1;
	if (background_drawn == 0) {
		if (background) {
			put_block(0, 0, 0, JNB_WIDTH, JNB_HEIGHT, background);
			put_block(1, 0, 0, JNB_WIDTH, JNB_HEIGHT, background);
			//put_block(0, 0, 0, rabbit_gobs.width[1], rabbit_gobs.height[1], rabbit_gobs.data[1]);
			//put_block(1, 0, 0, rabbit_gobs.width[1], rabbit_gobs.height[1], rabbit_gobs.data[1]);
		} else {
			clear_page(0, 0);
			clear_page(1, 0);
		}
		background_drawn = 1;
	}
}


void draw_end(void)
{
	assert(drawing_enable==1);

	drawing_enable = 0;
}


void setpalette(int index, int count, char *palette)
{
	SDL_Color colors[256];
	int i;

	assert(drawing_enable==0);

	for (i = 0; i < count; i++) {
		colors[i+index].r = palette[i * 3 + 0] << 2;
		colors[i+index].g = palette[i * 3 + 1] << 2;
		colors[i+index].b = palette[i * 3 + 2] << 2;
		current_pal[i+index] = SDL_MapRGB(jnb_surface->format, colors[i+index].r, colors[i+index].g, colors[i+index].b);
	}
#ifndef SCALE_UP
	SDL_SetColors(jnb_surface, &colors[index], index, count);
#endif
}


void fillpalette(int red, int green, int blue)
{
	SDL_Color colors[256];
	int i;

	assert(drawing_enable==0);

	for (i = 0; i < 256; i++) {
		colors[i].r = red << 2;
		colors[i].g = green << 2;
		colors[i].b = blue << 2;
		current_pal[i] = SDL_MapRGB(jnb_surface->format, colors[i].r, colors[i].g, colors[i].b);
	}
#ifndef SCALE_UP
	SDL_SetColors(jnb_surface, colors, 0, 256);
#endif
}


void get_block(int page, int x, int y, int width, int height, pixel_t *buffer)
{
	int h;
	pixel_t *buffer_ptr, *vga_ptr;

	assert(drawing_enable==1);

#ifdef SCALE_UP2
	x *= 2;
	y *= 2;
	width *= 2;
	height *= 2;
#endif

	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (y + height >= JNB_SURFACE_HEIGHT)
		height = JNB_SURFACE_HEIGHT - y;
	if (x + width >= JNB_SURFACE_WIDTH)
		width = JNB_SURFACE_WIDTH - x;
	if (width<=0)
		return;
	if(height<=0)
		return;

	vga_ptr = get_vgaptr(page, x, y);
	buffer_ptr = buffer;
	for (h = 0; h < height; h++) {
		memcpy(buffer_ptr, vga_ptr, width * JNB_BYTESPP);
		vga_ptr += JNB_SURFACE_WIDTH;
		buffer_ptr += width;
	}

}


void put_block(int page, int x, int y, int width, int height, pixel_t *buffer)
{
	int h;
	pixel_t *vga_ptr, *buffer_ptr;

	assert(drawing_enable==1);

#ifdef SCALE_UP2
	x *= 2;
	y *= 2;
	width *= 2;
	height *= 2;
#endif

	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (y + height >= JNB_SURFACE_HEIGHT)
		height = JNB_SURFACE_HEIGHT - y;
	if (x + width >= JNB_SURFACE_WIDTH)
		width = JNB_SURFACE_WIDTH - x;
	if (width<=0)
		return;
	if(height<=0)
		return;

	vga_ptr = get_vgaptr(page, x, y);
	buffer_ptr = buffer;
	for (h = 0; h < height; h++) {
		memcpy(vga_ptr, buffer_ptr, width * JNB_BYTESPP);
		vga_ptr += JNB_SURFACE_WIDTH;
		buffer_ptr += width;
	}
}


void put_text(int page, int x, int y, char *text, int align)
{
	int c1;
	int t1;
	int width;
	int cur_x;
	int image;

	assert(drawing_enable==1);

	if (text == NULL || strlen(text) == 0)
		return;
	if (font_gobs.num_images == 0)
		return;

	width = 0;
	c1 = 0;
	while (text[c1] != 0) {
		t1 = text[c1];
		c1++;
		if (t1 == ' ') {
			width += 5;
			continue;
		}
		if (t1 >= 33 && t1 <= 34)
			image = t1 - 33;

		else if (t1 >= 39 && t1 <= 41)
			image = t1 - 37;

		else if (t1 >= 44 && t1 <= 59)
			image = t1 - 39;

		else if (t1 >= 64 && t1 <= 90)
			image = t1 - 43;

		else if (t1 >= 97 && t1 <= 122)
			image = t1 - 49;

		else if (t1 == '~')
			image = 74;

		else if (t1 == 0x84)
			image = 75;

		else if (t1 == 0x86)
			image = 76;

		else if (t1 == 0x8e)
			image = 77;

		else if (t1 == 0x8f)
			image = 78;

		else if (t1 == 0x94)
			image = 79;

		else if (t1 == 0x99)
			image = 80;

		else
			continue;
		width += pob_width(image, &font_gobs) + 1;
	}

	switch (align) {
	case 0:
		cur_x = x;
		break;
	case 1:
		cur_x = x - width;
		break;
	case 2:
		cur_x = x - width / 2;
		break;
	default:
		cur_x = 0;	/* this should cause error? -Chuck */
		break;
	}
	c1 = 0;

	while (text[c1] != 0) {
		t1 = text[c1];
		c1++;
		if (t1 == ' ') {
			cur_x += 5;
			continue;
		}
		if (t1 >= 33 && t1 <= 34)
			image = t1 - 33;

		else if (t1 >= 39 && t1 <= 41)
			image = t1 - 37;

		else if (t1 >= 44 && t1 <= 59)
			image = t1 - 39;

		else if (t1 >= 64 && t1 <= 90)
			image = t1 - 43;

		else if (t1 >= 97 && t1 <= 122)
			image = t1 - 49;

		else if (t1 == '~')
			image = 74;

		else if (t1 == 0x84)
			image = 75;

		else if (t1 == 0x86)
			image = 76;

		else if (t1 == 0x8e)
			image = 77;

		else if (t1 == 0x8f)
			image = 78;

		else if (t1 == 0x94)
			image = 79;

		else if (t1 == 0x99)
			image = 80;

		else
			continue;
		put_pob(page, cur_x, y, image, &font_gobs, 1, mask_pic);
		cur_x += pob_width(image, &font_gobs) + 1;
	}
}


void put_pob(int page, int x, int y, int image, gob_t *gob, int use_mask, unsigned char *mask_pic)
{
	int c1, c2;
	int pob_offset;
	pixel_t *pob_ptr;
	pixel_t *vga_ptr;
	pixel_t *mask_ptr;
	int width, height;
	int draw_width, draw_height;
	int colour;

	assert(drawing_enable==1);
	assert(gob);
	assert(image>=0);
	assert(image<gob->num_images);

#ifdef SCALE_UP2
	x *= 2;
	y *= 2;
#endif

#ifdef SCALE_UP2
	width = draw_width = gob->width[image]*2;
	height = draw_height = gob->height[image]*2;
	x -= gob->hs_x[image]*2;
	y -= gob->hs_y[image]*2;
#else
	width = draw_width = gob->width[image];
	height = draw_height = gob->height[image];
	x -= gob->hs_x[image];
	y -= gob->hs_y[image];
#endif
	if ((x + width) <= 0 || x >= JNB_SURFACE_WIDTH)
		return;
	if ((y + height) <= 0 || y >= JNB_SURFACE_HEIGHT)
		return;
	pob_offset = 0;
	if (x < 0) {
		pob_offset -= x;
		draw_width += x;
		x = 0;
	}
	if ((x + width) > JNB_SURFACE_WIDTH)
		draw_width -= x + width - JNB_SURFACE_WIDTH;
	if (y < 0) {
		pob_offset -= y * width;
		draw_height += y;
		y = 0;
	}
	if ((y + height) > JNB_SURFACE_HEIGHT)
		draw_height -= y + height - JNB_SURFACE_HEIGHT;

	pob_ptr = &gob->data[image][pob_offset];
	vga_ptr = get_vgaptr(page, x, y);
	mask_ptr = &mask[(y * JNB_SURFACE_WIDTH) + x];
	for (c1 = 0; c1 < draw_height; c1++) {
		for (c2 = 0; c2 < draw_width; c2++) {
			colour = *mask_ptr;
			if (use_mask == 0 || (use_mask == 1 && colour == 0)) {
				colour = *pob_ptr;
				if (colour != 0) {
					*vga_ptr = colour;
				}
			}
			vga_ptr++;
			pob_ptr++;
			mask_ptr++;
		}
		pob_ptr += width - c2;
		vga_ptr += (JNB_SURFACE_WIDTH - c2);
		mask_ptr += (JNB_SURFACE_WIDTH - c2);
	}
}


int pob_width(int image, gob_t *gob)
{
	assert(gob);
	assert(image>=0);
	assert(image<gob->num_images);
	return gob->width[image];
}


int pob_height(int image, gob_t *gob)
{
	assert(gob);
	assert(image>=0);
	assert(image<gob->num_images);
	return gob->height[image];
}


int pob_hs_x(int image, gob_t *gob)
{
	assert(gob);
	assert(image>=0);
	assert(image<gob->num_images);
	return gob->hs_x[image];
}


int pob_hs_y(int image, gob_t *gob)
{
	assert(gob);
	assert(image>=0);
	assert(image<gob->num_images);
	return gob->hs_y[image];
}


int read_pcx(FILE * handle, char *buffer, int buf_len, char *pal)
{
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
	return 0;
}


void register_background(char *pixels, char pal[768])
{
	if (background) {
		free(background);
		background = NULL;
	}
	background_drawn = 0;
	if (!pixels)
		return;
	assert(pal);
#ifdef SCALE_UP2
	{
		int int_pal[256];
		int i;

		for (i=0; i<256; i++)
			int_pal[i] = SDL_MapRGB(jnb_surface->format, (Uint8)(pal[i*3+0]<<2), (Uint8)(pal[i*3+1]<<2), (Uint8)(pal[i*3+2]<<2));
		background = malloc(JNB_SURFACE_WIDTH*JNB_SURFACE_HEIGHT*JNB_BYTESPP);
		assert(background);
		Super2xSaI(pixels, JNB_WIDTH, 1, (unsigned char *)background, JNB_SURFACE_WIDTH*JNB_BYTESPP, JNB_BYTESPP, JNB_WIDTH, JNB_HEIGHT, int_pal);
	}
#else
	background = malloc(JNB_WIDTH*JNB_HEIGHT);
	assert(background);
	memcpy(background, pixels, JNB_WIDTH*JNB_HEIGHT);
#endif
}

int register_gob(FILE *handle, gob_t *gob, int len)
{
	unsigned char *gob_data;
	int i;

	gob_data = malloc(len);
	fread(gob_data, 1, len, handle);

	gob->num_images = *(short *)(&gob_data[0]);

	gob->width = malloc(gob->num_images*sizeof(int));
	gob->height = malloc(gob->num_images*sizeof(int));
	gob->hs_x = malloc(gob->num_images*sizeof(int));
	gob->hs_y = malloc(gob->num_images*sizeof(int));
	gob->data = malloc(gob->num_images*sizeof(pixel_t *));
	gob->orig_data = malloc(gob->num_images*sizeof(unsigned char *));
	for (i=0; i<gob->num_images; i++) {
		int image_size;
		int offset;

		offset = *(int *)(&gob_data[i*4+2]);

		gob->width[i]  = *(short *)(&gob_data[offset]); offset += 2;
		gob->height[i] = *(short *)(&gob_data[offset]); offset += 2;
		gob->hs_x[i]   = *(short *)(&gob_data[offset]); offset += 2;
		gob->hs_y[i]   = *(short *)(&gob_data[offset]); offset += 2;

		image_size = gob->width[i] * gob->height[i];
		gob->orig_data[i] = malloc(image_size);
		memcpy(gob->orig_data[i], &gob_data[offset], image_size);
#ifdef SCALE_UP2
		image_size = gob->width[i] * gob->height[i] * 4 * JNB_BYTESPP;
		gob->data[i] = malloc(image_size);
#else
		gob->data[i] = gob->orig_data[i];
#endif
	}
	free(gob_data);
	return 0;
}


void recalculate_gob(gob_t *gob, char pal[768])
{
#ifdef SCALE_UP2
	int int_pal[256];
	int i;

	for (i=1; i<256; i++) {
		int_pal[i] = SDL_MapRGB(jnb_surface->format, (Uint8)(pal[i*3+0]<<2), (Uint8)(pal[i*3+1]<<2), (Uint8)(pal[i*3+2]<<2));
		if (int_pal[i] == 0)
			int_pal[i] = SDL_MapRGB(jnb_surface->format, 8, 8, 8);
	}
	int_pal[0] = 0;

	for (i=0; i<gob->num_images; i++) {
		if ( (gob->width[i]>4) && (gob->height[i]>4) )
			Super2xSaI(gob->orig_data[i], gob->width[i], 1, (unsigned char *)gob->data[i], gob->width[i]*2*JNB_BYTESPP, JNB_BYTESPP, gob->width[i], gob->height[i], int_pal);
		else
			Scale2x(gob->orig_data[i], gob->width[i], 1, (unsigned char *)gob->data[i], gob->width[i]*2*JNB_BYTESPP, JNB_BYTESPP, gob->width[i], gob->height[i], int_pal);
	}
#endif
}

void register_mask(char *pixels)
{
	if (mask) {
		free(mask);
		mask = NULL;
	}
	assert(pixels);
#ifdef SCALE_UP2
	{
		int int_pal[256];
		int i;

		int_pal[0] = 0;
		for (i=1; i<256; i++)
			int_pal[i] = 1;
		mask = malloc(JNB_SURFACE_WIDTH*JNB_SURFACE_HEIGHT*JNB_BYTESPP);
		assert(mask);
		Scale2x(pixels, JNB_WIDTH, 1, (unsigned char *)mask, JNB_SURFACE_WIDTH*JNB_BYTESPP, JNB_BYTESPP, JNB_WIDTH, JNB_HEIGHT, int_pal);
	}
#else
	mask = malloc(JNB_WIDTH*JNB_HEIGHT);
	assert(mask);
	memcpy(mask, pixels, JNB_WIDTH*JNB_HEIGHT);
#endif
}
