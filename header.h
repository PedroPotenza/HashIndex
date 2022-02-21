#ifndef HEADER_H
#define HEADER_H

#define true 1
#define false 0
#define SizeOfREGISTER 156
#define SizeOfINDEXREGISTER 10
#define maxTestCases 15
#define maxRegisters 13

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int inserted = 0;
int searched = 0;
int removed  = 0;

typedef struct s_Key {
    char ClientId[3];
    char MovieId[3];
} KEY;

typedef struct s_Register {
    KEY Id;
    char ClientName[50];
    char MovieName[50];
    char Genre[50];
} REGISTER;

typedef struct s_IndexRegister {
    char Id[5];
    int rrn;
} INDEXREGISTER;

#include "code/util.c"
#include "code/insert.c"

// FILE * readPositions();

#endif