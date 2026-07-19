/* 
 *  this file contains the timer thing using SDL
 * */

typedef struct {
  int* start = NULL;
  int* end = NULL;
  int* result = NULL;
} xx_timer;

/* 
 *  function - declarations
 * */
xx_timer* xx_create_timer (); // allocates necessary memory
void xx_free_timer (xx_timer* i_timer); // deallocates memory                               

// functions to start, end, and get the current time
void xx_start_timer (xx_timer* i_timer);
void xx_end_timer (xx_timer* i_timer);
int xx_get_time_ms (xx_timer* i_timer);
int xx_get_time_seconds (xx_timer* i_timer);
/* 
 *  function - definitions
 * */

/* 
 *  function - xx_timer* xx_create_timer ()
 *      -- this function will allocate memory and return the pointer
 * */
xx_timer* xx_create_timer () {
  xx_timer* r_timer = (xx_timer*) malloc (sizeof(xx_timer) * 1);
  if (r_timer == NULL) {
    printf ("\t ! Memory allocation, failed, xx_timer\n");
    return NULL;
  } else {
    printf ("\t * Memory allocation, success, xx_timer\n");
  }

  r_timer -> start = (int*) malloc (sizeof(Uint32) * 1);
   if (r_timer == NULL) {
    printf ("\t ! Memory allocation, failed, r_timer -> start\n");
    return NULL;
  } else {
    printf ("\t * Memory allocation, success, r_timer -> start\n");
  }

  r_timer -> end = (int*) malloc (sizeof(Uint32) * 1);
  if (r_timer == NULL) {
    printf ("\t ! Memory allocation, failed, r_timer -> end\n");
    return NULL;
  } else {
    printf ("\t * Memory allocation, success, r_timer -> end\n");
  } 
 
  r_timer -> result = (int*) malloc (sizeof(Uint32) * 1);
  if (r_timer == NULL) {
    printf ("\t ! Memory allocation, failed, r_timer -> result\n");
    return NULL;
  } else {
    printf ("\t * Memory allocation, success, r_timer -> result\n");
  } 
   


  return r_timer; 
}

/* 
 *  function - void xx_free_timer (xx_timer* i_timer);
 * */
void xx_free_timer (xx_timer* i_timer) {
  if (i_timer -> start != NULL) {
    free (i_timer -> start);
    i_timer -> start = NULL;
  }

  if (i_timer -> end != NULL) {
    free (i_timer -> end);
    i_timer -> end = NULL;
  }

  if (i_timer -> result != NULL) {
    free (i_timer -> result);
    i_timer -> result = NULL;
  }

  if (i_timer != NULL) {

    free (i_timer);
    i_timer = NULL;
  }

}

/* 
 *  function - void xx_start_timer (xx_timer* i_timer);
 * */
void xx_start_timer (xx_timer* i_timer ){
  *i_timer -> start = SDL_GetTicks ();

}


/* 
 *  function - void xx_end_timer (xx_timer* i_timer);
 * */
void xx_end_timer (xx_timer* i_timer ) {
  *i_timer -> end = SDL_GetTicks () - *i_timer -> start;
}

/* 
 *  function - int xx_get_time_ms (xx_timer* i_timer) // in milliseconds
 * */
int xx_get_time_ms (xx_timer* i_timer) {
  xx_end_timer (i_timer);
  *i_timer -> result =  (*i_timer -> end - *i_timer -> start);

  return * i_timer -> result;
}


/* 
 *  function - int xx_get_time_seconds (xx_timer* i_timer) // in seconds
 * */
int xx_get_time_seconds (xx_timer* i_timer) {
  xx_end_timer (i_timer);
  *i_timer -> result = *i_timer -> end - *i_timer -> start;

  return (*i_timer -> result / 1000);
}
