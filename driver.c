/*
*   Name: Wang,jingqing
*   ID: 1509853G-I011-0202
*   CLASS: 1709-CS106
*   File:driver.c
*/

/****************************************************/
/* File: driver.c                                   */
/* Main program for C-Minus compiler                */
/* Compiler Construction: Principles and Practice   */
/* Programming Designed by Zhiyao Liang             */
/* MUST FIT 2017 Fall                               */
/****************************************************/


#include"libs.h"
#include"analyzer.h"
#include"util.h"
#include"parse_util.h"
#include"analyzer.h"
#include"s_analyzer.h"
#include"globals.h"
#include"rd_parser.h"
/* allocate global variables */


Boolean G_echoSource = TRUE; /* to print the source code when reading it. */
Boolean G_traceScanner = TRUE;  /* to print  the token list  obtained by the scanner. */
Boolean G_traceParser = TRUE;  /* to print  the parse tree  obtained by the parser. */
Boolean P_debugParser = TRUE;
Boolean P_keepParseTreeOnError = TRUE;
Boolean P_parserExitOnError = FALSE;
/* When TRUE< print the symbol table that it builds */
Boolean G_traceAnalyzer = TRUE;
Boolean S_debugScanner = FALSE;
Boolean S_scannerErrorFound = FALSE;
Boolean A_debugAnalyzer = FALSE;
FILE *InputFile;
FILE *listing;
Boolean G_pause = TRUE;
Boolean G_exitOnError = FALSE; // Whether to  exit the program when the first error is found

							  /* the major data structures generated by the compiler are here */
TOKENNODE * theTokenList; /* the result of the scanner will be saved here */

Parser * parser = NULL;

TreeNode * parseTree = NULL; /* The result of running the parser will be saved here */

Analyzer * analyzer = NULL;
SymbolTable  * symbtab = NULL;

void set_default_controls(void) {
	/* Set all of the controlling toggles here. These toggles are defined in the files of parser and scanner controlling their running behavior.
	* Cannot put the following statements at global scope,
	* cause doing so will see the error message of redefining variables with type int.
	* */
	S_debugScanner = FALSE;
	P_debugParser = TRUE;  /* Set it to true to print some debug information of the parser */
	P_keepParseTreeOnError = TRUE; /*  When error is found by some parser functions, TRUE to keep the partial tree, FALSE to fee the partial tree*/
	P_parserExitOnError = TRUE; /* when true, the quit the problem when parser find some error */
	A_debugAnalyzer = FALSE; /* TRUE to see debugging information of running the analyzer */
}


/* How to run the scanner at command line:
Running the scanner with default setting:
scannerName fileName
Running the scanner, without debug information, with Trace information, with Pause effect
scannerName fileName -d n -t y -p y
Running the scanner, with debug information, without Trace information, without Pause effect
scannerName fileName -d y -t n -p n
Y or y, N or n, are all fine.
*/
FILE *OpenTheFile()
{
	fprintf(listing, "press for\n1.arrayMax.cm\n2.complex_expression.cm\n3.do_sum100.cm\n4.fact.cm\n5.good_type.cm\n");
	fprintf(listing, "6.selection_sort.cm\n7.sum100.cm\n8.wrong_do_sum100.cm\n9.wrong_fact.cm\n10.wrong_fact2.cm\n");
	fprintf(listing, "11.wrong_fact3.cm\n12.wrong_program.cm\n13.wrong_program2.cm\n14.wrong_selection_sort.cm\n");
	int a;
	scanf("%d", &a);
	switch (a)
	{
	case 1:
		InputFile = fopen("arrayMax.cm", "r"); break;
	case 2:
		InputFile = fopen("complex_expression.cm", "r"); break;
	case 3:
		InputFile = fopen("do_sum100.cm", "r"); break;
	case 4:
		InputFile = fopen("fact.cm", "r"); break;
	case 5:
		InputFile = fopen("good_type.cm", "r"); break;
	case 6:
		InputFile = fopen("selection_sort.cm", "r"); break;
	case 7:
		InputFile = fopen("sum100.cm", "r"); break;
	case 8:
		InputFile = fopen("wrong_do_sum100.cm", "r"); break;
	case 9:
		InputFile = fopen("wrong_fact.cm", "r"); break;
	case 10:
		InputFile = fopen("wrong_fact2.cm", "r"); break;
	case 11:
		InputFile = fopen("wrong_fact3.cm", "r"); break;
	case 12:
		InputFile = fopen("wrong_program.cm", "r"); break;
	case 13:
		InputFile = fopen("wrong_program2.cm", "r"); break;
	case 14:
		InputFile = fopen("wrong_selection_sort.cm", "r"); break;
	case 15:
		InputFile = fopen("test.cm", "r"); break;
	}
	if (InputFile == NULL)
	{
		fprintf(listing, "File error!\n");
		system("pause");
		exit(0);
	}
	else
	{
		fprintf(listing, "success\n");
		return InputFile;
	}
}

int main(int argc, char * argv[])
{
	listing = stdout; /* send listing to screen */
	//InputFile = OpenTheFile();
	RDinfo *parserInfo = NULL;

	char pgm[120]; /* C- program code file name */


	set_default_controls();
	strcpy(pgm, argv[1]);
	if (strchr(pgm, '.') == NULL)
		strcat(pgm, ".cm");  // the C-Minus program has a .cm extension
	fprintf(stdout, "\nC-Minus COMPILATION: %s\n", pgm);
	InputFile = fopen(pgm, "r");
	/* ---------- now run the scanner --------------*/
	theTokenList = scanner();
	if (G_traceScanner) {
		fprintf(stdout, "\nThis is the token list obtained by the scanner:\n");
		//print_token_list(theTokenList);
		if (S_scannerErrorFound == TRUE) {
			puts("!!! Scanner found some error.\n");
			if (G_exitOnError == TRUE) {
				puts("exit the compiler, due to the error. ");
				system("pause");
			}
		}
		else
			puts(" Scanner is happy, no error is found.\n");

	}

	/* ---------- now run the parser --------------*/

	if (parser == NULL)
		parser = new_rd_parser(theTokenList);
	else
		parser->set_token_list(parser, theTokenList);
	parserInfo = (RDinfo *)parser->info;
	parser->parse(parser);
	parseTree = parserInfo->parseTree;
	if (G_traceParser == TRUE) {
		parser->print_tree(parser, parseTree);
	}
	if (parserInfo->parseErrorFound == TRUE) {
		puts("!!! Error of parsing found, cannot continue compiling the program.");
		if (G_exitOnError == TRUE) {
			puts("exit the compiler, due to the error. ");
			exit(2);
		}
	}
	else
		puts(":) Parser is happy, no error is found.\n");

	/*if (G_pause == TRUE)
		pause_msg("Grammar analysis is done. Hit \"Enter\" to continue\n");*/


		/* ---------- now run the analyzer --------------*/
	if (analyzer == NULL)
		analyzer = new_s_analyzer(parseTree);
	else
		analyzer->set_parse_tree(analyzer, parseTree); /* fixed an error */
	puts("Building the symbol table ...");
	analyzer->build_symbol_table(analyzer);
	symbtab = analyzer->get_symbol_table(analyzer);

	if (G_traceAnalyzer == TRUE) {
		fprintf(listing, "\nThe Symbol Table:\n");
		st_print(symbtab);
	}
	if (analyzer->check_semantic_error(analyzer) == TRUE) {
		fprintf(listing, "\n\n!!!Some error is detected in building the symbol table!!! \n\n"
			"It is recommended to fix these errors before other compiling tasks.\n"
			"Now the compiler quits.\n");
		//if (G_pause == TRUE)
			//pause_msg("Hit Return to continue.\n");
		goto END;
	}
	else {
		puts("Good:  no error is found by the semantic analyzer when building the symbol table. ");
		//if (G_pause == TRUE)
			//pause_msg("Hit Return to continue.\n");
	}
	//	analyzer->clear_error_status(analyzer);

	puts("Doing type checking ...");
	analyzer->type_check(analyzer);

	if (analyzer->check_semantic_error(analyzer) == TRUE) {
		fprintf(listing, "\n\n!!!Some error is detected during type checking!!! \n\n"
			"It is recommended to fix these errors before other compiling tasks.\n"
			"Now the compiler quits.\n");
		//if (G_pause == TRUE)
			//pause_msg("Hit Return to continue.\n");
		goto END;
	}
	else {
		puts("\nGood:  no error is found by the semantic analyzer when doing type checking.");
		//if (G_pause == TRUE)
		//	pause_msg("Hit Return to continue.\n");
	}

	//END: /* Done with compiling a program */
		//st_free(symbtab);
		//symbtab = NULL;
		//analyzer->clear(analyzer);
		//parser->free_tree(parser, parseTree);
END:
	free_tree(parseTree);
	parseTree = NULL;
	free_token_list(theTokenList);
	//theTokenList.head = theTokenList.tail = NULL;

	//puts("Bye.");
	fclose(InputFile);
	system("pause");
	return 0;
}
