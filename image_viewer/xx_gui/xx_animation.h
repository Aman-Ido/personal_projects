/* 
 * in this program I'm going to make a animation header file where
 *  -- it will be like grid, will store the coordination of the elements and we will input those
 *  -- when we call a specific animation, we will change that id's or thatn name's element 
 * */
typedef struct {
  int speed;
  int x;
  int y;
  int is_on;
  int reverse;
} xx_animation_data;

typedef struct {
  int data_n; // number of data 
  xx_animation_data** data; // this will be a 2d array - 0, will be the original, 1 will be the continuous change

  int n; // cursor - to keep track of the elements
  int screen_width;
  int screen_height;
} xx_animation;

/* 
 * function - declarations
 * */
xx_animation* xx_create_animation_cord ( int screen_width, int screen_height, int n );
void xx_free_animation_cord ( xx_animation* i_value );

int xx_add_to_animation_cord ( xx_animation* i_value, int speed , int x, int y );
void xx_remove_from_animation_cord ( xx_animation* i_value );

int xx_x_anime ( xx_animation* i_value, int i_n );
int xx_y_anime ( xx_animation* i_value, int i_n );

int xx_set_anime_on ( xx_animation* i_value, int i_n );

void xx_update_animation ( xx_animation* i_value, int i_n, int* x, int* y );
int xx_is_animation_on ( xx_animation* i_value, int i_n );


/* 
 * function - definitions
 * */

/* 
 * function - xx_animation* xx_create_animation_cord ( int screen_width, int screen_height );
 *    -- this function will create everything (allocate memory) and return the pointer
 * */
xx_animation* xx_create_animation_cord ( int screen_width, int screen_height, int n ) {
  xx_animation* r_value = (xx_animation*) malloc (sizeof(xx_animation) * 1);
  if ( r_value == NULL ) {
    printf ("\t Error! Memory Allocation, Failed, xx_animation* \n");
    return NULL;
  }

  r_value -> data_n = n; // default
  r_value -> n = 0;

  r_value -> screen_width = screen_width;
  r_value -> screen_height = screen_height;
  
  r_value -> data = (xx_animation_data**) malloc (sizeof(xx_animation_data*) * r_value -> data_n);
  if ( r_value -> data == NULL ) {
    printf ("\t Error! Memory Allocation, Failed, xx_animation* -> xx_animation_data** data \n");
    return NULL;
  }

  for ( int i = 0; i < r_value -> data_n; i++ ) {
    r_value -> data [i] = (xx_animation_data*) malloc (sizeof ( xx_animation_data ) * 2);

    if ( r_value -> data [i] == NULL ) {
      printf ("\t Error! Memory Allocation, Failed, xx_animation* -> xx_animation_data** data -> [%d]\n", i);
      return NULL;
    } 
  }

  return r_value;
}


/* 
 * function - void xx_free_animation_cord ( xx_animation* i_value )
 *  -- frees everything including the inputted arguments
 * */
void xx_free_animation_cord ( xx_animation* i_value ) {
  if ( i_value != NULL ) {
    for (int i = 0; i < i_value -> data_n; i++) {

      free ( i_value -> data [i] );
      i_value -> data [i] = NULL;
    }

    free ( i_value -> data );
    i_value -> data = NULL;

    free ( i_value );
    i_value = NULL;
  }
}


/* 
 * function - int xx_add_to_animation_cord ( xx_animation* i_value, int speed, int x, int y )
 *  -- function to add animation coordination to the arrray
 * */
int xx_add_to_animation_cord ( xx_animation* i_value, int speed, int x, int y ) {
  int return_value = -1; // failure

  if ( i_value -> n >= i_value -> data_n ) {
    printf ("\t Error! Max Number of Coordination (%d/%d) exceeeded\n", i_value -> data_n, i_value -> n);
    return_value = -1;
    return return_value;
  }

  i_value -> data [ i_value -> n ][0].speed = speed;
  i_value -> data [ i_value -> n ][0].x = x;
  i_value -> data [ i_value -> n ][0].y = y;
  i_value -> data [ i_value -> n ][0].is_on = 0;
  i_value -> data [ i_value -> n ][0].reverse = 0;

  i_value -> data [ i_value -> n ][1].speed = i_value -> data [ i_value -> n ][0].speed;
  i_value -> data [ i_value -> n ][1].x = i_value -> data [ i_value -> n ][0].x;
  i_value -> data [ i_value -> n ][1].y = i_value -> data [ i_value -> n ][0].y;
  i_value -> data [ i_value -> n ][1].is_on = i_value -> data [ i_value -> n ][0].is_on;
  i_value -> data [ i_value -> n ][1].reverse = i_value -> data [ i_value -> n ][0].reverse;

  i_value -> n += 1;

  return_value = i_value -> n - 1; // this will be the returning value

  return return_value;
}

/* 
 * function - void xx_remove_from_animation_cord ( xx_animation* i_value )
 *  -- just changes or decreases the cursor value
 * */
void xx_remove_from_animation_cord ( xx_animation* i_value ) {
  i_value -> n -= 1;
}

/* 
 * function - int xx_x_anime ( xx_animation* i_value, int i_n )
 *    -- this function will take in, the animation pointer and then, the curosr pointer (to know the items of which we are doing animation);
 *    -- will return the updated x coordination value
 * */
int xx_x_anime ( xx_animation* i_value, int i_n ) {
  return i_value -> data [i_n][1].x;
}

/* 
 * function - int xx_y_anime ( xx_animation* i_value, int i_n)
 *    -- this function wil return y coordination (for animation)
 * */
int xx_y_anime ( xx_animation* i_value, int i_n ) {
  // this function will strictly use index [1] of data 
  if ( i_value -> data [i_n][1].is_on ) {
    // update the coordination and return the value
    if ( i_value -> data [i_n][1].reverse ) {
      i_value -> data [i_n][1].y -= i_value -> data [i_n][1].speed;
    } else {
      i_value -> data [i_n][1].y += i_value -> data [i_n][1].speed;
    }

    if ( i_value -> data [i_n][1].y > i_value -> screen_height + 50 ) {
      i_value -> data [i_n][1].reverse = 1;
    } else if ( i_value -> data [i_n][1].y < i_value -> data [i_n][0].y )  {
      i_value -> data [i_n][1].reverse = 0;
      i_value -> data [i_n][1].is_on = 0;
      i_value -> data [i_n][1].y = i_value -> data [i_n][0].y;
    }
  }

  return i_value -> data [i_n][1].y;
}

/* 
 * function - int xx_set_anime_on ( xx_animation* i_value, int i_n )
 *    -- function to turn the animation on
 * */
int xx_set_anime_on ( xx_animation* i_value, int i_n ) {
  i_value -> data [i_n][1].is_on = 1;
  return i_value -> data [i_n][1].is_on;
}

/* 
 * function - void xx_update_animation ( xx_animation* i_value, int i_n, int* x, int* y );
 *    -- function to update the animation frames
 * */
void xx_update_animation ( xx_animation* i_value, int i_n, int* x, int* y ) {
  if ( i_value -> data [i_n][1].is_on ) {
    *x = xx_x_anime ( i_value, i_n );
    *y = xx_y_anime ( i_value, i_n );
  }
}

/* 
 * function - int xx_is_animation_on ( xx_animation* i_value, int i_n ) {
 * }
 * */
int xx_is_animation_on ( xx_animation* i_value, int i_n ) {
  return i_value -> data[i_n][1].is_on;
}
