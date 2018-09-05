/*
*   Name: Wang,jingqing
*   ID: 1509853G-I011-0202
*   CLASS: 1709-CS106
*   File:s_analyzer.h
*/

/* implements a semantic analyzer, according to the interface analyzer.h
*/

#ifndef _S_ANALYZER_H_
#define _S_ANALYZER_H_

//#include "symbol_table.h"
#include "analyzer.h"

// the symbol table on the top level.
//extern SymbTab * symbolTable;

/* Function buildSymtab constructs the symbol
* table by preorder traversal of the syntax tree
*/
//void build_symb_tab(TreeNode *);

/* Procedure typeCheck performs type checking
* by a postorder syntax tree traversal
*/
//void type_check(TreeNode *);

extern Boolean A_debugAnalyzer;

Analyzer * new_s_analyzer(TreeNode * parseTree);

#endif
