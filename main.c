#include "globals.h"

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

char *object_gobs;
char *number_gobs;

unsigned int ban_map[17][22] = {
	{1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0},
	{1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1},
	{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
	{1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
	{2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0, 0, 0, 1, 3, 3, 3, 1, 1, 1},
	{2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

struct {
	int num_frames;
	int restart_frame;
	struct {
		int image;
		int ticks;
	} frame[10];
} object_anims[8] = {
	{
		6, 0, {
			{
			0, 3}, {
			1, 3}, {
			2, 3}, {
			3, 3}, {
			4, 3}, {
			5, 3}, {
			0, 0}, {
			0, 0}, {
			0, 0}, {
			0, 0}
		}
	}, {
		9, 0, {
			{
			6, 2}, {
			7, 2}, {
			8, 2}, {
			9, 2}, {
			10, 2}, {
			11, 2}, {
			12, 2}, {
			13, 2}, {
			14, 2}, {
			0, 0}
		}
	}, {
		5, 0, {
			{
			15, 3}, {
			16, 3}, {
			16, 3}, {
			17, 3}, {
			18, 3}, {
			19, 3}, {
			0, 0}, {
			0, 0}, {
			0, 0}, {
			0, 0}
		}
	}, {
		10, 0, {
			{
			20, 2}, {
			21, 2}, {
			22, 2}, {
			23, 2}, {
			24, 2}, {
			25, 2}, {
			24, 2}, {
			23, 2}, {
			22, 2}, {
			21, 2}
		}
	}, {
		10, 0, {
			{
			26, 2}, {
			27, 2}, {
			28, 2}, {
			29, 2}, {
			30, 2}, {
			31, 2}, {
			30, 2}, {
			29, 2}, {
			28, 2}, {
			27, 2}
		}
	}, {
		10, 0, {
			{
			32, 2}, {
			33, 2}, {
			34, 2}, {
			35, 2}, {
			36, 2}, {
			37, 2}, {
			36, 2}, {
			35, 2}, {
			34, 2}, {
			33, 2}
		}
	}, {
		10, 0, {
			{
			38, 2}, {
			39, 2}, {
			40, 2}, {
			41, 2}, {
			42, 2}, {
			43, 2}, {
			42, 2}, {
			41, 2}, {
			40, 2}, {
			39, 2}
		}
	}, {
		4, 0, {
			{
			76, 4}, {
			77, 4}, {
			78, 4}, {
			79, 4}, {
			0, 0}, {
			0, 0}, {
			0, 0}, {
			0, 0}, {
			0, 0}, {
			0, 0}
		}
	}
};

struct {
	int x, y;
	int old_x, old_y;
	char back[2], back_defined[2];
} flies[NUM_FLIES];

struct {
	struct {
		short num_pobs;
		struct {
			int x, y;
			int image;
			char *pob_data;
		} pobs[NUM_LEFTOVERS];
	} page[2];
} leftovers;

char pogostick, bunnies_in_space, jetpack, lord_of_the_flies, blood_is_thicker_than_water;

int main(int argc, char *argv[])
{
	FILE *handle;
	int c1, c2 = 0, c3, c4;
	int l1;
	int s1, s2, s3, s4;
	int closest_player = 0, dist, cur_dist;
	char end_loop_flag, fade_flag;
	char mod_vol, sfx_vol, mod_fade_direction;
	char *ptr1 = (char *) NULL;
	char str1[100];
	char pal[768];
	char cur_pal[768];

	if (init_program(argc, argv, pal) != 0)
		deinit_program();

	if (main_info.fireworks == 1) {
		fireworks();
		deinit_program();
	}

	while (1) {

		if (menu() != 0)
			deinit_program();

		if (key_pressed(1) == 1) {
			break;
		}
		if (init_level(0, pal) != 0) {
			deinit_level();
			deinit_program();
		}

		memset(cur_pal, 0, 768);
		setpalette(0, 256, cur_pal);

		flippage(1);
		put_block(1, 0, 0, 400, 256, background_pic);
		put_block(0, 0, 0, 400, 256, background_pic);
		flippage(0);

		s1 = rnd(250) + 50;
		s2 = rnd(150) + 50;
		for (c1 = 0; c1 < NUM_FLIES; c1++) {
			while (1) {
				flies[c1].x = s1 + rnd(101) - 50;
				flies[c1].y = s2 + rnd(101) - 50;
				if (ban_map[flies[c1].y >> 4][flies[c1].x >> 4] == BAN_VOID)
					break;
			}
			flies[c1].back_defined[0] = 0;
			flies[c1].back_defined[1] = 0;
		}

		mod_vol = sfx_vol = 10;
		mod_fade_direction = 1;
		dj_ready_mod(MOD_GAME);
		dj_set_mod_volume(mod_vol);
		dj_set_sfx_volume(mod_vol);
		dj_start_mod();
		dj_play_sfx(SFX_FLY, SFX_FLY_FREQ, 0, 0, 0, 4);
		dj_set_nosound(0);

		lord_of_the_flies = bunnies_in_space = jetpack = pogostick = blood_is_thicker_than_water = 0;
		end_loop_flag = 0;
		main_info.page_info[0].num_pobs = 0;
		main_info.page_info[1].num_pobs = 0;
		main_info.view_page = 0;
		main_info.draw_page = 1;

		while (1) {

			if (key_pressed(1) == 1) {
				end_loop_flag = 1;
				memset(pal, 0, 768);
				mod_fade_direction = 0;
			}

			if (strncmp(last_keys, "kcitsogop", strlen("kcitsogop")) == 0) {
				pogostick ^= 1;
				last_keys[0] = 0;
			}
			if (strncmp(last_keys, "ecapsniseinnub", strlen("ecapsniseinnub")) == 0) {
				bunnies_in_space ^= 1;
				last_keys[0] = 0;
			}
			if (strncmp(last_keys, "kcaptej", strlen("kcaptej")) == 0) {
				jetpack ^= 1;
				last_keys[0] = 0;
			}
			if (strncmp(last_keys, "seilfehtfodrol", strlen("seilfehtfodrol")) == 0) {
				lord_of_the_flies ^= 1;
				last_keys[0] = 0;
			}
			if (strncmp(last_keys, "retawnahtrekcihtsidoolb", strlen("retawnahtrekcihtsidoolb")) == 0) {
				blood_is_thicker_than_water ^= 1;
				if (blood_is_thicker_than_water == 1) {
					pal[432] = 63;
					pal[433] = 32;
					pal[434] = 32;
					pal[435] = 53;
					pal[436] = 17;
					pal[437] = 17;
					pal[438] = 42;
					pal[439] = 7;
					pal[440] = 7;
					pal[441] = 28;
					pal[442] = 0;
					pal[443] = 0;
					pal[444] = 24;
					pal[445] = 0;
					pal[446] = 0;
					pal[447] = 19;
					pal[448] = 0;
					pal[449] = 0;
					pal[450] = 12;
					pal[451] = 0;
					pal[452] = 0;
					pal[453] = 7;
					pal[454] = 0;
					pal[455] = 0;
				} else {
					pal[432] = 63;
					pal[433] = 63;
					pal[434] = 63;
					pal[435] = 40;
					pal[436] = 53;
					pal[437] = 62;
					pal[438] = 19;
					pal[439] = 42;
					pal[440] = 60;
					pal[441] = 0;
					pal[442] = 33;
					pal[443] = 60;
					pal[444] = 3;
					pal[445] = 32;
					pal[446] = 46;
					pal[447] = 3;
					pal[448] = 26;
					pal[449] = 33;
					pal[450] = 3;
					pal[451] = 19;
					pal[452] = 21;
					pal[453] = 1;
					pal[454] = 8;
					pal[455] = 8;
				}
				last_keys[0] = 0;
			}

			steer_players();

			dj_mix();

			for (c3 = 0; c3 < 6; c3++) {
				if (c3 == 0) {
					c1 = 0;
					c2 = 1;
				} else if (c3 == 1) {
					c1 = 0;
					c2 = 2;
				} else if (c3 == 2) {
					c1 = 0;
					c2 = 3;
				} else if (c3 == 3) {
					c1 = 1;
					c2 = 2;
				} else if (c3 == 4) {
					c1 = 1;
					c2 = 3;
				} else if (c3 == 5) {
					c1 = 2;
					c2 = 3;
				}
				if (player[c1].enabled == 1 && player[c2].enabled == 1) {
					if (labs(player[c1].x - player[c2].x) < (12L << 16) && labs(player[c1].y - player[c2].y) < (12L << 16)) {
						if ((labs(player[c1].y - player[c2].y) >> 16) > 5) {
							if (player[c1].y < player[c2].y) {
								if (player[c1].y_add >= 0) {
									player[c1].y_add = -player[c1].y_add;
									if (player[c1].y_add > -262144L)
										player[c1].y_add = -262144L;
									player[c1].jump_abort = 1;
									player[c2].dead_flag = 1;
									if (player[c2].anim != 6) {
										player[c2].anim = 6;
										player[c2].frame = 0;
										player[c2].frame_tick = 0;
										player[c2].image = player_anims[player[c2].anim].frame[player[c2].frame].image + player[c2].direction * 9;
										if (main_info.no_gore == 0) {
											for (c4 = 0; c4 < 6; c4++)
												add_object(OBJ_FUR, (player[c2].x >> 16) + 6 + rnd(5), (player[c2].y >> 16) + 6 + rnd(5), (rnd(65535) - 32768) * 3, (rnd(65535) - 32768) * 3, 0, 44 + c2 * 8);
											for (c4 = 0; c4 < 6; c4++)
												add_object(OBJ_FLESH, (player[c2].x >> 16) + 6 + rnd(5), (player[c2].y >> 16) + 6 + rnd(5), (rnd(65535) - 32768) * 3, (rnd(65535) - 32768) * 3, 0, 76);
											for (c4 = 0; c4 < 6; c4++)
												add_object(OBJ_FLESH, (player[c2].x >> 16) + 6 + rnd(5), (player[c2].y >> 16) + 6 + rnd(5), (rnd(65535) - 32768) * 3, (rnd(65535) - 32768) * 3, 0, 77);
											for (c4 = 0; c4 < 8; c4++)
												add_object(OBJ_FLESH, (player[c2].x >> 16) + 6 + rnd(5), (player[c2].y >> 16) + 6 + rnd(5), (rnd(65535) - 32768) * 3, (rnd(65535) - 32768) * 3, 0, 78);
											for (c4 = 0; c4 < 10; c4++)
												add_object(OBJ_FLESH, (player[c2].x >> 16) + 6 + rnd(5), (player[c2].y >> 16) + 6 + rnd(5), (rnd(65535) - 32768) * 3, (rnd(65535) - 32768) * 3, 0, 79);
										}
										dj_play_sfx(SFX_DEATH, (unsigned short)(SFX_DEATH_FREQ + rnd(2000) - 1000), 64, 0, 0, -1);
										player[c1].bumps++;
										player[c1].bumped[c2]++;
										s1 = player[c1].bumps % 100;
										add_leftovers(0, 360, 34 + c1 * 64, s1 / 10, number_gobs);
										add_leftovers(1, 360, 34 + c1 * 64, s1 / 10, number_gobs);
										add_leftovers(0, 376, 34 + c1 * 64, s1 - (s1 / 10) * 10, number_gobs);
										add_leftovers(1, 376, 34 + c1 * 64, s1 - (s1 / 10) * 10, number_gobs);
									}
								} else {
									if (player[c2].y_add < 0)
										player[c2].y_add = 0;
								}
							} else {
								if (player[c2].y_add >= 0) {
									player[c2].y_add = -player[c2].y_add;
									if (player[c2].y_add > -262144L)
										player[c2].y_add = -262144L;
									player[c2].jump_abort = 1;
									player[c1].dead_flag = 1;
									if (player[c1].anim != 6) {
										player[c1].anim = 6;
										player[c1].frame = 0;
										player[c1].frame_tick = 0;
										player[c1].image = player_anims[player[c1].anim].frame[player[c1].frame].image + player[c1].direction * 9;
										if (main_info.no_gore == 0) {
											for (c4 = 0; c4 < 6; c4++)
												add_object(OBJ_FUR, (player[c1].x >> 16) + 6 + rnd(5), (player[c1].y >> 16) + 6 + rnd(5), (rnd(65535) - 32768) * 3, (rnd(65535) - 32768) * 3, 0, 44 + c1 * 8);
											for (c4 = 0; c4 < 6; c4++)
												add_object(OBJ_FLESH, (player[c1].x >> 16) + 6 + rnd(5), (player[c1].y >> 16) + 6 + rnd(5), (rnd(65535) - 32768) * 3, (rnd(65535) - 32768) * 3, 0, 76);
											for (c4 = 0; c4 < 7; c4++)
												add_object(OBJ_FLESH, (player[c1].x >> 16) + 6 + rnd(5), (player[c1].y >> 16) + 6 + rnd(5), (rnd(65535) - 32768) * 3, (rnd(65535) - 32768) * 3, 0, 77);
											for (c4 = 0; c4 < 8; c4++)
												add_object(OBJ_FLESH, (player[c1].x >> 16) + 6 + rnd(5), (player[c1].y >> 16) + 6 + rnd(5), (rnd(65535) - 32768) * 3, (rnd(65535) - 32768) * 3, 0, 78);
											for (c4 = 0; c4 < 10; c4++)
												add_object(OBJ_FLESH, (player[c1].x >> 16) + 6 + rnd(5), (player[c1].y >> 16) + 6 + rnd(5), (rnd(65535) - 32768) * 3, (rnd(65535) - 32768) * 3, 0, 79);
										}
										dj_play_sfx(SFX_DEATH, (unsigned short)(SFX_DEATH_FREQ + rnd(2000) - 1000), 64, 0, 0, -1);
										player[c2].bumps++;
										player[c2].bumped[c1]++;
										s1 = player[c2].bumps % 100;
										add_leftovers(0, 360, 34 + c2 * 64, s1 / 10, number_gobs);
										add_leftovers(1, 360, 34 + c2 * 64, s1 / 10, number_gobs);
										add_leftovers(0, 376, 34 + c2 * 64, s1 - (s1 / 10) * 10, number_gobs);
										add_leftovers(1, 376, 34 + c2 * 64, s1 - (s1 / 10) * 10, number_gobs);
									}
								} else {
									if (player[c1].y_add < 0)
										player[c1].y_add = 0;
								}
							}
						} else {
							if (player[c1].x < player[c2].x) {
								if (player[c1].x_add > 0)
									player[c1].x = player[c2].x - (12L << 16);
								else if (player[c2].x_add < 0)
									player[c2].x = player[c1].x + (12L << 16);
								else {
									player[c1].x -= player[c1].x_add;
									player[c2].x -= player[c2].x_add;
								}
								l1 = player[c2].x_add;
								player[c2].x_add = player[c1].x_add;
								player[c1].x_add = l1;
								if (player[c1].x_add > 0)
									player[c1].x_add = -player[c1].x_add;
								if (player[c2].x_add < 0)
									player[c2].x_add = -player[c2].x_add;
							} else {
								if (player[c1].x_add > 0)
									player[c2].x = player[c1].x - (12L << 16);
								else if (player[c2].x_add < 0)
									player[c1].x = player[c2].x + (12L << 16);
								else {
									player[c1].x -= player[c1].x_add;
									player[c2].x -= player[c2].x_add;
								}
								l1 = player[c2].x_add;
								player[c2].x_add = player[c1].x_add;
								player[c1].x_add = l1;
								if (player[c1].x_add < 0)
									player[c1].x_add = -player[c1].x_add;
								if (player[c2].x_add > 0)
									player[c2].x_add = -player[c2].x_add;
							}
						}
					}
				}
			}

			dj_mix();

			main_info.page_info[main_info.draw_page].num_pobs = 0;
			for (c1 = 0; c1 < 4; c1++) {
				if (player[c1].enabled == 1)
					main_info.page_info[main_info.draw_page].num_pobs++;
			}

			update_objects();

			dj_mix();

			/* get center of fly swarm */
			s1 = s2 = 0;
			for (c1 = 0; c1 < NUM_FLIES; c1++) {
				s1 += flies[c1].x;
				s2 += flies[c1].y;
			}
			s1 /= NUM_FLIES;
			s2 /= NUM_FLIES;

			/* get closest player to fly swarm */
			dist = 0x7fff;
			for (c1 = 0; c1 < 4; c1++) {
				if (player[c1].enabled == 1) {
					cur_dist = (int)sqrt((s1 - ((player[c1].x >> 16) + 8)) * (s1 - ((player[c1].x >> 16) + 8)) + (s2 - ((player[c1].y >> 16) + 8)) * (s2 - ((player[c1].y >> 16) + 8)));
					if (cur_dist < dist) {
						closest_player = c1;
						dist = cur_dist;
					}
				}
			}
			/* update fly swarm sound */
			s3 = 32 - dist / 3;
			if (s3 < 0)
				s3 = 0;
			dj_set_sfx_channel_volume(4, (char)(s3));

			for (c1 = 0; c1 < NUM_FLIES; c1++) {
				/* get closest player to fly */
				dist = 0x7fff;
				for (c2 = 0; c2 < 4; c2++) {
					if (player[c2].enabled == 1) {
						cur_dist = (int)sqrt((flies[c1].x - ((player[c2].x >> 16) + 8)) * (flies[c1].x - ((player[c2].x >> 16) + 8)) + (flies[c1].y - ((player[c2].y >> 16) + 8)) * (flies[c1].y - ((player[c2].y >> 16) + 8)));
						if (cur_dist < dist) {
							closest_player = c2;
							dist = cur_dist;
						}
					}
				}
				flies[c1].old_x = flies[c1].x;
				flies[c1].old_y = flies[c1].y;
				s3 = 0;
				if ((s1 - flies[c1].x) > 30)
					s3 += 1;
				else if ((s1 - flies[c1].x) < -30)
					s3 -= 1;
				if (dist < 30) {
					if (((player[closest_player].x >> 16) + 8) > flies[c1].x) {
						if (lord_of_the_flies == 0)
							s3 -= 1;
						else
							s3 += 1;
					} else {
						if (lord_of_the_flies == 0)
							s3 += 1;
						else
							s3 -= 1;
					}
				}
				s4 = rnd(3) - 1 + s3;
				if ((flies[c1].x + s4) < 16)
					s4 = 0;
				if ((flies[c1].x + s4) > 351)
					s4 = 0;
				if (ban_map[flies[c1].y >> 4][(flies[c1].x + s4) >> 4] != BAN_VOID)
					s4 = 0;
				flies[c1].x += s4;
				s3 = 0;
				if ((s2 - flies[c1].y) > 30)
					s3 += 1;
				else if ((s2 - flies[c1].y) < -30)
					s3 -= 1;
				if (dist < 30) {
					if (((player[closest_player].y >> 16) + 8) > flies[c1].y) {
						if (lord_of_the_flies == 0)
							s3 -= 1;
						else
							s3 += 1;
					} else {
						if (lord_of_the_flies == 0)
							s3 += 1;
						else
							s3 -= 1;
					}
				}
				s4 = rnd(3) - 1 + s3;
				if ((flies[c1].y + s4) < 0)
					s4 = 0;
				if ((flies[c1].y + s4) > 239)
					s4 = 0;
				if (ban_map[(flies[c1].y + s4) >> 4][flies[c1].x >> 4] != BAN_VOID)
					s4 = 0;
				flies[c1].y += s4;
			}

			dj_mix();

			s1 = 0;
			for (c1 = 0; c1 < 4; c1++) {
				if (player[c1].enabled == 1) {
					main_info.page_info[main_info.draw_page].pobs[s1].x = player[c1].x >> 16;
					main_info.page_info[main_info.draw_page].pobs[s1].y = player[c1].y >> 16;
					main_info.page_info[main_info.draw_page].pobs[s1].image = player[c1].image + c1 * 18;
					main_info.page_info[main_info.draw_page].pobs[s1].pob_data = rabbit_gobs;
					s1++;
				}
			}

			draw_pobs(main_info.draw_page);

			dj_mix();

			draw_flies(main_info.draw_page);

			if (mod_fade_direction == 1) {
				if (mod_vol < 30) {
					mod_vol++;
					dj_set_mod_volume(mod_vol);
				}
			} else {
				if (mod_vol > 0) {
					mod_vol--;
					dj_set_mod_volume(mod_vol);
				}
			}

			if (mod_fade_direction == 1) {
				if (sfx_vol < 64) {
					sfx_vol++;
					dj_set_sfx_volume(sfx_vol);
				}
			} else {
				if (sfx_vol > 0) {
					sfx_vol--;
					dj_set_sfx_volume(sfx_vol);
				}
			}

			fade_flag = 0;
			for (c1 = 0; c1 < 768; c1++) {
				if (cur_pal[c1] < pal[c1]) {
					cur_pal[c1]++;
					fade_flag = 1;
				} else if (cur_pal[c1] > pal[c1]) {
					cur_pal[c1]--;
					fade_flag = 1;
				}
			}
			if (fade_flag == 0 && end_loop_flag == 1)
				break;

			main_info.draw_page ^= 1;
			main_info.view_page ^= 1;

			flippage(main_info.view_page);

			wait_vrt(1);

			if (fade_flag == 1)
				setpalette(0, 256, cur_pal);

			redraw_flies_background(main_info.draw_page);

			redraw_pob_backgrounds(main_info.draw_page);

			draw_leftovers(main_info.draw_page);

			intr_sysupdate();

		}

		main_info.view_page = 0;
		main_info.draw_page = 1;

		dj_stop_sfx_channel(4);

		deinit_level();

		memset(mask_pic, 0, 102400L);

		clear_page(main_info.view_page, 0);

		put_text(main_info.view_page, 100, 50, "DOTT", 2);
		put_text(main_info.view_page, 160, 50, "JIFFY", 2);
		put_text(main_info.view_page, 220, 50, "FIZZ", 2);
		put_text(main_info.view_page, 280, 50, "MIJJI", 2);
		put_text(main_info.view_page, 40, 80, "DOTT", 2);
		put_text(main_info.view_page, 40, 110, "JIFFY", 2);
		put_text(main_info.view_page, 40, 140, "FIZZ", 2);
		put_text(main_info.view_page, 40, 170, "MIJJI", 2);

		for (c1 = 0; c1 < 4; c1++) {
			for (c2 = 0; c2 < 4; c2++) {
				if (c2 != c1) {
					sprintf(str1, "%d", player[c1].bumped[c2]);
					put_text(main_info.view_page, 100 + c2 * 60, 80 + c1 * 30, str1, 2);
				} else
					put_text(main_info.view_page, 100 + c2 * 60, 80 + c1 * 30, "-", 2);
			}
			sprintf(str1, "%d", player[c1].bumps);
			put_text(main_info.view_page, 350, 80 + c1 * 30, str1, 2);
		}

		put_text(main_info.view_page, 200, 230, "Press ESC to continue", 2);

		flippage(main_info.view_page);

		if ((handle = dat_open("menu.pcx", datfile_name, "rb")) == 0) {
			strcpy(main_info.error_str, "Error loading 'menu.pcx', aborting...\n");
			return 1;
		}
		if (read_pcx(handle, background_pic, 102400L, pal) != 0) {
			strcpy(main_info.error_str, "Error loading 'menu.pcx', aborting...\n");
			return 1;
		}
		fclose(handle);

		memset(cur_pal, 0, 768);

		setpalette(0, 256, cur_pal);

		mod_vol = 0;
		dj_ready_mod(MOD_SCORES);
		dj_set_mod_volume(mod_vol);
		dj_start_mod();
		dj_set_nosound(0);

		while (key_pressed(1) == 0) {
			if (mod_vol < 35)
				mod_vol++;
			dj_set_mod_volume(mod_vol);
			for (c1 = 0; c1 < 768; c1++) {
				if (cur_pal[c1] < pal[c1])
					cur_pal[c1]++;
			}
			dj_mix();
			intr_sysupdate();
			wait_vrt(0);
			setpalette(0, 256, cur_pal);
		}
		while (key_pressed(1) == 1) {
			dj_mix();
			intr_sysupdate();
		}

		memset(pal, 0, 768);

		while (mod_vol > 0) {
			mod_vol--;
			dj_set_mod_volume(mod_vol);
			for (c1 = 0; c1 < 768; c1++) {
				if (cur_pal[c1] > pal[c1])
					cur_pal[c1]--;
			}
			dj_mix();
			wait_vrt(0);
			setpalette(0, 256, cur_pal);
		}

		fillpalette(0, 0, 0);

		dj_set_nosound(1);
		dj_stop_mod();

	}

	deinit_program();

	return 0;
}


void steer_players(void)
{
	int c1, c2;
	int s1 = 0, s2 = 0;

	update_player_actions();

	for (c1 = 0; c1 < 4; c1++) {

		if (player[c1].enabled == 1) {

			if (player[c1].dead_flag == 0) {

				if (player[c1].action_left && player[c1].action_right) {
					if (player[c1].direction == 0) {
						if (player[c1].action_right) {
							s1 = (player[c1].x >> 16);
							s2 = (player[c1].y >> 16);
							if (ban_map[(s2 + 16) >> 4][(s1 + 8) >> 4] == BAN_ICE) {
								if (player[c1].x_add < 0)
									player[c1].x_add += 1024;
								else
									player[c1].x_add += 768;
							} else if ((ban_map[(s2 + 16) >> 4][s1 >> 4] != BAN_SOLID && ban_map[(s2 + 16) >> 4][(s1 + 15) >> 4] == BAN_ICE) || (ban_map[(s2 + 16) >> 4][s1 >> 4] == BAN_ICE && ban_map[(s2 + 16) >> 4][(s1 + 15) >> 4] != BAN_SOLID)) {
								if (player[c1].x_add > 0)
									player[c1].x_add += 1024;
								else
									player[c1].x_add += 768;
							} else {
								if (player[c1].x_add < 0) {
									player[c1].x_add += 16384;
									if (player[c1].x_add < 98304L && player[c1].in_water == 0 && ban_map[(s2 + 16) >> 4][(s1 + 8) >> 4] == BAN_SOLID)
										add_object(OBJ_SMOKE, (player[c1].x >> 16) + 2 + rnd(9), (player[c1].y >> 16) + 13 + rnd(5), 0, -16384 - rnd(8192), OBJ_ANIM_SMOKE, 0);
								} else
									player[c1].x_add += 12288;
							}
							if (player[c1].x_add > 98304L)
								player[c1].x_add = 98304L;
							player[c1].direction = 0;
							if (player[c1].anim == 0) {
								player[c1].anim = 1;
								player[c1].frame = 0;
								player[c1].frame_tick = 0;
								player[c1].image = player_anims[player[c1].anim].frame[player[c1].frame].image + player[c1].direction * 9;
							}
						}
					} else {
						if (player[c1].action_left) {
							s1 = (player[c1].x >> 16);
							s2 = (player[c1].y >> 16);
							if (ban_map[(s2 + 16) >> 4][(s1 + 8) >> 4] == BAN_ICE) {
								if (player[c1].x_add > 0)
									player[c1].x_add -= 1024;
								else
									player[c1].x_add -= 768;
							} else if ((ban_map[(s2 + 16) >> 4][s1 >> 4] != BAN_SOLID && ban_map[(s2 + 16) >> 4][(s1 + 15) >> 4] == BAN_ICE) || (ban_map[(s2 + 16) >> 4][s1 >> 4] == BAN_ICE && ban_map[(s2 + 16) >> 4][(s1 + 15) >> 4] != BAN_SOLID)) {
								if (player[c1].x_add > 0)
									player[c1].x_add -= 1024;
								else
									player[c1].x_add -= 768;
							} else {
								if (player[c1].x_add > 0) {
									player[c1].x_add -= 16384;
									if (player[c1].x_add > -98304L && player[c1].in_water == 0 && ban_map[(s2 + 16) >> 4][(s1 + 8) >> 4] == BAN_SOLID)
										add_object(OBJ_SMOKE, (player[c1].x >> 16) + 2 + rnd(9), (player[c1].y >> 16) + 13 + rnd(5), 0, -16384 - rnd(8192), OBJ_ANIM_SMOKE, 0);
								} else
									player[c1].x_add -= 12288;
							}
							if (player[c1].x_add < -98304L)
								player[c1].x_add = -98304L;
							player[c1].direction = 1;
							if (player[c1].anim == 0) {
								player[c1].anim = 1;
								player[c1].frame = 0;
								player[c1].frame_tick = 0;
								player[c1].image = player_anims[player[c1].anim].frame[player[c1].frame].image + player[c1].direction * 9;
							}
						}
					}
				} else if (player[c1].action_left) {
					s1 = (player[c1].x >> 16);
					s2 = (player[c1].y >> 16);
					if (ban_map[(s2 + 16) >> 4][(s1 + 8) >> 4] == BAN_ICE) {
						if (player[c1].x_add > 0)
							player[c1].x_add -= 1024;
						else
							player[c1].x_add -= 768;
					} else if ((ban_map[(s2 + 16) >> 4][s1 >> 4] != BAN_SOLID && ban_map[(s2 + 16) >> 4][(s1 + 15) >> 4] == BAN_ICE) || (ban_map[(s2 + 16) >> 4][s1 >> 4] == BAN_ICE && ban_map[(s2 + 16) >> 4][(s1 + 15) >> 4] != BAN_SOLID)) {
						if (player[c1].x_add > 0)
							player[c1].x_add -= 1024;
						else
							player[c1].x_add -= 768;
					} else {
						if (player[c1].x_add > 0) {
							player[c1].x_add -= 16384;
							if (player[c1].x_add > -98304L && player[c1].in_water == 0 && ban_map[(s2 + 16) >> 4][(s1 + 8) >> 4] == BAN_SOLID)
								add_object(OBJ_SMOKE, (player[c1].x >> 16) + 2 + rnd(9), (player[c1].y >> 16) + 13 + rnd(5), 0, -16384 - rnd(8192), OBJ_ANIM_SMOKE, 0);
						} else
							player[c1].x_add -= 12288;
					}
					if (player[c1].x_add < -98304L)
						player[c1].x_add = -98304L;
					player[c1].direction = 1;
					if (player[c1].anim == 0) {
						player[c1].anim = 1;
						player[c1].frame = 0;
						player[c1].frame_tick = 0;
						player[c1].image = player_anims[player[c1].anim].frame[player[c1].frame].image + player[c1].direction * 9;
					}
				} else if (player[c1].action_right) {
					s1 = (player[c1].x >> 16);
					s2 = (player[c1].y >> 16);
					if (ban_map[(s2 + 16) >> 4][(s1 + 8) >> 4] == BAN_ICE) {
						if (player[c1].x_add < 0)
							player[c1].x_add += 1024;
						else
							player[c1].x_add += 768;
					} else if ((ban_map[(s2 + 16) >> 4][s1 >> 4] != BAN_SOLID && ban_map[(s2 + 16) >> 4][(s1 + 15) >> 4] == BAN_ICE) || (ban_map[(s2 + 16) >> 4][s1 >> 4] == BAN_ICE && ban_map[(s2 + 16) >> 4][(s1 + 15) >> 4] != BAN_SOLID)) {
						if (player[c1].x_add > 0)
							player[c1].x_add += 1024;
						else
							player[c1].x_add += 768;
					} else {
						if (player[c1].x_add < 0) {
							player[c1].x_add += 16384;
							if (player[c1].x_add < 98304L && player[c1].in_water == 0 && ban_map[(s2 + 16) >> 4][(s1 + 8) >> 4] == BAN_SOLID)
								add_object(OBJ_SMOKE, (player[c1].x >> 16) + 2 + rnd(9), (player[c1].y >> 16) + 13 + rnd(5), 0, -16384 - rnd(8192), OBJ_ANIM_SMOKE, 0);
						} else
							player[c1].x_add += 12288;
					}
					if (player[c1].x_add > 98304L)
						player[c1].x_add = 98304L;
					player[c1].direction = 0;
					if (player[c1].anim == 0) {
						player[c1].anim = 1;
						player[c1].frame = 0;
						player[c1].frame_tick = 0;
						player[c1].image = player_anims[player[c1].anim].frame[player[c1].frame].image + player[c1].direction * 9;
					}
				} else if ((!player[c1].action_left) && (!player[c1].action_right)) {
					s1 = (player[c1].x >> 16);
					s2 = (player[c1].y >> 16);
					if (ban_map[(s2 + 16) >> 4][(s1 + 8) >> 4] == BAN_SOLID || ban_map[(s2 + 16) >> 4][(s1 + 8) >> 4] == BAN_SPRING || (((ban_map[(s2 + 16) >> 4][s1 >> 4] == BAN_SOLID || ban_map[(s2 + 16) >> 4][s1 >> 4] == BAN_SPRING) && ban_map[(s2 + 16) >> 4][(s1 + 15) >> 4] != BAN_ICE) || (ban_map[(s2 + 16) >> 4][s1 >> 4] != BAN_ICE && (ban_map[(s2 + 16) >> 4][(s1 + 15) >> 4] == BAN_SOLID || ban_map[(s2 + 16) >> 4][(s1 + 15) >> 4] == BAN_SPRING)))) {
						if (player[c1].x_add < 0) {
							player[c1].x_add += 16384;
							if (player[c1].x_add > 0)
								player[c1].x_add = 0;
						} else {
							player[c1].x_add -= 16384;
							if (player[c1].x_add < 0)
								player[c1].x_add = 0;
						}
						if (player[c1].x_add != 0 && ban_map[(s2 + 16) >> 4][(s1 + 8) >> 4] == BAN_SOLID)
							add_object(OBJ_SMOKE, (player[c1].x >> 16) + 2 + rnd(9), (player[c1].y >> 16) + 13 + rnd(5), 0, -16384 - rnd(8192), OBJ_ANIM_SMOKE, 0);
					}
					if (player[c1].anim == 1) {
						player[c1].anim = 0;
						player[c1].frame = 0;
						player[c1].frame_tick = 0;
						player[c1].image = player_anims[player[c1].anim].frame[player[c1].frame].image + player[c1].direction * 9;
					}
				}
				if (jetpack == 0) {
					if (pogostick == 1 || (player[c1].jump_ready == 1 && player[c1].action_up)) {
						s1 = (player[c1].x >> 16);
						s2 = (player[c1].y >> 16);
						if (s2 < -16)
							s2 = -16;
						if (ban_map[(s2 + 16) >> 4][s1 >> 4] == BAN_SOLID || ban_map[(s2 + 16) >> 4][s1 >> 4] == BAN_ICE || ban_map[(s2 + 16) >> 4][(s1 + 15) >> 4] == BAN_SOLID || ban_map[(s2 + 16) >> 4][(s1 + 15) >> 4] == BAN_ICE) {
							player[c1].y_add = -280000L;
							player[c1].anim = 2;
							player[c1].frame = 0;
							player[c1].frame_tick = 0;
							player[c1].image = player_anims[player[c1].anim].frame[player[c1].frame].image + player[c1].direction * 9;
							player[c1].jump_ready = 0;
							player[c1].jump_abort = 1;
							if (pogostick == 0)
								dj_play_sfx(SFX_JUMP, (unsigned short)(SFX_JUMP_FREQ + rnd(2000) - 1000), 64, 0, 0, -1);
							else
								dj_play_sfx(SFX_SPRING, (unsigned short)(SFX_SPRING_FREQ + rnd(2000) - 1000), 64, 0, 0, -1);
						}
						if ((ban_map[(s2 + 7) >> 4][s1 >> 4] == BAN_VOID || ban_map[(s2 + 7) >> 4][(s1 + 15) >> 4] == BAN_VOID) && (ban_map[(s2 + 8) >> 4][s1 >> 4] == BAN_WATER || ban_map[(s2 + 8) >> 4][(s1 + 15) >> 4] == BAN_WATER)) {
							player[c1].y_add = -196608L;
							player[c1].in_water = 0;
							player[c1].anim = 2;
							player[c1].frame = 0;
							player[c1].frame_tick = 0;
							player[c1].image = player_anims[player[c1].anim].frame[player[c1].frame].image + player[c1].direction * 9;
							player[c1].jump_ready = 0;
							player[c1].jump_abort = 1;
							if (pogostick == 0)
								dj_play_sfx(SFX_JUMP, (unsigned short)(SFX_JUMP_FREQ + rnd(2000) - 1000), 64, 0, 0, -1);
							else
								dj_play_sfx(SFX_SPRING, (unsigned short)(SFX_SPRING_FREQ + rnd(2000) - 1000), 64, 0, 0, -1);
						}
					}
					if (pogostick == 0 && (!player[c1].action_up)) {
						player[c1].jump_ready = 1;
						if (player[c1].in_water == 0 && player[c1].y_add < 0 && player[c1].jump_abort == 1) {
							if (bunnies_in_space == 0)
								player[c1].y_add += 32768;
							else
								player[c1].y_add += 16384;
							if (player[c1].y_add > 0)
								player[c1].y_add = 0;
						}
					}
				} else {

					if (player[c1].action_up) {
						player[c1].y_add -= 16384;
						if (player[c1].y_add < -400000L)
							player[c1].y_add = -400000L;
						if ((ban_map[(s2 + 7) >> 4][s1 >> 4] == BAN_VOID || ban_map[(s2 + 7) >> 4][(s1 + 15) >> 4] == BAN_VOID) && (ban_map[(s2 + 8) >> 4][s1 >> 4] == BAN_WATER || ban_map[(s2 + 8) >> 4][(s1 + 15) >> 4] == BAN_WATER))
							player[c1].in_water = 0;
						if (rnd(100) < 50)
							add_object(OBJ_SMOKE, (player[c1].x >> 16) + 6 + rnd(5), (player[c1].y >> 16) + 10 + rnd(5), 0, 16384 + rnd(8192), OBJ_ANIM_SMOKE, 0);
					}

				}

				player[c1].x += player[c1].x_add;
				if ((player[c1].x >> 16) < 0) {
					player[c1].x = 0;
					player[c1].x_add = 0;
				}
				if ((player[c1].x >> 16) + 15 > 351) {
					player[c1].x = 336L << 16;
					player[c1].x_add = 0;
				}
				if (player[c1].y > 0) {
					s1 = (player[c1].x >> 16);
					s2 = (player[c1].y >> 16);
					if (ban_map[s2 >> 4][s1 >> 4] == BAN_SOLID || ban_map[s2 >> 4][s1 >> 4] == BAN_ICE || ban_map[s2 >> 4][s1 >> 4] == BAN_SPRING || ban_map[(s2 + 15) >> 4][s1 >> 4] == BAN_SOLID || ban_map[(s2 + 15) >> 4][s1 >> 4] == BAN_ICE || ban_map[(s2 + 15) >> 4][s1 >> 4] == BAN_SPRING) {
						player[c1].x = (((s1 + 16) & 0xfff0)) << 16;
						player[c1].x_add = 0;
					}
					s1 = (player[c1].x >> 16);
					s2 = (player[c1].y >> 16);
					if (ban_map[s2 >> 4][(s1 + 15) >> 4] == BAN_SOLID || ban_map[s2 >> 4][(s1 + 15) >> 4] == BAN_ICE || ban_map[s2 >> 4][(s1 + 15) >> 4] == BAN_SPRING || ban_map[(s2 + 15) >> 4][(s1 + 15) >> 4] == BAN_SOLID || ban_map[(s2 + 15) >> 4][(s1 + 15) >> 4] == BAN_ICE || ban_map[(s2 + 15) >> 4][(s1 + 15) >> 4] == BAN_SPRING) {
						player[c1].x = (((s1 + 16) & 0xfff0) - 16) << 16;
						player[c1].x_add = 0;
					}
				} else {
					s1 = (player[c1].x >> 16);
					s2 = 0;
					if (ban_map[s2 >> 4][s1 >> 4] == BAN_SOLID || ban_map[s2 >> 4][s1 >> 4] == BAN_ICE || ban_map[s2 >> 4][s1 >> 4] == BAN_SPRING || ban_map[(s2 + 15) >> 4][s1 >> 4] == BAN_SOLID || ban_map[(s2 + 15) >> 4][s1 >> 4] == BAN_ICE || ban_map[(s2 + 15) >> 4][s1 >> 4] == BAN_SPRING) {
						player[c1].x = (((s1 + 16) & 0xfff0)) << 16;
						player[c1].x_add = 0;
					}
					s1 = (player[c1].x >> 16);
					s2 = 0;
					if (ban_map[s2 >> 4][(s1 + 15) >> 4] == BAN_SOLID || ban_map[s2 >> 4][(s1 + 15) >> 4] == BAN_ICE || ban_map[s2 >> 4][(s1 + 15) >> 4] == BAN_SPRING || ban_map[(s2 + 15) >> 4][(s1 + 15) >> 4] == BAN_SOLID || ban_map[(s2 + 15) >> 4][(s1 + 15) >> 4] == BAN_ICE || ban_map[(s2 + 15) >> 4][(s1 + 15) >> 4] == BAN_SPRING) {
						player[c1].x = (((s1 + 16) & 0xfff0) - 16) << 16;
						player[c1].x_add = 0;
					}
				}

				player[c1].y += player[c1].y_add;

				s1 = (player[c1].x >> 16);
				s2 = (player[c1].y >> 16);
				if (ban_map[(s2 + 15) >> 4][(s1 + 8) >> 4] == BAN_SPRING || ((ban_map[(s2 + 15) >> 4][s1 >> 4] == BAN_SPRING && ban_map[(s2 + 15) >> 4][(s1 + 15) >> 4] != BAN_SOLID) || (ban_map[(s2 + 15) >> 4][s1 >> 4] != BAN_SOLID && ban_map[(s2 + 15) >> 4][(s1 + 15) >> 4] == BAN_SPRING))) {
					player[c1].y = ((player[c1].y >> 16) & 0xfff0) << 16;
					player[c1].y_add = -400000L;
					player[c1].anim = 2;
					player[c1].frame = 0;
					player[c1].frame_tick = 0;
					player[c1].image = player_anims[player[c1].anim].frame[player[c1].frame].image + player[c1].direction * 9;
					player[c1].jump_ready = 0;
					player[c1].jump_abort = 0;
					for (c2 = 0; c2 < NUM_OBJECTS; c2++) {
						if (objects[c2].used == 1 && objects[c2].type == OBJ_SPRING) {
							if (ban_map[(s2 + 15) >> 4][(s1 + 8) >> 4] == BAN_SPRING) {
								if ((objects[c2].x >> 20) == ((s1 + 8) >> 4) && (objects[c2].y >> 20) == ((s2 + 15) >> 4)) {
									objects[c2].frame = 0;
									objects[c2].ticks = object_anims[objects[c2].anim].frame[objects[c2].frame].ticks;
									objects[c2].image = object_anims[objects[c2].anim].frame[objects[c2].frame].image;
									break;
								}
							} else {
								if (ban_map[(s2 + 15) >> 4][s1 >> 4] == BAN_SPRING) {
									if ((objects[c2].x >> 20) == (s1 >> 4) && (objects[c2].y >> 20) == ((s2 + 15) >> 4)) {
										objects[c2].frame = 0;
										objects[c2].ticks = object_anims[objects[c2].anim].frame[objects[c2].frame].ticks;
										objects[c2].image = object_anims[objects[c2].anim].frame[objects[c2].frame].image;
										break;
									}
								} else if (ban_map[(s2 + 15) >> 4][(s1 + 15) >> 4] == BAN_SPRING) {
									if ((objects[c2].x >> 20) == ((s1 + 15) >> 4) && (objects[c2].y >> 20) == ((s2 + 15) >> 4)) {
										objects[c2].frame = 0;
										objects[c2].ticks = object_anims[objects[c2].anim].frame[objects[c2].frame].ticks;
										objects[c2].image = object_anims[objects[c2].anim].frame[objects[c2].frame].image;
										break;
									}
								}
							}
						}
					}
					dj_play_sfx(SFX_SPRING, (unsigned short)(SFX_SPRING_FREQ + rnd(2000) - 1000), 64, 0, 0, -1);
				}
				s1 = (player[c1].x >> 16);
				s2 = (player[c1].y >> 16);
				if (s2 < 0)
					s2 = 0;
				if (ban_map[s2 >> 4][s1 >> 4] == BAN_SOLID || ban_map[s2 >> 4][s1 >> 4] == BAN_ICE || ban_map[s2 >> 4][s1 >> 4] == BAN_SPRING || ban_map[s2 >> 4][(s1 + 15) >> 4] == BAN_SOLID || ban_map[s2 >> 4][(s1 + 15) >> 4] == BAN_ICE || ban_map[s2 >> 4][(s1 + 15) >> 4] == BAN_SPRING) {
					player[c1].y = (((s2 + 16) & 0xfff0)) << 16;
					player[c1].y_add = 0;
					player[c1].anim = 0;
					player[c1].frame = 0;
					player[c1].frame_tick = 0;
					player[c1].image = player_anims[player[c1].anim].frame[player[c1].frame].image + player[c1].direction * 9;
				}
				s1 = (player[c1].x >> 16);
				s2 = (player[c1].y >> 16);
				if (s2 < 0)
					s2 = 0;
				if (ban_map[(s2 + 8) >> 4][(s1 + 8) >> 4] == BAN_WATER) {
					if (player[c1].in_water == 0) {
						player[c1].in_water = 1;
						player[c1].anim = 4;
						player[c1].frame = 0;
						player[c1].frame_tick = 0;
						player[c1].image = player_anims[player[c1].anim].frame[player[c1].frame].image + player[c1].direction * 9;
						if (player[c1].y_add >= 32768) {
							add_object(OBJ_SPLASH, (player[c1].x >> 16) + 8, ((player[c1].y >> 16) & 0xfff0) + 15, 0, 0, OBJ_ANIM_SPLASH, 0);
							if (blood_is_thicker_than_water == 0)
								dj_play_sfx(SFX_SPLASH, (unsigned short)(SFX_SPLASH_FREQ + rnd(2000) - 1000), 64, 0, 0, -1);
							else
								dj_play_sfx(SFX_SPLASH, (unsigned short)(SFX_SPLASH_FREQ + rnd(2000) - 5000), 64, 0, 0, -1);
						}
					}
					player[c1].y_add -= 1536;
					if (player[c1].y_add < 0 && player[c1].anim != 5) {
						player[c1].anim = 5;
						player[c1].frame = 0;
						player[c1].frame_tick = 0;
						player[c1].image = player_anims[player[c1].anim].frame[player[c1].frame].image + player[c1].direction * 9;
					}
					if (player[c1].y_add < -65536L)
						player[c1].y_add = -65536L;
					if (player[c1].y_add > 65535L)
						player[c1].y_add = 65535L;
					if (ban_map[(s2 + 15) >> 4][s1 >> 4] == BAN_SOLID || ban_map[(s2 + 15) >> 4][s1 >> 4] == BAN_ICE || ban_map[(s2 + 15) >> 4][(s1 + 15) >> 4] == BAN_SOLID || ban_map[(s2 + 15) >> 4][(s1 + 15) >> 4] == BAN_ICE) {
						player[c1].y = (((s2 + 16) & 0xfff0) - 16) << 16;
						player[c1].y_add = 0;
					}
				} else if (ban_map[(s2 + 15) >> 4][s1 >> 4] == BAN_SOLID || ban_map[(s2 + 15) >> 4][s1 >> 4] == BAN_ICE || ban_map[(s2 + 15) >> 4][s1 >> 4] == BAN_SPRING || ban_map[(s2 + 15) >> 4][(s1 + 15) >> 4] == BAN_SOLID || ban_map[(s2 + 15) >> 4][(s1 + 15) >> 4] == BAN_ICE || ban_map[(s2 + 15) >> 4][(s1 + 15) >> 4] == BAN_SPRING) {
					player[c1].in_water = 0;
					player[c1].y = (((s2 + 16) & 0xfff0) - 16) << 16;
					player[c1].y_add = 0;
					if (player[c1].anim != 0 && player[c1].anim != 1) {
						player[c1].anim = 0;
						player[c1].frame = 0;
						player[c1].frame_tick = 0;
						player[c1].image = player_anims[player[c1].anim].frame[player[c1].frame].image + player[c1].direction * 9;
					}
				} else {
					if (player[c1].in_water == 0) {
						if (bunnies_in_space == 0)
							player[c1].y_add += 12288;
						else
							player[c1].y_add += 6144;
						if (player[c1].y_add > 327680L)
							player[c1].y_add = 327680L;
					} else {
						player[c1].y = (player[c1].y & 0xffff0000) + 0x10000;
						player[c1].y_add = 0;
					}
					player[c1].in_water = 0;
				}
				if (player[c1].y_add > 36864 && player[c1].anim != 3 && player[c1].in_water == 0) {
					player[c1].anim = 3;
					player[c1].frame = 0;
					player[c1].frame_tick = 0;
					player[c1].image = player_anims[player[c1].anim].frame[player[c1].frame].image + player[c1].direction * 9;
				}

			}

			player[c1].frame_tick++;
			if (player[c1].frame_tick >= player_anims[player[c1].anim].frame[player[c1].frame].ticks) {
				player[c1].frame++;
				if (player[c1].frame >= player_anims[player[c1].anim].num_frames) {
					if (player[c1].anim != 6)
						player[c1].frame = player_anims[player[c1].anim].restart_frame;
					else
						position_player(c1);
				}
				player[c1].frame_tick = 0;
			}
			player[c1].image = player_anims[player[c1].anim].frame[player[c1].frame].image + player[c1].direction * 9;

		}

	}

}


void position_player(int player_num)
{
	int c1;
	int s1, s2;

	while (1) {
		while (1) {
			s1 = rnd(22);
			s2 = rnd(16);
			if (ban_map[s2][s1] == BAN_VOID && (ban_map[s2 + 1][s1] == BAN_SOLID || ban_map[s2 + 1][s1] == BAN_ICE))
				break;
		}
		for (c1 = 0; c1 < 4; c1++) {
			if (c1 != player_num && player[c1].enabled == 1) {
				if (abs((s1 << 4) - (player[c1].x >> 16)) < 32 && abs((s2 << 4) - (player[c1].y >> 16)) < 32)
					break;
			}
		}
		if (c1 == 4) {
			player[player_num].dead_flag = 0;
			player[player_num].x = (long) s1 << 20;
			player[player_num].y = (long) s2 << 20;
			player[player_num].x_add = player[player_num].y_add = 0;
			player[player_num].direction = 0;
			player[player_num].jump_ready = 1;
			player[player_num].in_water = 0;
			player[player_num].anim = 0;
			player[player_num].frame = 0;
			player[player_num].frame_tick = 0;
			player[player_num].image = player_anims[player[player_num].anim].frame[player[player_num].frame].image;
			break;
		}
	}

}


void add_object(int type, int x, int y, int x_add, int y_add, int anim, int frame)
{
	int c1;

	for (c1 = 0; c1 < NUM_OBJECTS; c1++) {
		if (objects[c1].used == 0) {
			objects[c1].used = 1;
			objects[c1].type = type;
			objects[c1].x = (long) x << 16;
			objects[c1].y = (long) y << 16;
			objects[c1].x_add = x_add;
			objects[c1].y_add = y_add;
			objects[c1].x_acc = 0;
			objects[c1].y_acc = 0;
			objects[c1].anim = anim;
			objects[c1].frame = frame;
			objects[c1].ticks = object_anims[anim].frame[frame].ticks;
			objects[c1].image = object_anims[anim].frame[frame].image;
			break;
		}
	}

}


void update_objects(void)
{
	int c1;
	int s1 = 0;

	for (c1 = 0; c1 < NUM_OBJECTS; c1++) {
		if (objects[c1].used == 1) {
			switch (objects[c1].type) {
			case OBJ_SPRING:
				objects[c1].ticks--;
				if (objects[c1].ticks <= 0) {
					objects[c1].frame++;
					if (objects[c1].frame >= object_anims[objects[c1].anim].num_frames) {
						objects[c1].frame--;
						objects[c1].ticks = object_anims[objects[c1].anim].frame[objects[c1].frame].ticks;
					} else {
						objects[c1].ticks = object_anims[objects[c1].anim].frame[objects[c1].frame].ticks;
						objects[c1].image = object_anims[objects[c1].anim].frame[objects[c1].frame].image;
					}
				}
				if (objects[c1].used == 1)
					add_pob(main_info.draw_page, objects[c1].x >> 16, objects[c1].y >> 16, objects[c1].image, object_gobs);
				break;
			case OBJ_SPLASH:
				objects[c1].ticks--;
				if (objects[c1].ticks <= 0) {
					objects[c1].frame++;
					if (objects[c1].frame >= object_anims[objects[c1].anim].num_frames)
						objects[c1].used = 0;
					else {
						objects[c1].ticks = object_anims[objects[c1].anim].frame[objects[c1].frame].ticks;
						objects[c1].image = object_anims[objects[c1].anim].frame[objects[c1].frame].image;
					}
				}
				if (objects[c1].used == 1)
					add_pob(main_info.draw_page, objects[c1].x >> 16, objects[c1].y >> 16, objects[c1].image, object_gobs);
				break;
			case OBJ_SMOKE:
				objects[c1].x += objects[c1].x_add;
				objects[c1].y += objects[c1].y_add;
				objects[c1].ticks--;
				if (objects[c1].ticks <= 0) {
					objects[c1].frame++;
					if (objects[c1].frame >= object_anims[objects[c1].anim].num_frames)
						objects[c1].used = 0;
					else {
						objects[c1].ticks = object_anims[objects[c1].anim].frame[objects[c1].frame].ticks;
						objects[c1].image = object_anims[objects[c1].anim].frame[objects[c1].frame].image;
					}
				}
				if (objects[c1].used == 1)
					add_pob(main_info.draw_page, objects[c1].x >> 16, objects[c1].y >> 16, objects[c1].image, object_gobs);
				break;
			case OBJ_YEL_BUTFLY:
			case OBJ_PINK_BUTFLY:
				objects[c1].x_acc += rnd(128) - 64;
				if (objects[c1].x_acc < -1024)
					objects[c1].x_acc = -1024;
				if (objects[c1].x_acc > 1024)
					objects[c1].x_acc = 1024;
				objects[c1].x_add += objects[c1].x_acc;
				if (objects[c1].x_add < -32768)
					objects[c1].x_add = -32768;
				if (objects[c1].x_add > 32768)
					objects[c1].x_add = 32768;
				objects[c1].x += objects[c1].x_add;
				if ((objects[c1].x >> 16) < 16) {
					objects[c1].x = 16 << 16;
					objects[c1].x_add = -objects[c1].x_add >> 2;
					objects[c1].x_acc = 0;
				} else if ((objects[c1].x >> 16) > 350) {
					objects[c1].x = 350 << 16;
					objects[c1].x_add = -objects[c1].x_add >> 2;
					objects[c1].x_acc = 0;
				}
				if (ban_map[objects[c1].y >> 20][objects[c1].x >> 20] != 0) {
					if (objects[c1].x_add < 0) {
						objects[c1].x = (((objects[c1].x >> 16) + 16) & 0xfff0) << 16;
					} else {
						objects[c1].x = ((((objects[c1].x >> 16) - 16) & 0xfff0) + 15) << 16;
					}
					objects[c1].x_add = -objects[c1].x_add >> 2;
					objects[c1].x_acc = 0;
				}
				objects[c1].y_acc += rnd(64) - 32;
				if (objects[c1].y_acc < -1024)
					objects[c1].y_acc = -1024;
				if (objects[c1].y_acc > 1024)
					objects[c1].y_acc = 1024;
				objects[c1].y_add += objects[c1].y_acc;
				if (objects[c1].y_add < -32768)
					objects[c1].y_add = -32768;
				if (objects[c1].y_add > 32768)
					objects[c1].y_add = 32768;
				objects[c1].y += objects[c1].y_add;
				if ((objects[c1].y >> 16) < 0) {
					objects[c1].y = 0;
					objects[c1].y_add = -objects[c1].y_add >> 2;
					objects[c1].y_acc = 0;
				} else if ((objects[c1].y >> 16) > 255) {
					objects[c1].y = 255 << 16;
					objects[c1].y_add = -objects[c1].y_add >> 2;
					objects[c1].y_acc = 0;
				}
				if (ban_map[objects[c1].y >> 20][objects[c1].x >> 20] != 0) {
					if (objects[c1].y_add < 0) {
						objects[c1].y = (((objects[c1].y >> 16) + 16) & 0xfff0) << 16;
					} else {
						objects[c1].y = ((((objects[c1].y >> 16) - 16) & 0xfff0) + 15) << 16;
					}
					objects[c1].y_add = -objects[c1].y_add >> 2;
					objects[c1].y_acc = 0;
				}
				if (objects[c1].type == OBJ_YEL_BUTFLY) {
					if (objects[c1].x_add < 0 && objects[c1].anim != OBJ_ANIM_YEL_BUTFLY_LEFT) {
						objects[c1].anim = OBJ_ANIM_YEL_BUTFLY_LEFT;
						objects[c1].frame = 0;
						objects[c1].ticks = object_anims[objects[c1].anim].frame[objects[c1].frame].ticks;
						objects[c1].image = object_anims[objects[c1].anim].frame[objects[c1].frame].image;
					} else if (objects[c1].x_add > 0 && objects[c1].anim != OBJ_ANIM_YEL_BUTFLY_RIGHT) {
						objects[c1].anim = OBJ_ANIM_YEL_BUTFLY_RIGHT;
						objects[c1].frame = 0;
						objects[c1].ticks = object_anims[objects[c1].anim].frame[objects[c1].frame].ticks;
						objects[c1].image = object_anims[objects[c1].anim].frame[objects[c1].frame].image;
					}
				} else {
					if (objects[c1].x_add < 0 && objects[c1].anim != OBJ_ANIM_PINK_BUTFLY_LEFT) {
						objects[c1].anim = OBJ_ANIM_PINK_BUTFLY_LEFT;
						objects[c1].frame = 0;
						objects[c1].ticks = object_anims[objects[c1].anim].frame[objects[c1].frame].ticks;
						objects[c1].image = object_anims[objects[c1].anim].frame[objects[c1].frame].image;
					} else if (objects[c1].x_add > 0 && objects[c1].anim != OBJ_ANIM_PINK_BUTFLY_RIGHT) {
						objects[c1].anim = OBJ_ANIM_PINK_BUTFLY_RIGHT;
						objects[c1].frame = 0;
						objects[c1].ticks = object_anims[objects[c1].anim].frame[objects[c1].frame].ticks;
						objects[c1].image = object_anims[objects[c1].anim].frame[objects[c1].frame].image;
					}
				}
				objects[c1].ticks--;
				if (objects[c1].ticks <= 0) {
					objects[c1].frame++;
					if (objects[c1].frame >= object_anims[objects[c1].anim].num_frames)
						objects[c1].frame = object_anims[objects[c1].anim].restart_frame;
					else {
						objects[c1].ticks = object_anims[objects[c1].anim].frame[objects[c1].frame].ticks;
						objects[c1].image = object_anims[objects[c1].anim].frame[objects[c1].frame].image;
					}
				}
				if (objects[c1].used == 1)
					add_pob(main_info.draw_page, objects[c1].x >> 16, objects[c1].y >> 16, objects[c1].image, object_gobs);
				break;
			case OBJ_FUR:
				if (rnd(100) < 30)
					add_object(OBJ_FLESH_TRACE, objects[c1].x >> 16, objects[c1].y >> 16, 0, 0, OBJ_ANIM_FLESH_TRACE, 0);
				if (ban_map[objects[c1].y >> 20][objects[c1].x >> 20] == 0) {
					objects[c1].y_add += 3072;
					if (objects[c1].y_add > 196608L)
						objects[c1].y_add = 196608L;
				} else if (ban_map[objects[c1].y >> 20][objects[c1].x >> 20] == 2) {
					if (objects[c1].x_add < 0) {
						if (objects[c1].x_add < -65536L)
							objects[c1].x_add = -65536L;
						objects[c1].x_add += 1024;
						if (objects[c1].x_add > 0)
							objects[c1].x_add = 0;
					} else {
						if (objects[c1].x_add > 65536L)
							objects[c1].x_add = 65536L;
						objects[c1].x_add -= 1024;
						if (objects[c1].x_add < 0)
							objects[c1].x_add = 0;
					}
					objects[c1].y_add += 1024;
					if (objects[c1].y_add < -65536L)
						objects[c1].y_add = -65536L;
					if (objects[c1].y_add > 65536L)
						objects[c1].y_add = 65536L;
				}
				objects[c1].x += objects[c1].x_add;
				if ((objects[c1].y >> 16) > 0 && (ban_map[objects[c1].y >> 20][objects[c1].x >> 20] == 1 || ban_map[objects[c1].y >> 20][objects[c1].x >> 20] == 3)) {
					if (objects[c1].x_add < 0) {
						objects[c1].x = (((objects[c1].x >> 16) + 16) & 0xfff0) << 16;
						objects[c1].x_add = -objects[c1].x_add >> 2;
					} else {
						objects[c1].x = ((((objects[c1].x >> 16) - 16) & 0xfff0) + 15) << 16;
						objects[c1].x_add = -objects[c1].x_add >> 2;
					}
				}
				objects[c1].y += objects[c1].y_add;
				if ((objects[c1].x >> 16) < -5 || (objects[c1].x >> 16) > 405 || (objects[c1].y >> 16) > 260)
					objects[c1].used = 0;
				if ((objects[c1].y >> 16) > 0 && (ban_map[objects[c1].y >> 20][objects[c1].x >> 20] != 0)) {
					if (objects[c1].y_add < 0) {
						if (ban_map[objects[c1].y >> 20][objects[c1].x >> 20] != 2) {
							objects[c1].y = (((objects[c1].y >> 16) + 16) & 0xfff0) << 16;
							objects[c1].x_add >>= 2;
							objects[c1].y_add = -objects[c1].y_add >> 2;
						}
					} else {
						if (ban_map[objects[c1].y >> 20][objects[c1].x >> 20] == 1) {
							if (objects[c1].y_add > 131072L) {
								objects[c1].y = ((((objects[c1].y >> 16) - 16) & 0xfff0) + 15) << 16;
								objects[c1].x_add >>= 2;
								objects[c1].y_add = -objects[c1].y_add >> 2;
							} else
								objects[c1].used = 0;
						} else if (ban_map[objects[c1].y >> 20][objects[c1].x >> 20] == 3) {
							objects[c1].y = ((((objects[c1].y >> 16) - 16) & 0xfff0) + 15) << 16;
							if (objects[c1].y_add > 131072L)
								objects[c1].y_add = -objects[c1].y_add >> 2;
							else
								objects[c1].y_add = 0;
						}
					}
				}
				if (objects[c1].x_add < 0 && objects[c1].x_add > -16384)
					objects[c1].x_add = -16384;
				if (objects[c1].x_add > 0 && objects[c1].x_add < 16384)
					objects[c1].x_add = 16384;
				if (objects[c1].used == 1) {
					s1 = (int)(atan2(objects[c1].y_add, objects[c1].x_add) * 4 / M_PI);
					if (s1 < 0)
						s1 += 8;
					if (s1 < 0)
						s1 = 0;
					if (s1 > 7)
						s1 = 7;
					add_pob(main_info.draw_page, objects[c1].x >> 16, objects[c1].y >> 16, objects[c1].frame + s1, object_gobs);
				}
				break;
			case OBJ_FLESH:
				if (rnd(100) < 30) {
					if (objects[c1].frame == 76)
						add_object(OBJ_FLESH_TRACE, objects[c1].x >> 16, objects[c1].y >> 16, 0, 0, OBJ_ANIM_FLESH_TRACE, 1);
					else if (objects[c1].frame == 77)
						add_object(OBJ_FLESH_TRACE, objects[c1].x >> 16, objects[c1].y >> 16, 0, 0, OBJ_ANIM_FLESH_TRACE, 2);
					else if (objects[c1].frame == 78)
						add_object(OBJ_FLESH_TRACE, objects[c1].x >> 16, objects[c1].y >> 16, 0, 0, OBJ_ANIM_FLESH_TRACE, 3);
				}
				if (ban_map[objects[c1].y >> 20][objects[c1].x >> 20] == 0) {
					objects[c1].y_add += 3072;
					if (objects[c1].y_add > 196608L)
						objects[c1].y_add = 196608L;
				} else if (ban_map[objects[c1].y >> 20][objects[c1].x >> 20] == 2) {
					if (objects[c1].x_add < 0) {
						if (objects[c1].x_add < -65536L)
							objects[c1].x_add = -65536L;
						objects[c1].x_add += 1024;
						if (objects[c1].x_add > 0)
							objects[c1].x_add = 0;
					} else {
						if (objects[c1].x_add > 65536L)
							objects[c1].x_add = 65536L;
						objects[c1].x_add -= 1024;
						if (objects[c1].x_add < 0)
							objects[c1].x_add = 0;
					}
					objects[c1].y_add += 1024;
					if (objects[c1].y_add < -65536L)
						objects[c1].y_add = -65536L;
					if (objects[c1].y_add > 65536L)
						objects[c1].y_add = 65536L;
				}
				objects[c1].x += objects[c1].x_add;
				if ((objects[c1].y >> 16) > 0 && (ban_map[objects[c1].y >> 20][objects[c1].x >> 20] == 1 || ban_map[objects[c1].y >> 20][objects[c1].x >> 20] == 3)) {
					if (objects[c1].x_add < 0) {
						objects[c1].x = (((objects[c1].x >> 16) + 16) & 0xfff0) << 16;
						objects[c1].x_add = -objects[c1].x_add >> 2;
					} else {
						objects[c1].x = ((((objects[c1].x >> 16) - 16) & 0xfff0) + 15) << 16;
						objects[c1].x_add = -objects[c1].x_add >> 2;
					}
				}
				objects[c1].y += objects[c1].y_add;
				if ((objects[c1].x >> 16) < -5 || (objects[c1].x >> 16) > 405 || (objects[c1].y >> 16) > 260)
					objects[c1].used = 0;
				if ((objects[c1].y >> 16) > 0 && (ban_map[objects[c1].y >> 20][objects[c1].x >> 20] != 0)) {
					if (objects[c1].y_add < 0) {
						if (ban_map[objects[c1].y >> 20][objects[c1].x >> 20] != 2) {
							objects[c1].y = (((objects[c1].y >> 16) + 16) & 0xfff0) << 16;
							objects[c1].x_add >>= 2;
							objects[c1].y_add = -objects[c1].y_add >> 2;
						}
					} else {
						if (ban_map[objects[c1].y >> 20][objects[c1].x >> 20] == 1) {
							if (objects[c1].y_add > 131072L) {
								objects[c1].y = ((((objects[c1].y >> 16) - 16) & 0xfff0) + 15) << 16;
								objects[c1].x_add >>= 2;
								objects[c1].y_add = -objects[c1].y_add >> 2;
							} else {
								if (rnd(100) < 10) {
									s1 = rnd(4) - 2;
									add_leftovers(0, objects[c1].x >> 16, (objects[c1].y >> 16) + s1, objects[c1].frame, object_gobs);
									add_leftovers(1, objects[c1].x >> 16, (objects[c1].y >> 16) + s1, objects[c1].frame, object_gobs);
								}
								objects[c1].used = 0;
							}
						} else if (ban_map[objects[c1].y >> 20][objects[c1].x >> 20] == 3) {
							objects[c1].y = ((((objects[c1].y >> 16) - 16) & 0xfff0) + 15) << 16;
							if (objects[c1].y_add > 131072L)
								objects[c1].y_add = -objects[c1].y_add >> 2;
							else
								objects[c1].y_add = 0;
						}
					}
				}
				if (objects[c1].x_add < 0 && objects[c1].x_add > -16384)
					objects[c1].x_add = -16384;
				if (objects[c1].x_add > 0 && objects[c1].x_add < 16384)
					objects[c1].x_add = 16384;
				if (objects[c1].used == 1)
					add_pob(main_info.draw_page, objects[c1].x >> 16, objects[c1].y >> 16, objects[c1].frame, object_gobs);
				break;
			case OBJ_FLESH_TRACE:
				objects[c1].ticks--;
				if (objects[c1].ticks <= 0) {
					objects[c1].frame++;
					if (objects[c1].frame >= object_anims[objects[c1].anim].num_frames)
						objects[c1].used = 0;
					else {
						objects[c1].ticks = object_anims[objects[c1].anim].frame[objects[c1].frame].ticks;
						objects[c1].image = object_anims[objects[c1].anim].frame[objects[c1].frame].image;
					}
				}
				if (objects[c1].used == 1)
					add_pob(main_info.draw_page, objects[c1].x >> 16, objects[c1].y >> 16, objects[c1].image, object_gobs);
				break;
			}
		}
	}

}


int add_pob(int page, int x, int y, int image, char *pob_data)
{

	if (main_info.page_info[page].num_pobs >= NUM_POBS)
		return 1;

	main_info.page_info[page].pobs[main_info.page_info[page].num_pobs].x = x;
	main_info.page_info[page].pobs[main_info.page_info[page].num_pobs].y = y;
	main_info.page_info[page].pobs[main_info.page_info[page].num_pobs].image = image;
	main_info.page_info[page].pobs[main_info.page_info[page].num_pobs].pob_data = pob_data;
	main_info.page_info[page].num_pobs++;

	return 0;

}


void draw_flies(int page)
{
	char *ptr1;
	int c1,c2;

#ifdef DOS
	ptr1 = (char *) (0xa0000 + ((long) main_info.draw_page << 15) - __djgpp_base_address);
	for (c1 = 0; c1 < 4; c1++) {
		outportw(0x3ce, (c1 << 8) + 0x04);
		outportw(0x3c4, ((1 << c1) << 8) + 0x02);
		for (c2 = 0; c2 < NUM_FLIES; c2++) {
			if ((flies[c2].x & 3) == c1) {
				flies[c2].back[main_info.draw_page] = *(char *) (ptr1 + flies[c2].y * 100 + (flies[c2].x >> 2));
				flies[c2].back_defined[main_info.draw_page] = 1;
				if (mask_pic[flies[c2].y * 400 + flies[c2].x] == 0)
					*(char *) (ptr1 + flies[c2].y * 100 + (flies[c2].x >> 2)) = 0;
			}
		}
	}
#else
	ptr1 = (char *) get_vgaptr(main_info.draw_page, 0, 0);
	for (c2 = 0; c2 < NUM_FLIES; c2++) {
		flies[c2].back[main_info.draw_page] = *(char *) (ptr1 + flies[c2].y * JNB_WIDTH + (flies[c2].x));
		flies[c2].back_defined[main_info.draw_page] = 1;
		if (mask_pic[(flies[c2].y * JNB_WIDTH) + flies[c2].x] == 0)
			*(char *) (ptr1 + flies[c2].y * JNB_WIDTH + (flies[c2].x)) = 0;
	}
#endif
}

void draw_pobs(int page)
{
	int c1;
	int back_buf_ofs;

	back_buf_ofs = 0;

	for (c1 = main_info.page_info[page].num_pobs - 1; c1 >= 0; c1--) {
		main_info.page_info[page].pobs[c1].back_buf_ofs = back_buf_ofs;
		get_block(page, main_info.page_info[page].pobs[c1].x - pob_hs_x(main_info.page_info[page].pobs[c1].image, main_info.page_info[page].pobs[c1].pob_data), main_info.page_info[page].pobs[c1].y - pob_hs_y(main_info.page_info[page].pobs[c1].image, main_info.page_info[page].pobs[c1].pob_data), pob_width(main_info.page_info[page].pobs[c1].image, main_info.page_info[page].pobs[c1].pob_data), pob_height(main_info.page_info[page].pobs[c1].image, main_info.page_info[page].pobs[c1].pob_data), (char *) (main_info.pob_backbuf[page] + back_buf_ofs));
		back_buf_ofs += pob_width(main_info.page_info[page].pobs[c1].image, main_info.page_info[page].pobs[c1].pob_data) * pob_height(main_info.page_info[page].pobs[c1].image, main_info.page_info[page].pobs[c1].pob_data);
		put_pob(page, main_info.page_info[page].pobs[c1].x, main_info.page_info[page].pobs[c1].y, main_info.page_info[page].pobs[c1].image, main_info.page_info[page].pobs[c1].pob_data, 1, mask_pic);
	}

}


void redraw_flies_background(int page)
{
	char *ptr1;
	int c1,c2;
#ifdef DOS
	ptr1 = (char *) (0xa0000 + ((long) page << 15) - __djgpp_base_address);
	for (c1 = 0; c1 < 4; c1++) {
		outportw(0x3c4, ((1 << c1) << 8) + 0x02);
		for (c2 = NUM_FLIES - 1; c2 >= 0; c2--) {
			if ((flies[c2].old_x & 3) == c1 && flies[c2].back_defined[page] == 1)
				*(char *) (ptr1 + flies[c2].old_y * 100 + (flies[c2].old_x >> 2)) = flies[c2].back[page];
		}
	}
#else
	ptr1 = (char *) get_vgaptr(page, 0, 0);
	for (c2 = NUM_FLIES - 1; c2 >= 0; c2--) {
		if (flies[c2].back_defined[page] == 1)
			*(char *) (ptr1 + flies[c2].old_y * JNB_WIDTH + (flies[c2].old_x)) = flies[c2].back[page];
	}
#endif
}


void redraw_pob_backgrounds(int page)
{
	int c1;

	for (c1 = 0; c1 < main_info.page_info[page].num_pobs; c1++)
		put_block(page, main_info.page_info[page].pobs[c1].x - pob_hs_x(main_info.page_info[page].pobs[c1].image, main_info.page_info[page].pobs[c1].pob_data), main_info.page_info[page].pobs[c1].y - pob_hs_y(main_info.page_info[page].pobs[c1].image, main_info.page_info[page].pobs[c1].pob_data), pob_width(main_info.page_info[page].pobs[c1].image, main_info.page_info[page].pobs[c1].pob_data), pob_height(main_info.page_info[page].pobs[c1].image, main_info.page_info[page].pobs[c1].pob_data), (char *) (main_info.pob_backbuf[page] + main_info.page_info[page].pobs[c1].back_buf_ofs));

}


int add_leftovers(int page, int x, int y, int image, char *pob_data)
{

	if (leftovers.page[page].num_pobs >= NUM_LEFTOVERS)
		return 1;

	leftovers.page[page].pobs[leftovers.page[page].num_pobs].x = x;
	leftovers.page[page].pobs[leftovers.page[page].num_pobs].y = y;
	leftovers.page[page].pobs[leftovers.page[page].num_pobs].image = image;
	leftovers.page[page].pobs[leftovers.page[page].num_pobs].pob_data = pob_data;
	leftovers.page[page].num_pobs++;

	return 0;

}


void draw_leftovers(int page)
{
	int c1;

	for (c1 = leftovers.page[page].num_pobs - 1; c1 >= 0; c1--)
		put_pob(page, leftovers.page[page].pobs[c1].x, leftovers.page[page].pobs[c1].y, leftovers.page[page].pobs[c1].image, leftovers.page[page].pobs[c1].pob_data, 1, mask_pic);

	leftovers.page[page].num_pobs = 0;

}


int init_level(int level, char *pal)
{
	FILE *handle;
	int c1, c2;
	int s1, s2;

	if ((handle = dat_open("level.pcx", datfile_name, "rb")) == 0) {
		strcpy(main_info.error_str, "Error loading 'level.pcx', aborting...\n");
		return 1;
	}
	if (read_pcx(handle, background_pic, 102400L, pal) != 0) {
		strcpy(main_info.error_str, "Error loading 'level.pcx', aborting...\n");
		return 1;
	}
	fclose(handle);
	if ((handle = dat_open("mask.pcx", datfile_name, "rb")) == 0) {
		strcpy(main_info.error_str, "Error loading 'mask.pcx', aborting...\n");
		return 1;
	}
	if (read_pcx(handle, mask_pic, 102400L, 0) != 0) {
		strcpy(main_info.error_str, "Error loading 'mask.pcx', aborting...\n");
		return 1;
	}
	fclose(handle);

	for (c1 = 0; c1 < 4; c1++) {
		if (player[c1].enabled == 1) {
			player[c1].bumps = 0;
			player[c1].bumped[0] = 0;
			player[c1].bumped[1] = 0;
			player[c1].bumped[2] = 0;
			player[c1].bumped[3] = 0;
			position_player(c1);
		}
	}

	for (c1 = 0; c1 < NUM_OBJECTS; c1++)
		objects[c1].used = 0;

	for (c1 = 0; c1 < 16; c1++) {
		for (c2 = 0; c2 < 22; c2++) {
			if (ban_map[c1][c2] == BAN_SPRING)
				add_object(OBJ_SPRING, c2 << 4, c1 << 4, 0, 0, OBJ_ANIM_SPRING, 5);
		}
	}

	while (1) {
		s1 = rnd(22);
		s2 = rnd(16);
		if (ban_map[s2][s1] == BAN_VOID) {
			add_object(OBJ_YEL_BUTFLY, (s1 << 4) + 8, (s2 << 4) + 8, (rnd(65535) - 32768) * 2, (rnd(65535) - 32768) * 2, 0, 0);
			break;
		}
	}
	while (1) {
		s1 = rnd(22);
		s2 = rnd(16);
		if (ban_map[s2][s1] == BAN_VOID) {
			add_object(OBJ_YEL_BUTFLY, (s1 << 4) + 8, (s2 << 4) + 8, (rnd(65535) - 32768) * 2, (rnd(65535) - 32768) * 2, 0, 0);
			break;
		}
	}
	while (1) {
		s1 = rnd(22);
		s2 = rnd(16);
		if (ban_map[s2][s1] == BAN_VOID) {
			add_object(OBJ_PINK_BUTFLY, (s1 << 4) + 8, (s2 << 4) + 8, (rnd(65535) - 32768) * 2, (rnd(65535) - 32768) * 2, 0, 0);
			break;
		}
	}
	while (1) {
		s1 = rnd(22);
		s2 = rnd(16);
		if (ban_map[s2][s1] == BAN_VOID) {
			add_object(OBJ_PINK_BUTFLY, (s1 << 4) + 8, (s2 << 4) + 8, (rnd(65535) - 32768) * 2, (rnd(65535) - 32768) * 2, 0, 0);
			break;
		}
	}

	return 0;

}


void deinit_level(void)
{
	dj_set_nosound(1);
	dj_stop_mod();
}


int init_program(int argc, char *argv[], char *pal)
{
	FILE *handle = (FILE *) NULL;
	int c1 = 0, c2 = 0;
	char load_flag = 0;
	char force2, force3;
	sfx_data fly;
	int player_anim_data[] = {
		1, 0, 0, 0x7fff, 0, 0, 0, 0, 0, 0,
		4, 0, 0, 4, 1, 4, 2, 4, 3, 4,
		1, 0, 4, 0x7fff, 0, 0, 0, 0, 0, 0,
		4, 2, 5, 8, 6, 10, 7, 3, 6, 3,
		1, 0, 6, 0x7fff, 0, 0, 0, 0, 0, 0,
		2, 1, 5, 8, 4, 0x7fff, 0, 0, 0, 0,
		1, 0, 8, 5, 0, 0, 0, 0, 0, 0
	};

#ifdef DOS
	if (__djgpp_nearptr_enable() == 0)
		return 1;
#endif

	srand(time(NULL));

	if (hook_keyb_handler() != 0)
		return 1;

	memset(&main_info, 0, sizeof(main_info));

	strcpy(datfile_name, "data/jumpbump.dat");

	force2 = force3 = 0;

	if (argc > 1) {
		for (c1 = 1; c1 < argc; c1++) {
			if (stricmp(argv[c1], "-nosound") == 0)
				main_info.no_sound = 1;
			else if (stricmp(argv[c1], "-nogore") == 0)
				main_info.no_gore = 1;
			else if (stricmp(argv[c1], "-nojoy") == 0)
				main_info.joy_enabled = 0;
			else if (stricmp(argv[c1], "-fireworks") == 0)
				main_info.fireworks = 1;
#ifdef USE_SDL
			else if (stricmp(argv[c1], "-fullscreen") == 0)
				fs_toggle();
#endif
			else if (stricmp(argv[c1], "-dat") == 0) {
				if (c1 < (argc - 1)) {
					if ((handle = fopen(argv[c1 + 1], "rb")) != NULL) {
						fclose(handle);
						strcpy(datfile_name, argv[c1 + 1]);
					}
				}
			} else if (stricmp(argv[c1], "-mouse") == 0) {
				if (c1 < (argc - 1)) {
					if (stricmp(argv[c1 + 1], "2") == 0)
						force2 = 1;
					if (stricmp(argv[c1 + 1], "3") == 0)
						force3 = 1;
				}
			}
		}
	}

	for (c1 = 0; c1 < 7; c1++) {
		player_anims[c1].num_frames = player_anim_data[c1 * 10];
		player_anims[c1].restart_frame = player_anim_data[c1 * 10 + 1];
		for (c2 = 0; c2 < 4; c2++) {
			player_anims[c1].frame[c2].image = player_anim_data[c1 * 10 + c2 * 2 + 2];
			player_anims[c1].frame[c2].ticks = player_anim_data[c1 * 10 + c2 * 2 + 3];
		}
	}

	if ((background_pic = malloc(102400)) == NULL)
		return 1;
	if ((mask_pic = malloc(102400)) == NULL)
		return 1;
	memset(mask_pic, 0, 102400);

	main_info.pob_backbuf[0] = malloc(65535);
	main_info.pob_backbuf[1] = malloc(65535);
	if (main_info.pob_backbuf[0] == 0 || main_info.pob_backbuf[1] == 0)
		return 1;

	if ((handle = dat_open("rabbit.gob", datfile_name, "rb")) == 0) {
		strcpy(main_info.error_str, "Error loading 'rabbit.gob', aborting...\n");
		return 1;
	}
	if ((rabbit_gobs = malloc(dat_filelen("rabbit.gob", datfile_name))) == 0) {
		strcpy(main_info.error_str, "Not enough memory, aborting...\n");
		fclose(handle);
		return 1;
	}
	fread(rabbit_gobs, 1, dat_filelen("rabbit.gob", datfile_name), handle);
	fclose(handle);

	if ((handle = dat_open("objects.gob", datfile_name, "rb")) == 0) {
		strcpy(main_info.error_str, "Error loading 'objects.gob', aborting...\n");
		return 1;
	}
	if ((object_gobs = malloc(dat_filelen("objects.gob", datfile_name))) == 0) {
		strcpy(main_info.error_str, "Not enough memory, aborting...\n");
		fclose(handle);
		return 1;
	}
	fread(object_gobs, 1, dat_filelen("objects.gob", datfile_name), handle);
	fclose(handle);

	if ((handle = dat_open("font.gob", datfile_name, "rb")) == 0) {
		strcpy(main_info.error_str, "Error loading 'font.gob', aborting...\n");
		return 1;
	}
	if ((font_gobs = malloc(dat_filelen("font.gob", datfile_name))) == 0) {
		strcpy(main_info.error_str, "Not enough memory, aborting...\n");
		fclose(handle);
		return 1;
	}
	fread(font_gobs, 1, dat_filelen("font.gob", datfile_name), handle);
	fclose(handle);

	if ((handle = dat_open("numbers.gob", datfile_name, "rb")) == 0) {
		strcpy(main_info.error_str, "Error loading 'numbers.gob', aborting...\n");
		return 1;
	}
	if ((number_gobs = malloc(dat_filelen("numbers.gob", datfile_name))) == 0) {
		strcpy(main_info.error_str, "Not enough memory, aborting...\n");
		fclose(handle);
		return 1;
	}
	fread(number_gobs, 1, dat_filelen("numbers.gob", datfile_name), handle);
	fclose(handle);

	if (read_level() != 0) {
		strcpy(main_info.error_str, "Error loading 'levelmap.txt', aborting...\n");
		fclose(handle);
		return 1;
	}

	dj_init();
	if (main_info.no_sound == 0) {
		dj_autodetect_sd();
		dj_set_mixing_freq(20000);
		dj_set_stereo(0);
		dj_set_auto_mix(0);
		dj_set_dma_time(8);
		dj_set_num_sfx_channels(5);
		dj_set_sfx_volume(64);
		dj_set_nosound(1);
		dj_start();

		if ((handle = dat_open("jump.mod", datfile_name, "rb")) == 0) {
			strcpy(main_info.error_str, "Error loading 'jump.mod', aborting...\n");
			return 1;
		}
		if (dj_load_mod(handle, 0, MOD_MENU) != 0) {
			strcpy(main_info.error_str, "Error loading 'jump.mod', aborting...\n");
			return 1;
		}
		fclose(handle);

		if ((handle = dat_open("bump.mod", datfile_name, "rb")) == 0) {
			strcpy(main_info.error_str, "Error loading 'bump.mod', aborting...\n");
			return 1;
		}
		if (dj_load_mod(handle, 0, MOD_GAME) != 0) {
			strcpy(main_info.error_str, "Error loading 'bump.mod', aborting...\n");
			return 1;
		}
		fclose(handle);

		if ((handle = dat_open("scores.mod", datfile_name, "rb")) == 0) {
			strcpy(main_info.error_str, "Error loading 'scores.mod', aborting...\n");
			return 1;
		}
		if (dj_load_mod(handle, 0, MOD_SCORES) != 0) {
			strcpy(main_info.error_str, "Error loading 'scores.mod', aborting...\n");
			return 1;
		}
		fclose(handle);

		if ((handle = dat_open("jump.smp", datfile_name, "rb")) == 0) {
			strcpy(main_info.error_str, "Error loading 'jump.smp', aborting...\n");
			return 1;
		}
		if (dj_load_sfx(handle, 0, dat_filelen("jump.smp", datfile_name), DJ_SFX_TYPE_SMP, SFX_JUMP) != 0) {
			strcpy(main_info.error_str, "Error loading 'jump.smp', aborting...\n");
			return 1;
		}
		fclose(handle);

		if ((handle = dat_open("death.smp", datfile_name, "rb")) == 0) {
			strcpy(main_info.error_str, "Error loading 'death.smp', aborting...\n");
			return 1;
		}
		if (dj_load_sfx(handle, 0, dat_filelen("death.smp", datfile_name), DJ_SFX_TYPE_SMP, SFX_DEATH) != 0) {
			strcpy(main_info.error_str, "Error loading 'death.smp', aborting...\n");
			return 1;
		}
		fclose(handle);

		if ((handle = dat_open("spring.smp", datfile_name, "rb")) == 0) {
			strcpy(main_info.error_str, "Error loading 'spring.smp', aborting...\n");
			return 1;
		}
		if (dj_load_sfx(handle, 0, dat_filelen("spring.smp", datfile_name), DJ_SFX_TYPE_SMP, SFX_SPRING) != 0) {
			strcpy(main_info.error_str, "Error loading 'spring.smp', aborting...\n");
			return 1;
		}
		fclose(handle);

		if ((handle = dat_open("splash.smp", datfile_name, "rb")) == 0) {
			strcpy(main_info.error_str, "Error loading 'splash.smp', aborting...\n");
			return 1;
		}
		if (dj_load_sfx(handle, 0, dat_filelen("splash.smp", datfile_name), DJ_SFX_TYPE_SMP, SFX_SPLASH) != 0) {
			strcpy(main_info.error_str, "Error loading 'splash.smp', aborting...\n");
			return 1;
		}
		fclose(handle);

		if ((handle = dat_open("fly.smp", datfile_name, "rb")) == 0) {
			strcpy(main_info.error_str, "Error loading 'fly.smp', aborting...\n");
			return 1;
		}
		if (dj_load_sfx(handle, 0, dat_filelen("fly.smp", datfile_name), DJ_SFX_TYPE_SMP, SFX_FLY) != 0) {
			strcpy(main_info.error_str, "Error loading 'fly.smp', aborting...\n");
			return 1;
		}
		fclose(handle);

		dj_get_sfx_settings(SFX_FLY, &fly);
		fly.priority = 10;
		fly.default_freq = SFX_FLY_FREQ;
		fly.loop = 1;
		fly.loop_start = 0;
		fly.loop_length = fly.length;
		dj_set_sfx_settings(SFX_FLY, &fly);
	}

	if ((handle = dat_open("menu.pcx", datfile_name, "rb")) == 0) {
		strcpy(main_info.error_str, "Error loading 'menu.pcx', aborting...\n");
		return 1;
	}
	if (read_pcx(handle, background_pic, 102400L, pal) != 0) {
		strcpy(main_info.error_str, "Error loading 'menu.pcx', aborting...\n");
		return 1;
	}
	fclose(handle);

	setpalette(0, 256, pal);

	init_inputs();

	if (main_info.joy_enabled == 1 && main_info.fireworks == 0) {
		load_flag = 0;
		put_text(0, 200, 40, "JOYSTICK CALIBRATION", 2);
		put_text(0, 200, 100, "Move the joystick to the", 2);
		put_text(0, 200, 115, "UPPER LEFT", 2);
		put_text(0, 200, 130, "and press button A", 2);
		put_text(0, 200, 200, "Or press ESC to use", 2);
		put_text(0, 200, 215, "previous settings", 2);
		if (calib_joy(0) != 0)
			load_flag = 1;
		else {
			clear_page(1, 0);

			main_info.view_page = 1;
			flippage(1);

			wait_vrt(0);

			put_text(1, 200, 40, "JOYSTICK CALIBRATION", 2);
			put_text(1, 200, 100, "Move the joystick to the", 2);
			put_text(1, 200, 115, "LOWER RIGHT", 2);
			put_text(1, 200, 130, "and press button A", 2);
			put_text(1, 200, 200, "Or press ESC to use", 2);
			put_text(1, 200, 215, "previous settings", 2);
			if (calib_joy(1) != 0)
				load_flag = 1;
			else {
				clear_page(0, 0);
				flippage(0);

				wait_vrt(0);

				put_text(0, 200, 40, "JOYSTICK CALIBRATION", 2);
				put_text(0, 200, 100, "Move the joystick to the", 2);
				put_text(0, 200, 115, "CENTER", 2);
				put_text(0, 200, 130, "and press button A", 2);
				put_text(0, 200, 200, "Or press ESC to use", 2);
				put_text(0, 200, 215, "previous settings", 2);
				if (calib_joy(2) != 0)
					load_flag = 1;
				else {
					if (joy.calib_data.x1 == joy.calib_data.x2)
						joy.calib_data.x1 -= 10;
					if (joy.calib_data.x3 == joy.calib_data.x2)
						joy.calib_data.x3 += 10;
					if (joy.calib_data.y1 == joy.calib_data.y2)
						joy.calib_data.y1 -= 10;
					if (joy.calib_data.y3 == joy.calib_data.y2)
						joy.calib_data.y3 += 10;
					write_calib_data();
				}
			}
		}
		if (load_flag == 1) {
			if ((handle = dat_open("calib.dat", datfile_name, "rb")) == 0) {
				strcpy(main_info.error_str, "Error loading 'calib.dat', aborting...\n");
				return 1;
			}
			joy.calib_data.x1 = fgetc(handle) + (fgetc(handle) << 8) + (fgetc(handle) << 16) + (fgetc(handle) << 24);
			joy.calib_data.x2 = fgetc(handle) + (fgetc(handle) << 8) + (fgetc(handle) << 16) + (fgetc(handle) << 24);
			joy.calib_data.x3 = fgetc(handle) + (fgetc(handle) << 8) + (fgetc(handle) << 16) + (fgetc(handle) << 24);
			joy.calib_data.y1 = fgetc(handle) + (fgetc(handle) << 8) + (fgetc(handle) << 16) + (fgetc(handle) << 24);
			joy.calib_data.y2 = fgetc(handle) + (fgetc(handle) << 8) + (fgetc(handle) << 16) + (fgetc(handle) << 24);
			joy.calib_data.y3 = fgetc(handle) + (fgetc(handle) << 8) + (fgetc(handle) << 16) + (fgetc(handle) << 24);
			fclose(handle);
		}
	}

	return 0;

}


void deinit_program(void)
{
#ifdef DOS
	__dpmi_regs regs;
#endif

	dj_stop();
	dj_free_mod(MOD_MENU);
	dj_free_mod(MOD_GAME);
	dj_free_sfx(SFX_DEATH);
	dj_free_sfx(SFX_SPRING);
	dj_free_sfx(SFX_SPLASH);
	dj_deinit();

	if (rabbit_gobs != 0)
		free(rabbit_gobs);
	if (object_gobs != 0)
		free(object_gobs);
	if (number_gobs != 0)
		free(number_gobs);

	if (main_info.pob_backbuf[0] != 0)
		free(main_info.pob_backbuf[0]);
	if (main_info.pob_backbuf[1] != 0)
		free(main_info.pob_backbuf[1]);

	if (background_pic != 0)
		free(background_pic);
	if (mask_pic != 0)
		free(mask_pic);

	remove_keyb_handler();

#ifdef DOS
	regs.x.ax = 0x3;
	__dpmi_int(0x10, &regs);
#endif

	if (main_info.error_str[0] != 0) {
		printf(main_info.error_str);
		exit(1);
	} else
		exit(0);

}


unsigned short rnd(unsigned short max)
{
	return (rand() % max);
}


int read_level(void)
{
	FILE *handle;
	int c1, c2;
	int chr;

	if ((handle = dat_open("levelmap.txt", datfile_name, "rb")) == 0) {
		strcpy(main_info.error_str, "Error loading 'levelmap.txt', aborting...\n");
		return 1;
	}

	for (c1 = 0; c1 < 16; c1++) {
		for (c2 = 0; c2 < 22; c2++) {
			while (1) {
				chr = fgetc(handle);
				if (chr == EOF) {
					fclose(handle);
					return 1;
				}
				if (chr >= '0' && chr <= '4')
					break;
			}
			ban_map[c1][c2] = chr - '0';
		}
	}

	for (c2 = 0; c2 < 22; c2++)
		ban_map[16][c2] = BAN_SOLID;

	fclose(handle);
	return 0;

}


FILE *dat_open(char *file_name, char *dat_name, char *mode)
{
	FILE *handle;
	int num;
	int c1;
	char name[21];
	int ofs;

	handle = fopen(dat_name, mode);
	if (!handle)
		return 0;

	memset(name, 0, sizeof(name));

	num = fgetc(handle) + (fgetc(handle) << 8) + (fgetc(handle) << 16) + (fgetc(handle) << 24);
	
	for (c1 = 0; c1 < num; c1++) {
		if (!fread(name, 1, 12, handle)) {
			fclose(handle);
			return 0;
		}
		if (strnicmp(name, file_name, strlen(file_name)) == 0) {
			ofs = fgetc(handle);
			ofs += (fgetc(handle) << 8);
			ofs += (fgetc(handle) << 16);
			ofs += (fgetc(handle) << 24);
			fseek(handle, ofs, SEEK_SET);
			return handle;
		}
		fseek(handle, 8, SEEK_CUR);
	}

	fclose(handle);

	return 0;
}


int dat_filelen(char *file_name, char *dat_name)
{
	FILE *handle;
	int num;
	int c1;
	char name[21];
	int len;

	handle = fopen(dat_name, "rb");
	if (!handle)
		return 0;

	memset(name, 0, sizeof(name));
	
	num = fgetc(handle) + (fgetc(handle) << 8) + (fgetc(handle) << 16) + (fgetc(handle) << 24);

	for (c1 = 0; c1 < num; c1++) {
		if (!fread(name, 1, 12, handle)) {
			fclose(handle);
			return 0;
		}
		if (strnicmp(name, file_name, strlen(file_name)) == 0) {
			fseek(handle, 4, SEEK_CUR);
			len = fgetc(handle);
			len += (fgetc(handle) << 8);
			len += (fgetc(handle) << 16);
			len += (fgetc(handle) << 24);

			fclose(handle);
			return len;
		}
		fseek(handle, 8, SEEK_CUR);
	}

	fclose(handle);
	return 0;
}

void write_calib_data(void)
{
	FILE *handle;
	int c1;
	int len, num;
	char *mem;
	int ofs;

	if ((handle = fopen(datfile_name, "rb")) == NULL)
		return;
	len = filelength(fileno(handle));
	if ((mem = malloc(len)) == NULL)
		return;
	fread(mem, 1, len, handle);
	fclose(handle);

	ofs = 4;
	num = *(int *) (&mem[0]);
	for (c1 = 0; c1 < num; c1++) {
		if (strnicmp(&mem[ofs], "calib.dat", strlen("calib.dat")) == 0) {
			ofs = *(int *) (&mem[ofs + 12]);
			break;
		}
		ofs += 20;
	}

	mem[ofs] = joy.calib_data.x1 & 0xff;
	mem[ofs + 1] = (joy.calib_data.x1 >> 8) & 0xff;
	mem[ofs + 2] = (joy.calib_data.x1 >> 16) & 0xff;
	mem[ofs + 3] = (joy.calib_data.x1 >> 24) & 0xff;
	mem[ofs + 4] = joy.calib_data.x2 & 0xff;
	mem[ofs + 5] = (joy.calib_data.x2 >> 8) & 0xff;
	mem[ofs + 6] = (joy.calib_data.x2 >> 16) & 0xff;
	mem[ofs + 7] = (joy.calib_data.x2 >> 24) & 0xff;
	mem[ofs + 8] = joy.calib_data.x3 & 0xff;
	mem[ofs + 9] = (joy.calib_data.x3 >> 8) & 0xff;
	mem[ofs + 10] = (joy.calib_data.x3 >> 16) & 0xff;
	mem[ofs + 11] = (joy.calib_data.x3 >> 24) & 0xff;
	mem[ofs + 12] = joy.calib_data.y1 & 0xff;
	mem[ofs + 13] = (joy.calib_data.y1 >> 8) & 0xff;
	mem[ofs + 14] = (joy.calib_data.y1 >> 16) & 0xff;
	mem[ofs + 15] = (joy.calib_data.y1 >> 24) & 0xff;
	mem[ofs + 16] = joy.calib_data.y2 & 0xff;
	mem[ofs + 17] = (joy.calib_data.y2 >> 8) & 0xff;
	mem[ofs + 18] = (joy.calib_data.y2 >> 16) & 0xff;
	mem[ofs + 19] = (joy.calib_data.y2 >> 24) & 0xff;
	mem[ofs + 20] = joy.calib_data.y3 & 0xff;
	mem[ofs + 21] = (joy.calib_data.y3 >> 8) & 0xff;
	mem[ofs + 22] = (joy.calib_data.y3 >> 16) & 0xff;
	mem[ofs + 23] = (joy.calib_data.y3 >> 24) & 0xff;

	if ((handle = fopen(datfile_name, "wb")) == NULL)
		return;
	fwrite(mem, 1, len, handle);
	fclose(handle);

}
