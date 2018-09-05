/*
*   Name: Wang,jingqing
*   ID: 1509853G-I011-0202
*   CLASS: 1709-CS106
*   File:symbtab.c
*/

#include "libs.h"
#include "globals.h"
#include "util.h"
#include "parse.h"
#include "symbol_table.h"


#define SHIFT 4

static int hash(const char * key) {
	int temp = 0;
	int i = 0;
	while (key[i] != '\0') {
		temp = ((temp << SHIFT) + key[i]) % ST_SIZE;
		++i;
	}
	return temp;
}


void st_insert_dcl(TreeNode *dclNd, SymbolTable *st) {
	int h;
	const char * name;
	BucketList l;
	struct BucketListRec * b;
	name = dclNd->attr.dclAttr.name;
	h = hash(name);
	l = st->hashTable[h];
	
	b = (struct BucketListRec *) malloc(sizeof(struct BucketListRec));
	b->st = st;
	b->nd = dclNd; 
	dclNd->something = b;
	b->lines = NULL;
	b->next = l;
	b->prev = NULL;
	if (l != NULL)
		l->prev = b;
	st->hashTable[h] = b;
	return;
}


void  st_insert_ref(TreeNode *refNd, struct BucketListRec* bk) {
	LineList list = bk->lines;
	struct LineListRec * b = (struct LineListRec *) malloc(sizeof(struct LineListRec));
	b->bk = bk;
	b->nd = refNd;
	refNd->something = b;
	b->next = NULL;
	b->prev = NULL;
	if (list == NULL) 
		bk->lines = b;
	else { 
		while (list->next != NULL)
			list = list->next;
		list->next = b;
		b->prev = list;
	}
	return;
}


struct BucketListRec*  st_lookup(SymbolTable* st, const char * name) {
	
	int h = hash(name);

	while (st) {
		BucketList bl = st->hashTable[h];

		while (bl != NULL) {

			if (strcmp(bl->nd->attr.dclAttr.name, name) == 0)
				return bl;

			bl = bl->next;
		}

		st = st->upper;
	}

	return NULL;
}


void st_print(SymbolTable* st) {
	int i;
	int flag = 0;
	if (st == NULL) return;
	if ((flag == 0)&&(st!=NULL))
	{
		fprintf(listing, "%-6s%-15s%-12s%-5s%-9s\n", "Table", "Name", "Kind", "Dcl", "Ref");
		fprintf(listing, "%-6s%-15s%-12s%-5s%-9s\n", "ID", "", "", "line", "lines");
		fprintf(listing, "%-6s%-15s%-12s%-5s%-9s\n", "----", "----", "----", "----", "----");
	}
	for (i = 0; i < ST_SIZE; ++i) {
		BucketList bl = st->hashTable[i];
		while (bl != NULL) {
			LineList lines;
			TreeNode * nd = bl->nd;
			fprintf(listing, "%-6d", st->id);
			fprintf(listing, "%-15s", nd->attr.dclAttr.name);
			if (nd->nodeKind == DCL_ND) 
				switch (nd->kind.dcl) {
				case VAR_DCL: fprintf(listing, "%-12s", "Var"); break;
				case ARRAY_DCL: fprintf(listing, "%-12s", "Array"); break;
				case FUN_DCL: fprintf(listing, "%-12s", "Func"); break;
				default:
					fprintf(listing, "\n%s\n", "st_print(): wrong declaration type, the symbol table records a wrong node.");
					system("pause");
					exit(0);
					break;
				}
			else if (nd->nodeKind == PARAM_ND)
				switch (nd->kind.param) {
				case VAR_PARAM: fprintf(listing, "%-12s", "Var_Param"); break;
				case ARRAY_PARAM: fprintf(listing, "%-12s", "Array_Param"); break;
				case VOID_PARAM: fprintf(listing, "%-12s", "Void_Param"); break;
				default:
					fprintf(listing, "\n%s\n", "st_print(): wrong parameter type, the symbol table records a wrong node.");
					system("pause");
					exit(0);
					break;
				}
			else {
				fprintf(listing, "\n%s\n", "st_print(): wrong node type, the symbol table records a wrong node.");
				system("pause");
				exit(0);
			}
			fprintf(listing, "%-5d", nd->lineNum);
			lines = bl->lines;
			while (lines != NULL) {
				fprintf(listing, "%d ", lines->nd->lineNum);
				lines = lines->next;
			}
			fprintf(listing, "\n");
			bl = bl->next;
		}
	}
	flag++;
	st_print(st->lower); /* print tables of  nested scope */
	st_print(st->next);  /* print tables of sibling scopes */
	flag--;
}


SymbolTable*  st_initialize() {
	int i;
	SymbolTable* tab = NULL;
	static int tabId = 0;
	tab = (SymbolTable*)malloc(sizeof(SymbolTable));
	tab->id = tabId++;
	tab->nd = NULL;
	tab->upper = NULL;
	tab->lower = NULL;
	tab->next = NULL;
	tab->prev = NULL;
	for (i = 0; i < ST_SIZE; i++)
		tab->hashTable[i] = NULL;
	return tab;
}


SymbolTable * st_attach(SymbolTable* st) {
	SymbolTable* newSt = st_initialize();
	SymbolTable* last = st->lower;
	newSt->upper = st;
	if (last == NULL)
		st->lower = newSt;
	else {
		while (last->next != NULL)
			last = last->next;
		last->next = newSt;
		newSt->prev = last;
	}
	return newSt;
}
