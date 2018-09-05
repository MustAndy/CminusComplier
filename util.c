/*
*   Name: Wang,jingqing
*   ID: 1509853G-I011-0202
*   CLASS: 1709-CS106
*   File:util.c
*/

#include"util.h"
#include"globals.h"
#include "libs.h"


const char * string_clone(const char * s)
{
	int n;
	char * t;

	if (s == NULL) return NULL;

	n = strlen(s) + 1;
	t = (char *)malloc(n*sizeof(char));

	strcpy(t, s);

	return t;
}