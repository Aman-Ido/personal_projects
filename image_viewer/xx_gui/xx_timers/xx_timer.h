/* 
 * this is same as xx_timer_sdl.h but we won't be using SDL for this one,
 * except we will use time.h header file with clock to measure time and others
 * */
typedef struct {

  clock_t start;
  clock_t end;
  double result;
} xx_timer;

/* 
 *
 *  function - declarations
 * */
xx_timer* xx_create_timer ();
void xx_free_timer (xx_timer* i_timer);

/* 
 *
 *  functions - to manipulate time
 * */
void xx_start_timer (xx_timer* i_timer);
void xx_stop_timer (xx_timer* i_timer);
double xx_get_time_ms (xx_timer* i_timer); // this one will return the value of i_timer -> result
void xx_load_time_ms (xx_timer* i_timer); // this one just loads to the i_timer -> result
int xx_get_time_sec (xx_timer* i_timer); // just returns the value in seconds


/* 
 *
 *  function definitions
 * */

/* 
 * function - xx_timer* xx_create_timer ();
 *    -- returns the memory address for the allocated xx_timer
 * */
xx_timer* xx_create_timer () {
  xx_timer* r_timer = (xx_timer*) malloc (sizeof(xx_timer) * 1);
  if (r_timer == NULL) {
    printf ("\t ! Could not allocate memory to -> r_timer \n");
    return NULL;
  }

  r_timer -> start = 0;
  r_timer -> end = 0;
  r_timer -> result = 0.0f;


  return r_timer;

}


/* 
 * function - void xx_free_timer (xx_timer* i_timer);
 *  -- function to free everything, you don't have to worry about anything
 * */
void xx_free_timer (xx_timer* i_timer) {
  if (i_timer != NULL) {
    i_timer -> result = 0.0f;
    i_timer -> end = 0;
    i_timer -> start = 0;

    free (i_timer);
    i_timer = NULL;
  }
}


/*
 * function - void xx_start_timer (xx_timer* i_timer);
 *  -- function to start the timer it will start counting the time
 * */
void xx_start_timer (xx_timer* i_timer) {

  i_timer -> start = clock ();
}



/*
 * function - void xx_stop_timer (xx_timer* i_timer);
 *  -- function to start the timer it will start counting the time
 * */
void xx_stop_timer (xx_timer* i_timer) {

  i_timer -> end = clock ();
}



/*
 * function - void xx_get_time_ms (xx_timer* i_timer);
 *  -- function will return the time in milliseconds
 * */
double xx_get_time_ms (xx_timer* i_timer) {

  double r_value = 0.2f;

  xx_stop_timer (i_timer);
  r_value = difftime (i_timer -> end, i_timer -> start);

  return r_value;
}

/* 
 * function - void xx_load_time_ms (xx_timer* i_timer)
 *  -- function will load the result to i_timer -> result and won't return
 * */
void xx_load_time_ms (xx_timer* i_timer ) {


  double r_value = 0.2f;

  xx_stop_timer (i_timer);
  r_value = difftime (i_timer -> end, i_timer -> start);

  i_timer -> result = r_value;

}


/* 
 * function - int xx_get_time_sec ( xx_timer* i_timer )
 *  -- function will return the time in seconds (integer)
 * */
int xx_get_time_sec (xx_timer* i_timer) {
  int r_value = 0;

  xx_stop_timer (i_timer);
  r_value = (int) difftime (i_timer -> end, i_timer -> start);



  return r_value / 1000;
}
