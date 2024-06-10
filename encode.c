#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include "common.h"

/* Function Definitions */
Status read_and_validate_encode_args(char *argv[],EncodeInfo *encInfo)
{
	/*
	   check argv[2] is .bmp file
	   yes -> store it in encInfo -> src_image_fname
	   no -> return e_failure

	   check argv[3] is .txt file 
	   yes -> store it in encInfo -> secret_fname
	   no -> return e_failure

	   check argv[4] is passed or not

	   passed -> check argv[4] is .bmp
	   yes -> store in encInfo -> stego_image_fname
	   no  -> return e_failure


	   not passed -> encInfo -> stego_image_fname= "default.bmp"

	   return e_success
	 */

	if (strcmp(strstr(argv[2],"."),".bmp")==0)
	{
		encInfo -> src_image_fname=argv[2];
	}
	else
	{
		return e_failure;
	}


	//if ((strcmp(strstr(argv[3],"."),".txt"))==0)
	if ((strcmp(strstr(argv[3],"."),".txt")==0|| strcmp(strstr(argv[3],"."),".c")==0|| strcmp(strstr(argv[3],"."),".sh")==0)==1)
	{
		encInfo -> secret_fname=argv[3];
		strcpy(encInfo -> extn_secret_file, strstr(argv[3], "."));
	}
	else
	{
		return e_failure;
	}

	if (argv[4]==NULL)
	{
		encInfo -> stego_image_fname="default.bmp";
	}

	else if (strcmp(strstr(argv[4],"."),".bmp")==0)
	{
		encInfo -> stego_image_fname=argv[4];
	}
	else
	{
		return e_failure;
	}

	return e_success;


}

Status do_encoding(EncodeInfo *encInfo)
{

	//function calls
		printf("INFO : opening files\n");
	if (open_files(encInfo)==e_success)
	{
		//printf("INFO : opening files\n");
		printf("INFO : Opened beautiful.bmp\n");
		printf("INFO : Opened secret.txt\n");
		printf("INFO : Opened stego_image.bmp\n");
		printf("INFO : Done\n");
		printf("INFO : ##Encoding Procedure Started##\n");

		printf("INFO : Checking for beautiful.bmp capacity to handle secret.txt\n");
		if(check_capacity(encInfo)==e_success)
		{
			//printf("INFO : Checking for beautiful.bmp capacity to handle secret.txt\n");
			printf("INFO : Done. Found OK\n");

			printf("INFO : Copying Image Header\n");

			if ((copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image))==e_success)
			{
				//printf("INFO : Copying Image Header\n");
				printf("INFO : Done\n");

				printf("INFO : Encoding Magic String Signature\n");
				if (encode_magic_string(MAGIC_STRING, encInfo)==e_success)
				{
					//printf("INFO : Encoding Magic String Signature\n");
					printf("INFO : Done\n");

					printf("INFO : Encoding secret.txt File extension size\n");
					if (encode_secret_file_extn_size(strlen(encInfo -> extn_secret_file), encInfo -> fptr_src_image,encInfo -> fptr_stego_image)==e_success)
					{
						printf("INFO : Done \n");
					    printf("INFO : Encoding secret.txt File extension\n");
						if (encode_secret_file_extn(encInfo -> extn_secret_file,encInfo)==e_success)
						{
							printf("INFO : ENcoding secret.txt File size\n");
							printf("INFO : Done\n");
							if (encode_secret_file_size(encInfo -> size_secret_file,encInfo)==e_success)
							{
								//printf("INFO : Encoding secret.txt File Size\n");
								printf("INFO : Done\n");
								printf("INFO : Encoding secret.txt File Data\n");
								if(encode_secret_file_data(encInfo)==e_success)
								{
									//printf("INFO : Encoding secret.txt File Data\n");
									printf("INFO : Done\n");
									printf("INFO : Copying Left Over Data\n");
									if (copy_remaining_img_data(encInfo -> fptr_src_image , encInfo -> fptr_stego_image)==e_success)
									{
										//printf("INFO : Copying Left Over Data\n");
										printf("INFO : Done\n");
										return e_success;
									}
									else
									{
										printf("INFO : Copy of remaining data not success\n");
										return e_failure;
									}

								}
								else
								{
									printf("INFO : Encoding of file data is not success\n");
								}
							}
							else
							{
								printf("INFO : encoding of file size is not success\n");
							}

						}
						else
						{
							printf("INFO : encoding of secret file extension is not success\n");
						}

					}
					else
					{
						printf("INFO : encoding of secret file extension size is not success\n");
					}

				}
				else
				{
					printf("INFO : The magic string encoding is not success\n");
				}

			}
			else
			{
				printf("INFO : File copy is not success\n");
			}


		}
		else
		{
			printf("INFO : The beautiful.bmp capacity is lesser than capacity\n");

		}

	}
	else
	{
		printf("INFO : Open files are Failure\n");
	}

}

Status check_capacity(EncodeInfo *encInfo)
{
	//find the bmp file size
	encInfo -> image_capacity =get_image_size_for_bmp(encInfo ->fptr_src_image);
	encInfo -> size_secret_file=get_file_size(encInfo ->fptr_secret);

	if ((encInfo -> image_capacity)> ((strlen(MAGIC_STRING)*8)+32+32+32+(encInfo -> size_secret_file *8)))
	{
		return e_success;

	}
	else
	{
		return e_failure;
	}

}

uint get_file_size(FILE *fptr)
{
	//move the file pointer to last
	//use ftell to get file pointer position (return ftell)

	fseek(fptr, 0,SEEK_END);
	return ftell(fptr);

}
/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image)
{
	/*
	   move fptr_src_image to first position 
	   char str[54];
	   use fread func -> read 54 bytes
	   fwrite -> write 54 bytes in fptr_dest_image
	   return e_success

	 */

	rewind(fptr_src_image);
	char str[54];
	fread(str,54,1,fptr_src_image);
	fwrite(str,54,1,fptr_stego_image);

	return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{

	encode_data_to_image(MAGIC_STRING, strlen(MAGIC_STRING),encInfo -> fptr_src_image, encInfo -> fptr_stego_image);
	return e_success;
}

Status encode_secret_file_extn_size(int size, FILE *fptr_src_image,FILE *fptr_stego_image)
{
	char str[32];
	//read 32 bytes from fptr_src_image
	//encode_size_to_lsb(size,str)
	//write 32 bytes
	//return e_success
	fread(str,32,1,fptr_src_image);
	encode_size_to_lsb(size,str);
	fwrite(str,32,1,fptr_stego_image);
	return e_success;

}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
	// encode data_to_image
	encode_data_to_image(strstr(encInfo -> secret_fname, "."),strlen(strstr(encInfo -> secret_fname, ".")),encInfo -> fptr_src_image,encInfo -> fptr_stego_image);
	return e_success;

}

Status encode_secret_file_size(long file_size,EncodeInfo * encInfo)
{
	//declare char array -> size 32 bytes
	//read 32 bytes 
	//call encode_size_to_lsb
	//write 32 bytes
	//return e_success
	char str[32];
	fread(str,32,1,encInfo -> fptr_src_image);
	encode_size_to_lsb(file_size,str);
	fwrite(str,32,1,encInfo -> fptr_stego_image);

	return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
	//char arr[size_secret_file];
	//fread(arr,size_secret_file,1,fptr_secret);
	rewind (encInfo -> fptr_secret);
	char arr[encInfo -> size_secret_file];
	fread(arr,encInfo -> size_secret_file,1,encInfo -> fptr_secret);
	//encode_data_to_image(arr,strlen(arr), encInfo -> fptr_src_image,encInfo -> fptr_stego_image);
	encode_data_to_image(arr,encInfo -> size_secret_file, encInfo -> fptr_src_image,encInfo -> fptr_stego_image);
	return e_success;

}

Status copy_remaining_img_data(FILE *fptr_src_image, FILE *fptr_stego_image)
{
	// read 1 byte from fptr_src_image
	// write 1 byte to fptr_stego_image
	// repeat it untill EOF
	char ch;
	while (fread(&ch,1,1,fptr_src_image)>0)
	{
		fwrite(&ch,1,1,fptr_stego_image);
	}
	return e_success;

}


Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
	char str[8];
	for(int i=0; i<size; i++)
	{
		fread(str,8,1,fptr_src_image);
		encode_byte_to_lsb(data[i],str);
		fwrite(str,8,1,fptr_stego_image);

	}
	return e_success;

}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
	for(int i=7; i>=0; i--)
	{
		image_buffer[7-i]=(image_buffer[7-i] & 0xFE) | (data & (1<<i))>>i;

	}
	return e_success;
}

Status encode_size_to_lsb(int size, char *image_buffer)
{
	for(int i=31; i>=0; i--)
	{
		image_buffer[31-i]=((image_buffer[31-i] &(~1)) |((unsigned)(size & (1<<i))>>i) );
	}
	return e_success;
}
uint get_image_size_for_bmp(FILE *fptr_image)
{
	uint width, height;
	// Seek to 18th byte
	fseek(fptr_image, 18, SEEK_SET);

	// Read the width (an int)
	fread(&width, sizeof(int), 1, fptr_image);
	printf("INFO : width = %u\n", width);

	// Read the height (an int)
	fread(&height, sizeof(int), 1, fptr_image);
	printf("INFO : height = %u\n", height);

	// Return image capacity
	return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
	// Src Image file
	encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
	// Do Error handling
	if (encInfo->fptr_src_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

		return e_failure;
	}

	// Secret file
	encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
	// Do Error handling
	if (encInfo->fptr_secret == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

		return e_failure;
	}

	// Stego Image file
	encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
	// Do Error handling
	if (encInfo->fptr_stego_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

		return e_failure;
	}

	// No failure return e_success
	return e_success;
}

