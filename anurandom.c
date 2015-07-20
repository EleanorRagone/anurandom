/***********************************************
 *This file is provided along with it's license*
 *file "anurandom_C_implementation-COPYING".   *
 *Contained therein are the licenses specific  *
 *to this anurandom C programming language     *
 *implementation.  The license may be opened   *
 *for viewing with any text editor.            *
 ***********************************************/



#include "anurandom.h"

int main(int argc, char* argv[])
{
/*
 * Command line options and functionality
 */
	static int opt_chk=0;
	int QRNG_source_type;
	if (argc == 1)
	{
		help_with_help(argv[0]);
		return 0;
	}
	else if (opt_chk=strcmp(argv[1], "--help") == 0)
	{
		help_text(argv[0]);
		return 0;
	}
	else if (opt_chk=strcmp(argv[1], "-rb") == 0)
	{
		QRNG_source_type=1;
	}
	else if (opt_chk=strcmp(argv[1], "-rh") == 0)
	{
		QRNG_source_type=2;
	}
	else if (opt_chk=strcmp(argv[1], "-rc") == 0)
	{
		QRNG_source_type=3;
	}
	else if (opt_chk=strcmp(argv[1], "-bb") == 0)
	{
		QRNG_source_type=4;
	}
	else if (opt_chk=strcmp(argv[1], "-cb") == 0)
	{
		QRNG_source_type=5;
	}
	else if (opt_chk=strcmp(argv[1], "-hb") == 0)
	{
		QRNG_source_type=6;
	}
	else
	{
		printf("Invalid option, no option, or no filename present.  Try \"%s --help\"", argv[0]);
		return 0;
	}

	CURL *curl;
	CURLcode res;
	mem_struct ANU_raw;
	char* ran_data;

	ANU_raw.memory=malloc(1);
	ANU_raw.size=0;

	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if(curl)
	{
		switch (QRNG_source_type)  //possibly better way to do this
		{
			case 1:
				curl_easy_setopt(curl, CURLOPT_URL, RAW_BIN);
				break;
			case 2:
				curl_easy_setopt(curl, CURLOPT_URL, RAW_HEX);
				break;
			case 3:
				curl_easy_setopt(curl, CURLOPT_URL, RAW_COLOR);
				break;
			case 4:
				curl_easy_setopt(curl, CURLOPT_URL, BIN_BLOCK);
				break;
			case 5:
				curl_easy_setopt(curl, CURLOPT_URL, CHAR_BLOCK);
				break;				
			case 6:
				curl_easy_setopt(curl, CURLOPT_URL, HEX_BLOCK);
				break;
		}
		
		/*Function Pointer "StoreCB" manages the required buffer size*/
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, StoreCB);

		/*Data Pointer address &ANU_raw stores downloaded web content */
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&ANU_raw);

/*
 * Next, we skip verification of cert.
 * This is not very secure.
 * If accessing a site with a cert not in your bundle use CURLOPT_CAPATH 
 */
		#ifdef SKIP_PEER_VERIFICATION
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		#endif

/*
 * Next, we skip hostname verification for the cert.
 * Again, less secure, but not important ATM.
 */
		#ifdef SKIP_HOSTNAME_VERIFICATION
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		#endif

		/*Request, check for errors, and cleanup*/
		res = curl_easy_perform(curl);
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		else
		{
			/*Removing this out of personal taste.*/
			//fprintf(stdout, "%lu bytes retrieved\n", (long)ANU_raw.size);
		}
		curl_easy_cleanup(curl);
	}

/*
 * Important stuff going on here.
 * Possible inefficiencies
 * Most probable point of new implementations/modifications/etc..
 */
	
	if (QRNG_source_type > 5)
	{
		ran_data=malloc(THIS_IS_MADNESS);
        	ran_data=extract_block(ANU_raw, ran_data);
		fprintf(stdout, "%s", ran_data);
		free(ran_data);
		
	}
	else if (QRNG_source_type > 3)
	{
		ran_data=malloc(THIS_IS_SPARTA);
        	ran_data=extract_block(ANU_raw, ran_data);
		fprintf(stdout, "%s", ran_data);
		free(ran_data);
	}
	else
	{

		fprintf(stdout, "%s", ANU_raw.memory);
	}
	free(ANU_raw.memory);

	curl_global_cleanup();

	return 0;
}

/*The mysterious StoreCB()*/
size_t StoreCB(char *contents, size_t size, size_t nmemb, void *userpntr)
{
	size_t true_size=size*nmemb;
	mem_struct *mem=(mem_struct*)userpntr;

	mem->memory=realloc(mem->memory, mem->size+true_size+1);
	if (mem->memory == NULL)
	{
		/*out of memory*/
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	memcpy(&(mem->memory[mem->size]), contents, true_size);
	mem->size += true_size;
	mem->memory[mem->size]=0;

	return true_size;
}

char* extract_block (mem_struct steven_segal, char* schnitzel)
{	
	/*Movement*/
	int cntr1=0;
	int cntr2=0;
	static int move=3;

	/*Processing*/
	char td_token[]="<td>";
	char buffer[]={'N', 'U', 'L', 'L', '\0'};
	int match_check;

	/*Perform*/
	/*Find the tag with the data*/
	while ((match_check=strcmp(td_token, buffer))!=0)
	{
		buffer[cntr1]=*steven_segal.memory;
		steven_segal.memory++;
		cntr1++;
		cntr2++;

		if (cntr1>3)
		{
			cntr1=0;

			steven_segal.memory-=move;
			cntr2-=move;

			continue;
		}
	}
	/*Return pointed to address to beginning of block.*/
	steven_segal.memory+=move+1;
	/*Store the block*/
	for (cntr1=0; *steven_segal.memory != '<'; cntr1++)
	{
		*schnitzel=*steven_segal.memory;
		steven_segal.memory++;
		schnitzel++;
	}
	/*Terminate string and then return the pointer to the adress of the start of the string*/
	*schnitzel='\0';
	schnitzel-=cntr1;
	
	SERVE schnitzel; //fuck yeah we do..!
}

/*crap*/
void help_with_help(char* CharlesBronson)
{
	printf("\nUsage: %s [OPTION]\n", CharlesBronson);
	printf("Try: %s --help\n\n");
}
void help_text(char* NoIdea)
{
printf("\nThe data will be sent to STDOUT.\
\nProtip: use redirection to write output to a file\
\nExample: you@your.host:/location/of/executable/%s -rb 1> output.txt\n\
\n---Options are as follows---\n\
\n-rb for 1 byte of ascii binary numbers\
\n-rh for 1 byte of ascii hexadecimal numbers\
\n-rc for 1 ascii hexadecimal color code\
\n-bb for a block of 1024 bits of ascii binary numbers\
\n-cb for a block of 1024 ascii characters\
\n-hb for a block of 1024 hexadecimal ascii numbers\n\n", NoIdea);
}