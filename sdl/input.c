/*
 * input.c
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

int calib_joy(int type)
{
	return 1;
}

void update_player_actions(void)
{
	player[0].action_left	= key_pressed(KEY_PL1_LEFT) == 1;
	player[0].action_right	= key_pressed(KEY_PL1_RIGHT) == 1;
	player[0].action_up	= key_pressed(KEY_PL1_JUMP) == 1;
	player[1].action_left	= key_pressed(KEY_PL2_LEFT) == 1;
	player[1].action_right	= key_pressed(KEY_PL2_RIGHT) == 1;
	player[1].action_up	= key_pressed(KEY_PL2_JUMP) == 1;
	player[2].action_left	= key_pressed(KEY_PL3_LEFT) == 1;
	player[2].action_right	= key_pressed(KEY_PL3_RIGHT) == 1;
	player[2].action_up	= key_pressed(KEY_PL3_JUMP) == 1;
	player[3].action_left	= key_pressed(KEY_PL4_LEFT) == 1;
	player[3].action_right	= key_pressed(KEY_PL4_RIGHT) == 1;
	player[3].action_up	= key_pressed(KEY_PL4_JUMP) == 1;
}

void init_inputs(void)
{
	main_info.mouse_enabled = 0;
	main_info.joy_enabled = 0;
}
