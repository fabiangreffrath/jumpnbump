/*
 * interrpt.c
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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include "globals.h"

char keyb[256];
char last_keys[50];

unsigned char scancode2ascii[256] = {
	0, 0, 49, 50, 51, 52, 53, 54, 55, 56,		/* 0-9 */
	57, 48, 45, 0, 0, 0, 113, 119, 101, 114,	/* 10-19 */
	116, 121, 117, 105, 111, 112, 0, 0, 0, 0,	/* 20-29 */
	97, 115, 100, 102, 103, 104, 106, 107, 108, 0,	/* 30-39 */
	0, 0, 0, 0, 122, 120, 99, 118, 98, 110,		/* 40-49 */
	109, 44, 46, 47, 0, 0, 0, 32, 0, 0,		/* 50-59 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0
};

int hook_keyb_handler(void)
{
	SDL_EnableUNICODE(1);
	memset((void *) last_keys, 0, sizeof(last_keys));
	return 0;

}


void remove_keyb_handler(void)
{
}


int key_pressed(int key)
{
	return keyb[(unsigned char) key];
}

int addkey(unsigned int key)
{
	int c1;

	if (!(key & 0x8000)) {
		keyb[key & 0x7fff] = 1;
		for (c1 = 48; c1 > 0; c1--)
			last_keys[c1] = last_keys[c1 - 1];
		last_keys[0] = key & 0x7fff;
	} else
		keyb[key & 0x7fff] = 0;
	return 0;
}

int intr_sysupdate()
{
	SDL_Event e;
	int i = 0;
	static int last_time = 0;
	int now, time_diff;

	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_MOUSEBUTTONDOWN:
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			switch (e.key.keysym.sym) {
			case SDLK_F12:
				if (e.type == SDL_KEYDOWN) {
					SDL_Quit();
					exit(1);
				}
				break;
			case SDLK_F10:
				if (e.type == SDL_KEYDOWN) {
					fs_toggle();
				}
				break;
			case SDLK_ESCAPE:
				if (e.type == SDL_KEYUP)
					addkey(1 | 0x8000);
				else
					addkey(1 & 0x7f);
				break;
			default:
				e.key.keysym.sym &= 0x7f;
				if (e.type == SDL_KEYUP)
					e.key.keysym.sym |= 0x8000;
				addkey(e.key.keysym.sym);

				break;
			}
			break;
		default:
			break;
		}
		i++;
	}
	SDL_Delay(1);
	now = SDL_GetTicks();
	time_diff = now - last_time;
	if (time_diff>0) {
		i = time_diff / (1000 / 60);
		if (i) {
			last_time = now;
		} else {
			int tmp;

			tmp = (1000/60) - i - 10;
			if (tmp>0)
				SDL_Delay(tmp);
		}
	}
/*
	if (!then)
		SDL_Delay(1);
	else {
		then = (1000 / 60) - (now - then);
		if (then > 0 && then < 1000)
			SDL_Delay(then);
	}
	then = now;
*/

	return i;
}
