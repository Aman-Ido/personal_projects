/* 
 * this file will have functions to show version of different libraries of sdl, and gcc and g++k
 * */
void xx_print_version_of_c () {
  printf ("GNU C Version %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
}

#if defined(__GNUG__)
void xx_print_version_of_cpp () {
  int array_size = 6;
  const int version_array[array_size] = {199711, 201103, 201402, 201703, 202002, 202302};
  const char version_string [array_size][15] = {"C++98/C++03", "C++11", "C++14", "C++17", "C++20", "C++23"};
  int cpp_version = __cplusplus;
  int cursor = 0;
  
  if ( cpp_version <= version_array [0] ) {
    printf ("GNU C++ Version %s (%d)\n", version_string [cursor], version_array[cursor]);
  } else if ( cpp_version <= version_array[1] ) {
    cursor = 1;
    printf ("GNU C++ Version %s (%d)\n", version_string[cursor], version_array[cursor]);
  } else if ( cpp_version <= version_array[2] ) {
    cursor = 2;
    printf ("GNU C++ Version %s (%d)\n", version_string[cursor], version_array[cursor]);
  } else if ( cpp_version <= version_array[3] ) {
    cursor = 3;
    printf ("GNU C++ Version %s (%d)\n", version_string[cursor], version_array[cursor]);
  } else if ( cpp_version <= version_array[4] ) {
    cursor = 4;
    printf ("GNU C++ Version %s (%d)\n", version_string[cursor], version_array[cursor]);
  } else if ( cpp_version <= version_array[5] ) {
    cursor = 5;
    printf ("GNU C++ Version %s (%d)\n", version_string[cursor], version_array[cursor]);
  }
}
#endif


#if !defined(SDL_MAJOR_VERSION)
#elif defined(SDL_MAJOR_VERSION)
void xx_print_version_of_sdl () {
  SDL_version ver;
  SDL_GetVersion (&ver);
  printf ("SDL Version %d.%d.%d\n", ver.major, ver.minor, ver.patch);
}
#endif

#if !defined(SDL_IMAGE_MAJOR_VERSION)
#elif defined(SDL_IMAGE_MAJOR_VERSION)
void xx_print_version_of_sdl_image () {
  SDL_version ver;
  SDL_IMAGE_VERSION (&ver);
  printf ("SDL_image Version %d.%d.%d\n", ver.major, ver.minor, ver.patch);
}
#endif

#if !defined(SDL_MIXER_MAJOR_VERSION)
#elif defined(SDL_MIXER_MAJOR_VERSION)
void xx_print_version_of_sdl_mixer () {
  SDL_version ver;
  SDL_MIXER_VERSION (&ver);
  printf ("SDL_mixer Version %d.%d.%d\n", ver.major, ver.minor, ver.patch);
}
#endif

#if !defined(SDL_TTF_MAJOR_VERSION)
#elif defined(SDL_TTF_MAJOR_VERSION)
void xx_print_version_of_sdl_ttf () {
  SDL_version ver;
  SDL_TTF_VERSION (&ver);
  printf ("SDL_ttf Version %d.%d.%d\n", ver.major, ver.minor, ver.patch);
}
#endif

#if !defined(SDL_NET_MAJOR_VERSION)
#elif defined(SDL_NET_MAJOR_VERSION)
void xx_print_version_of_sdl_net () {
  SDL_version ver;
  SDL_NET_VERSION (&ver);
  printf ("SDL_net Version %d.%d.%d\n", ver.major, ver.minor, ver.patch);
}
#endif
