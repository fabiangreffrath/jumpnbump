#include "globals.h"

extern unsigned int ban_map[17][22];

void fireworks(void)
{
	FILE *handle;
	int c1, c2;
	int s1, s2, s3;
	char pal[768];
	struct {
		char used, direction, colour;
		int x, y;
		int x_add, y_add;
		int timer;
		int anim, frame, frame_tick, image;
	} rabbits[20];
	struct {
		int x, y;
		int old_x, old_y;
		char col, back[2];
	} stars[300];

	register_background(NULL);

	if ((handle = dat_open("level.pcx", datfile_name, "rb")) == 0) {
		strcpy(main_info.error_str, "Error loading 'level.pcx', aborting...\n");
		return;
	}
	read_pcx(handle, mask_pic, 102400, pal);
	fclose(handle);

	memset(mask_pic, 0, 102400);

	memset(ban_map, 0, sizeof(ban_map));

	fillpalette(0, 0, 0);

	draw_begin();

	for (c2 = 193; c2 < 256; c2++) {
		clear_lines(0, c2, 1, (c2 - 192) >> 2);
		clear_lines(1, c2, 1, (c2 - 192) >> 2);
	}

	draw_end();

	setpalette(0, 256, pal);

	for (c1 = 0; c1 < 20; c1++)
		rabbits[c1].used = 0;

	rabbits[0].used = 1;
	rabbits[0].colour = (char)rnd(4);
	rabbits[0].x = (int) (150 + rnd(100)) << 16;
	rabbits[0].y = 256 << 16;
	rabbits[0].x_add = ((int) rnd(65535) << 1) - 65536;
	if (rabbits[0].x_add > 0)
		rabbits[0].direction = 0;
	else
		rabbits[0].direction = 1;
	rabbits[0].y_add = -262144 + (rnd(16384) * 5);
	rabbits[0].timer = 30 + rnd(150);
	rabbits[0].anim = 2;
	rabbits[0].frame = 0;
	rabbits[0].frame_tick = 0;
	rabbits[0].image = player_anims[rabbits[0].anim].frame[rabbits[0].frame].image + rabbits[0].colour * 18 + rabbits[0].direction * 9;

	draw_begin();
	for (c1 = 0; c1 < 300; c1++) {
		s1 = rnd(400);
		s2 = rnd(256);
		s3 = 30 - rnd(7);
		stars[c1].x = stars[c1].old_x = (s1 << 16);
		stars[c1].y = stars[c1].old_y = (s2 << 16);
		stars[c1].col = s3;
		stars[c1].back[0] = stars[c1].back[1] = get_pixel(0, s1, s2);
	}
	draw_end();

	dj_set_nosound(0);

	main_info.page_info[0].num_pobs = 0;
	main_info.page_info[1].num_pobs = 0;
	main_info.view_page = 0;
	main_info.draw_page = 1;

	while (key_pressed(1) == 0) {

		dj_mix();
		intr_sysupdate();

		for (c1 = 0; c1 < 300; c1++) {
			stars[c1].old_x = stars[c1].x;
			stars[c1].old_y = stars[c1].y;
			stars[c1].y -= (int) (31 - stars[c1].col) * 16384;
			if ((stars[c1].y >> 16) < 0)
				stars[c1].y += 256 << 16;
			if ((stars[c1].y >> 16) >= 256)
				stars[c1].y -= 256 << 16;
		}

		for (c1 = 0, c2 = 0; c1 < 20; c1++) {
			if (rabbits[c1].used == 1)
				c2++;
		}
		if ((c2 == 0 && rnd(10000) < 200) || (c2 == 1 && rnd(10000) < 150) || (c2 == 2 && rnd(10000) < 100) || (c2 == 3 && rnd(10000) < 50)) {
			for (c1 = 0; c1 < 20; c1++) {
				if (rabbits[c1].used == 0) {
					rabbits[c1].used = 1;
					rabbits[c1].colour = (char)rnd(4);
					rabbits[c1].x = (int) (150 + rnd(100)) << 16;
					rabbits[c1].y = 256 << 16;
					rabbits[c1].x_add = ((int) rnd(65535) << 1) - 65536;
					if (rabbits[c1].x_add > 0)
						rabbits[c1].direction = 0;
					else
						rabbits[c1].direction = 1;
					rabbits[c1].y_add = -262144 + (rnd(16384) * 5);
					rabbits[c1].timer = 30 + rnd(150);
					rabbits[c1].anim = 2;
					rabbits[c1].frame = 0;
					rabbits[c1].frame_tick = 0;
					rabbits[c1].image = player_anims[rabbits[c1].anim].frame[rabbits[c1].frame].image + rabbits[c1].colour * 18 + rabbits[c1].direction * 9;
					break;
				}
			}
		}

		dj_mix();

		main_info.page_info[main_info.draw_page].num_pobs = 0;

		for (c1 = 0; c1 < 20; c1++) {
			if (rabbits[c1].used == 1) {
				rabbits[c1].y_add += 2048;
				if (rabbits[c1].y_add > 36864 && rabbits[c1].anim != 3) {
					rabbits[c1].anim = 3;
					rabbits[c1].frame = 0;
					rabbits[c1].frame_tick = 0;
					rabbits[c1].image = player_anims[rabbits[c1].anim].frame[rabbits[c1].frame].image + rabbits[c1].colour * 18 + rabbits[c1].direction * 9;
				}
				rabbits[c1].x += rabbits[c1].x_add;
				rabbits[c1].y += rabbits[c1].y_add;
				if ((rabbits[c1].x >> 16) < 16 || (rabbits[c1].x >> 16) > 400 || (rabbits[c1].y >> 16) > 256) {
					rabbits[c1].used = 0;
					continue;
				}
				rabbits[c1].timer--;
				if (rabbits[c1].timer <= 0) {
					rabbits[c1].used = 0;
					for (c2 = 0; c2 < 6; c2++)
						add_object(OBJ_FUR, (rabbits[c1].x >> 16) + 6 + rnd(5), (rabbits[c1].y >> 16) + 6 + rnd(5), rabbits[c1].x_add + (rnd(65535) - 32768) * 3, rabbits[c1].y_add + (rnd(65535) - 32768) * 3, 0, 44 + rabbits[c1].colour * 8);
					for (c2 = 0; c2 < 6; c2++)
						add_object(OBJ_FLESH, (rabbits[c1].x >> 16) + 6 + rnd(5), (rabbits[c1].y >> 16) + 6 + rnd(5), rabbits[c1].x_add + (rnd(65535) - 32768) * 3, rabbits[c1].y_add + (rnd(65535) - 32768) * 3, 0, 76);
					for (c2 = 0; c2 < 6; c2++)
						add_object(OBJ_FLESH, (rabbits[c1].x >> 16) + 6 + rnd(5), (rabbits[c1].y >> 16) + 6 + rnd(5), rabbits[c1].x_add + (rnd(65535) - 32768) * 3, rabbits[c1].y_add + (rnd(65535) - 32768) * 3, 0, 77);
					for (c2 = 0; c2 < 8; c2++)
						add_object(OBJ_FLESH, (rabbits[c1].x >> 16) + 6 + rnd(5), (rabbits[c1].y >> 16) + 6 + rnd(5), rabbits[c1].x_add + (rnd(65535) - 32768) * 3, rabbits[c1].y_add + (rnd(65535) - 32768) * 3, 0, 78);
					for (c2 = 0; c2 < 10; c2++)
						add_object(OBJ_FLESH, (rabbits[c1].x >> 16) + 6 + rnd(5), (rabbits[c1].y >> 16) + 6 + rnd(5), rabbits[c1].x_add + (rnd(65535) - 32768) * 3, rabbits[c1].y_add + (rnd(65535) - 32768) * 3, 0, 79);
					dj_play_sfx(SFX_DEATH, SFX_DEATH_FREQ, 64, 0, 0, -1);
					continue;
				}
				rabbits[c1].frame_tick++;
				if (rabbits[c1].frame_tick >= player_anims[rabbits[c1].anim].frame[rabbits[c1].frame].ticks) {
					rabbits[c1].frame++;
					if (rabbits[c1].frame >= player_anims[rabbits[c1].anim].num_frames)
						rabbits[c1].frame = player_anims[rabbits[c1].anim].restart_frame;
					rabbits[c1].frame_tick = 0;
				}
				rabbits[c1].image = player_anims[rabbits[c1].anim].frame[rabbits[c1].frame].image + rabbits[c1].colour * 18 + rabbits[c1].direction * 9;
				if (rabbits[c1].used == 1)
					add_pob(main_info.draw_page, rabbits[c1].x >> 16, rabbits[c1].y >> 16, rabbits[c1].image, rabbit_gobs);
			}
		}

		dj_mix();

		update_objects();

		draw_begin();

		for (c1 = 0; c1 < 300; c1++) {
			stars[c1].back[main_info.draw_page] = get_pixel(main_info.draw_page, stars[c1].x >> 16, stars[c1].y >> 16);
			set_pixel(main_info.draw_page, stars[c1].x >> 16, stars[c1].y >> 16, stars[c1].col);
		}

		dj_mix();

		draw_pobs(main_info.draw_page);
		
		draw_end();

		main_info.draw_page ^= 1;
		main_info.view_page ^= 1;

		flippage(main_info.view_page);

		wait_vrt(1);

		draw_begin();

		redraw_pob_backgrounds(main_info.draw_page);

		dj_mix();
		intr_sysupdate();

		for (c1 = 299; c1 >= 0; c1--) {
			set_pixel(main_info.draw_page, stars[c1].old_x >> 16, stars[c1].old_y >> 16, stars[c1].back[main_info.draw_page]);
		}

		draw_end();

	}

	dj_set_nosound(1);

}

