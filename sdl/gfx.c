#include "globals.h"

#define JNB_BPP 8
static SDL_Surface *jnb_surface;
static int fullscreen = 0;
static int vinited = 0;
static unsigned char screen_buffer[JNB_WIDTH*JNB_HEIGHT*2];


char *get_vgaptr(int page, int x, int y)
{
	if (page == 1)
		return &screen_buffer[JNB_WIDTH*JNB_HEIGHT+((y * JNB_WIDTH) + x)];
	else
		return &screen_buffer[(y * JNB_WIDTH) + x];
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
		jnb_surface = SDL_SetVideoMode(JNB_WIDTH, JNB_HEIGHT, JNB_BPP, SDL_SWSURFACE | SDL_FULLSCREEN);
	else
		jnb_surface = SDL_SetVideoMode(JNB_WIDTH, JNB_HEIGHT, JNB_BPP, SDL_SWSURFACE);
	if (!jnb_surface) {
		fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_ShowCursor(0);

	vinited = 1;

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
	memset((void *) get_vgaptr(page, 0, 0), color, JNB_WIDTH * JNB_HEIGHT);
}


void clear_lines(int page, int y, int count, int color)
{
	int i;

	for (i=0; i<count; i++)
		if ((i+y)<JNB_HEIGHT)
			memset((void *) get_vgaptr(page, 0, i+y), color, JNB_WIDTH);
}


int get_pixel(int page, int x, int y)
{
	return *(char *) get_vgaptr(page, x, y);
}


void set_pixel(int page, int x, int y, int color)
{
	*(char *) get_vgaptr(page, x, y) = color;
}


void flippage(int page)
{
	int h;
	int w;
	char *src;
	char *dest;

	SDL_LockSurface(jnb_surface);
        dest=(char *)jnb_surface->pixels;
	if (page == 1)
		src=&screen_buffer[JNB_WIDTH*JNB_HEIGHT];
	else
		src=&screen_buffer[0];
        w=(jnb_surface->clip_rect.w>JNB_WIDTH)?(JNB_WIDTH):(jnb_surface->clip_rect.w);
        h=(jnb_surface->clip_rect.h>JNB_HEIGHT)?(JNB_HEIGHT):(jnb_surface->clip_rect.h);
        for (; h>0; h--)
        {
		memcpy(dest,src,w);
		dest+=jnb_surface->pitch;
		src+=JNB_WIDTH;
        }
        SDL_UnlockSurface(jnb_surface);
	SDL_Flip(jnb_surface);
}


void setpalette(int index, int count, char *palette)
{
	SDL_Color colors[256];
	int i;

	for (i = 0; i < count; i++) {
		colors[i].r = palette[i * 3 + 0] << 2;
		colors[i].g = palette[i * 3 + 1] << 2;
		colors[i].b = palette[i * 3 + 2] << 2;
	}
	SDL_SetColors(jnb_surface, colors, index, count);
}


void fillpalette(int red, int green, int blue)
{
	SDL_Color colors[256];
	int i;

	for (i = 0; i < 256; i++) {
		colors[i].r = red << 2;
		colors[i].g = green << 2;
		colors[i].b = blue << 2;
	}
	SDL_SetColors(jnb_surface, colors, 0, 256);
}


void get_block(int page, int x, int y, int width, int height, char *buffer)
{
	short w, h;
	char *buffer_ptr, *vga_ptr;

	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (y + height >= JNB_HEIGHT)
		height = JNB_HEIGHT - y;
	if (x + width >= JNB_WIDTH)
		width = JNB_WIDTH - x;

	for (h = 0; h < height; h++) {
		buffer_ptr = &buffer[h * width];

		vga_ptr = get_vgaptr(page, x, h + y);

		for (w = 0; w < width; w++) {
			unsigned char ch;
			ch = *vga_ptr;
			*buffer_ptr = ch;
			buffer_ptr++;
			vga_ptr++;
		}
	}

}


void put_block(int page, int x, int y, int width, int height, char *buffer)
{
	short w, h;
	char *vga_ptr, *buffer_ptr;

	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (y + height >= JNB_HEIGHT)
		height = JNB_HEIGHT - y;
	if (x + width >= JNB_WIDTH)
		width = JNB_WIDTH - x;

	for (h = 0; h < height; h++) {
		vga_ptr = get_vgaptr(page, x, y + h);

		buffer_ptr = &buffer[h * width];
		for (w = 0; w < width; w++) {
			*vga_ptr = *buffer_ptr;
			vga_ptr++;
			buffer_ptr++;
		}
	}
}


void put_text(int page, int x, int y, char *text, int align)
{
	int c1;
	int t1;
	int width;
	int cur_x;
	int image;

	if (text == NULL || strlen(text) == 0)
		return;
	if (font_gobs == NULL)
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
		width += pob_width(image, font_gobs) + 1;
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
		put_pob(page, cur_x, y, image, font_gobs, 1, mask_pic);
		cur_x += pob_width(image, font_gobs) + 1;
	}
}


void put_pob(int page, int x, int y, int image, char *pob_data, int mask, char *mask_pic)
{
	long c1, c2;
	long pob_offset;
	char *pob_ptr, *vga_ptr, *mask_ptr;
	long width, height;
	long draw_width, draw_height;
	char colour;

	if (image < 0 || image >= *(short *) (pob_data))
		return;

	vga_ptr = get_vgaptr(page, 0, 0);
	pob_offset = *(unsigned long *) (pob_data + (image * 4) + 2);
	width = draw_width = *(short *) (pob_data + pob_offset);
	height = draw_height = *(short *) (pob_data + pob_offset + 2);
	x -= *(short *) (pob_data + pob_offset + 4);
	y -= *(short *) (pob_data + pob_offset + 6);
	pob_offset += 8;
	if ((x + width) <= 0 || x >= 400)
		return;
	if ((y + height) <= 0 || y >= 256)
		return;
	if (x < 0) {
		pob_offset -= x;
		draw_width += x;
		x = 0;
	}
	if ((x + width) > 400)
		draw_width -= x + width - 400;
	if (y < 0) {
		pob_offset += -y * width;
		draw_height -= -y;
		y = 0;
	}
	if ((y + height) > 256)
		draw_height -= y + height - 256;

	pob_ptr = &pob_data[pob_offset];


	vga_ptr = get_vgaptr(page, x, y);
	mask_ptr = (char *) (mask_pic + (y * 400) + x);
	for (c1 = 0; c1 < draw_height; c1++) {
		for (c2 = 0; c2 < draw_width; c2++) {
			colour = *mask_ptr;
			if (mask == 0 || (mask == 1 && colour == 0)) {
				colour = *pob_ptr;
				if (colour != 0)
					*vga_ptr = colour;
			}
			pob_ptr++;
			vga_ptr++;
			mask_ptr++;
		}
		pob_ptr += width - c2;
		vga_ptr += (400 - c2);
		mask_ptr += (400 - c2);
	}
}


int pob_col(int x1, int y1, int image1, char *pob_data1, int x2, int y2, int image2, char *pob_data2)
{
	short c1, c2;
	long pob_offset1, pob_offset2;
	short width1, width2;
	short height1, height2;
	short check_width, check_height;
	char *pob_ptr1, *pob_ptr2;

	pob_offset1 = *(long *) (pob_data1 + image1 * 4 + 2);
	width1 = *(short *) (pob_data1 + pob_offset1);
	height1 = *(short *) (pob_data1 + pob_offset1 + 2);
	x1 -= *(short *) (pob_data1 + pob_offset1 + 4);
	y1 -= *(short *) (pob_data1 + pob_offset1 + 6);
	pob_offset1 += 8;
	pob_offset2 = *(long *) (pob_data2 + image2 * 4 + 2);
	width2 = *(short *) (pob_data2 + pob_offset2);
	height2 = *(short *) (pob_data2 + pob_offset2 + 2);
	x2 -= *(short *) (pob_data2 + pob_offset2 + 4);
	y2 -= *(short *) (pob_data2 + pob_offset2 + 6);
	pob_offset2 += 8;

	if (x1 < x2) {
		if ((x1 + width1) <= x2)
			return 0;

		else if ((x1 + width1) <= (x2 + width2)) {
			pob_offset1 += x2 - x1;
			check_width = x1 + width1 - x2;
		}

		else {
			pob_offset1 += x2 - x1;
			check_width = width2;
		}
	}

	else {
		if ((x2 + width2) <= x1)
			return 0;

		else if ((x2 + width2) <= (x1 + width1)) {
			pob_offset2 += x1 - x2;
			check_width = x2 + width2 - x1;
		}

		else {
			pob_offset2 += x1 - x2;
			check_width = width1;
		}
	}
	if (y1 < y2) {
		if ((y1 + height1) <= y2)
			return 0;

		else if ((y1 + height1) <= (y2 + height2)) {
			pob_offset1 += (y2 - y1) * width1;
			check_height = y1 + height1 - y2;
		}

		else {
			pob_offset1 += (y2 - y1) * width1;
			check_height = height2;
		}
	}

	else {
		if ((y2 + height2) <= y1)
			return 0;

		else if ((y2 + height2) <= (y1 + height1)) {
			pob_offset2 += (y1 - y2) * width2;
			check_height = y2 + height2 - y1;
		}

		else {
			pob_offset2 += (y1 - y2) * width2;
			check_height = height1;
		}
	}
	pob_ptr1 = (char *) (pob_data1 + pob_offset1);
	pob_ptr2 = (char *) (pob_data2 + pob_offset2);
	for (c1 = 0; c1 < check_height; c1++) {
		for (c2 = 0; c2 < check_width; c2++) {
			if (*pob_ptr1 != 0 && *pob_ptr2 != 0)
				return 1;
			pob_ptr1++;
			pob_ptr2++;
		}
		pob_ptr1 += width1 - check_width;
		pob_ptr2 += width2 - check_width;
	}
	return 0;
}


int pob_width(int image, char *pob_data)
{
	return *(short *) (pob_data + *(long *) (pob_data + image * 4 + 2));
}


int pob_height(int image, char *pob_data)
{
	return *(short *) (pob_data + *(long *) (pob_data + image * 4 + 2) + 2);
}


int pob_hs_x(int image, char *pob_data)
{
	return *(short *) (pob_data + *(long *) (pob_data + image * 4 + 2) + 4);
}


int pob_hs_y(int image, char *pob_data)
{
	return *(short *) (pob_data + *(long *) (pob_data + image * 4 + 2) + 6);
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


#ifndef _MSC_VER
int filelength(int handle)
{
	struct stat buf;

	if (fstat(handle, &buf) == -1) {
		perror("filelength");
		exit(EXIT_FAILURE);
	}

	return buf.st_size;
}
#endif
