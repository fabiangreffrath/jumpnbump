/*
 * gfx.c
 * Copyright (C) 1998 Brainchild Design - http://brainchilddesign.com/
 * 
 * Copyright (C) 2001 Chuck Mason <cemason@users.sourceforge.net>
 *
 * Copyright (C) 2002 Florian Schulze <crow@icculus.org>
 *
 * Portions of this code are from the MPEG software simulation group
 * idct implementation. This code will be replaced with a new
 * implementation soon.
 *
 * This file is part of Jump'n'Bump.
 *
 * Jump'n'Bump is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Jump'n'Bump is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "globals.h"
#include <SDL_endian.h>

#ifdef _MSC_VER
    #include "jumpnbump32.xpm"
#elif __APPLE__
    #include "jumpnbump128.xpm"
#else
    #include "jumpnbump64.xpm"
#endif
SDL_Surface *icon;

int screen_width=400;
int screen_height=256;
int screen_pitch=400;
int scale_up=0;
int bytes_per_pixel=1;
int dirty_block_shift=4;

static int current_pal[256];
static SDL_Surface *jnb_surface;
static int fullscreen = 0;
static int vinited = 0;
static void *screen_buffer[2];
static int drawing_enable = 0;
static void *background = NULL;
static int background_drawn;
static void *mask = NULL;
static int dirty_blocks[2][25*16*2];

static SDL_Surface *load_xpm_from_array(char **xpm)
{
#define NEXT_TOKEN { \
	while ((*p != ' ') && (*p != '\t')) p++; \
	while ((*p == ' ') || (*p == '\t')) p++; }

	SDL_Surface *surface;
	char *p;
	int width;
	int height;
	int colors;
	int images;
	int color;
	int pal[256];
	int x,y;

	p = *xpm++;

	width = atoi(p);
	if (width <= 0)
		return NULL;
	NEXT_TOKEN;

	height = atoi(p);
	if (height <= 0)
		return NULL;
	NEXT_TOKEN;

	colors = atoi(p);
	if (colors <= 0)
		return NULL;
	NEXT_TOKEN;

	images = atoi(p);
	if (images <= 0)
		return NULL;

	surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	if (!surface)
		return NULL;

	SDL_SetColorKey(surface, SDL_SRCCOLORKEY, SDL_MapRGBA(surface->format, 0, 0, 0, 0));
	while (colors--) {
		p = *xpm++;

		color = *p++;
		NEXT_TOKEN;

		if (*p++ != 'c') {
			SDL_FreeSurface(surface);
			return NULL;
		}
		NEXT_TOKEN;

		if (*p == '#')
			pal[color] = strtoul(++p, NULL, 16) | 0xff000000;
		else
			pal[color] = 0;
	}

	y = 0;
	while (y < height) {
		int *pixels;

		p = *xpm++;

		pixels = (int *)&((char *)surface->pixels)[y++ * surface->pitch];
		x = 0;
		while (x < width) {
			Uint8 r,g,b,a;

			if (*p == '\0') {
				SDL_FreeSurface(surface);
				return NULL;
			}
			r = (pal[*p] >> 16) & 0xff;
			b = (pal[*p] & 0xff);
			g = (pal[*p] >> 8) & 0xff;
			a = (pal[*p] >> 24) & 0xff;
			pixels[x] = SDL_MapRGBA(surface->format, r, g, b, a);
			x++;
			p++;
		}
	}

	return surface;
}

void *get_vgaptr(int page, int x, int y)
{
	assert(drawing_enable==1);

	return (unsigned char *)screen_buffer[page] + (y*screen_pitch)+(x*bytes_per_pixel);
}


void set_scaling(int scale)
{
	if (scale==1) {
		screen_width=800;
		screen_height=512;
		scale_up=1;
		bytes_per_pixel=2;
		dirty_block_shift=5;
		screen_pitch=screen_width*bytes_per_pixel;
	} else {
		screen_width=400;
		screen_height=256;
		scale_up=0;
		bytes_per_pixel=1;
		dirty_block_shift=4;
		screen_pitch=screen_width*bytes_per_pixel;
	}
}

void open_screen(void)
{
	int lval = 0;
	int bpp;
	int flags;

#ifdef __APPLE__
	lval = SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO);
#else
	lval = SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_AUDIO);
#endif
	if (lval < 0) {
		fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	if (scale_up)
		bpp = 16;
	else
		bpp = 8;
	flags = SDL_SWSURFACE;
	if (fullscreen)
		flags |= SDL_FULLSCREEN;
	jnb_surface = SDL_SetVideoMode(screen_width, screen_height, bpp, flags);

	if (!jnb_surface) {
		fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	if(fullscreen)
		SDL_ShowCursor(0);
	else
		SDL_ShowCursor(1);

	SDL_WM_SetCaption("Jump n Bump","");

	icon=load_xpm_from_array(jumpnbump_xpm);
	if (icon==NULL) {
	    printf("Couldn't load icon\n");
	} else {
	    SDL_WM_SetIcon(icon,NULL);
	}

	vinited = 1;

	memset(current_pal, 0, sizeof(current_pal));
	memset(dirty_blocks, 0, sizeof(dirty_blocks));

	screen_buffer[0]=malloc(screen_width*screen_height*bytes_per_pixel);
	screen_buffer[1]=malloc(screen_width*screen_height*bytes_per_pixel);

/*
	dirty_blocks[0]=malloc(sizeof(int)*25*16+1000);
	dirty_blocks[1]=malloc(sizeof(int)*25*16+1000);
*/

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

	assert(drawing_enable==1);

	for (i=0; i<(25*16); i++)
		dirty_blocks[page][i] = 1;

	if (bytes_per_pixel==1) {
		unsigned char *buf = get_vgaptr(page, 0, 0);

		for (i=0; i<screen_height; i++)
			for (j=0; j<screen_width; j++)
				*buf++ = color;
	} else if (bytes_per_pixel==2) {
		unsigned short *buf = get_vgaptr(page, 0, 0);

		for (i=0; i<screen_height; i++)
			for (j=0; j<screen_width; j++)
				*buf++ = color;
	} else {
		unsigned int *buf = get_vgaptr(page, 0, 0);

		for (i=0; i<screen_height; i++)
			for (j=0; j<screen_width; j++)
				*buf++ = color;
	}
}


void clear_lines(int page, int y, int count, int color)
{
	int i,j;

	assert(drawing_enable==1);

	if (scale_up) {
		count *= 2;
		y *= 2;
	}

	if (bytes_per_pixel==1) {
		for (i=0; i<count; i++) {
			if ((i+y)<screen_height) {
				unsigned char *buf = get_vgaptr(page, 0, i+y);
				for (j=0; j<screen_width; j++)
					*buf++ = color;
			}
		}
	} else if (bytes_per_pixel==2) {
		for (i=0; i<count; i++) {
			if ((i+y)<screen_height) {
				unsigned short *buf = get_vgaptr(page, 0, i+y);
				for (j=0; j<screen_width; j++)
					*buf++ = color;
			}
		}
	} else {
		for (i=0; i<count; i++) {
			if ((i+y)<screen_height) {
				unsigned int *buf = get_vgaptr(page, 0, i+y);
				for (j=0; j<screen_width; j++)
					*buf++ = color;
			}
		}
	}
	count = ((y+count)>>dirty_block_shift) - (y>>dirty_block_shift) + 1;
	y >>= dirty_block_shift;
	for (i=0; i<count; i++)
		for (j=0; j<25; j++)
			dirty_blocks[page][(y+i)*25+j] = 1;
}


int get_color(int color, char pal[768])
{
	assert(color<256);
	assert(pal);
	return SDL_MapRGB(jnb_surface->format, (Uint8)(pal[color*3+0]<<2), (Uint8)(pal[color*3+1]<<2), (Uint8)(pal[color*3+2]<<2));
}


int get_pixel(int page, int x, int y)
{
	assert(drawing_enable==1);

	if (scale_up) {
		x *= 2;
		y *= 2;
	}

	assert(x<screen_width);
	assert(y<screen_height);

	if (bytes_per_pixel==1)
		return *(unsigned char *)get_vgaptr(page, x, y);
	else if (bytes_per_pixel==2)
		return *(unsigned short *)get_vgaptr(page, x, y);
	else
		return *(unsigned int *)get_vgaptr(page, x, y);
}


void set_pixel(int page, int x, int y, int color)
{
	assert(drawing_enable==1);

	if (scale_up) {
		x *= 2;
		y *= 2;
	}

	assert(x<screen_width);
	assert(y<screen_height);

	dirty_blocks[page][(y>>dirty_block_shift)*25+(x>>dirty_block_shift)] = 1;

	if (bytes_per_pixel==1)
		*(unsigned char *)get_vgaptr(page, x, y) = color;
	else if (bytes_per_pixel==2)
		*(unsigned short *)get_vgaptr(page, x, y) = color;
	else
		*(unsigned int *)get_vgaptr(page, x, y) = color;
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
	colorMask = (0xF7DEF7DE);
	lowPixelMask = (0x08210821);
	qcolorMask = (0xE79CE79C);
	qlowpixelMask = (0x18631863);
	redblueMask = (0xF81F);
	greenMask = (0x7E0);
    }
    else if (BitFormat == 555)
    {
	colorMask = (0x7BDE7BDE);
	lowPixelMask = (0x04210421);
	qcolorMask = (0x739C739C);
	qlowpixelMask = (0x0C630C63);
	redblueMask = (0x7C1F);
	greenMask = (0x3E0);
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

	if ( (width<2) || (height<2) ) {
		Scale2x(src, src_pitch, src_bytes_per_pixel, dst, dst_pitch, dst_bytes_per_pixel, width, height, pal);
		return;
	}

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
				unsigned long tmp;
				//*((unsigned long *) (&dst_line[0][x * 4])) = product1a | (product1b << 16);
				//*((unsigned long *) (&dst_line[1][x * 4])) = product2a | (product2b << 16);
				tmp = SDL_SwapLE16(product1a) | SDL_SwapLE16(product1b) << 16;
				*((unsigned long *) (&dst_line[0][x * 4])) = SDL_SwapLE32(tmp);
				tmp = SDL_SwapLE16(product2a) | SDL_SwapLE16(product2b) << 16;
				*((unsigned long *) (&dst_line[1][x * 4])) = SDL_SwapLE32(tmp);
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


void flippage(int page)
{
	int x,y;
	unsigned char *src;
	unsigned char *dest;

	assert(drawing_enable==0);

	SDL_LockSurface(jnb_surface);
	if (!jnb_surface->pixels) {
		
		for (x=0; x<(25*16); x++) {
			dirty_blocks[0][x] = 1;
			dirty_blocks[1][x] = 1;
		}

		return;
	}
#ifdef SCALE_UP
        dest=(unsigned char *)jnb_surface->pixels;
	src=screen_buffer[page];
	Super2xSaI(src, JNB_WIDTH, 1, dest, jnb_surface->pitch, 2, JNB_WIDTH, JNB_HEIGHT, current_pal);
#else
	dest=(unsigned char *)jnb_surface->pixels;
	src=screen_buffer[page];
	for (y=0; y<screen_height; y++) {
		//memset(&dest[y*jnb_surface->pitch],0,JNB_WIDTH*bytes_per_pixel);
		for (x=0; x<25; x++) {
			int count;
			int test_x;

			count=0;
			test_x=x;
			while ( (test_x<25) && (dirty_blocks[page][(y>>dirty_block_shift)*25+test_x]) ) {
				count++;
				test_x++;
			}
			if (count) {
				memcpy(	&dest[y*jnb_surface->pitch+(x<<dirty_block_shift)*bytes_per_pixel],
					&src[y*screen_pitch+((x<<dirty_block_shift)*bytes_per_pixel)],
					((16<<dirty_block_shift)>>4)*bytes_per_pixel*count);
				//*((pixel_t *)(&dest[(y>>dirty_block_shift)*jnb_surface->pitch+x*bytes_per_pixel]))=0xe0e0;
			}
			x = test_x;
		}
	}
	memset(&dirty_blocks[page], 0, sizeof(int)*25*16);
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
	if (!scale_up)
		SDL_SetColors(jnb_surface, &colors[index], index, count);
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
	if (!scale_up)
		SDL_SetColors(jnb_surface, colors, 0, 256);
}


void get_block(int page, int x, int y, int width, int height, void *buffer)
{
	unsigned char *buffer_ptr, *vga_ptr;
	int h;

	assert(drawing_enable==1);

	if (scale_up) {
		x *= 2;
		y *= 2;
		width *= 2;
		height *= 2;
	}

	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (y + height >= screen_height)
		height = screen_height - y;
	if (x + width >= screen_width)
		width = screen_width - x;
	if (width<=0)
		return;
	if(height<=0)
		return;

	vga_ptr = get_vgaptr(page, x, y);
	buffer_ptr = buffer;
	for (h = 0; h < height; h++) {
		memcpy(buffer_ptr, vga_ptr, width * bytes_per_pixel);
		vga_ptr += screen_pitch;
		buffer_ptr += width * bytes_per_pixel;
	}

}


void put_block(int page, int x, int y, int width, int height, void *buffer)
{
	int h;
	unsigned char *vga_ptr, *buffer_ptr;

	assert(drawing_enable==1);

	if (scale_up) {
		x *= 2;
		y *= 2;
		width *= 2;
		height *= 2;
	}

	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (y + height >= screen_height)
		height = screen_height - y;
	if (x + width >= screen_width)
		width = screen_width - x;
	if (width<=0)
		return;
	if(height<=0)
		return;

	vga_ptr = get_vgaptr(page, x, y);
	buffer_ptr = buffer;
	for (h = 0; h < height; h++) {
		memcpy(vga_ptr, buffer_ptr, width * bytes_per_pixel);
		vga_ptr += screen_pitch;
		buffer_ptr += width * bytes_per_pixel;
	}
	width = ((x+width)>>dirty_block_shift) - (x>>dirty_block_shift) + 1;
	height = ((y+height)>>dirty_block_shift) - (y>>dirty_block_shift) + 1;
	x >>= dirty_block_shift;
	y >>= dirty_block_shift;
	while (width--)
		for (h=0; h<height; h++)
			dirty_blocks[page][(y+h)*25+(x+width)] = 1;
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


void put_pob(int page, int x, int y, int image, gob_t *gob, int use_mask, void *mask_pic)
{
	int c1, c2;
	int pob_x, pob_y;
	int width, height;
	int draw_width, draw_height;
	int colour;

	assert(drawing_enable==1);
	assert(gob);
	assert(image>=0);
	assert(image<gob->num_images);

	if (scale_up) {
		x *= 2;
		y *= 2;
		width = draw_width = gob->width[image]*2;
		height = draw_height = gob->height[image]*2;
		x -= gob->hs_x[image]*2;
		y -= gob->hs_y[image]*2;
	} else {
		width = draw_width = gob->width[image];
		height = draw_height = gob->height[image];
		x -= gob->hs_x[image];
		y -= gob->hs_y[image];
	}

	if ((x + width) <= 0 || x >= screen_width)
		return;
	if ((y + height) <= 0 || y >= screen_height)
		return;

	pob_x = 0;
	pob_y = 0;
	if (x < 0) {
		pob_x -= x;
		draw_width += x;
		x = 0;
	}
	if ((x + width) > screen_width)
		draw_width -= x + width - screen_width;
	if (y < 0) {
		pob_y -= y;
		draw_height += y;
		y = 0;
	}
	if ((y + height) > screen_height)
		draw_height -= y + height - screen_height;


	if (bytes_per_pixel==1) {
		unsigned char *vga_ptr;
		unsigned char *pob_ptr;
		unsigned char *mask_ptr;

		vga_ptr = get_vgaptr(page, x, y);
		pob_ptr = ((unsigned char *)gob->data[image]) + ((pob_y * width) + pob_x);
		mask_ptr = ((unsigned char *)mask) + ((y * screen_pitch) + (x*bytes_per_pixel));
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
			vga_ptr += (screen_width - c2);
			mask_ptr += (screen_width - c2);
		}
	} else if (bytes_per_pixel==2) {
		unsigned short *vga_ptr;
		unsigned short *pob_ptr;
		unsigned short *mask_ptr;

		vga_ptr = get_vgaptr(page, x, y);
		pob_ptr = (unsigned short *)(((unsigned char *)gob->data[image]) + ((pob_y * width) + pob_x));
		mask_ptr = (unsigned short *)(((unsigned char *)mask) + ((y * screen_pitch) + (x*bytes_per_pixel)));
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
			vga_ptr += (screen_width - c2);
			mask_ptr += (screen_width - c2);
		}
	} else {
		unsigned int *vga_ptr;
		unsigned int *pob_ptr;
		unsigned int *mask_ptr;

		vga_ptr = get_vgaptr(page, x, y);
		pob_ptr = (unsigned int *)(((unsigned char *)gob->data[image]) + ((pob_y * width) + pob_x));
		mask_ptr = (unsigned int *)(((unsigned char *)mask) + ((y * screen_pitch) + (x*bytes_per_pixel)));
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
			vga_ptr += (screen_width - c2);
			mask_ptr += (screen_width - c2);
		}
	}
	draw_width = ((x+draw_width)>>dirty_block_shift) - (x>>dirty_block_shift) + 1;
	draw_height = ((y+draw_height)>>dirty_block_shift) - (y>>dirty_block_shift) + 1;
	x >>= dirty_block_shift;
	y >>= dirty_block_shift;
	while (draw_width--)
		for (c1=0; c1<draw_height; c1++)
			dirty_blocks[page][(y+c1)*25+(x+draw_width)] = 1;
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


int read_pcx(unsigned char * handle, void *buf, int buf_len, char *pal)
{
	unsigned char *buffer=buf;
	short c1;
	short a, b;
	long ofs1;
	if (buffer != 0) {
		handle += 128;
		ofs1 = 0;
		while (ofs1 < buf_len) {
			a = *(handle++);
			if ((a & 0xc0) == 0xc0) {
				b = *(handle++);
				a &= 0x3f;
				for (c1 = 0; c1 < a && ofs1 < buf_len; c1++)
					buffer[ofs1++] = (char) b;
			} else
				buffer[ofs1++] = (char) a;
		}
		if (pal != 0) {
			handle++;
			for (c1 = 0; c1 < 768; c1++)
				pal[c1] = *(handle++) /*fgetc(handle)*/ >> 2;
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
	if (scale_up) {
		int int_pal[256];
		int i;

		for (i=0; i<256; i++)
			int_pal[i] = SDL_MapRGB(jnb_surface->format, (Uint8)(pal[i*3+0]<<2), (Uint8)(pal[i*3+1]<<2), (Uint8)(pal[i*3+2]<<2));
		background = malloc(screen_pitch*screen_height);
		assert(background);
		Init_2xSaI(565);
		Super2xSaI(pixels, JNB_WIDTH, 1, (unsigned char *)background, screen_pitch, bytes_per_pixel, JNB_WIDTH, JNB_HEIGHT, int_pal);
	} else {
		background = malloc(JNB_WIDTH*JNB_HEIGHT);
		assert(background);
		memcpy(background, pixels, JNB_WIDTH*JNB_HEIGHT);
	}
}

int register_gob(unsigned char *handle, gob_t *gob, int len)
{
	unsigned char *gob_data;
	int i;

	gob_data = malloc(len);
	memcpy(gob_data, handle, len);

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
		if (scale_up) {
			image_size = gob->width[i] * gob->height[i] * 4 * bytes_per_pixel;
			gob->data[i] = malloc(image_size);
		} else {
			gob->data[i] = (unsigned short *)gob->orig_data[i];
		}
	}
	free(gob_data);
	return 0;
}


void recalculate_gob(gob_t *gob, char pal[768])
{
	int int_pal[256];
	int i;

	if (!scale_up)
		return;

	Init_2xSaI(565);

	for (i=1; i<256; i++) {
		int_pal[i] = SDL_MapRGB(jnb_surface->format, (Uint8)(pal[i*3+0]<<2), (Uint8)(pal[i*3+1]<<2), (Uint8)(pal[i*3+2]<<2));
		if (int_pal[i] == 0)
			int_pal[i] = SDL_MapRGB(jnb_surface->format, 8, 8, 8);
	}
	int_pal[0] = 0;

	for (i=0; i<gob->num_images; i++) {
		Super2xSaI(gob->orig_data[i], gob->width[i], 1, (unsigned char *)gob->data[i], gob->width[i]*2*bytes_per_pixel, bytes_per_pixel, gob->width[i], gob->height[i], int_pal);
	}
}

void register_mask(void *pixels)
{
	if (mask) {
		free(mask);
		mask = NULL;
	}
	assert(pixels);
	if (scale_up) {
		int int_pal[256];
		int i;

		int_pal[0] = 0;
		for (i=1; i<256; i++)
			int_pal[i] = 0xffffffff;
		mask = malloc(screen_pitch*screen_height);
		assert(mask);
		Init_2xSaI(565);
		Scale2x(pixels, JNB_WIDTH, 1, (unsigned char *)mask, screen_pitch, bytes_per_pixel, JNB_WIDTH, JNB_HEIGHT, int_pal);
	} else {
		mask = malloc(JNB_WIDTH*JNB_HEIGHT);
		assert(mask);
		memcpy(mask, pixels, JNB_WIDTH*JNB_HEIGHT);
	}
}
