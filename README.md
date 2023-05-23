# lexical_analyser_c

Do not forget adding your code.psi and code.lex file path on your computer.

This is my Programming languages project at university about writing a lexical analyser for Psi++ language with C.

Lexical rules for Psi++ are:

1- Identifiers:
   - Max identifier size is 20 characters. If you use larger than that, the lexical analyser issues an error message.
   - Psi++ is not case sensitive and all the identifier names are standardized as upper case.
   - Identifiers start with an alphabetic character(a letter) and are composed of one or more letters, digits or _(underscore)
   - Example Token: Identifier(my_var_1)

2- Integer Constants: 
  - Max integer size is 10 digits. If you use longer than that, the lexical analyser issues an error message.
  - Negative values are not supported.
  - Example Token: IntConst(1290)

3- Operators:
  -Valid operators of the language are +,-,*,/,++,--,:=
  -Example Token: Operator(++)

4- Brackets:
    • LeftPar: (       			RightPar: )
    • LeftSquareBracket: [		RightSquareBracket: ]
    • LeftCurlyBracket:  {		RightCurlyBracket:  }
    • Example Token: LeftCurlyBracket
    
5- String Constants:
    • String constants of Psi++ are delimited by double quotes (ASCII code 34)as in “this is a string”
    • String constants have unlimited size
    • String constants cannot contain the double quote character. when you reach one, the string terminates.
    • If a string constant cannot terminate before the file end, there should be a lexical error issued.

6- Keywords:
      • Keywords are: break,case,char,const,continue,do,else,enum,float,for,goto,if,int,
long,record,return,static,while
    • Psi++ language is not case sensitive and all the keywords are standardized as lower case. You can write the same word as “while” OR “While” OR “WHILE” and they all generate the 
    • Example Token: Keyword(while)
    
7- End of Line:
  Example Token: EndOfLine
  
8- Comments: Anything between (* and *) is a comment.
    • If a comment cannot terminate before the file end, there should be a lexical error issued.
    • Comments are just like blank space and they provide no tokens.
    
    
Example:
if code.psi contains:
hi:=hello+25; (*addition *)
hello++; (*increment*)
code.lex would be:
Identifier(hi)
Operator(:=)
Identifier(hello)
Operator(+)
IntConst(25)
EndOfLine
Identifier(hello)
Operator(++)
EndOfLine       
