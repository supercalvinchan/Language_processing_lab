%option noyywrap

%{
/* Now in a section of C that will be embedded
   into the auto-generated code. Flex will not
   try to interpret code surrounded by %{ ... %} */

/* Bring in our declarations for token types and
   the yylval variable. */
#include "histogram.hpp"
#include "string.h"

// This is to work around an irritating bug in Flex
// https://stackoverflow.com/questions/46213840/get-rid-of-warning-implicit-declaration-of-function-fileno-in-flex
extern "C" int fileno(FILE *stream);

/* End the embedded code section. */
%}


%%


-?([0-9])+(\.([0-9])+)?\/([0-9])+(\.([0-9])+)?   { fprintf(stderr, "Number : %s\n", yytext); std::string tmp(yytext);
 const char* pYYText = yytext;
 const char* ptr = strchr(pYYText,'/');
 if(ptr == NULL) {yylval.numberValue = atof(pYYText);}
 else { std::string tmp1 = tmp.substr(0, ptr - pYYText);
 double p = atof(tmp1.c_str()); double p1 = atof(ptr + 1); yylval.numberValue = p/p1; };
 return Number;}

 -?([0-9])+(\.([0-9])+)?   { fprintf(stderr, "Number : %s\n", yytext); std::string tmp(yytext);
  const char* pYYText = yytext;
  const char* ptr = strchr(pYYText,'/');
  if(ptr == NULL) {yylval.numberValue = atof(pYYText);}
  else { std::string tmp1 = tmp.substr(0, ptr - pYYText);
  double p = atof(tmp1.c_str()); double p1 = atof(ptr + 1); yylval.numberValue = p/p1; };
  return Number;}


 \[([^\[\]\n])*\]          { fprintf(stderr, "Word : %s\n", yytext);
  std::string tmp(yytext); tmp = tmp.substr(1, tmp.size() - 2);
  yylval.wordValue = new std::string(tmp);/* TODO: get value out of yytext and into yylval.wordValue */;  return Word; }


[a-zA-Z]+          { fprintf(stderr, "Word : %s\n", yytext); yylval.wordValue = new std::string(yytext);/* TODO: get value out of yytext and into yylval.wordValue */;  return Word; }

\n              { fprintf(stderr, "Newline\n",  *yytext); return Unmatch; }

.               {return Unmatch;}



%%

/* Error handler. This will get called if none of the rules match. */
void yyerror (char const *s)
{
  fprintf (stderr, "Flex Error: %s\n", s); /* s is the text that wasn't matched */
  exit(1);
}
