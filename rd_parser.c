/*
*   Name: Wang,jingqing
*   ID: 1509853G-I011-0202
*   CLASS: 1709-CS106
*   File:rd_parser.c
*/

#include"rd_parser.h"
#include"scan.h"
#include"parse.h"
#include"globals.h"
#include"libs.h"
#include"parse_util.h"

static void rd_free_tree(Parser * p, TreeNode * top){
	RDinfo* info = (RDinfo *) p->info;
	if(info == NULL){
		puts("Null info of parser found by rd_free_tree");
		return;
	}
	free_tree(info->parseTree);
	if(top == info->parseTree){
		info->parseTree = NULL;
	}
	return;
}

/* Do parsing based on the parser p's info, which contains the token list */
static TreeNode * rd_parse(Parser * p){
	RDinfo* info = (RDinfo *) p->info;
	info->parseTree = parse();
	return info->parseTree;
}

/* Set a parser's token list field of its information with the parameter lis */
static void rd_set_token_list(Parser * p, TOKENNODE * lis){
	RDinfo * info = (RDinfo *) p->info;
	info->CurrentToken = lis;
	
}

/* Wrap print_tree() into a function of the parser to print a parse tree*/
static void rd_print_tree(Parser * p,  TreeNode * tree){
	RDinfo * info = (RDinfo *) p->info;
	if(tree == info->parseTree)
		puts("Now printing parse tree of the parser.");
	print_tree(tree);
}

Parser * new_rd_parser(TOKENNODE *tklist)
{
	Parser * parser = (Parser *)malloc(sizeof(Parser));
	RDinfo * info = (RDinfo *)malloc(sizeof(RDinfo));
	info->parseTree = NULL;
	info->CurrentToken = tklist;

	parser->info = info;
	parser->free_tree = rd_free_tree;
	parser->parse = rd_parse;
	parser->set_token_list = rd_set_token_list;
	parser->print_tree = rd_print_tree;
	return parser;
}