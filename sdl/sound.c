#include "globals.h"

static Mix_Music *current_music = (Mix_Music *) NULL;
static struct {
	int id;
	Mix_Chunk *chunk;
	int used;
	int loop;
} soundsamples[128];

/* misc handling */

char dj_init(void)
{
	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_U16;
	int audio_channels = 2;
	int audio_buffers = 4096;

	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) < 0) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		return 0;
	}

	Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
	printf("Opened audio at %dHz %dbit %s, %d bytes audio buffer\n", audio_rate, (audio_format & 0xFF), (audio_channels > 1) ? "stereo" : "mono", audio_buffers);

	Mix_SetMusicCMD(getenv("MUSIC_CMD"));

	memset(soundsamples, 0, sizeof(soundsamples));

	return 0;
}

void dj_deinit(void)
{
//      int i;

	Mix_FadeOutMusic(1500);
	SDL_Delay(1500);
	Mix_HaltMusic();
	if (current_music)
		Mix_FreeMusic(current_music);
	current_music = NULL;

/*	for(i = 0; i < 128; i++) {
		if(soundsamples[i].used && soundsamples[i].chunk)
			Mix_FreeChunk(soundsamples[i].chunk);
	}*/

	Mix_CloseAudio();

	SDL_Quit();
}

void dj_start(void)
{
}

void dj_stop(void)
{
}

char dj_autodetect_sd(void)
{
	return 0;
}

char dj_set_stereo(char flag)
{
	return 0;
}

void dj_set_auto_mix(char flag)
{
}

unsigned short dj_set_mixing_freq(unsigned short freq)
{
	return freq;
}

void dj_set_dma_time(unsigned short time)
{
}

void dj_set_nosound(char flag)
{
}

/* mix handling */

void dj_mix(void)
{
}

/* sfx handling */

char dj_set_num_sfx_channels(char num_channels)
{
	return num_channels;
}

void dj_set_sfx_volume(char volume)
{
}

void dj_play_sfx(unsigned char sfx_num, unsigned short freq, char volume, char panning, unsigned short delay, char channel)
{
	int i;
	int used_channel;

	for (i = 0; i < 128; i++) {
		if (soundsamples[i].id == sfx_num)
			break;
	}
	if (i == 128)
		return;
	else if (!soundsamples[i].used)
		return;
	else if (!soundsamples[i].chunk)
		return;

	used_channel = Mix_PlayChannel(channel, soundsamples[i].chunk, soundsamples[i].loop);
	Mix_Volume(used_channel, volume * 2);
}

char dj_get_sfx_settings(unsigned char sfx_num, sfx_data * data)
{
	int i;

	data->priority = 0;
	data->default_freq = 0;
	data->default_volume = 0;
	data->length = 0;
	data->loop = 0;
	data->loop_start = 0;
	data->loop_length = 0;
	data->buf = 0;

	for (i = 0; i < 128; i++) {
		if (soundsamples[i].id == sfx_num)
			break;
	}
	if (i == 128)
		return 1;
	else if (!soundsamples[i].used)
		return 1;
	else if (!soundsamples[i].chunk)
		return 1;

	data->loop = soundsamples[i].loop;

	return 0;
}

char dj_set_sfx_settings(unsigned char sfx_num, sfx_data * data)
{
	int i;

	for (i = 0; i < 128; i++) {
		if (soundsamples[i].id == sfx_num)
			break;
	}
	if (i == 128)
		return 1;
	else if (!soundsamples[i].used)
		return 1;
	else if (!soundsamples[i].chunk)
		return 1;

	soundsamples[i].loop = (data->loop) ? (-1) : (0);

	return 0;
}

void dj_set_sfx_channel_volume(char channel_num, char volume)
{
	Mix_Volume(channel_num, volume * 2);
}

void dj_stop_sfx_channel(char channel_num)
{
	Mix_HaltChannel(channel_num);
}

char dj_load_sfx(FILE * file_handle, char *filename, int file_length, char sfx_type, unsigned char sfx_num)
{
	int i;
	typedef struct {
		char RIFF_ID[4];
		long riff_size;
		char WAVE_ID[4];
		char FMT_ID[4];
		long fmt_size;
		short FormatTag;
		unsigned short Channels;
		unsigned long SamplesPerSec;
		unsigned long AvgBytesPerSec;
		unsigned short BlockAlign;
		unsigned short BitsPerSample;
		char DATA_ID[4];
		long data_size;
		unsigned char data[0];
	} wave_file_t;

	wave_file_t *wave_buffer;
	SDL_RWops *rwop;

	for (i = 0; (i < 128) && soundsamples[i].used; i++);
	if (i == 128)
		return -1;

	wave_buffer = (wave_file_t *) malloc(sizeof(wave_file_t) + file_length);
	memset(wave_buffer, 0, sizeof(wave_file_t) + file_length);

	strncpy(wave_buffer->RIFF_ID, "RIFF", 4);
	wave_buffer->riff_size = sizeof(wave_file_t) + file_length - 8;
	strncpy(wave_buffer->WAVE_ID, "WAVE", 4);
	strncpy(wave_buffer->FMT_ID, "fmt ", 4);
	wave_buffer->fmt_size = 16;
	wave_buffer->FormatTag = 1;
	wave_buffer->Channels = 1;
	switch (sfx_num) {
	case SFX_JUMP:
		wave_buffer->SamplesPerSec = SFX_JUMP_FREQ;
		break;
	case SFX_LAND:
		wave_buffer->SamplesPerSec = SFX_LAND_FREQ;
		break;
	case SFX_DEATH:
		wave_buffer->SamplesPerSec = SFX_DEATH_FREQ;
		break;
	case SFX_SPRING:
		wave_buffer->SamplesPerSec = SFX_SPRING_FREQ;
		break;
	case SFX_SPLASH:
		wave_buffer->SamplesPerSec = SFX_SPLASH_FREQ;
		break;
	case SFX_FLY:
		wave_buffer->SamplesPerSec = SFX_FLY_FREQ;
		break;
	default:
		wave_buffer->SamplesPerSec = 22050;
		break;
	}
	wave_buffer->BitsPerSample = 16;
	wave_buffer->AvgBytesPerSec = wave_buffer->SamplesPerSec * wave_buffer->Channels * (wave_buffer->BitsPerSample / 8);
	wave_buffer->BlockAlign = 1;
	strncpy(wave_buffer->DATA_ID, "data", 4);
	wave_buffer->data_size = file_length;
	fread(wave_buffer->data, file_length, 1, file_handle);

	rwop = SDL_RWFromMem(wave_buffer, sizeof(wave_file_t) + file_length);

	soundsamples[i].chunk = Mix_LoadWAV_RW(rwop, 1);
	soundsamples[i].chunk->allocated = 1;
	soundsamples[i].used = 1;
	soundsamples[i].id = sfx_num;
	soundsamples[i].loop = 0;

	return 0;
}

void dj_free_sfx(unsigned char sfx_num)
{
	if (sfx_num >= 128 || !soundsamples[sfx_num].used)
		return;

	Mix_FreeChunk(soundsamples[sfx_num].chunk);
	soundsamples[sfx_num].chunk = (Mix_Chunk *) NULL;
	soundsamples[sfx_num].used = 0;
}

/* mod handling */

char dj_ready_mod(char mod_num)
{
	FILE *tmp;
#ifdef _MSC_VER
	char filename[] = "jnb.tmpmusic.mod";
#else
	char filename[] = "/tmp/jnb.tmpmusic.mod";
#endif
	FILE *fp;
	int len;
	switch (mod_num) {
	case MOD_MENU:
		fp = dat_open("jump.mod", datfile_name, "rb");
		len = dat_filelen("jump.mod", datfile_name);
		break;
	case MOD_GAME:
		fp = dat_open("bump.mod", datfile_name, "rb");
		len = dat_filelen("bump.mod", datfile_name);
		break;
	case MOD_SCORES:
		fp = dat_open("scores.mod", datfile_name, "rb");
		len = dat_filelen("scores.mod", datfile_name);
		break;
	default:
		break;
	}

	if (Mix_PlayingMusic())
		Mix_FadeOutMusic(1500);

	if (current_music) {
		Mix_FreeMusic(current_music);
		current_music = NULL;
	}
	tmp = fopen(filename, "wb");
	for (; len > 0; len--)
		fputc(fgetc(fp), tmp);
	fflush(tmp);
	fclose(tmp);
	fclose(fp);

	current_music = Mix_LoadMUS(filename);
	if (current_music == NULL) {
		fprintf(stderr, "Couldn't load music: %s\n", SDL_GetError());
		return 0;
	}

	return 0;
}

char dj_start_mod(void)
{
	Mix_VolumeMusic(64);

	Mix_FadeInMusic(current_music, -1, 2000);

	return 0;
}

void dj_stop_mod(void)
{
	Mix_HaltMusic();
}

void dj_set_mod_volume(char volume)
{
	//Mix_VolumeMusic(volume*4);
}

char dj_load_mod(FILE * file_handle, char *filename, char mod_num)
{
	return 0;
}

void dj_free_mod(char mod_num)
{
}
