#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int is_identifier(char*);


int is_identifier(char* str){
    char* p = NULL;
    if (strlen(str) > 30){
        return -1;
    }
    str = strlwr(str);
    if (str[0] > 'z' || str[0] < 'a'){
        return -2;
    }
    


}