/*
*   Name: Wang,jingqing
*   ID: 1509853G-I011-0202
*   CLASS: 1709-CS106
*   File:s_analyzer.c
*/

#include"globals.h"
#include"analyzer.h"
#include"s_analyzer.h"
void s_set_parse_tree(Analyzer * self, TreeNode * tree)
{
	if ((self != NULL) && (tree != NULL))
		self->info = tree;
}
void s_build_symbol_table(Analyzer * self)
{
	if (self != NULL)
		build_symbol_table(self->info);
}

SymbolTable *  s_get_symbol_table(Analyzer * self)
{
	SymbolTable *n;
	if (self != NULL)
	{
		n = get_symbol_table(self);
	}
	return n;
}

void s_type_check(Analyzer * self)
{
	if (self != NULL)
		type_check(self->info);
}

Boolean s_check_semantic_error(Analyzer * self)
{
	return 0;
}

void s_clear(Analyzer * self)
{
}

void s_clear_error_status(Analyzer * self)
{
}


Analyzer * new_s_analyzer(TreeNode * parseTree)
{
	Analyzer *analyzer = (Analyzer*)malloc(sizeof(Analyzer));
	analyzer->set_parse_tree = s_set_parse_tree;
	analyzer->build_symbol_table = s_build_symbol_table;
	analyzer->check_semantic_error = s_check_semantic_error;
	analyzer->get_symbol_table = s_get_symbol_table;
	analyzer->type_check = s_type_check;
	analyzer->info = parseTree;
	return analyzer;
}
