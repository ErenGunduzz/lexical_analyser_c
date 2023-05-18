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
    char ch, ch2, buffer[15] = {0};
    char* str_temp = NULL;
    int i = 0, index = 0;

    FILE* file_ptr, *file_out_ptr;
    file_ptr = fopen("//home//cicikov//Desktop//lexical_analyser_c//code.psi", "r");
    file_out_ptr = fopen("//home//cicikov//Desktop//lexical_analyser_c//code.lex", "w");

    if(file_ptr == NULL || file_out_ptr == NULL){
        printf("Error while opening the file...\n");
        exit(1);
    }
    while((ch = fgetc(file_ptr)) != EOF){
        if(isdigit(ch) && (isspace(fpeek(file_ptr)) || isdigit(fpeek(file_ptr)) )){
            str_temp = (char*) malloc(11);
            i = 0;
            str_temp[i] = ch;
            while ((ch = fgetc(file_ptr)) != EOF && i <= 10)
            {
                str_temp[++i] = ch;
                if(!isdigit(fpeek(file_ptr))){
                    str_temp[++i] = '\0';
                    break;
                }
            }
            if(i > 10){
                fprintf(file_out_ptr, "ERROR: INTEGER CONSTANT cannot be longer than 10 characters!\n");            
            }
            else if(ch == EOF){fprintf(file_out_ptr, "ERROR: END OF FILE.\n");}
            
            fprintf(file_out_ptr, "IntConst(%s)\n", str_temp);
            free(str_temp);
        }
        else if(isalnum(ch) || ch == '_'){
            buffer[index++] = ch;

            if(isalnum(fpeek(file_ptr))){}
            else if(fpeek(file_ptr) == '_'){}
            else{
                buffer[index] = '\0';
                index = 0;

                if(is_keyword(buffer)){
                    fprintf(file_out_ptr, "Keyword(%s)\n", buffer);
                if((i = is_identifier(buffer)) == 0)
                    fprintf(file_out_ptr, "ERROR: All names must be lower case!\n");
                if((i = is_identifier(buffer)) == 7)
                    fprintf(file_out_ptr, "Identifier(%s)\n", buffer);
                if(i == -1)
                    fprintf(file_out_ptr, "ERROR: <%s> must be maximum 30 characters\n", buffer);
                if(i == -2)
                    fprintf(file_out_ptr, "ERROR: <%s> must start with alphabetic character\n", buffer);
                if(i == -3)
                    fprintf(file_out_ptr, "ERROR: <%s> cannot contain non-alphanumeric characters\n", buffer);    
                }
            }
        }
        else{
            
            if(ch == '"'){
                str_temp = (char*) malloc(200);
                i = 0;
                while ((ch = fgetc(file_ptr)) != EOF){
                    str_temp[i++] = ch; // ch'nin kontrolunu yaptigi icin bir sonraya gecer
                    if(fpeek(file_ptr) == '"'){
                        fgetc(file_ptr);
                        str_temp[i] = '\0';
                        break;
                    }
                }
                if (ch == EOF){
                    // tekrardan " olmadigi icin string kapanmamis olur
                    fprintf(file_out_ptr, "ERROR: String not closed\n");
                }
                fprintf(file_out_ptr, "StringConst(%s)\n", str_temp);
                free(str_temp);
            }
            else if(get_brckt_name(ch) != NULL){fprintf(file_out_ptr, "%s\n", get_brckt_name(ch));}
            else if(ch == '/' && fpeek(file_ptr) == '*'){
                printf("Comment start\n");
                fgetc(file_ptr);
                while((ch = fgetc(file_ptr)) != EOF){
                    if(ch == '*' && fpeek(file_ptr) == '/'){
                        fgetc(file_ptr);
                        break;
                    }
                }
                if(ch == EOF){fprintf(file_out_ptr, "ERROR: Comment not closed!\n");}
                printf("Comment end!\n");
            }
            else if(check_for_oprtr(ch, fpeek(file_ptr))){
                ch2 = fgetc(file_ptr);
                fprintf(file_out_ptr, "Operator(%c%c)\n", ch, ch2);
            }
            else if(is_operator(one_chr_oprtr(ch))){
                fprintf(file_out_ptr, "Operator(%c)\n", ch);
            }
            else if(ch == ';'){
                fprintf(file_out_ptr, "EndOfLine!\n");
            }
        }
    }
    fclose(file_ptr);
    fclose(file_out_ptr);
    return 0;
}

int is_identifier(char* str){
    char* p = NULL;
    int a = strlen(str);
    // max 30 char  
    if (strlen(str) > 30){
        return -1;
    }
    for(int i=0; i < a; ++i){
        if(str[i]>='A' || str[i]<='Z'){
            return 0;
        }
    }
    // being a letter case
    if (str[0] > 'z' || str[0] < 'a'){
        return -2;
    }

    for(p = str; *p != '\0'; ++p){
        if(isalnum(*p) || *p == '_'){}
        else{return -3;}
    }
    if(str[0]>='a' && str[0]<='z')
        return 1;
    return 7;
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

char* get_brckt_name(char ch){
    if(ch == '('){return "LeftPar";}
    else if (ch == ')'){return "RightPar";}
    else if (ch == '['){return "Left Square Bracket";}
    else if (ch == ']'){return "Right Square Bracket";}
    else if (ch == '{'){return "Left Curly Bracket";} 
    else if (ch == '}'){return "Right Curly Bracket";}
    else{return NULL;}
}

int check_for_oprtr(char chr1, char chr2){
    char* temp = two_chr_oprtr(chr1, chr2);
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

    //used for reading all characters from file
    c = fgetc(stream);
    ungetc(c, stream);

    return c;
}

char* one_chr_oprtr(char chr){
    char* temp = (char*) malloc(2);
    temp[0] = chr;
    temp[1] = '\0';
    return temp;
}

char* two_chr_oprtr(char chr1, char chr2){
    char* temp = (char*)malloc(sizeof(char) * 3);
    temp[0] = chr1;
    temp[1] = chr2;
    temp[2] = '\0';
    return temp;
}

