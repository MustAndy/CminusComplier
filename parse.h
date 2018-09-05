/*
*   Name: Wang,jingqing
*   ID: 1509853G-I011-0202
*   CLASS: 1709-CS106
*   File:parse.h
*/
#ifndef _PARSE_H_
#define _PARSE_H_


#include "scan.h"

/* Naming convention. If the word is no more than 5 character use the full word, like param, array, otherwise use, 3 characters, like varaible -> var, assign -> asn.
*/

typedef enum { DCL_ND, PARAM_ND, STMT_ND, EXPR_ND } NodeKind;
typedef enum { VAR_DCL, ARRAY_DCL, FUN_DCL } DclKind;
typedef enum { VAR_PARAM, ARRAY_PARAM, VOID_PARAM } ParamKind;
typedef enum { SLCT_STMT, WHILE_STMT, DO_WHILE_STMT, FOR_STMT, EXPR_STMT, CMPD_STMT, RTN_STMT, NULL_STMT } StmtKind;
typedef enum { OP_EXPR, CONST_EXPR, ID_EXPR,ARRAY_EXPR , CALL_EXPR } ExprKind;

/* The type of the value of an expression
ExprType is used for type checking
*/
/* No need for Boolean type, since C- uses integer as
boolean valus */

typedef enum { VOID_TYPE, INT_TYPE, ADDR_TYPE } ExprType;

#define MAX_CHILDREN 4


/**************************************************/
/***********   Syntax tree for parsing ************/
/**************************************************/

typedef struct treeNode {
	struct treeNode * child[MAX_CHILDREN];
	struct treeNode * lSibling;
	struct treeNode * rSibling;
	// sibling is useful for declaration_list, param_list, local_declarations, statement_list, arg_list.
	struct treeNode * parent;
	/* parent is useful to check the containing structure of a node during parsing. So, the connected tree nodes can be found in all directions, 
	up (to parents), down (to children), and horizontally (left and right to siblings).  */
	/* LineNum:  At the momemt in parsing, when this treeNode is constructed, what is the line number of the token being handled. */
	int lineNum;
	NodeKind nodeKind;
	union { DclKind dcl; ParamKind param; StmtKind stmt; ExprKind expr; } kind;
	union {
		union {
			TokenType op; // used by Op_EXPR
			int val;      // used by Const_EXPR,
			const char * name;  // used by ID_EXPR, Call_EXPR, Array_EXPR
		} exprAttr;
		struct { // it is a struct, not union, because an array declaration need all the three fields.
			ExprType type; // used by all dcl and param
			const char * name;  // used by all dcl and param
			int size;     // used by array declaration
						  /* size is only used for and array declaration; i.e., when  Dcl_Kind is Array_DCL. 
						  The requirement that size must be a constant should be checked by semantic analyzer.  
						  For parameters, for Array_PARAM, size is ignored. 
						  For array element argument, the index is a child of the node, and should not be considered as dclAttr. */
		} dclAttr; // for declaration and parameters.
	}attr;
	ExprType type;
	/* type is for type-checking of exps, will be updated by type-checker,  the parser does not touch it.  */
	void * something; //can carry something possibly useful for other tasks of compiling
} TreeNode;

typedef struct parser Parser;

/* Each function has a parameter p, that is a pointer to the parser itself, in order to use the resources belong to the parser */
typedef struct parser {
	TreeNode * (*parse)(); /* returning a parse tree, based on the tokenList that the parser knows */
	void(*set_token_list)(Parser * p, TOKENNODE *theTokenList); /* let the parser remember some tokenList */
	void(*print_tree)(Parser * p,TreeNode * tree); /* can print some parser tree */
	void(*free_tree)(Parser *p, TreeNode * tree); /* free the space of a parse tree */
	void  *info; /* Some data belonging to this parser object. It can contain the tokenList that the parser knows. */
} Parser;
/*这个struct 中包含的是parse用到的函数 */
TreeNode *parse();


/*
extern TreeNode * syntaxTree;

extern TOKENNODE * thisTokenNode;
*/

#endif
