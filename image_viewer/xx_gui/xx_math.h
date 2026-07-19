#define XX_MATH 0x01


void xx_itoa ( char* i_string, int value ); // converts integer to ascii character 

void xx_reverse_string ( char* i_string ) {
  int str_size = xx_strlen ( i_string );

  char temp_string [str_size + 1];

  int j = str_size;
  for (int i = 0; i < str_size; i++) {
    temp_string [i] = i_string [j - 1];
    j--;
  }
  temp_string [str_size] = '\0';

  xx_strcpy ( i_string, temp_string );
}

void xx_itoa ( char* i_string, int value ) {
  int is_negative = value < 0;

  xx_abs ( value ); // creating absolute 

  int l_value = value;
  int var_2 = l_value;


  int i = 0;
  for (i; var_2 > 9; i++) {
    l_value = var_2;
    l_value %= 10;
    var_2 /= 10;

    i_string[i] = l_value + 48;
  }
  i_string [i] = var_2 + 48;
  if ( is_negative ) {
    i_string [i + 1] = '-';
    i_string [i + 2] = '\0';
  } else {
    i_string [i + 1] = '\0';
  }

  


  xx_reverse_string ( i_string );

}
