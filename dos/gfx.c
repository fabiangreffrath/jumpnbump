/*
 * gfx.h
 * Copyright (C) 1998 Brainchild Design - http://brainchilddesign.com/
 * 
 * Copyright (C) 2002 Florian Schulze - crow@icculus.org
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


void open_screen(void)
{
	__dpmi_regs regs;
	char *ptr1;

	regs.x.ax = 0x13;
	__dpmi_int(0x10, &regs);

	outportw(0x3c4, 0x0604);
	outportw(0x3c4, 0x0100);
	outportb(0x3c2, 0xe7);
	outportw(0x3c4, 0x0300);

	outportb(0x3d4, 0x11);
	outportb(0x3d5, inportb(0x3d5) & 0x7f);

	outportw(0x3d4, 0x7100);
	outportw(0x3d4, 0x6301);
	outportw(0x3d4, 0x6402);
	outportw(0x3d4, 0x9203);
	outportw(0x3d4, 0x6604);
	outportw(0x3d4, 0x8205);
	outportw(0x3d4, 0x2b06);
	outportw(0x3d4, 0xb207);
	outportw(0x3d4, 0x0008);
	outportw(0x3d4, 0x6109);
	outportw(0x3d4, 0x1310);
	outportw(0x3d4, 0xac11);
	outportw(0x3d4, 0xff12);
	outportw(0x3d4, 0x3213);
	outportw(0x3d4, 0x0014);
	outportw(0x3d4, 0x0715);
	outportw(0x3d4, 0x1a16);
	outportw(0x3d4, 0xe317);

	outportw(0x3d4, 0x3213);

	ptr1 = (char *) (0xa0000 + __djgpp_conventional_base);
	outportw(0x3c4, 0x0f02);
	memset(ptr1, 0, 65535);

}


void wait_vrt(int mix)
{
	if (mix) {
		while ((inportb(0x3da) & 8) == 0)
			dj_mix();
		while ((inportb(0x3da) & 8) == 8)
			dj_mix();
	} else {
		while ((inportb(0x3da) & 8) == 0);
		while ((inportb(0x3da) & 8) == 8);
	}
}

void clear_page(int page, int color)
{
	outportw(0x3c4, 0x0f02);
	memset((char *) (0xa0000 - __djgpp_base_address), 0, 32768);
}

void clear_lines(int page, int y, int count, int color)
{
	int i;

	outportw(0x3c4, 0x0f02);
	for (i=0; i<count; i++)
		if ((i+y)<256)
			memset((char *) (0xa0000 + (i+y) * 100 + __djgpp_conventional_base), 0, 100);
}

int get_pixel(int page, int x, int y)
{
	outportw(0x3ce, (((x) & 3) << 8) + 0x04);
	//outportw(0x3c4, ((1 << ((x) & 3)) << 8) + 0x02);
	return *(char *) (0xa0000 + (y * 100 + (x >> 2)) + ((long) page << 15) - __djgpp_base_address);
}

void set_pixel(int page, int x, int y, int color)
{
	//outportw(0x3ce, (((x) & 3) << 8) + 0x04);
	outportw(0x3c4, ((1 << ((x) & 3)) << 8) + 0x02);
	*(char *) (0xa0000 + (y * 100 + (x >> 2)) + ((long) page << 15) - __djgpp_base_address) = color;
}

void flippage(int page)
{
	outportw(0x3d4, (page << 23) + 0x0d);
	outportw(0x3d4, ((page << 15) & 0xff00) + 0x0c);
}

#if 0
void get_block(char page, short x, short y, short width, short height, char *buffer)
{
	short c1, c2, c3;
	char *buffer_ptr, *vga_ptr;

	for (c3 = 0; c3 < 4; c3++) {
		outportw(0x3ce, (((x + c3) & 3) << 8) + 0x04);
		for (c1 = 0; (c1 + c3) < width; c1 += 4) {
			buffer_ptr = &buffer[(c1 + c3) * height];
			vga_ptr = (char *) (0xa0000 + ((long) page << 15) + (long) y * 100 + ((x + c1 + c3) >> 2) + __djgpp_conventional_base);
			for (c2 = 0; c2 < height; c2++) {
				*buffer_ptr = *vga_ptr;
				buffer_ptr++;
				vga_ptr += 100;
			}
		}
	}

}
#endif

#if 0
void put_block(char page, short x, short y, short width, short height, char *buffer)
{
	short c1, c2, c3;
	char *vga_ptr, *buffer_ptr;

	for (c3 = 0; c3 < 4; c3++) {
		outportw(0x3c4, ((1 << ((x + c3) & 3)) << 8) + 0x02);
		for (c1 = 0; (c1 + c3) < width; c1 += 4) {
			vga_ptr = (char *) (0xa0000 + ((long) page << 15) + (long) y * 100 + ((x + c1 + c3) >> 2) + __djgpp_conventional_base);
			buffer_ptr = &buffer[(c1 + c3) * height];
			for (c2 = 0; c2 < height; c2++) {
				*vga_ptr = *buffer_ptr;
				vga_ptr += 100;
				buffer_ptr++;
			}
		}
	}

}
#endif

void put_text(char page, int x, int y, char *text, char align)
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


void put_pob(char page, short x, short y, short image, char *pob_data, char mask, char *mask_pic)
{
	long c1, c2, c3;
	long pob_offset;
	char *pob_ptr, *vga_ptr, *mask_ptr;
	long width, height;
	long draw_width, draw_height;
	char colour;

	if (image < 0 || image >= *(short *) (pob_data))
		return;

	pob_offset = *(long *) (pob_data + image * 4 + 2);

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

	for (c3 = 0; c3 < 4; c3++) {
		outportw(0x3c4, ((1 << ((x + c3) & 3)) << 8) + 0x02);
		pob_ptr = &pob_data[pob_offset + c3];
		vga_ptr = (char *) (0xa0000 + (long) (page << 15) + (long) y * 100L + ((x + c3) >> 2) + __djgpp_conventional_base);
		mask_ptr = (char *) (mask_pic + (long) y * 400L + x + c3);
		for (c1 = 0; c1 < draw_height; c1++) {
			for (c2 = c3; c2 < draw_width; c2 += 4) {
				colour = *mask_ptr;
				if (mask == 0 || (mask == 1 && colour == 0)) {
					colour = *pob_ptr;
					if (colour != 0)
						*vga_ptr = colour;
				}
				pob_ptr += 4;
				vga_ptr++;
				mask_ptr += 4;
			}
			pob_ptr += width - c2 + c3;
			vga_ptr += (400 - c2 + c3) >> 2;
			mask_ptr += 400 - c2 + c3;
		}
	}

}


char pob_col(short x1, short y1, short image1, char *pob_data1, short x2, short y2, short image2, char *pob_data2)
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
		} else {
			pob_offset1 += x2 - x1;
			check_width = width2;
		}
	} else {
		if ((x2 + width2) <= x1)
			return 0;
		else if ((x2 + width2) <= (x1 + width1)) {
			pob_offset2 += x1 - x2;
			check_width = x2 + width2 - x1;
		} else {
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
		} else {
			pob_offset1 += (y2 - y1) * width1;
			check_height = height2;
		}
	} else {
		if ((y2 + height2) <= y1)
			return 0;
		else if ((y2 + height2) <= (y1 + height1)) {
			pob_offset2 += (y1 - y2) * width2;
			check_height = y2 + height2 - y1;
		} else {
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


short pob_width(short image, char *pob_data)
{
	return *(short *) (pob_data + *(long *) (pob_data + image * 4 + 2));
}


short pob_height(short image, char *pob_data)
{
	return *(short *) (pob_data + *(long *) (pob_data + image * 4 + 2) + 2);
}


short pob_hs_x(short image, char *pob_data)
{
	return *(short *) (pob_data + *(long *) (pob_data + image * 4 + 2) + 4);
}


short pob_hs_y(short image, char *pob_data)
{
	return *(short *) (pob_data + *(long *) (pob_data + image * 4 + 2) + 6);
}


char read_pcx(FILE * handle, char *buffer, long buf_len, char *pal)
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
				for (c1 = 0; c1 < a; c1++)
					buffer[ofs1++] = b;
			} else
				buffer[ofs1++] = a;
		}

		if (pal != 0) {
			fseek(handle, 1, SEEK_CUR);
			for (c1 = 0; c1 < 768; c1++)
				pal[c1] = fgetc(handle) >> 2;
		}

	}

	fclose(handle);
	return 0;
}
