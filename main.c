#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int is_identifier(char*);
int is_operator(char*);
int is_keyword(char*);
int reverse_search(char*, char, int);
char* get_brckt_name(char);
int check_for_oprtr(char, char);
char* bracketer(char*, char, char, int);
char fpeek(FILE *);
char* one_chr_oprtr(char);
char* two_chr_oprtr(char, char);


int main(){
    char ch, ch2, buffer[15] = {0} ;
    char* str_temp = NULL;
    int i = 0, index = 0;
    FILE* f_ptr, *f_out_ptr;
    // Enter the code.psi and code.lex file paths on your computer
    f_ptr = fopen("path/code.psi","r");
    f_out_ptr = fopen("path/code.lex", "w");

    if(f_ptr == NULL || f_out_ptr == NULL){
        printf("Error while opening the file\n");
        exit(1);
    }
    // reading code.psi
    while((ch = fgetc(f_ptr)) != EOF){
        // controlling integer constants
        if(isdigit(ch) && (isspace(fpeek(f_ptr)) || isdigit(fpeek(f_ptr)) )){
            str_temp = (char*) malloc(11); // 10 digit integer + 1 null
            i = 0;
            str_temp[i] = ch;
            while((ch = fgetc(f_ptr)) != EOF && i <= 10){
                str_temp[++i] = ch;
                if(!isdigit(fpeek(f_ptr))){
                    str_temp[++i] = '\0';
                    break;
                }
            }
            if(i > 10){
                fprintf(f_out_ptr, "ERROR: Maximum integer size 10 digits!.\n");
            }else if(i < 0){
                fprintf(f_out_ptr, "ERROR: Negative integer not supported!\n");
            }
            else if(ch == EOF){
                fprintf(f_out_ptr, "ERROR: End Of File(EOF)!.\n");
            }
            if(i<10 && i > 0)
                fprintf(f_out_ptr, "IntConst(%s)\n", str_temp);
            free(str_temp);
        }
        //checking keyword or identifier
        else if(isalnum(ch) || ch == '_'){
            buffer[index++] = ch;

            if(isalnum(fpeek(f_ptr))){}
            else if (fpeek(f_ptr) == '_') {}
            else{
                buffer[index] = '\0';
                index = 0;
                // is any word is keyword, its keyword. if not, it may be identifer or not.
                if(is_keyword(buffer))
                    fprintf(f_out_ptr, "Keyword(%s)\n", buffer);
                else{
                    if((i = is_identifier(buffer)) == -1)
                        fprintf(f_out_ptr, "ERROR: All names must be lower case!\n");
                    if(i == 3)
                        fprintf(f_out_ptr, "Identifier(%s)\n", buffer);
                    if(i == 0)
                        fprintf(f_out_ptr, "ERROR: <%s> must be maxiumum 30 characters!\n", buffer);
                    if(i == 1)
                        fprintf(f_out_ptr, "ERROR: <%s> must start with an alphabetic character!\n", buffer);
                    if(i == 2)
                        fprintf(f_out_ptr, "ERROR: <%s> cannot contain non-alphanumeric characters!\n", buffer);
                }            
            }
        }
        // if ch is not alphanumeric
        else{
            // string
            if (ch == '"'){
                str_temp = (char*) malloc(216);
                i = 0;
                while((ch = fgetc(f_ptr)) != EOF){
                    str_temp[i++] = ch; // ch control done, so i++
                    if(fpeek(f_ptr) == '"') {
                        fgetc(f_ptr); 
                        str_temp[i] = '\0';
                        break;
                    }
                }

                if(ch == EOF){
                    //because of EOF, string not closed
                    fprintf(f_out_ptr, "ERROR: String not closed!\n");
                }
                fprintf(f_out_ptr, "StringConst(%s)\n", str_temp);
                free(str_temp);
            }
            // checkşing brackets
            else if(get_brckt_name(ch) != NULL) {
                fprintf(f_out_ptr, "%s\n", get_brckt_name(ch));
            }
            // checking comments
            else if(ch == '/' && fpeek(f_ptr) == '*'){

                printf("Comment starting...\n");
                fgetc(f_ptr); // waste the asterisk
                while((ch = fgetc(f_ptr)) != EOF){
                    if(ch == '*' && fpeek(f_ptr) == '/'){
                        fgetc(f_ptr); // waste the slash
                        break;
                    }
                }
                if(ch == EOF){
                    fprintf(f_out_ptr, "ERROR: Comment not closed!\n");
                }
                printf("Comment end...\n");
            }
            // checking operators
            else if(check_for_oprtr(ch, fpeek(f_ptr))){
                ch2 = fgetc(f_ptr); // waste one char
                fprintf(f_out_ptr, "Operator(%c%c)\n", ch, ch2);

            }
            else if(is_operator(one_chr_oprtr(ch))){
                fprintf(f_out_ptr, "Operator(%c)\n", ch);
            }
            // EOF
            else if(ch == ';'){
                fprintf(f_out_ptr, "EndOfLine!\n");
            }
        }
    }
    fclose(f_ptr);
    fclose(f_out_ptr);
    return 0;
}

int is_identifier(char* str){
    char* p = NULL;
    if(strlen(str) > 30){   // maximum 30 characters
        return 0;
    }
    // not alphabetic
    if(str[0] > 'z' || str[0] < 'a'){
        if (str[0] >= 'A' && str[0] <= 'Z'){
            return -1;
        }
        else{
            return 1;
        }
        } 
    for(p = str; *p != '\0'; ++p){ 
       if (isalnum(*p) || *p == '_') {} // alphanumerical
       else {return 2;} // not alphanumeric
    }
    return 3;
}

int is_operator(char* str){
    char operators[7][3] = {"+","-","*","/","++","--",":="}; // acceptable operators
    int i, flag = 0;

    for(i = 0; i <7; ++i){
        if(strcmp(operators[i], str) == 0){
            flag = 1;
            break;
        }
    }
    return flag;
}

int is_keyword(char* str){
    //acceptable keywords
    char keywords[18][10] = {"break","case","char","const","continue","do","else","enum",
                            "float","for","goto","if","int","long","record","return","static","while"};
    int i, flag = 0;

    for(i = 0; i < 18; ++i){
        if(strcmp(keywords[i], str) == 0){ 
            flag = 1;
            break;
        }
    }
    return flag;
}

int reverse_search(char* str, char chr, int offset){
    int len = strlen(str) - offset;
    char* p = str;
    for(int i = len; i >= 0; i--){
        if(*(p+1) == chr){
            return i;
        }
    }
    return -1;
}

char* get_brckt_name(char ch){
    if(ch == '('){return "LeftPar";}
    else if(ch == ')'){return "RightPar";}
    else if(ch == '['){return "Left_Square";}
    else if(ch == ']'){return "Right_Square";}
    else if(ch == '{'){return "Left_Curly";}
    else if(ch == '}'){return "Right_Curly";}
    else{return NULL;}
}

int check_for_oprtr(char chr1, char chr2){
    char* str = two_chr_oprtr(chr1, chr2);
    int isOperator = is_operator(str);
    free(str);
    return isOperator;
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

    c = fgetc(stream); // read a character from file
    ungetc(c, stream); // relocate character

    return c; // return character after read
}

char* one_chr_oprtr(char chr){
    char* temp = (char*)malloc(2); // allocate 2 byte at memory
    temp[0] = chr; // assign to 1st byte
    temp[1] = '\0'; // assign null character to end
    return temp; // return characters
}

char* two_chr_oprtr(char chr1, char chr2){
    char* temp = (char*) malloc(sizeof(char) * 3);
    temp[0] = chr1;
    temp[1] = chr2;
    temp[2] = '\0';
    return temp;
}
