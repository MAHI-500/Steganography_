
#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>
#include "common.h"

/* Function Definitions */
Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)
{

    /*check whether the 2nd passed argument is .bmp file extension or not*/
	if (strcmp(strstr(argv[2],"."),".bmp")==0)
	{
		/*store the file name in the file pointer src_image_fname*/
		decInfo -> src_image_fname=argv[2];
	}
	else
	{
		return e_failure;
	}
    
	/*check whether we have passed the 3rd argument are not if not passed then give default name as output*/
	if (argv[3]==NULL)
	{
		strcpy(decInfo -> output_fname,"output");
	}
	else
	{
		/*store the file name in the output_fname*/
		strcpy(decInfo -> output_fname,argv[3]);
	}

	return e_success;


}


#if 1
Status do_decoding(DecodeInfo *decInfo)
{
	//function calls
	printf("INFO : Opening files\n");
	if (_open_files(decInfo)==e_success)
	{
		printf("INFO : Open files are done\n");
		printf("INFO : Opened output.bmp\n");
		fseek(decInfo -> fptr_src_image,54,SEEK_SET);	
		printf("INFO : Decoding magic string\n");
		if (decode_magic_string(MAGIC_STRING, decInfo)==e_success)

		{
			//printf("INFO : Decoding magic string\n");
			printf("INFO : Done");
			printf("INFO : Decoding of file extn size\n");
			if ((decode_secret_file_extn_size(&(decInfo -> size_extn),decInfo -> fptr_src_image,decInfo -> fptr_output)==e_success))
			{
				//printf("%d\n", decInfo -> size_extn);
				//printf("INFO : Decoding of file extn size\n");
				printf("INFO : Done\n");
				printf("INFO : Decoding of file extn\n");
				//if (decode_secret_file_extn(".txt", decInfo)==e_success)
				if (decode_secret_file_extn(decInfo -> extn_output_file, decInfo)==e_success)
				{
					//printf("%s\n", decInfo -> extn_output_file);
					//printf("INFO : Decoding of file extn\n");
					printf("INFO : Done\n");
					printf("INFO : Output File not mentioned. Creating output.txt as default \n");
				    printf("INFO : Opened output.txt\n");
				    printf("INFO : Opening of files\n");
					if (open_output_file(decInfo)==e_success)
					{
						printf("INFO : Done\n");
						printf("INFO : Decoding of file size success\n");
						if ((decode_secret_file_size(&(decInfo -> size_output_file),decInfo))==e_success)	
						{
					      	//printf("INFO : Decoding of file size success\n");
							printf("INFO : Done\n");
							printf("INFO : Decoding of file data\n");
							if ((decode_secret_file_data(decInfo))==e_success)
							{
								printf("INFO : Done\n");
								//printf("INFO : ##Decoding of file data is success##\n");
								return e_success;
							}
							else
							{
								printf("INFO : Decoding of file data is not success\n");
								return e_failure;
							}
						}
						else
						{
							printf("INFO : Decoding of file size not success\n");
						}
					}
					else
					{
						printf("INFO : OPening of output file is not success\n");
					}
				}
				else
				{
					printf("INFO : Decoding of file extn is not success\n");
				}
			}

			else
			{
				printf("INFO : file extension size decoding is not success\n");
			}

		}
		else
		{
			printf("INFO : The magic string decoding is not success\n");
		}
	}
	else
	{
		printf("INFO : Open files are failure\n");
	}

}


#if 0
uint _get_file_size(FILE *fptr)
{

	fseek(fptr, 0,SEEK_END);
	return ftell(fptr);

}

#endif

#if 1


/*decoding the magic string*/
Status decode_magic_string( char *magic_string, DecodeInfo *decInfo)
{
	/*decode the magic string using the decode_image_to_data*/

	decode_image_to_data(MAGIC_STRING, strlen(MAGIC_STRING),decInfo ->fptr_src_image,decInfo ->fptr_output);
	if (strcmp(MAGIC_STRING,magic_string))      // we need to compare the original magic string with the decoded magic string
	{
		return e_failure;
	}
	return e_success;
}
#endif 
#if 1

/*Decoding the secret file extension size*/
Status decode_secret_file_extn_size(int *size, FILE *fptr_src_image,FILE *fptr_output)
{

	/*take one char[32] array of size 32 since int having 4 bytes *8=32
	  read the encoded values from the fptr_source_image
	  decode_size_to_lsb
	   */

	char str[32];
	fread(str,32,1,fptr_src_image);
	decode_size_to_lsb(size,str);
	//	fwrite(str,32,1,fptr_output);
	return e_success;
}
#endif
#if 1
Status decode_secret_file_extn( char *file_extn, DecodeInfo *decInfo)
{
	/*
	   we don't know the file extension so we will take one pointer and decode the file extension and store that file in the file_extn and we will store the last value of the array with the null character
	 */
	int i;
	char str[8];
	for( i=0; i<decInfo -> size_extn; i++)
	{
		fread(str,8,1,decInfo -> fptr_src_image);
		decode_lsb_of_byte(&file_extn[i],str);
	}
	file_extn[i] = '\0';
	return e_success;

}
#endif

#if 1
Status decode_secret_file_size(int *file_size,DecodeInfo * decInfo)
{

	/*take one char array[32] array of size 32 since int having 4 bytes *8=32
	  read the encoded values from the fptr_source_image
	  decode_size_to_lsb
	  here we dont know the size of the file size then we will decode the file size and create the file with that size with the 
	  extension that we decoded from the decode_file_extn function
	  */

	char str[32];
	fread(str,32,1,decInfo -> fptr_src_image);
	decode_size_to_lsb(file_size,str);
	//fwrite(str,32,1,decInfo -> fptr_output);

	return e_success;

}
#endif 

#if 1
Status decode_secret_file_data(DecodeInfo *decInfo)
{

	/*we will decode the file that we have passed from the command line with the capacity of decINfo -> size_output_file
	  now do decoding of the information and we will write the information in the output file
	 */

	char arr[decInfo -> size_output_file];
	char str[8];
	for(int i=0; i<decInfo -> size_output_file; i++)
	{
		fread(str,8,1,decInfo -> fptr_src_image);
		decode_lsb_of_byte(&arr[i],str);
	}
	//printf("%c\n",*data);
	//printf("%s\n", arr);
	fwrite(arr,decInfo -> size_output_file,1,decInfo -> fptr_output);
	return e_success;

}
#endif
#if 1
Status decode_image_to_data(char *data, int size,FILE *fptr_src_image,FILE *fptr_output)
{

	/*
	   create an array of size 8 since 1 byte requires 8 bytes if data
	   declare an array of size=size+1, to store the decoded magic string
	   iterate the loop for the the size times since the size represents the no of characters
	   now read the data from the fptr_src_image and store it in the str
	   decode_lsb_of_byte
	   */

	char str[8];
	char ch[size+1];// to store the magic_string we should give the size as 1 extra
	for(int i=0; i<size; i++)
	{
		fread(str,8,1,fptr_src_image);
		decode_lsb_of_byte(&ch[i],str);
	}
	//printf("%c\n",*data);
	return e_success;

}
#endif

#if 1
Status decode_lsb_of_byte(char *data, char *image_buffer)
{
	/*take one character whose value is 0
	  now decode, in this we will get the lsb and store it in the msb of the character
	  we will repeat it for 8 times since character
	 
	 */
	*data = 0;
	for(int i=7; i>=0; i--)
	{
		*data=*data|((image_buffer[7-i] & 1)<<i);	
	}
//	printf("%c\n",*data);
	return e_success;
}
#endif

#if 1
Status decode_size_to_lsb(int *size, char *image_buffer)
{

	/*take one integer whose value is 0
	  now decode, in this we will get the lsb and store it in the msb of the character
	  we will repeat it for 32 times since int
	 
	 */
	*size=0;
	for(int i=31; i>=0; i--)
	{
		*size=(*size)|((image_buffer[31-i]&1)<<i);
	}
	//printf("%d\n",*size);
	return e_success;
}

#endif


Status open_output_file(DecodeInfo *decInfo)
{
	/*Before opening of the output file we have merge the filename we have passed in the command line with the decoded file extension
	 now open the file in the write mode and store it in the fptr_output 
	 if the file is not opened then print the error message
	 */
	strcat(decInfo -> output_fname,decInfo -> extn_output_file);
	decInfo->fptr_output = fopen(decInfo->output_fname, "w");
	// Do Error handling
	if (decInfo->fptr_output== NULL)
	{
		perror("INFO : fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_fname);

		return e_failure;
	}
	return e_success;
}

Status _open_files(DecodeInfo *decInfo)
{
	// open the source image file
	decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r");
	// Do Error handling
	if (decInfo->fptr_src_image == NULL)
	{
		perror("INFO : fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname);

		return e_failure;
	}
	return e_success;
}

#endif
