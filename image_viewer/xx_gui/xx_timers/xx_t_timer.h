/* 
 * this is just another timer program but this time I' going to use time except for clock
 * */
typedef struct {

  time_t start;
  time_t end;
  long int result;
} xx_t_timer;

/* 
 *
 *  function - declarations
 * */
xx_t_timer* xx_create_timer ();
void xx_free_timer (xx_t_timer* i_timer);

/* 
 *
 *  functions - to manipulate time
 * */
void xx_start_timer (xx_t_timer* i_timer);
void xx_stop_timer (xx_t_timer* i_timer);
long int xx_get_time_sec (xx_t_timer* i_timer); // just returns the value in seconds
long int xx_diff_time ( time_t i_end, time_t i_start );


/* 
 *
 *  function definitions
 * */

/* 
 * function - xx_t_timer* xx_create_timer ();
 *    -- returns the memory address for the allocated xx_t_timer
 * */
xx_t_timer* xx_create_timer () {
  xx_t_timer* r_timer = (xx_t_timer*) malloc (sizeof(xx_t_timer) * 1);
  if (r_timer == NULL) {
    printf ("\t ! Could not allocate memory to -> r_timer \n");
    return NULL;
  }

  r_timer -> start = 0;
  r_timer -> end = 0;
  r_timer -> result = 0;


  return r_timer;

}


/* 
 * function - void xx_free_timer (xx_t_timer* i_timer);
 *  -- function to free everything, you don't have to worry about anything
 * */
void xx_free_timer (xx_t_timer* i_timer) {
  if (i_timer != NULL) {
    i_timer -> result = 0;
    i_timer -> end = 0;
    i_timer -> start = 0;

    free (i_timer);
    i_timer = NULL;
  }
}


/*
 * function - void xx_start_timer (xx_t_timer* i_timer);
 *  -- function to start the timer it will start counting the time
 * */
void xx_start_timer (xx_t_timer* i_timer) {

  //i_timer -> start = clock ();
  i_timer -> start = time (NULL);
}



/*
 * function - void xx_stop_timer (xx_t_timer* i_timer);
 *  -- function to start the timer it will start counting the time
 * */
void xx_stop_timer (xx_t_timer* i_timer) {

  i_timer -> end = time (NULL);
  //i_timer -> end = clock ();
}

/* 
 * function - int xx_get_time_sec ( xx_t_timer* i_timer )
 *  -- function will return the time in seconds (integer)
 * */
long int xx_get_time_sec (xx_t_timer* i_timer) {
  long int r_value = 0;

  xx_stop_timer (i_timer);
  // r_value = (int) difftime (i_timer -> end, i_timer -> start);
  r_value = xx_diff_time ( i_timer -> end, i_timer -> start );



  return r_value;
}

/* 
 * function - long int xx_diff_time ( time_t end, time_t i_start )
 *    -- this function will subtract the timme and return long int value (in seconds only)
 * */
long int xx_diff_time ( time_t i_end, time_t i_start ) {
  long int result = 0;

  result = i_end - i_start;
  return result;
}
