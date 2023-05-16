#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int is_identifier(char*);
int is_operator(char*);
int is_keyword(char*);
int reverse_search(char*, char, int);
char* get_bracket_name(char);
int combine_and_check_for_operator(char, char);
char* bracketer(char*, char, char, int);
char fpeek(FILE *);
char* char_to_str(char);
char* two_char_to_str(char, char);

int main(){
    char ch, ch2, buffer[15] = {0};
    char* str_temp = NULL;
    int i = 0, index = 0;

    FILE* f_ptr, *f_out_ptr;
    f_ptr = fopen("code.psi", "r");
    f_out_ptr = fopen("code.lex", "w");

    if(f_ptr == NULL || f_out_ptr == NULL){
        printf("Error while opening the file...\n");
        exit(1);
    }
    while((ch = fgetc(f_ptr)) != EOF){
        if(isdigit(ch) && (isspace(fpeek(f_ptr)) || isdigit(fpeek(f_ptr)) )){
            str_temp = (char*) malloc(11);
            i = 0;
            str_temp[i] = ch;
            while ((ch = fgetc(f_ptr)) != EOF && i <= 10)
            {
                str_temp[++i] = ch;
                if(!isdigit(fpeek(f_ptr))){
                    str_temp[++i] = '\0';
                    break;
                }
            }
            if(i > 10){
                fprintf(f_out_ptr, "ERROR: INTEGER CONSTANT cannot be longer than 10 characters!\n");            
            }
            else if(ch == EOF){fprintf(f_out_ptr, "ERROR: END OF FILE.\n");}
            
            fprintf(f_out_ptr, "IntConst(%s)\n", str_temp);
            free(str_temp);
        }
        else if(isalnum(ch) || ch == '_'){
            buffer[index++] = ch;

            if(isalnum(fpeek(f_ptr))){}
            else if(fpeek(f_ptr) == '_'){}
            else{
                buffer[index] = '\0';
                index = 0;

                if(is_keyword(buffer)){
                    fprintf(f_out_ptr, "Keyword(%s)\n", buffer);
                i = -10;
                if((i = is_identifier(buffer))==1)
                    fprintf(f_out_ptr, "Identifier(%s)\n", strlwr(buffer));
                if(i == -1)
                    fprintf(f_out_ptr, "ERROR: <%s> is too long\n", buffer);
                if(i == -2)
                    fprintf(f_out_ptr, "ERROR: <%s> is not starting with alphabetic character\n", buffer);
                if(i == -3)
                    fprintf(f_out_ptr, "ERROR: <%s> is contains non-alphanumeric characters\n", buffer);    
                }
            }
        }
        else{
            
            if(ch == '"'){
                
            }
        }
    }
}







int is_identifier(char* str){
    char* p = NULL;
    // max 30 char  
    if (strlen(str) > 30){
        return -1;
    }
    str = strlwr(str); // lower case 
    // being a letter case
    if (str[0] > 'z' || str[0] < 'a'){
        return -2;
    }

    for (p = str; *p != '\0'; ++p){
        if(isalnum(*p)){}
        else if (*p == '_'){}
        else{return -3;}
    }
    return 1;
}

int is_operator(char* str){
    char operators[7][3] = {"+","-", "*", "/", "++", "--", ":="};
    int i, flag = 0;

    for(i=0; i<7; ++i){
        if(strcmp(operators[i], str) == 0){
            flag = 1;
            break;
        }
    }
    return flag;
}

int is_keyword(char* str){
    char keywords[18][10] = {"break","case","char","const","continue","do","else","enum",
                            "float","for","goto","if","int","long","record","return","static","while"};
    int i, flag = 0;

    for(i=0; i<18; ++i){
        if(strcmp(keywords[i], str) == 0){
            flag = 1;
            break;
        }
    }
    return flag;                        
}

int reverse_search(char* str, char chr, int offset){
    int length = strlen(str) - offset;
    int i = length;
    char* p = str;
    for(; i>=0; i--){
        if(*(p+i) == chr){
            return i;
        }
    }
    return -1;
}

char* get_bracket_name(char ch){
    if(ch == '('){return "LeftPar";}
    else if (ch == ')'){return "RightPar";}
    else if (ch == '['){return "Left Square Bracket";}
    else if (ch == ']'){return "Right Square Bracket";}
    else if (ch == '{'){return "Left Curly Bracket";} 
    else if (ch == '}'){return "Right Curly Bracket";}
    else{return NULL;}
}

int combine_and_check_for_operator(char char1, char char2){
    char* temp = two_char_to_str(chr1, chr2);
    int rv; 
    rv = is_operator(temp);
    free(temp);
    return rv;
}

char* bracketer(char *str, char srch, char srch_cls, int offset){
    int closing, opening;
    char* result; 
    int length = 0;
    closing = reverse_search(str, srch_cls, offset);
    if(closing >= 0){
        opening = reverse_search(str, srch, closing);
        if(opening >= 0){
            length = closing - opening - 1;
            result = (char*)malloc(length);
            memcpy(result, str + opening + 1, length);
            result[length] = '\0';
        }
        else{return NULL;}
    }
    return result;
}

char fpeek(FILE *stream){
    char c;

    c = fgetc(stream);
    ungetc(c, stream);

    return c;
}

char* char_to_str(char chr){
    char* temp = (char*) malloc(2);
    temp[0] = chr;
    temp[1] = '\0';
    return temp;
}

char* two_char_to_str(char chr1, char chr2){
    char* temp = (char*)malloc(sizeof(char) * 3);
    temp[0] = chr1;
    temp[1] = chr2;
    temp[2] = '\0';
    return temp;
}

