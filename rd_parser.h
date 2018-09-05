/*
*   Name: Wang,jingqing
*   ID: 1509853G-I011-0202
*   CLASS: 1709-CS106
*   File:rd_parser.h
*/

#ifndef _RD_PARSER_H_
#define _RD_PARSER_H_

#include "parse.h"
#include "scan.h"
#include"parse_util.h"
#include"libs.h"
extern Boolean P_debugParser  ;  /* When true, print out some debug information. For example, what functions are called */

extern Boolean P_keepParseTreeOnError  ; /* when true, the quit the problem when parser find some error */

extern Boolean P_parserExitOnError  ;  /* when true, the quit the problem when parser find some error */

/* Returns a recursive-descent parser. The token list parameter will be used by the parser*/
Parser * new_rd_parser(TOKENNODE *tklist);

typedef struct rdParserInfo{
	//TokenList theTokenList;
	TOKENNODE * CurrentToken;
	TreeNode * parseTree;
	Boolean parseErrorFound;
} RDinfo;

#endif
