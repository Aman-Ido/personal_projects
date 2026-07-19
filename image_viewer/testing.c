#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include "xx_gui/xx_string.h"
#include "xx_gui/xx_list.h"

#define STRING_SIZE 20

struct Bundle {
  xx_list* my_list;
  DIR* directory;
  struct dirent* entry;
  struct stat* fileStat;
};

int main () {
  struct Bundle* bun = (struct Bundle*) calloc (sizeof(struct Bundle), 1);
  if ( bun == NULL ) {
    fprintf (stderr, "Could not allocate memory - to bundle manager\n");
    return -1;
  }

  bun -> fileStat = (struct stat*) calloc (sizeof(struct stat), 1);

  bun -> directory = opendir (".");
  if ( bun -> directory == NULL ) {
    fprintf (stderr, "Could not open directory\n");
    return -1;
  }

  int count = 0;
  while ( (bun -> entry = readdir ( bun -> directory )) != NULL ) {
    if ( bun -> entry -> d_name[0] != '.' ) {
      count += 1;
    }
  }

  fprintf (stdout, "Count - %d\n", count);
  bun -> my_list = xx_create_list ( count, STRING_SIZE );
  if ( bun -> my_list == NULL ) {
    fprintf ( stderr, "Could not allocate memory - my_list\n" );
    return -1;
  }

  rewinddir ( bun -> directory );
  while ( (bun -> entry = readdir ( bun -> directory )) != NULL ) {
    if ( bun -> entry -> d_name[0] != '.' ) {
      if ( stat ( bun -> entry -> d_name, bun -> fileStat ) == 0 ) {
        if ( S_ISREG ( bun -> fileStat -> st_mode ) ) {
          xx_add_list ( bun -> my_list, bun -> entry -> d_name );
        }
      }
    }
  }


  fprintf (stdout, "The program is reading here\n");
  xx_print_list ( bun -> my_list );

  free ( bun -> fileStat );
  xx_free_list ( bun -> my_list );
  closedir ( bun -> directory );

  free ( bun );

  return 0;

}

