#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dpmi.h>
#include <sys/nearptr.h>
#include <pc.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <dj.h>

#define KEY_PL1_LEFT 0xcb
#define KEY_PL1_RIGHT	0xcd
#define KEY_PL1_JUMP 0xc8
#define KEY_PL2_LEFT 0x1e
#define KEY_PL2_RIGHT	0x20
#define KEY_PL2_JUMP 0x11

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


struct {
	char joy_enabled, mouse_enabled, num_mouse_buttons;
	char no_sound, no_gore, fireworks;
	char error_str[256];
  char draw_page, view_page;
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
	char enabled, dead_flag;
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
	char used, type;
	int x, y;
  int x_add, y_add;
	int x_acc, y_acc;
  int anim;
  int frame, ticks;
  int image;
} objects[300];

struct {
	int x, y;
	int raw_x, raw_y;
	char but1, but2;
	struct {
		int x1, x2, x3;
		int y1, y2, y3;
	} calib_data;
} joy;

struct {
	char but1, but2, but3;
} mouse;

char datfile_name[256];

char *background_pic;
char *mask_pic;

char *rabbit_gobs;
char *font_gobs;


// main.c

void steer_players(void);
void position_player(short player_num);
void fireworks(void);
void add_object(char type, short x, short y, long x_add, long y_add, short anim, short frame);
void update_objects(void);
char add_pob(char page, short x, short y, short image, char *pob_data);
void draw_pobs(char page);
void redraw_pob_backgrounds(char page);
char add_leftovers(char page, short x, short y, short image, char *pob_data);
void draw_leftovers(char page);
char init_level(short level);
void deinit_level(void);
char init_program(int argc, char *argv[]);
void deinit_program(void);
void read_joy(void);
char calib_joy(char type);
void read_mouse(void);
unsigned short rnd(unsigned short max);
char read_level(void);
FILE *dat_open(char *file_name, char *dat_name, char *mode);
int dat_filelen(char *file_name, char *dat_name);
void write_calib_data(void);


// menu.c

char menu(void);
char menu_init(void);
void menu_deinit(void);


// gfx.c

void open_screen(void);
void wait_vrt(void);
//void get_block(char page, short x, short y, short width, short height, char *buffer);
//void put_block(char page, short x, short y, short width, short height, char *buffer);
void put_text(char page, int x, int y, char *text, char align);
void put_pob(char page, short x, short y, short image, char *pob_data, char mask, char *mask_pic);
char pob_col(short x1, short y1, short image1, char *pob_data1, short x2, short y2, short image2, char *pob_data2);
short pob_width(short image, char *pob_data);
short pob_height(short image, char *pob_data);
short pob_hs_x(short image, char *pob_data);
short pob_hs_y(short image, char *pob_data);
char read_pcx(FILE *handle, char *buffer, long buf_len, char *pal);

// gfx.s

void get_block(char page, long x, long y, long width, long height, char *buffer);
void put_block(char page, long x, long y, long width, long height, char *buffer);


// interrpt.c

volatile char last_keys[50];

char hook_keyb_handler(void);
void remove_keyb_handler(void);
char key_pressed(unsigned char key);

