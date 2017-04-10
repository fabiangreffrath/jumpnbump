/*
 * interrpt.c
 * Copyright (C) 1998 Brainchild Design - http://brainchilddesign.com/
 *
 * Copyright (C) 2001 Chuck Mason <cemason@users.sourceforge.net>
 *
 * Copyright (C) 2002 Florian Schulze <crow@icculus.org>
 *
 * Copyright (C) 2015 Côme Chilliet <come@chilliet.eu>
 *
 * This file is part of Jump 'n Bump.
 *
 * Jump 'n Bump is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Jump 'n Bump is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
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

void remove_keyb_handler(void)
{
}

int hook_keyb_handler(void)
{
	memset((void *) last_keys, 0, sizeof(last_keys));

	return 0;
}

int key_pressed(int key)
{
	return keyb[(unsigned char) key];
}

int intr_sysupdate()
{
	SDL_Event e;
	int i = 0;
	static int last_time = 0;
	int now, time_diff;

	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT:
				SDL_Quit();
				exit(1);
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				if(e.button.state == SDL_PRESSED &&
						((key_pressed(KEY_PL3_LEFT) && e.button.button == SDL_BUTTON_RIGHT) ||
						(key_pressed(KEY_PL3_RIGHT) && e.button.button == SDL_BUTTON_LEFT) ||
						(e.button.button == SDL_BUTTON_LEFT && e.button.button == SDL_BUTTON_RIGHT) ||
			  e.button.button == SDL_BUTTON_MIDDLE))
					{
					addkey(KEY_PL3_JUMP & 0x7f);
					}
				else if(e.button.state == SDL_RELEASED &&
						((key_pressed(KEY_PL3_LEFT) && e.button.button == SDL_BUTTON_RIGHT) ||
						(key_pressed(KEY_PL3_RIGHT) && e.button.button == SDL_BUTTON_LEFT) ||
			  e.button.button == SDL_BUTTON_MIDDLE))
					{
					addkey((KEY_PL3_JUMP & 0x7f) | 0x8000);
					}

				if(e.button.button == SDL_BUTTON_LEFT)
					{
					SDL_Scancode scancode = KEY_PL3_LEFT;
					scancode &= 0x7f;
					if(e.button.state == SDL_RELEASED)
						{
						if(key_pressed(KEY_PL3_JUMP) && (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(SDL_BUTTON_RIGHT)))
							addkey(KEY_PL3_RIGHT & 0x7f);
						else
							scancode |= 0x8000;
						}
					addkey(scancode);
					}
				else if(e.button.button == SDL_BUTTON_RIGHT)
					{
					SDL_Scancode scancode = KEY_PL3_RIGHT;
					scancode &= 0x7f;
					if (e.button.state == SDL_RELEASED)
						{
						if(key_pressed(KEY_PL3_JUMP) && (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(SDL_BUTTON_LEFT)))
							addkey(KEY_PL3_LEFT & 0x7f);
						else
							scancode |= 0x8000;
						}
					addkey(scancode);
					}
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				switch (e.key.keysym.scancode) {
					case SDL_SCANCODE_F12:
						if (e.type == SDL_KEYDOWN) {
							SDL_Quit();
							exit(1);
						}
						break;
					case SDL_SCANCODE_F10:
						if (e.type == SDL_KEYDOWN) {
							fs_toggle();
						}
						break;
					case SDL_SCANCODE_1:
						if (e.type == SDL_KEYUP)
							ai[0] = !ai[0];

						/* Release keys, otherwise it will continue moving that way */
						addkey((KEY_PL1_LEFT & 0x7f) | 0x8000);
						addkey((KEY_PL1_RIGHT & 0x7f) | 0x8000);
						addkey((KEY_PL1_JUMP & 0x7f) | 0x8000);
						break;
					case SDL_SCANCODE_2:
						if (e.type == SDL_KEYUP)
							ai[1] = !ai[1];

						/* Release keys, otherwise it will continue moving that way */
						addkey((KEY_PL2_LEFT & 0x7f) | 0x8000);
						addkey((KEY_PL2_RIGHT & 0x7f) | 0x8000);
						addkey((KEY_PL2_JUMP & 0x7f) | 0x8000);
						break;
					case SDL_SCANCODE_3:
						if (e.type == SDL_KEYUP)
							ai[2] = !ai[2];

						/* Release keys, otherwise it will continue moving that way */
						addkey((KEY_PL3_LEFT & 0x7f) | 0x8000);
						addkey((KEY_PL3_RIGHT & 0x7f) | 0x8000);
						addkey((KEY_PL3_JUMP & 0x7f) | 0x8000);
						break;
					case SDL_SCANCODE_4:
						if (e.type == SDL_KEYUP)
							ai[3] = !ai[3];

						/* Release keys, otherwise it will continue moving that way */
						addkey((KEY_PL4_LEFT & 0x7f) | 0x8000);
						addkey((KEY_PL4_RIGHT & 0x7f) | 0x8000);
						addkey((KEY_PL4_JUMP & 0x7f) | 0x8000);
						break;
					case SDL_SCANCODE_ESCAPE:
						if (e.type == SDL_KEYUP)
							addkey(1 | 0x8000);
						else
							addkey(1 & 0x7f);
						break;
					default:
						e.key.keysym.scancode &= 0x7f;
						if (e.type == SDL_KEYUP)
							e.key.keysym.scancode |= 0x8000;
						addkey(e.key.keysym.scancode);
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

	return i;
}
