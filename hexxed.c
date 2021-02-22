/************************************************************************
*                                                                       *
*            _     _ _______ _    _  _    _ _______ _____               *
*           | |   | (_______) \  / /\ \  / (_______|____ \              *
*           | |__ | |_____   \ \/ /  \ \/ / _____   _   \ \             *
*           |  __)| |  ___)   )  (    )  ( |  ___) | |   | |            *
*           | |   | | |_____ / /\ \  / /\ \| |_____| |__/ /             *
*           |_|   |_|_______)_/  \_\/_/  \_\_______)_____/              *
*                                                                       *
* PURPOSE:                                                              *
*  Hexdump a file in Big Endian notation                                *
*                                                                       *
* FILE REFERENCES:                                                      *
*                                                                       *
* Name              I/O                     Description                 *
* ----              ---                     -----------                 *
*                                                                       *
* EXTERNAL VARIABLES:                                                   *
* Source: < >                                                           *
*                                                                       *
* Name        Type            I/O           Description                 *
* -----       ----            ---           -----------                 *
*                                                                       *
* EXTERNAL REFERENCES:                                                  *
*                                                                       *
* Name            Description                                           *
* ----            -----------                                           *
* <stdio.h>       printf(), fgetc(), fopen(), fclose(), putc()          *
*                                                                       *
* ABNORMAL TERMINATION CONDITIONS, ERROR AND WARNING MESSAGES:          *
*                                                                       *
* Error: File not found                                                 *
*                                                                       *
* ASSUMPTIONS, CONSTRAINTS, RESTRICTIONS:                               *
*                                                                       *
*                                                                       *
* NOTES:                                                                *
*                                                                       *
* REQUIREMENTS/FUNCTIONAL SPECIFICATIONS REFERENCES:                    *
* _CRT_SECURE_NO_WARNINGS - Windows only                                *
*                                                                       *
* DEVELOPMENT HISTORY:                                                  *
*                                                                       *
* Date          Author      Change Id Release     Description Of Change *
* -----       ---------     -----------------     --------------------- *
* 2/21/2021   Carl-Be            1.0              First Edit/Finished   *
*                                                                       *
* ALGORITHM (PDL)                                                       *
*                                                                       *
************************************************************************/

/*********************************
*                                *
*          Includes              *
*                                *
**********************************/
#include <stdio.h>

/*********************************
*                                *
*           Macros               *
*                                *
**********************************/
#define HEX_ARRAY_SIZE 10 /*Size of the arrary holding the inputted chars*/
#define INITAL_LINE_SIZE 9/*The initial line size of each printed out hexdump line*/
#define PADDING_RIGHT 34/*The amount of padding needed beofre printing ASCII*/
#define EXIT_ERROR -1 /*Indicates that an arror has occured*/
#define CORRECTIVE_COUNT (charCount - 1) /*Used for calculations*/
#define HEADER "\n\
 _     _ _______ _    _  _    _ _______ _____     \n\
| |   | (_______) \\  / /\\ \\  / (_______|____ \\    \n\
| |__ | |_____   \\ \\/ /  \\ \\/ / _____   _   \\ \\   \n\
|  __)| |  ___)   )  (    )  ( |  ___) | |   | |  \n\
| |   | | |_____ / /\\ \\  / /\\ \\| |_____| |__/ /   \n\
|_|   |_|_______)_/  \\_\\/_/  \\_\\_______)_____/    \n"



/*********************************
*                                *
*     Typedefs & Structures      *
*                                *
**********************************/
typedef unsigned int hex;/*This repersents a hex value outputted to the console*/

/*********************************
*                                *
*        Global Variables        *
*                                *
**********************************/

/*********************************
*                                *
*   Declear Function Prototypes  *
*                                *
**********************************/

/*Opens the file in the correct mode*/
void openFile(char* path, char* mode);

/*Reads the file*/
void readFile(FILE* thisFile, char* path);

/*Prints the ascii values at the end of the row*/
void printAscii(hex* hexCodes);

/*Prints the hex dump*/
int printHex(int charCount, int contLineChars, hex ch, hex* hexCodes);

/*
Prints any additional padding needed for the last row
before the ascii is printed. 
*/
void printPadding(int contLineChars);

/*Decides wether or not to print a new row*/
void printStartOfRowLogic(int remainder, int charCount);

/*
Checks to see if hex values are done printing so that 
ASCII values can be appended to the end of the row. 
*/
int endOfHexCheck(hex* hexCodes, int charCount, int contLineChars); 	

/************************************************************************/

/************************************************************************
*                                                                       *
* FUNCTION NAME: main                                                   *
*                                                                       *
* ARGUMENT LIST:                                                        *
*                                                                       *
* ARGUMENT        TYPE       I/O            DESCRIPTION                 *
* --------        ----       ---            -----------                 *
*  argc           int         I             CLI argument count          *
*                                                                       *
*  argv           char**      I             CLI argument pointer to the *
*                                           CLI input string pointers.  *
*                                                                       *
* RETURNS:                                                              *
* int 0                                                                 *
*                                                                       *
************************************************************************/
int main(int argc, char** argv)
{
	char* path = argv[1];/*Take the file name from stdin*/
	const char* READ_MODE = "r";/*Read the file*/

	openFile(path, READ_MODE);/*Open the file for reading*/

	return 0;/*Exit normal*/
}

/************************************************************************/

/************************************************************************
*                                                                       *
* FUNCTION NAME: openFile                                               *
*                                                                       *
* ARGUMENT LIST:                                                        *
*                                                                       *
* ARGUMENT        TYPE       I/O            DESCRIPTION                 *
* --------        ----       ---            -----------                 *
* path            char*       I             The path of the file.       *
*                                                                       *
* mode            char*      I/O            The current file mode.      *
*                                                                       *
* RETURNS:                                                              *
* void                                                                  *
*                                                                       *
************************************************************************/

void openFile(char* path, char* mode) {

	/*
		Opens a file with error checking
	*/

	FILE* filePointer; /*Declear the file pointer*/

	/*Check for null return value when opening the file*/
	if ((filePointer = fopen(path, mode)) == NULL)
	{
		printf("File not found\n");/*Print error message to user*/
		exit(EXIT_ERROR); /*Error occruded exit program with exit code -1*/
	}
	else
	{
		printf("%s\n\n", HEADER); /*Print Success Message*/
		readFile(filePointer, path);/*Read the file*/
	}
}

/************************************************************************/

/************************************************************************
*                                                                       *
* FUNCTION NAME: readFile                                               *
*                                                                       *
* ARGUMENT LIST:                                                        *
*                                                                       *
* ARGUMENT        TYPE       I/O            DESCRIPTION                 *
* --------        ----       ---            -----------                 *
* thisFile        File*       I             Points to the current file  *
*                                           stream.                     *
*                                                                       *
* path            char*       O             The path of the file.       *
*                                                                       *
* RETURNS:                                                              *
* int 0                                                                 *
*                                                                       *
************************************************************************/

void readFile(FILE* thisFile, char* path) {

	/*
		Reads the file to stdout in hexdump formatting
	*/

	hex ch; /*Declear ch*/
	int charCount = 1;/*start char count at one*/
	hex hexCodes[HEX_ARRAY_SIZE] = {'\0'};/*This array holds the chars from the file to print the ascci text*/
	int contLineChars = INITAL_LINE_SIZE;/*Keeps track of the amount of chars in a row*/

	/*Big Endian Hex Dump*/
	while ((ch = fgetc(thisFile)) != EOF)
	{	
		int index = CORRECTIVE_COUNT % HEX_ARRAY_SIZE;/*Calculate the correct index to place ch into*/
		hexCodes[index] = ch;/*Place ch into the calculated index of ascii*/
		printStartOfRowLogic(index, charCount);/*Decides to print a new row or not*/
		contLineChars = printHex(charCount, contLineChars, ch, hexCodes);/*Print hex values*/
		charCount = charCount + 1; /*add one to count*/
	}
	
	printPadding(contLineChars);/*Print the needed additional padding*/
	printAscii(hexCodes);/*Print the last bit of ASCII values to stdout*/

	/*Print the result to the user*/
	fclose(thisFile);/*Close the file*/
}

/************************************************************************/

/************************************************************************
*                                                                       *
* FUNCTION NAME: printAscii                                             *
*                                                                       *
* ARGUMENT LIST:                                                        *
*                                                                       *
* ARGUMENT        TYPE       I/O            DESCRIPTION                 *
* --------        ----       ---            -----------                 *
* hexCodes        hex*        O             holds the hex codes that are*
*                                           to be printed out to stdout.*
*                                                                       *
* RETURNS:                                                              *
* void                                                                  *
*                                                                       *
************************************************************************/

void printAscii(hex* hexCodes) {
	
	/*
		Prints ASCII values to the end of the current row with white space checking
	*/

	for (int i = 0; i < HEX_ARRAY_SIZE; i++) {

		/*If whitespace char print a period. Else print the ASCII value*/
		if (hexCodes[i] == ' ' || hexCodes[i] == '\n' || hexCodes[i] == '\t' || hexCodes[i] == '\r') {
			printf(".");
			hexCodes[i] = '\0';/*Set this index back to null*/
		}
		else {
			printf("%c", hexCodes[i]);
			hexCodes[i] = '\0';/*Set this index back to null*/
		}
	}
	printf("\n");/*Print a newline to start a new row*/
}

/************************************************************************/

/************************************************************************
*                                                                       *
* FUNCTION NAME: printPadding                                           *
*                                                                       *
* ARGUMENT LIST:                                                        *
*                                                                       *
* ARGUMENT        TYPE       I/O            DESCRIPTION                 *
* --------        ----       ---            -----------                 *
* contLineChars   int                       Used to calcualte how much  *
*                                           remaining padding to print  *
*                                           to stdout.                  *
*                                                                       *
* RETURNS:                                                              *
* void                                                                  *
*                                                                       *
************************************************************************/
void printPadding(int contLineChars) {

	/*
		Calculates the remaining padding needed for the last row before
		the ASCII values and prints it to stdout
	*/

	int additionalPadding = PADDING_RIGHT - contLineChars; /*Calculates the needed padding*/
	for (int i = 0; i < additionalPadding; i++) {
		printf(" ");/*Print the needed padding to stdout*/
	}
}

/************************************************************************/

/************************************************************************
*                                                                       *
* FUNCTION NAME: endOfLineCheck                                         *
*                                                                       *
* ARGUMENT LIST:                                                        *
*                                                                       *
* ARGUMENT        TYPE       I/O            DESCRIPTION                 *
* --------        ----       ---            -----------                 *
* hexCodes        hex*        O             holds the hex codes that are*
*                                           to be printed out to stdout.*
*                                                                       *
* contLineChars   int                       Used to calcualte how much  *
*                                           remaining padding to print  *
*                                           to stdout.                  *
*                                                                       *
* charCount       int                       The count of read bytes so  *
*                                           far.                        *
*                                                                       *     
* RETURNS:                                                              *
* int contLineChars                                                     *
*                                                                       *
************************************************************************/

int endOfHexCheck(hex* hexCodes, int charCount, int contLineChars) {
	
	/*Checks to see if the program needs to print ASCII or not*/

	if ((charCount % HEX_ARRAY_SIZE) == 0 && charCount != 1) {
		contLineChars = INITAL_LINE_SIZE; /*Reset line count*/
		printAscii(hexCodes);/*Print ASCII to stdout*/
	}

	return contLineChars;
}

/************************************************************************/

/************************************************************************
*                                                                       *
* FUNCTION NAME: printHex                                               *
*                                                                       *
* ARGUMENT LIST:                                                        *
*                                                                       *
* ARGUMENT        TYPE       I/O            DESCRIPTION                 *
* --------        ----       ---            -----------                 *
* hexCodes        hex*        O             holds the hex codes that are*
*                                           to be printed out to stdout.*
*                                                                       *
* contLineChars   int                       Used to calcualte how much  *
*                                           remaining padding to print  *
*                                           to stdout.                  *
*                                                                       *
* charCount       int                       The count of read bytes so  *
*                                           far.                        *
*                                                                       *
* ch              hex                       The current hex char from   *
*                                           stdin.                      *
*                                                                       *
* RETURNS:                                                              *
* int contLineChars                                                     *
*                                                                       *
************************************************************************/

int printHex(int charCount, int contLineChars,  hex ch, hex* hexCodes) {

	/*
	Prints the hex values to stdout. If the charCount is an even number
	print the hex value with a trailing space char. Else print the 
	hex value with no trailing space char.
	*/

	if (!(charCount % 2)) {
		printf("%.2X ", ch);/*Print hex char to stdout*/
		contLineChars += 3;
		/*Check to see if the program is ready to print ASCII values or not*/
		contLineChars = endOfHexCheck(hexCodes, charCount, contLineChars);
	}
	else {
		printf("%.2X", ch);/*Print hex char to stdout*/
		contLineChars += 2;
	}

	return contLineChars;
}

/************************************************************************/

/************************************************************************
*                                                                       *
* FUNCTION NAME: printStartOfRowLogic                                   *
*                                                                       *
* ARGUMENT LIST:                                                        *
*                                                                       *
* ARGUMENT        TYPE       I/O            DESCRIPTION                 *
* --------        ----       ---            -----------                 *
* charCount       int                       The count of read bytes so  *
*                                           far.                        *
*                                                                       *
* remainder       int                       The remainder from the index*
*                                           modulus calculation.        *
*                                                                       *
* RETURNS:                                                              *
* void                                                                  *
*                                                                       *
************************************************************************/

void printStartOfRowLogic(int remainder, int charCount) {

	/*
     Prints the start of a new row if needed.
	*/

	if (remainder == 0) {
		printf("%.8d: ", CORRECTIVE_COUNT);/*Print the start of the row*/
	}
}