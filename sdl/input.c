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
