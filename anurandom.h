/***********************************************
 *This file is provided along with it's license*
 *file "anurandom_C_implementation-COPYING".   *
 *Contained therein are the licenses specific  *
 *to this anurandom C programming language     *
 *implementation.  The license may be opened   *
 *for viewing with any text editor.            *
 ***********************************************/


#ifndef _ANURANDOM_H_
#define _ANURANDOM_H_

/*stdio.h included in curl.h
#include <stdio.h>
*/
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/easy.h>

#define THIS_IS_MADNESS (sizeof(char)*2049)
#define THIS_IS_SPARTA (sizeof(char)*1025)

#define SERVE return

/*https://qrng.anu.edu.au	<- Australian National University Quantum Random Number Generator webpage	*/
#define RAW_BIN "https://qrng.anu.edu.au/ran_bin.php"		/*1 byte binary					*/
#define RAW_HEX "https://qrng.anu.edu.au/ran_hex.php"		/*1 byte hex					*/
#define RAW_COLOR "https://qrng.anu.edu.au/ran_col.php"		/*1 hexadecimal color value			*/
#define BIN_BLOCK "https://qrng.anu.edu.au/RawBin.php"		/*1 1024 bit block				*/
#define HEX_BLOCK "https://qrng.anu.edu.au/RawHex.php"		/*1 1024 hex block				*/
#define CHAR_BLOCK "https://qrng.anu.edu.au/RawChar.php"	/*1 1024 char block				*/

/*Security is not currently a concern.*/
#define SKIP_PEER_VERIFICATION
#define SKIP_HOSTNAME_VERIFICATION

typedef struct memory_struct
{
	char* memory;
	size_t size;
}mem_struct;

size_t StoreCB (char *contents, size_t size, size_t nmemb, void *userpntr);
char* extract_block (mem_struct steven_segal, char* schnitzel);

/*crap*/
void help_with_help(char* CharlesBronson);
void help_text(char* NoIdea);

#endif