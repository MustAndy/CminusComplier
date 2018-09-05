/*
*   Name: Wang,jingqing
*   ID: 1509853G-I011-0202
*   CLASS: 1709-CS106
*   File:scan.h
*/

#ifndef _SCAN_H_
#define _SCAN_H_

/* MAXRESERVED = the number of reserved words/ keywords */
#include"util.h"
#include"libs.h"
#define MAX_RESERVED 8

/* MAXTOKENLEN is the maximum size of a token */
#define MAX_TOKEN_LEN 40

extern Boolean S_debugScanner;
extern Boolean S_scannerErrorFound;

/* tokenString array stores the lexeme of each token */
//extern char tokenString[MAX_TOKEN_LEN+1];

/* the line number of the current token */
//extern int lineNum; /* source line number for listing */

typedef enum
{
	NONE, ERROR,/* book-keeping tokens, they are used to label non-final states in the DFA graph.*/
				/* reserved words
				if else int return void while
				*/
	IF, ELSE, INT, RETURN, VOID, WHILE, DO, FOR,
	/* multicharacter tokens */
	ID, NUMBER, COMMENT,  /* comment token may can be discarded by scanner */
						  /* special symbols for
						  +  -  *  /  <  <=  >  >=  ==  !=  =  ;  ,  (  )  [  ]  {  }
						  */
	PLUS, MINUS, MULTP, OVER, LT, LTE, GT, GTE, EQ, NEQ, ASSIGN, SEMI, COMMA, LPAR, RPAR, LBR, RBR, LCUR, RCUR,
	/* book-keeping token, representing the EOS (end of stream) signal received at the end of the streamer */
	END
} TokenType;


typedef struct {
  char * string;   
  // It is ok to assign  the return of copy_string(), which is char *, to string, 
  TokenType type;
} Token;

// A node in a double linked list
typedef struct TkNd{
  Token* token;
  int lineNum;
  struct TkNd * prev;
  struct TkNd * next;
}TOKENNODE;

typedef struct {
	TOKENNODE * head;
	TOKENNODE * tail;
}TokenList;

extern TOKENNODE* theTokenList;

/* given a token type, return a string. For example, when parameter is INT, a string constant "INT" is returned */
const char * token_type_to_string(TokenType);

/* function getToken returns the
* next token in source file
*/
TOKENNODE* get_token(void);

/* Procedure print_token prints a token
* and its lexeme to the listing file
*/
//void printToken( TokenType, const char* );

void print_token(const TOKENNODE *);

/* print all tokens in the tokenlist */
void print_token_list(TOKENNODE *);

/* Free the token List (all of the token nodes) described by tklist.
* The TOKENNODE struct pointed by tklist is not freed, and it is changed to describe an empty list */
void free_token_list(TOKENNODE *);


/* open the file with fileaName, scan it, and return a Token List. Report lexical errors along the process
*/
TOKENNODE *scanner();


#endif
