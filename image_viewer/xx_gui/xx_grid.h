/* 
 * so I think this will be my final try to make a grid system
 * I have decided to make the grid system kind of like a how do I put it? 
 *      -- I will only take row as input from the user and the column will be automatically filled.
 * */

// this struct is for units, meaning individual elements, we just store their information in this 
typedef struct {
  int x; int y; // the x and y which we will fill during add and return when the user uses xx_get_x 
  int width; int height; // the width and height of that element 
  int row; int col; // the row and column information of that elements 
  int id; // the id - just the cursor at the time of recording
} xx_grid_unit_info;

// for row this are the essential ones 
typedef struct {
  int elements; // for number of elements, can be called col
  int curr_x; // for the current position in the row
  int curr_height; // for the current highest vlaue in the row

  int* store_index; // array to store the id of elements on its own row
} xx_grid_row_info;


// the main struct which we are going  to use 
typedef struct {
  int unit_n; // just like item n ( is the cursor )
  int max_unit_n; // this is for the max number of elements ( for the max number of elements )

  xx_grid_row_info* row_info; // for storing row information
  xx_grid_unit_info* unit_info; // for storing unit information 

  int* padding; // for padding ( 0 - horizontal, 1 - vertical )

  int XX_SCREEN_WIDTH; // for storing screen width and height
  int XX_SCREEN_HEIGHT;

  int total_elements; // this will indicate the number of total elements that can be stored, not just the same number as the row number or size
  // placeholder for slider
} xx_grid;

/* 
 * funciton - declarations
 * */
xx_grid* xx_create_grid ( int screen_width, int screen_height, int total_number_of_units, size_t all_units ); // creating the xx_grid pointer nad returns it
int xx_add_to_grid ( xx_grid* i_grid, size_t row, size_t width, size_t height ); // adds an element to the grid
void xx_free_grid ( xx_grid* i_grid ); // frees everything of the grid (not tested yet)
int xx_get_x ( xx_grid* i_grid, int n ); // this one returns the 'x' coordinate
int xx_get_y ( xx_grid* i_grid, int n );// this one returns the 'y' coordinate

// just some printing functions
void xx_show_row_info_grid_n ( xx_grid* i_grid, int n ); // show information of a particular row 
void xx_show_unit_info_grid_n ( xx_grid* i_grid, int n ); // show information of a particular unit (cursor)
void xx_print_grid_info ( xx_grid* i_grid ); // show information of everything in side of the xx_grid

// functions to get 
int xx_get_total_number_of_grid ( xx_grid* i_grid ); // this will return the number of grids
int xx_get_total_number_of_elements ( xx_grid* i_grid ); // this will return the total number of units or elements 
int xx_get_current_number_of_elements ( xx_grid* i_grid ); // this will return the current cursor or index of elements

/* 
 * funciton - definitions
 * */
xx_grid* xx_create_grid ( int screen_width, int screen_height, int total_number_of_units, size_t all_units ) {

  // allocating memory to xx_grid
  xx_grid* r_grid = (xx_grid*) malloc (sizeof(xx_grid) * 1);
  if ( r_grid == NULL ) {
    printf ("\t Error! Memory Allocation, Failed, xx_grid* r_grid \n");
    return NULL;
  }

  r_grid -> max_unit_n = 0;
  r_grid -> padding = NULL;
  r_grid -> unit_info = NULL;
  r_grid -> row_info = NULL;
  r_grid -> total_elements = 0;

  // checking if the max number of units is less than 0, if yes we will convert it to positive number
  if ( total_number_of_units < 0 ) {
    total_number_of_units *= -1;
  } else {
    // if no, then we will just assign the value
    r_grid -> max_unit_n = total_number_of_units;
  }

  r_grid -> total_elements = all_units;

  r_grid -> unit_n = 0; // the cursor
  r_grid -> XX_SCREEN_WIDTH = screen_width;
  r_grid -> XX_SCREEN_HEIGHT = screen_height;
  // allocating memory for the padding
  r_grid -> padding = (int*) malloc (sizeof(int) * 2);
  if ( r_grid -> padding == NULL ) {
    printf ("\nError! Memory Allocation, Failed, xx_grid* r_grid -> int* padding \n");
    return NULL;
  }

  // putting in the default value for padding 
  r_grid -> padding [0] = 5;
  r_grid -> padding [1] = 5;

  // allocating memory to the structs as per the max user input
  // TODO: here we allocate the same number of units as same number of row, which is in sufficient, thus in future I will make another arguments just for this
  r_grid -> unit_info = (xx_grid_unit_info*) calloc (r_grid -> total_elements, sizeof(xx_grid_unit_info));
  if ( r_grid -> unit_info == NULL ) {
    printf ("\t Error! Memory Allocation, Failed, xx_grid* r_grid -> xx_grid_unit_info* unit_info\n ");
    return NULL;
  }

  // allocating memory to the structs (r_grid -> row_info) as per the max user input
  r_grid -> row_info = (xx_grid_row_info*) calloc (r_grid -> max_unit_n, sizeof(xx_grid_row_info));
  if ( r_grid -> row_info == NULL ) {
    printf ("\t Error! Memory Allocation, Failed, xx_grid* r_grid -> xx_grid_row_info* row_info\n ");
    return NULL;
  }

  // filling in values for row 
  for ( int i = 0; i < r_grid -> max_unit_n; i++ ) {
    r_grid -> row_info [i].store_index = (int*) calloc (r_grid -> total_elements, sizeof(int));
    if ( r_grid -> row_info [i].store_index == NULL) {
      printf ("\t Error! Memory Allocation, Failed, xx_grid* r_grid -> xx_grid_row_info* row_info -> int* store_index\n");
    }
  }


  return r_grid;
}

/* 
 * function - int xx_add_to_grid ( xx_grid* i_grid, size_t row, size_t width, size_t height );
 *    -- this function will add the elements to the grid 
 * */
int xx_add_to_grid ( xx_grid* i_grid, size_t row, size_t width, size_t height ) {

  if ( i_grid -> row_info [row].elements == 0 ) {
    // now updating the real unit's data 
    i_grid -> unit_info [i_grid -> unit_n].x = i_grid -> padding [0];
    i_grid -> row_info [row].curr_x += width + i_grid -> padding [0];
  } else {
    i_grid -> unit_info [i_grid -> unit_n].x = i_grid -> row_info [row].curr_x;
    i_grid -> row_info [row].curr_x += width + i_grid -> padding [0];
  }

  
  if ( i_grid -> row_info [row].curr_height < height ) {
    i_grid -> row_info [row].curr_height = height + i_grid -> padding [1];
  }


  if ( row == 0 ) {
    i_grid -> unit_info [i_grid -> unit_n].y = i_grid -> padding[1];
  } else {
    int result = 0;
    // i_grid -> unit_info [i_grid -> unit_n].y = i_grid -> row_info [row - 1].curr_height;
    for (int i = 0; i < row; i++) {
      result += i_grid -> row_info [i].curr_height;
    }
    i_grid -> unit_info [i_grid -> unit_n].y = result;
  }

  i_grid -> unit_info [i_grid -> unit_n].width = width;
  i_grid -> unit_info [i_grid -> unit_n].height = height;
  i_grid -> unit_info [i_grid -> unit_n].row = row;
  i_grid -> unit_info [i_grid -> unit_n].col = i_grid -> row_info [row].elements;

  i_grid -> unit_info [i_grid -> unit_n].id = i_grid -> unit_n;

  // storing the index on the row's store_index variable
  i_grid -> row_info [row].store_index [ i_grid -> row_info[row].elements ] = i_grid -> unit_info [i_grid -> unit_n].id;

  i_grid -> unit_n += 1;
  i_grid -> row_info [row].elements += 1; // registers that there has been an element added to that row
  return i_grid -> unit_n - 1;
}

/* 
 * function - void xx_show_unit_info_grid_n ( xx_grid* i_grid, int n )
 *    --  just prints out the information of that unit
 *    -- shows the information as per the n (meaning cursor) rather than row value
 * */
void xx_show_unit_info_grid_n ( xx_grid* i_grid, int n ) {
  printf ("\t *** Information of the Cursor - %d *** \n", n);
  printf ("Position: {%d, %d}\n", i_grid -> unit_info [n].x, i_grid -> unit_info[n].y);
  printf ("Proportion: {%d, %d}\n", i_grid -> unit_info [n].width, i_grid -> unit_info[n].height);
  printf ("Grid Value (Row, Col): {%d, %d}\n", i_grid -> unit_info [n].row, i_grid -> unit_info[n].col);
  printf ("Id (Just the Cursor): %d\n", i_grid -> unit_info[n].id );
}

/* 
 * function - void xx_show_row_info_grid_n ( xx_grid* i_grid, int n )
 *    -- just prints out the information of that row (inputted n)
 * */
void xx_show_row_info_grid_n ( xx_grid* i_grid, int n ) {
  printf ("\t **** Information of the Row - %d ***\n");
  printf ("Total Number of Elements : %d\n", i_grid -> row_info [n].elements);
  printf ("Current X Position in row (%d) : %d\n", n, i_grid -> row_info [n].curr_x);
  printf ("Current Max Height in row (%d) : %d\n", n, i_grid -> row_info [n].curr_height);
  printf ("Id of Elements which belong to row (%d): \n", n);
  for (int i = 0; i < i_grid -> row_info[n].elements; i++) {
    printf ("\t %d ", i_grid -> row_info[n].store_index[i]);
  }
  printf ("\n");
}

/* 
 * function - void xx_free_grid ( xx_grid* i_grid )
 *    -- this function will free everything of the grid along with it self
 * */
void xx_free_grid ( xx_grid* i_grid ) {
  if ( i_grid != NULL ) {
    // freeing the padding 
    free ( i_grid -> padding );
    i_grid -> padding = NULL;

    for (int i = 0; i < i_grid -> max_unit_n; i++) {
      free ( i_grid -> row_info [i].store_index );
      i_grid -> row_info [i].store_index = NULL;
    }

    // freeing the row_info 
    free ( i_grid -> row_info );
    i_grid -> row_info = NULL;

    free ( i_grid -> unit_info );
    i_grid -> unit_info = NULL;

    free ( i_grid );
    i_grid = NULL;

  }
}

/* 
 * function - int xx_get_x ( xx_grid* i_grid, int n );
 *    -- this function will return the x coordination of the n (cursor) index
 * */
int xx_get_x ( xx_grid* i_grid, int n ) {
  return i_grid -> unit_info [n].x;
}

/* 
 * function - int xx_get_y ( xx_grid* i_grid, int n );
 *    -- this function will return the y coordination of the n (cursor) index
 * */
int xx_get_y ( xx_grid* i_grid, int n ) {
  return i_grid -> unit_info [n].y;
}

/* 
 * function - void xx_print_grid_info ( xx_grid* i_grid );
 *    -- this function will print everything of the grid from the array to the unit things
 * */
void xx_print_grid_info ( xx_grid* i_grid ) {
  printf ("***** Main Information *****\n");
  printf ("\t ** Infomation - Units **\n");
  printf ("\t Units : %d/%d (Max)\n", i_grid -> unit_n, i_grid -> total_elements );
  printf ("\t\t (id)\t-\tx\ty\tw\th\tro\tco\t\n");
  for (int i = 0; i < i_grid -> unit_n; i++) {
    printf ("\t\t (%d)\t-\t%d\t%d\t%d\t%d\t%d\t%d\t\n", 
        i_grid -> unit_info[i].id,
        i_grid -> unit_info[i].x,
        i_grid -> unit_info[i].y,
        i_grid -> unit_info[i].width,
        i_grid -> unit_info[i].height,
        i_grid -> unit_info[i].row,
        i_grid -> unit_info[i].col
        );
  }
  printf ("\n\n");
  //printing out information about row 
  printf ("\t ** Information - Row **\n");
  for (int i = 0; i < i_grid -> max_unit_n; i++) {
    printf ("\t Row - %d\n", i);
    printf ("\t Units : %d/%d (Max)\n", i_grid -> row_info [i].elements, i_grid -> total_elements);
    printf ("\t Current X (pixels) : %d\n", i_grid -> row_info[i].curr_x);
    printf ("\t Max Height of This row (pixels) : %d\n", i_grid -> row_info[i].curr_height);
    printf ("\t Index : ");
    for (int j = 0; j < i_grid -> row_info[i].elements;j++) {
      printf ("\t%d\t", i_grid -> row_info[i].store_index[j]);
    }
    printf ("\n");
  }

  printf ("\n\n");
  printf ("\t ** Information - Grid System **\n");
  printf ("\t Size of Grid (The Grid) : %d\n", sizeof(xx_grid));
  int result = 0;

  // this result = 
  /* 
   * 0 -> sizeof xx_grid struct 
   * 1 -> size of xx_grid_row_info times the number of row 
   * 2 -> size of xx_grid_unit_info times the number of elements 
   * 3 -> size of integer (4) times the number of elements times the number of row (because each row has this array, and every array is allocated to the number of elements)
   * TODO: number 3 is inefficient because it takes up alot of memory even when not in use, Please update it
   * */
  result = sizeof(xx_grid) + i_grid -> max_unit_n * sizeof(xx_grid_row_info) + ( i_grid -> total_elements * sizeof(xx_grid_unit_info) ) + (( i_grid -> total_elements * sizeof(int) ) * i_grid -> max_unit_n);
  printf ("\t Size of This Grid : %ld\n", result);
  printf ("\t Padding (Horizontal, Vertical) - (%d, %d)\n", i_grid -> padding[0], i_grid -> padding[1]);
  printf ("\t Stored (Screen_Width, Screen_Height) - (%d, %d)\n", i_grid -> XX_SCREEN_WIDTH, i_grid -> XX_SCREEN_HEIGHT);
  printf ("\t **** \n");
}

/*
 * some getters function 
 * */

int xx_get_total_number_of_elements ( xx_grid* i_grid ) {
  return i_grid -> total_elements;
}

int xx_get_total_number_of_grid ( xx_grid* i_grid ) {
  return i_grid -> max_unit_n;
}

int xx_get_current_number_of_elements ( xx_grid* i_grid ) {
  return i_grid -> unit_n;
}
