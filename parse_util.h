/*
*   Name: Wang,jingqing
*   ID: 1509853G-I011-0202
*   CLASS: 1709-CS106
*   File:parse_util.h
*/

// This file should only be used by some code implementing the parser, since all it does is for helping parsing.

#ifndef _PARSE_UTIL_H_
#define _PARSE_UTIL_H_

#include "util.h"
#include "parse.h"



/* Function new_stmt_node creates a new statement
node for syntax tree construction.
Will not return NULL.
Set the node kind as statement node, statement kind as k, and its line number is lineNum.
All pointer fields should be NULL. Other fields are considered not initialzed
and should be assigned with some value by some code of using the node.
*/
TreeNode * new_stmt_node(StmtKind k, int lineNum);


//  Similar to the new_stmt_node() function
TreeNode * new_expr_node(ExprKind k, int lineNum);

//  Similar to the new_stmt_node() function
TreeNode * new_dcl_node(DclKind k, int lineNum);

//  Similar to the new_stmt_node() function
TreeNode * new_param_node(ParamKind k, int);

/* when The parser found that something wrong with the current token,
The message msg is printed out.
Also print out the token information, the token string, line number
Set the flag in parseErrorFound in the info as TRUE.
*/
void syntax_error(const TOKENNODE * tknd, const char * msg);


/* match()
parameters:
- tknd    the current token node during parsing
- expected  The token type that the current token should match.
-  msg   The error message that should be printed out when something is wrong.

behavior:
-  Does not move the curren token node.
-  When the current token match with the expected token type.
-  Returns TRUE if the token type matches, otherwise return FALSE.
-  If the token type does not match, error messages are printed out (call the syntax_error function)
*/
Boolean match(TokenType expected);



// move the current token node to its next one. 
// If the current token node is NULL, it is an error. 
void next_token_node();

// match the current token node (thisTokenNode )  with the expected type
// If the match is successful, move the current token node to its next neighbor
Boolean match_move( TokenType expected);



/* Given an array of expected token types (expected), and the number of elements of the array (expectedLen),
match the current token node (thisTokenNode ) with one of the token types in the array.
If the match is successful, move the current token node to its next neighbor, and returns the matched token type.
If the match fails, returns NONE (a token type that impossible to appear in a token list), without advancing the current token node, and, the error message is printed out (call the syntaxError function).
The errorMsg should indicate what are the expected tokens, in order to clearly identify the error.
*/
TokenType match_move_one_of( TokenType expected[], int expectedLen, const char * errorMsg);



/* return a node, call it theOne, which is decided by the parameters nd and steps.
if steps is 0,  theOne is nd
if steps is 1,  theOne is nd->next
if steps is -1, theOne is nd->prev
if steps is 2,  theOne is nd->next->next
if steps is -2, theOne is nd->prev->prev
When steps is other integers, theOne is defined similarly.
Note that no error is reported. when NULl is met
*/
TOKENNODE * reach_node(const TOKENNODE * nd, int steps);

/* check if the type of nd is tp. No error is reported when nd is NULL */
Boolean check(const TOKENNODE * nd, TokenType tp);

/* Given an array of token types (types), together with the number of elements in the array (typesLen),
* check if the type of nd is one of the types described in the array types. No error is reported when nd is NULL
* */
Boolean check_one_of(const TOKENNODE * nd, TokenType types[], int typesLen);

void print_tree(TreeNode * tree);

/* find_closing_mark()
Giving a starting mark of [, (, or {, find the token node of the paring closing mark of ], ) or }
parameter:  from should be the starting place of searching, and from should be a left mark, i.e, [ { or (.
The token type of from should be LPAREN, LBR, or LCUR; otherwise, NULL is returned.
*/
TOKENNODE* find_closing_mark(const TOKENNODE* from);

/* make sure that every node is connected to its parent.
parent should be the parent node of nd.
*/
void connect_parent(TreeNode * parent, TreeNode * nd);


/* free the space allocated to the tree whose top is nd.
All the sub-trees should be freed recursively.
*/
void free_tree(TreeNode * nd);
void print_token_type(TokenType tokenTp);
extern TOKENNODE* CurrentToken;
extern TokenType CurrentTokenType;
void getToken();
/* The function called if some bad thing happened (error) in the parsing.
top: the top node of a tree after some parsing task.
ok: a boolean variable. Some code in the parser use it to see if a task of parsing succeeded or not.
TRUE means ok, no error, FALSE means something wrong.
Computation:
Set ok as FALSE
if P_keepParseTreeOnError is true, return top; otherwise, free the tree, adn return NULL.
*/
TreeNode * parse_bad_return(TreeNode * top, Boolean * ok);


/* The function called if no bad thing happened (error) in the parsing.
top: the top node of a tree after some parsing task.
ok: a boolean variable. Some code in the parser use it to see if a task of parsing succeeded or not.
TRUE means ok, no error, FALSE means something wrong.
Computation:
Set ok as TRUE, and return top.
*/
TreeNode * parse_good_return(TreeNode * top, Boolean * ok);

#endif
