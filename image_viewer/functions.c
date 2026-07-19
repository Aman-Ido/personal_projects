// function declarations
int iv_init_lib (); // initializes libraries (SDL and SDL_image)
struct Bundle* iv_init_bundle (); // allocates memory to the bundle 
int iv_allocate_directory ( struct Bundle* bun ); // for allocating and decising the path for home also, directory and entry

int iv_event ( struct Bundle* bun ); // this if the main function - main loop
int iv_allocate_list ( struct Bundle* bun ); // this function will allocate memory to the list, by counting the number of pictures there is
int iv_load_image ( struct Bundle* bun ); // this function will load the image 
void iv_free_image ( struct Bundle* bun ); // this is just to remove the texture, though free_bundle will destroy it too

void iv_free_bundle ( struct Bundle* bun ); // frees memory allocated by the init_bundle function
void iv_free_lib (); // quits the libraries

// function definitions

/* 
 * initializes sdl and sdl_image
 * */
int iv_init_lib () {
  if ( SDL_Init ( SDL_INIT_VIDEO ) < 0 ) {
    fprintf ( stderr, "SDL_Init () : %s\n", SDL_GetError() );
    return 0;
  }

  int image_flag = IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF | IMG_INIT_WEBP;
  if ( (IMG_Init ( image_flag ) & image_flag) < 0 ) {
    fprintf (stderr, "IMG_Init () : %s\n", IMG_GetError());
    return 0;
  }

  return 1;
}

/* 
 * this functin just allocates stuff of the bundle 
 * */
struct Bundle* iv_init_bundle () {
  // allocating memory to the bundle
  struct Bundle* bun = (struct Bundle*) calloc (sizeof(struct Bundle), 1);
  if ( bun == NULL ) {
    fprintf (stderr, "Memory Allocation, failed, struct Bundle* bun\n");
    return NULL;
  }

  bun -> window = NULL;
  bun -> font = NULL;
  bun -> directory = NULL;
  bun -> home_path = NULL;
  bun -> entry = NULL;
  bun -> full_path = NULL;
  bun -> full_path_size = STRING_SIZE;
  bun -> path_record = NULL;
  bun -> quit = 0;
  bun -> m_texture = NULL;
  bun -> image_width = 0;
  bun -> image_height = 0;
  bun -> window_width = 0;
  bun -> window_height = 0;
  bun -> image_index = 0;
  bun -> max_image_index = 0;
  bun -> rendering_rect.x = 0;
  bun -> rendering_rect.y = 0;
  
  // allocating necessary memory 
  // creating window and renderer
  bun -> window = xx_create_window_renderer ( WINDOW_WIDTH, WINDOW_HEIGHT, (char*) "Image Viewer" );
  if ( bun -> window == NULL ) {
    fprintf (stderr, "xx_create_window_renderer () - failed\n");
    return NULL;
  }

  bun -> font = bm_create ();
  bm_set_zoom ( bun -> font, -48 );
  bm_load ( bun -> window -> renderer, bun -> font, (char*) "Font.png" );

  if ( bun -> font == NULL ) {
    fprintf ( stderr, "bm_create () - failed\n" );
    return NULL;
  }

  // for the directories
  //allocating memory for the full_path 
  bun -> full_path = (char*) calloc (sizeof(char), bun -> full_path_size);
  if ( bun -> full_path == NULL ) {
    fprintf ( stderr, "Memory Allocation, Failed, bun -> full_path\n" );
    return NULL;
  }

  if ( iv_allocate_directory ( bun ) < 0 ) {
    fprintf (stderr, "iv_allocate_directory() - error\n");
    return NULL;
  }

  return bun;
}

/* 
 * this function will do the necessary stuff for directory
 * */
int iv_allocate_directory ( struct Bundle* bun ) {
  // finding the home directory
  // presuming that this program is only made for linux, thus, I ain't checking for windows headers
  bun -> home_path = getenv ("HOME");
  if ( bun -> home_path == NULL ) {
    fprintf (stderr, "Could not get the home directory path\n");
    return 0;
  }

  // creating a full path to the pictures directory
  snprintf ( bun -> full_path, bun -> full_path_size, "%s/Pictures", bun -> home_path );
  if ( bun -> full_path[0] == '\0' ) {
    fprintf (stderr, "Could not set the full path\n");
    return 0;
  } else {
    fprintf ( stdout, "Full Path -> %s\n", bun -> full_path );
  }

  // opening the directory to the full path
  bun -> directory = opendir ( bun -> full_path );
  if ( bun -> directory == NULL ) {
    fprintf (stderr, "Could not open directory\n");
    return 0;
  }
  
  return 1;
}

/* 
 * this function will free everything of the bundle 
 * */
void iv_free_bundle ( struct Bundle* bun ) {
  if ( bun -> full_path != NULL ) {
    free ( bun -> full_path );
    bun -> full_path = NULL;
    bun -> full_path_size = 0;
  }

  iv_free_image ( bun );

  if ( bun -> directory != NULL ) {
    closedir ( bun -> directory );
    bun -> directory = NULL;
  }

  if ( bun -> font != NULL ) {
    bm_free ( bun -> font);
    bun -> font = NULL;
  }

  if ( bun -> window != NULL ) {
    xx_destroy_window_renderer ( bun -> window );
    bun -> window = NULL;
  }

  if ( bun -> path_record != NULL  ) {
    xx_free_list ( bun -> path_record );
    bun -> path_record = NULL;
  }

  if ( bun != NULL ) {
    free ( bun );
    bun = NULL;
  }
}

/* 
 * this function is to close the libraries, sdl and sdl_image
 * */
void iv_free_lib () {
  IMG_Quit ();
  SDL_Quit ();
}

/* 
 * this function will store the information / path names inside of a string (list)
 * */
int iv_allocate_list ( struct Bundle* bun ) {

  int index = 0;
  rewinddir ( bun -> directory );
  while ( (bun -> entry = readdir ( bun -> directory )) != NULL ) {
    if ( bun -> entry -> d_name[0] != '.' ) {
      index += 1;
    }
  }

  // allocating memory to the list
  fprintf (stdout, "There are %d Images\n", index);
  bun -> path_record = xx_create_list ( index, STRING_SIZE );
  if ( bun -> path_record == NULL ) {
    fprintf (stderr, "xx_create_list (int, int) - failed\n");
    return 0;
  }

  char full_path [STRING_SIZE];

  rewinddir ( bun -> directory );
  while ( (bun -> entry = readdir ( bun -> directory )) != NULL ) {
    if ( bun -> entry -> d_name[0] != '.' ) {
      snprintf (full_path, STRING_SIZE, "%s/%s", bun -> full_path, bun -> entry -> d_name);
      // checking if the file is a folder or something else
      if ( stat ( full_path, &bun -> file_stat ) == 0 ) {
        if ( S_ISREG ( bun -> file_stat.st_mode )  ) {
          // adding to the list - if regular and is not . or ..
          xx_add_list ( bun -> path_record, full_path );
        }
      }
    }
  }

  bun -> max_image_index = bun -> path_record -> n;

  return 1;
}

/* 
 * this function will load the image to the texture using SDL_image
 * */
int iv_load_image ( struct Bundle* bun ) {
  // freeing the image if it is already loaded
  iv_free_image ( bun );

  // loading the image
  bun -> m_texture = IMG_LoadTexture ( bun -> window -> renderer, xx_get_list_n ( bun -> path_record, bun -> image_index ) );
  if ( bun -> m_texture == NULL ) {
    // TODO: if we don't find the image, we will just show some default image, but for now this will do
    fprintf (stderr, "Could not load image : %s\n ", IMG_GetError());
    return 0;
  }

  // querying the width and height of the image
  SDL_QueryTexture ( bun -> m_texture, NULL, NULL, &bun -> image_width, &bun -> image_height );
  bun -> window_width = bun -> image_width;
  bun -> window_height = bun -> image_height;

  // changing the title of the window 
  SDL_SetWindowTitle ( bun -> window -> window, xx_get_list_n ( bun -> path_record, bun -> image_index ) );
  // changing the window width and height
  SDL_SetWindowSize ( bun -> window -> window, bun -> window_width, bun -> window_height );

  bun -> rendering_rect.w = bun -> window_width;
  bun -> rendering_rect.h = bun -> window_height;

  return 1;
}

/* 
 * this function destroyes the texture only and also sets the width and height 
 * */
void iv_free_image ( struct Bundle* bun ) {
  if ( bun -> m_texture != NULL ) {
    SDL_DestroyTexture ( bun -> m_texture );
    bun -> m_texture = NULL;
    bun -> image_width = 0;
    bun -> image_height = 0;
  }
}

/* 
 * this function is the main loop
 * */
int iv_event ( struct Bundle* bun ) {
  SDL_Event event;
  SDL_Point mouse_pos; // for mouse position

  if ( iv_allocate_list ( bun ) != 1 ) {
    fprintf ( stderr, "iv_allocate_list (bun) - failed\n" );
    return 0;
  }

  if ( iv_load_image ( bun ) != 1 ) {
    fprintf (stderr, "iv_load_image (bun) - failed\n");
    return 0;
  }


  while (!bun -> quit) {
    while (SDL_PollEvent (&event)) {
      if ( event.type == SDL_QUIT ) {
        bun -> quit = 1;
      }
      if ( event.type == SDL_MOUSEBUTTONDOWN ) {
        SDL_GetMouseState ( &mouse_pos.x, &mouse_pos.y );
        if ( mouse_pos.x < bun -> window_width / 2 ) {
          bun -> image_index -= 1;
          if ( bun -> image_index < 0 ) {
            bun -> image_index = bun -> max_image_index - 1;
          }
          // loading the image
          iv_load_image (bun);
        } else if ( mouse_pos.x > bun -> window_width / 2 ) {
          bun -> image_index += 1;
          if ( bun -> image_index => bun -> max_image_index ) {
            bun -> image_index = 0;
          }
          // loading thep icture
          iv_load_image ( bun );
        }
      }
    }
    SDL_SetRenderDrawColor ( bun -> window -> renderer, 255, 255, 255, 255 );
    SDL_RenderClear ( bun -> window -> renderer );

    // displaying the picture
    SDL_RenderCopy ( bun -> window -> renderer, bun -> m_texture, NULL, &bun -> rendering_rect );

    SDL_RenderPresent ( bun -> window -> renderer );
  }
  return 1;
}

