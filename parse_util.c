/*
*   Name: Wang,jingqing
*   ID: 1509853G-I011-0202
*   CLASS: 1709-CS106
*   File:parse_util.c
*/

#include"parse_util.h"
#include"libs.h"
#include"parse.h"
#include"util.h"
#include"globals.h"

TreeNode *CreateNewNode(int lineno);
#define INDENT_GAP 2
TreeNode * new_stmt_node(StmtKind k, int lineNum)
{
	TreeNode *t;
	t = CreateNewNode(lineNum);
	if (t)
	{
		t->nodeKind = STMT_ND;
		t->kind.stmt = k;
	}
	return t;
}

//  Similar to the new_stmt_node() function
TreeNode * new_expr_node(ExprKind k, int lineNum)
{
	TreeNode *t;
	t = CreateNewNode(lineNum);
	if (t)
	{
		t->nodeKind = EXPR_ND;
		t->kind.expr = k;
		t->attr.exprAttr.op = ERROR;
		t->attr.exprAttr.val = 0;
		t->attr.exprAttr.name = "";
	}
	return t;
}

//  Similar to the new_stmt_node() function
TreeNode * new_dcl_node(DclKind k, int lineNum)
{
	TreeNode *t;
	t = CreateNewNode(lineNum);
	if (t)
	{
		t->nodeKind = DCL_ND;
		t->kind.dcl = k;
		t->attr.dclAttr.type = VOID_TYPE;
		t->attr.dclAttr.name = "";
		t->attr.dclAttr.size = 0;
	}
	return t;
}

//  Similar to the new_stmt_node() function
TreeNode * new_param_node(ParamKind k, int lineNum)
{
	TreeNode *t;
	t = CreateNewNode(lineNum);
	if (t)
	{
		t->nodeKind = PARAM_ND;
		t->kind.param = k;
		t->attr.dclAttr.type = VOID_TYPE;
		t->attr.dclAttr.name = "";
		t->attr.dclAttr.size = 0;
	}
	return t;
}

TreeNode *CreateNewNode(int lineno)
{
	TreeNode *t;
	int      i;

	t = (TreeNode*)malloc(sizeof(TreeNode));
	if (!t)
	{
		fprintf(listing, "*** Out of memory at line %d.\n", lineno);
	}
	else
	{
		for (i = 0; i < MAX_CHILDREN; ++i) t->child[i] = NULL;
		t->lSibling = NULL;
		t->rSibling = NULL;
		t->parent = NULL;
		t->lineNum = lineno;

	}

	return t;
}

void syntax_error(const TOKENNODE * tknd, const char * msg)
{
	fprintf(listing, " !!! Error !!!  >>> ");
	fprintf(listing, "%s\n", msg);
	if (tknd == NULL)
		fprintf(listing, "Found a NULL token node.\n");
	else {
		int lineNum;
		/* One problem is that when last token in a line is missing, in the error message
		 * The next line number is printed. That is not preferred, so, one choice is tknd->prev->lineNum.
		 */
		lineNum = tknd->lineNum;
		fprintf(listing, "Syntax error at the token %s, in line %d \n  ",
			tknd->token->string,
			lineNum);
	}

	if (G_exitOnError)
		exit(0);
}

TOKENNODE *reach_node(const TOKENNODE * nd, int steps)
{
	int j = 0;
	const TOKENNODE * theOne = nd;
	do {
		if (theOne == NULL) {
			break;
		}
		if (j == steps)
			break;
		if (steps > 0)
		{
			theOne = theOne->next; j++;
		}
		else
		{
			theOne = theOne->prev; j--;
		}
	} while (1);
	return  (TOKENNODE *)theOne;
}

void getToken()
{
	if (CurrentToken != NULL)
	{
		CurrentToken = CurrentToken->next;
		CurrentTokenType = CurrentToken->token->type;
	}
	else
	{
		fprintf(listing, "NULL TOKEN ERROR! ");
		exit(1);
	}
}
Boolean match(TokenType expected)
{
	if (CurrentToken->token->type = expected)
	{
		return TRUE;
	}
	else
	{
		syntax_error(CurrentToken, "Unexpected Token!"); return FALSE;
	}
}
Boolean match_move(TokenType expected)
{
	if (CurrentToken->token->type == expected)
	{
		getToken(); return TRUE;
	}
	else
	{
		syntax_error(CurrentToken, "Unexpected Token!"); return FALSE;
	}
}
Boolean check(const TOKENNODE * nd, TokenType expected)
{
	if ((nd != NULL) && nd->token->type == expected)
	{
		return TRUE;
	}
	else
		return FALSE;
}

void free_tree(TreeNode * nd)
{

	int j;
	if (nd == NULL)
		return;
	// free the children
	for (j = 0; j < MAX_CHILDREN; j++)
	{
		if (nd->child[j] != NULL)
			free_tree(nd->child[j]);
		else
			continue;
	}
	free_tree(nd->rSibling);
	// free this token node
	free(nd);
	return;
}

TreeNode * parse_bad_return(TreeNode * top, Boolean * ok)
{
	*ok = FALSE;
	if (P_keepParseTreeOnError == TRUE) { /* keep the tree */
		return top;
	}
	else {
		free_tree(top);  /*release the memory of the tree */
		return NULL;
	}
}


TreeNode * parse_good_return(TreeNode * top, Boolean * ok)
{
	*ok = TRUE;
	return top;
}

TOKENNODE* find_closing_mark(const TOKENNODE* from)
{
	TokenType openType = from->token->type;
	TokenType closeType;
	TOKENNODE *closeNode;
	int j = 1; // count the number of opening marks that have not found its pair. 
	if (from == NULL) return NULL;
	switch (openType) {
	case LPAR: closeType = RPAR; break;
	case LCUR: closeType = RCUR; break;
	case LBR: closeType = RBR; break;
	default:
		fprintf(listing, "Wrong token in find_closing_mark()\n");
		return NULL;
	}
	closeNode = from->next;
	while (closeNode != NULL) {
		if (check(closeNode, closeType))
			if (j == 1)
				return closeNode;
			else
				j--;
		else if (check(closeNode, openType))
			j++;
		closeNode = closeNode->next;
	}
	return NULL;
}

static void print_spaces(int indentNum) {
	int i;
	for (i = 0; i < indentNum; i++)
		fprintf(listing, " ");
}

void print_expr_type(ExprType t) {
	switch (t) {
	case VOID_TYPE: fprintf(listing, "void"); break;
	case INT_TYPE: fprintf(listing, "int"); break;
	case ADDR_TYPE: fprintf(listing, "address"); break;
	default: fprintf(listing, "Error ExpType"); break;
	}
}

void print_token_type(TokenType tokenTp) {
	switch (tokenTp) {
	case IF: fprintf(listing, "if"); break;
	case ELSE: fprintf(listing, "else"); break;
	case WHILE: fprintf(listing, "while"); break;
	case DO: fprintf(listing, "do"); break;
	case FOR: fprintf(listing, "for"); break;
	case INT: fprintf(listing, "INT"); break;
	case VOID: fprintf(listing, "void"); break;
	case RETURN: fprintf(listing, "return"); break;
	case ASSIGN: fprintf(listing, "="); break;
	case EQ: fprintf(listing, "=="); break;
	case NEQ: fprintf(listing, "!="); break;
	case LT: fprintf(listing, "<"); break;
	case LTE: fprintf(listing, "<="); break;
	case GT: fprintf(listing, ">"); break;
	case GTE: fprintf(listing, ">="); break;
	case LPAR: fprintf(listing, "("); break;
	case RPAR: fprintf(listing, ")"); break;
	case LBR: fprintf(listing, "["); break;
	case RBR: fprintf(listing, "]"); break;
	case LCUR: fprintf(listing, "{"); break;
	case RCUR: fprintf(listing, "}"); break;
	case SEMI: fprintf(listing, ";"); break;
	case COMMA: fprintf(listing, ","); break;
	case PLUS: fprintf(listing, "+"); break;
	case MINUS: fprintf(listing, "-"); break;
	case MULTP: fprintf(listing, "*"); break;
	case OVER: fprintf(listing, "/"); break;
	case END: fprintf(listing, "EOF"); break;
	case NUMBER: fprintf(listing, "NUM");  break;
	case ID:  fprintf(listing, "ID");   break;
	case ERROR: fprintf(listing, "ERROR:"); break;
	default: 
		fprintf(listing, "Unknown token type: ");
		break;
	}
}
void print_tree(TreeNode * tree) {
	int i;
	static int indentNum = 0;

	indentNum += INDENT_GAP;
	while (tree != NULL) {
		print_spaces(indentNum); /* Each case only prints one line, If print more than one line, need use printSpaces() first.*/
		//fprintf(listing,"%d ",  tree->lineNum);
		if (tree->nodeKind == DCL_ND) {
			fprintf(listing, "Declare:  ");
			print_expr_type(tree->attr.dclAttr.type);
			fprintf(listing, " %s ", tree->attr.dclAttr.name);
			// print the [size] only if it is an array.
			switch (tree->kind.dcl) {
			case ARRAY_DCL:
				fprintf(listing, "[%d]\n", tree->attr.dclAttr.size);
				break;
			case FUN_DCL:
				fprintf(listing, "function with parameters :\n");
				// Function parameters will be saved as child[0] of the node
				break;
			case VAR_DCL:
				// do nothing
				fprintf(listing, "\n");
				break;
			default:
				fprintf(listing, "Unknown DclNode kind\n");
				break;
			}
		}
		else if (tree->nodeKind == PARAM_ND) {
			fprintf(listing, "Parameter: ");
			//print_expr_type(tree->attr.dclAttr.type);
			if (tree->attr.dclAttr.type != VOID_TYPE) {
				fprintf(listing, " %s", tree->attr.dclAttr.name);
				if (tree->kind.param == ARRAY_PARAM)
					fprintf(listing, "[ ]");
			}
			fprintf(listing, "\n");
		}
		else if (tree->nodeKind == STMT_ND) {
			switch (tree->kind.stmt) {
			case SLCT_STMT:
				fprintf(listing, "If ");
				if (tree->child[2] != NULL)
					fprintf(listing, " with ELSE \n");
				else
					fprintf(listing, " without ELSE \n");
				break;
			case WHILE_STMT:
				fprintf(listing, "while stmt: \n");
				break;
			case FOR_STMT:
				fprintf(listing, "for stmt: \n");
				break;
			case DO_WHILE_STMT:
				fprintf(listing, "do while stmt: \n");
				break;
			case EXPR_STMT:
				fprintf(listing, "Expression stmt: \n");
				break;
			case CMPD_STMT:
				fprintf(listing, "Compound Stmt:\n");
				break;
			case RTN_STMT:
				fprintf(listing, "Return \n");
				break;
			case NULL_STMT:
				fprintf(listing, "Null statement:  ;\n");
				break;
			default:
				fprintf(listing, "Unknown StmtNode kind\n");
				break;
			}
		}
		else if (tree->nodeKind == EXPR_ND) {
			switch (tree->kind.expr) {
			case OP_EXPR:
				fprintf(listing, "Operator: ");
				print_token_type(tree->attr.exprAttr.op);
				fprintf(listing, "\n");
				break;
			case CONST_EXPR:
				fprintf(listing, "Const: %d\n", tree->attr.exprAttr.val);
				break;
			case ID_EXPR:
				fprintf(listing, "ID: %s\n", tree->attr.exprAttr.name);
				break;
			case ARRAY_EXPR:
				fprintf(listing, "Array: %s, with member index:\n", tree->attr.exprAttr.name);
				break;
			case CALL_EXPR:
				fprintf(listing, "Call function: %s, with arguments:\n", tree->attr.exprAttr.name);
				break;
			default:
				fprintf(listing, "Unknown ExpNode kind\n");
				break;
			}
		}
		else fprintf(listing, "Unknown node kind\n");
		for (i = 0; i < MAX_CHILDREN; i++)
			print_tree(tree->child[i]);
		tree = tree->rSibling;
	}// end of while loop.
	indentNum -= INDENT_GAP;
}