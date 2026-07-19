/* 
 * this file is strictly for audio manager only. (like chunk sounds)
 * make sure to include SDL_mixer 
 * */
typedef struct {
  int left;
  int right;
} xx_chunk_data_pan;

typedef struct {
  Mix_Chunk* sound;
  int repeat;
  int volume;
  xx_chunk_data_pan* pan;
} xx_chunk_manager;

typedef struct {
  Mix_Music* music;
  xx_chunk_data_pan* pan;
  int volume;
} xx_music_manager;

/* 
 * function - declarations 
 *
 * */

xx_chunk_manager* xx_create_chunk_manager (  char* file_path, int times );
void xx_free_chunk_manager ( xx_chunk_manager* i_chunk );

void xx_play_chunk ( xx_chunk_manager* i_chunk );

void xx_set_chunk_volume ( xx_chunk_manager* i_chunk, int percent );
void xx_set_chunk_repeat ( xx_chunk_manager* i_chunk, int repeat );
void xx_set_chunk_pan ( xx_chunk_manager* i_chunk, int l, int r ); // l - left, r - right

/* 
 * function - definitions
 * */

/* 
 * function - xx_chunk_manager* xx_create_chunk_manager ( char* file_path, int times );
 *    -- this function will allocate memory for xx_chunk_manager and return the pointer
 * */
xx_chunk_manager* xx_create_chunk_manager ( char* file_path, int times ) {
  xx_chunk_manager* r_chunk = (xx_chunk_manager*) malloc (sizeof(xx_chunk_manager) * 1);
  if ( r_chunk == NULL ) {
    printf ("\t Error! Memory Allocation, Failed, xx_chunk_manager*\n");
    return NULL;
  }

  r_chunk -> sound = Mix_LoadWAV ( file_path );
  if ( r_chunk -> sound == NULL ) {
    printf ("\t Error! Memory Allocation, Failed, xx_chunk_manager* -> Mix_Chunk*\n");
    return NULL;
  }

  if ( times < 0 ) {
    if ( times == -1 ) {
      r_chunk -> repeat = 0;
    } else {
      times *= -1;
      r_chunk -> repeat = times;
    }
  }
  r_chunk -> volume = 100; // default (max is 128) ( x * 128 ) / 100 -> ans
  Mix_VolumeChunk ( r_chunk -> sound, r_chunk -> volume );

  r_chunk -> pan = (xx_chunk_data_pan*) malloc (sizeof(xx_chunk_data_pan) * 1);
  if ( r_chunk -> pan == NULL ) {
    printf ("\t Error! Memory Allocation, Failed, xx_chunk_manager* -> xx_chunk_data_pan*\n");
    return NULL;
  }

  r_chunk -> pan -> left = r_chunk -> volume * 2; // max is 255
  r_chunk -> pan -> right = r_chunk -> volume * 2; // max is 255

  return r_chunk;
}

/* 
 * function - void xx_free_chunk_manager ( xx_chunk_manager* i_chunk )
 *    -- this function will free the memory
 * */
void xx_free_chunk_manager ( xx_chunk_manager* i_chunk ) {
  if ( i_chunk != NULL ) {
    free ( i_chunk -> pan );
    i_chunk -> pan = NULL;

    Mix_FreeChunk ( i_chunk -> sound );
    i_chunk -> sound = NULL;

    free ( i_chunk );
    i_chunk = NULL;
  }
}

/* 
 * function - void xx_play_chunk ( xx_chunk_manager* i_chunk )
 *    -- this function will play the sound (with set up settings)
 * */
void xx_play_chunk ( xx_chunk_manager* i_chunk ) {
  if ( i_chunk -> sound != NULL ) {
    Mix_SetPanning ( -1, i_chunk -> pan -> right, i_chunk -> pan -> left );
    Mix_PlayChannel ( -1, i_chunk -> sound, i_chunk -> repeat );
  }
}

/* 
 * function - void xx_set_chunk_pan ( xx_chunk_manager* i_chunk, int l, int r )
 *    -- this function will set the panning
 * */
void xx_set_chunk_pan ( xx_chunk_manager* i_chunk, int l, int r ) {
  if ( l > 100 ) {
    l = 100;
  } else if ( l < 0) {
    l = 0;
  }

  if ( r > 100 ) {
    r = 100;
  } else if ( r < 0 ) {
    r = 0;
  }

  i_chunk -> pan -> left = ( l * 255 ) / 100;
  i_chunk -> pan -> right = ( r * 255 ) / 100;
}

/* 
 * function - void xx_set_chunk_volume ( xx_chunk_manager* i_chunk, int percent );
 *    -- function to set the volume of the chunk
 * */
void xx_set_chunk_volume ( xx_chunk_manager* i_chunk, int percent ) {
  if ( percent > 100 ) {
    percent = 100;
  } else if ( percent < 0) {
    percent = 0;
  }

  i_chunk -> volume = (percent * 128) / 100;
  Mix_VolumeChunk ( i_chunk -> sound, i_chunk -> volume );
}

/* 
 * function - void xx_set_chunk_repeat ( xx_chunk_manager* i_chunk, int repeat )
 *    -- sets the repeat of the sound
 * */
void xx_set_chunk_repeat ( xx_chunk_manager* i_chunk, int repeat ) {
  if ( repeat < 0 ) {
    repeat = 0;
  }
  i_chunk -> repeat = repeat;
}
