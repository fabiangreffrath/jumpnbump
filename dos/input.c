/*
 * input.h
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

static int num_mouse_buttons;

int init_joy(void)
{
	int c1;

	outportb(0x201, 0);
	while (c1 < 0x7fff) {
		if ((inportb(0x201) & 1) == 0)
			break;
		c1++;
	}
	if (c1 != 0x7fff)
		return 1;
	else
		return 0;
}


void read_joy(void)
{
	int c1;
	int x, y;
	int s1 = 0;
	char flag;

	c1 = x = y = flag = 0;
	outportb(0x201, 0);

	while (1) {

		s1 = inportb(0x201);

		if (x == 0) {
			if ((s1 & 1) == 0)
				x = c1;
		}
		if (y == 0) {
			if ((s1 & 2) == 0)
				y = c1;
		}
		if (x != 0 && y != 0)
			break;

		c1++;
		if (c1 == 0x7fff) {
			flag = 1;
			break;
		}

	}

	if (flag == 0) {
		joy.raw_x = x;
		joy.raw_y = y;

		if (joy.raw_x < joy.calib_data.x2)
			joy.x = ((long) (joy.raw_x - joy.calib_data.x2) << 10) / (joy.calib_data.x2 - joy.calib_data.x1);
		else
			joy.x = ((long) (joy.raw_x - joy.calib_data.x2) << 10) / (joy.calib_data.x3 - joy.calib_data.x2);
		if (joy.raw_y < joy.calib_data.y2)
			joy.y = ((long) (joy.raw_y - joy.calib_data.y2) << 10) / (joy.calib_data.y2 - joy.calib_data.y1);
		else
			joy.y = ((long) (joy.raw_y - joy.calib_data.y2) << 10) / (joy.calib_data.y3 - joy.calib_data.y2);

		if (joy.x < -1024)
			joy.x = -1024;
		if (joy.x > 1024)
			joy.x = 1024;
		if (joy.y < -1024)
			joy.y = -1024;
		if (joy.y > 1024)
			joy.y = 1024;

		s1 = inportb(0x201);
		joy.but1 = (((s1 >> 4) & 1) ^ 1);
		joy.but2 = (((s1 >> 5) & 1) ^ 1);
	} else {
		joy.raw_x = joy.calib_data.x2;
		joy.raw_y = joy.calib_data.y2;

		joy.x = joy.y = 0;

		joy.but1 = joy.but2 = 0;
	}

}


int calib_joy(int type)
{
	int c1;
	int x, y;
	int s1 = 0;
	int num_times;
	char flag = 0;

	while (joy.but1 == 1) {
		s1 = inportb(0x201);
		joy.but1 = (((s1 >> 4) & 1) ^ 1);
		if (key_pressed(1) == 1) {
			while (key_pressed(1) == 1);
			return 1;
		}
	}

	num_times = 0;

	while (joy.but1 == 0) {

		c1 = x = y = flag = 0;
		outportb(0x201, 0);

		while (1) {

			s1 = inportb(0x201);

			if (x == 0) {
				if ((s1 & 1) == 0)
					x = c1;
			}
			if (y == 0) {
				if ((s1 & 2) == 0)
					y = c1;
			}
			if (x != 0 && y != 0)
				break;

			c1++;
			if (c1 == 0x7fff) {
				flag = 1;
				break;
			}

		}

		joy.raw_x = x;
		joy.raw_y = y;

		s1 = inportb(0x201);
		joy.but1 = (((s1 >> 4) & 1) ^ 1);

		if (num_times < 0x7fffffff)
			num_times++;

		if (flag == 1)
			break;

		if (key_pressed(1) == 1) {
			while (key_pressed(1) == 1);
			return 1;
		}

	}

	if (num_times < 16)
		return 1;

	if (flag == 0) {

		switch (type) {
		case 0:
			joy.calib_data.x1 = joy.raw_x;
			joy.calib_data.y1 = joy.raw_y;
			break;
		case 1:
			joy.calib_data.x3 = joy.raw_x;
			joy.calib_data.y3 = joy.raw_y;
			break;
		case 2:
			joy.calib_data.x2 = joy.raw_x;
			joy.calib_data.y2 = joy.raw_y;
			break;
		}

		while (joy.but1 == 1) {
			s1 = inportb(0x201);
			joy.but1 = (((s1 >> 4) & 1) ^ 1);
		}

	}

	return 0;

}


int init_mouse(int *_num_buttons)
{
	__dpmi_regs regs;
	int mouse_enabled, num_mouse_buttons;

	regs.x.ax = 0;
	__dpmi_int(0x33, &regs);
	if (regs.x.ax == 0xffff) {
		mouse_enabled = 1;
		num_mouse_buttons = regs.x.bx;
		if (force2 == 1)
			num_mouse_buttons = 2;
		if (force3 == 1)
			num_mouse_buttons = 3;
	} else {
		mouse_enabled = 0;
	}
	if (_num_buttons)
		_num_buttons = num_mouse_buttons;

	return mouse_enabled;
}


void read_mouse(void)
{

	regs.x.ax = 3;
	__dpmi_int(0x33, &regs);
	mouse.but1 = regs.x.bx & 1;
	mouse.but2 = (regs.x.bx & 2) >> 1;
	mouse.but3 = (regs.x.bx & 4) >> 2;

}


void update_player_actions(void)
{
	if (main_info.mouse_enabled == 1)
		read_mouse();
	if (main_info.joy_enabled == 1)
		read_joy();
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
	main_info.mouse_enabled = init_mouse(&num_mouse_buttons);
	main_info.joy_enabled = init_joy(&num_mouse_buttons);
}

