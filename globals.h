#ifndef __GLOBALS_H
#define __GLOBALS_H

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#include <strings.h>
#endif
#include <time.h>
#include <math.h>
#include <dj.h>

#ifdef DOS
# include <conio.h>
# include <dpmi.h>
# include <sys/nearptr.h>
# include <pc.h>
#endif

#ifdef _MSC_VER
# include <sys/stat.h>
# include <io.h>
# include <SDL.h>
# include <SDL_mixer.h>
#else
# ifdef USE_SDL
#  include <sys/stat.h>
#  include <SDL/SDL.h>
#  include <SDL/SDL_mixer.h>
# endif
#endif

#ifndef USE_SDL
#define KEY_PL1_LEFT 0xcb
#define KEY_PL1_RIGHT	0xcd
#define KEY_PL1_JUMP 0xc8
#define KEY_PL2_LEFT 0x1e
#define KEY_PL2_RIGHT	0x20
#define KEY_PL2_JUMP 0x11
#else
#define KEY_PL1_LEFT SDLK_LEFT
#define KEY_PL1_RIGHT	SDLK_RIGHT
#define KEY_PL1_JUMP SDLK_UP
#define KEY_PL2_LEFT SDLK_a
#define KEY_PL2_RIGHT	SDLK_d
#define KEY_PL2_JUMP SDLK_w
#define KEY_PL3_LEFT SDLK_j
#define KEY_PL3_RIGHT	SDLK_l
#define KEY_PL3_JUMP SDLK_i
#define KEY_PL4_LEFT SDLK_KP4
#define KEY_PL4_RIGHT	SDLK_KP6
#define KEY_PL4_JUMP SDLK_KP8
#endif

#define NUM_POBS 200
#define NUM_OBJECTS 200
#define NUM_FLIES 20
#define NUM_LEFTOVERS 50

#define OBJ_SPRING 0
#define OBJ_SPLASH 1
#define OBJ_SMOKE 2
#define OBJ_YEL_BUTFLY 3
#define OBJ_PINK_BUTFLY 4
#define OBJ_FUR 5
#define OBJ_FLESH 6
#define OBJ_FLESH_TRACE 7

#define OBJ_ANIM_SPRING 0
#define OBJ_ANIM_SPLASH 1
#define OBJ_ANIM_SMOKE 2
#define OBJ_ANIM_YEL_BUTFLY_RIGHT 3
#define OBJ_ANIM_YEL_BUTFLY_LEFT 4
#define OBJ_ANIM_PINK_BUTFLY_RIGHT 5
#define OBJ_ANIM_PINK_BUTFLY_LEFT 6
#define OBJ_ANIM_FLESH_TRACE 7

#define MOD_MENU 0
#define MOD_GAME 1
#define MOD_SCORES 2

#define SFX_JUMP 0
#define SFX_LAND 1
#define SFX_DEATH 2
#define SFX_SPRING 3
#define SFX_SPLASH 4
#define SFX_FLY 5

#define SFX_JUMP_FREQ 15000
#define SFX_LAND_FREQ 15000
#define SFX_DEATH_FREQ 20000
#define SFX_SPRING_FREQ 15000
#define SFX_SPLASH_FREQ 12000
#define SFX_FLY_FREQ 12000

#define BAN_VOID	0
#define BAN_SOLID	1
#define BAN_WATER	2
#define BAN_ICE		3
#define BAN_SPRING	4


struct {
	int joy_enabled, mouse_enabled;
	int no_sound, no_gore, fireworks;
	char error_str[256];
	int draw_page, view_page;
	struct {
		int num_pobs;
		struct {
			int x, y;
			int image;
			char *pob_data;
			int back_buf_ofs;
		} pobs[NUM_POBS];
	} page_info[2];
	char *pob_backbuf[2];
} main_info;

struct {
	int action_left,action_up,action_right;
	int enabled, dead_flag;
	int bumps;
	int bumped[4];
	int x, y;
	int x_add, y_add;
	char direction, jump_ready, jump_abort, in_water;
	int anim, frame, frame_tick, image;
} player[4];

struct {
	int num_frames;
	int restart_frame;
	struct {
		int image;
		int ticks;
	} frame[4];
} player_anims[7];

struct {
	int used, type;
	int x, y;
	int x_add, y_add;
	int x_acc, y_acc;
	int anim;
	int frame, ticks;
	int image;
} objects[NUM_OBJECTS];

struct {
	int x, y;
	int raw_x, raw_y;
	int but1, but2;
	struct {
		int x1, x2, x3;
		int y1, y2, y3;
	} calib_data;
} joy;

struct {
	int but1, but2, but3;
} mouse;

char datfile_name[2048];

char *background_pic;
char *mask_pic;

char *rabbit_gobs;
char *font_gobs;


/* fireworks.c */

void fireworks(void);


/* main.c */

void steer_players(void);
void position_player(int player_num);
void fireworks(void);
void add_object(int type, int x, int y, int x_add, int y_add, int anim, int frame);
void update_objects(void);
int add_pob(int page, int x, int y, int image, char *pob_data);
void draw_flies(int page);
void draw_pobs(int page);
void redraw_flies_background(int page);
void redraw_pob_backgrounds(int page);
int add_leftovers(int page, int x, int y, int image, char *pob_data);
void draw_leftovers(int page);
int init_level(int level, char *pal);
void deinit_level(void);
int init_program(int argc, char *argv[], char *pal);
void deinit_program(void);
unsigned short rnd(unsigned short max);
int read_level(void);
FILE *dat_open(char *file_name, char *dat_name, char *mode);
int dat_filelen(char *file_name, char *dat_name);
void write_calib_data(void);


/* input.c */

void update_player_actions(void);
void init_inputs(void);
int calib_joy(char type);

/* menu.c */

int menu(void);
int menu_init(void);
void menu_deinit(void);


/* gfx.c */

void open_screen(void);
void wait_vrt(int mix);
void flippage(int page);
void clear_page(int page, int color);
void clear_lines(int page, int y, int count, int color);
void setpalette(int index, int count, char *palette);
void fillpalette(int red, int green, int blue);
#ifdef DOS
void get_block(char page, short x, short y, short width, short height, char *buffer);
void put_block(char page, short x, short y, short width, short height, char *buffer);
#else
void get_block(int page, int x, int y, int width, int height, char *buffer);
void put_block(int page, int x, int y, int width, int height, char *buffer);
#endif
void put_text(int page, int x, int y, char *text, int align);
void put_pob(int page, int x, int y, int image, char *pob_data, int mask, char *mask_pic);
int pob_col(int x1, int y1, int image1, char *pob_data1, int x2, int y2, int image2, char *pob_data2);
int pob_width(int image, char *pob_data);
int pob_height(int image, char *pob_data);
int pob_hs_x(int image, char *pob_data);
int pob_hs_y(int image, char *pob_data);
int read_pcx(FILE * handle, char *buffer, int buf_len, char *pal);

/* gfx.c */

#ifdef USE_SDL
#ifndef _MSC_VER
long filelength(int handle);
#endif
void fs_toggle();
char *get_vgaptr(int, int, int);
int intr_sysupdate();
#endif

/* interrpt.c */

extern char last_keys[50];

int hook_keyb_handler(void);
void remove_keyb_handler(void);
int key_pressed(int key);

/* sound-linux.c */
#ifdef LINUX


#endif

#endif
