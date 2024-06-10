
/*
NAME          : KANCHI VENKATA MAHITH
DATE          : 20-12-2023
DESCRIPTION   : Steganography project
SAMPLE INPUT  :
SAMPLE OUTPUT :
 */
#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "string.h"
#include "decode.h"

int main(int argc, char *argv[])
{
	if (argc>=2)
	{
		int ret=check_operation_type(argv);
		EncodeInfo encInfo;
		DecodeInfo decInfo;
		if(ret==e_encode)
		{
			if (argc>=4)
			{
				printf("INFO : Selected Encoding\n");
				if (read_and_validate_encode_args(argv,&encInfo)==e_success)

				{
					printf("INFO : read_and_validate_encode_args successfuly done\n");

					if (do_encoding(&encInfo)==e_success)
					{
						printf("INFO : ##Encoding success##\n");
					}
					else
					{
						printf("INFO : Encoding failure\n");

					}
				}
				else
				{
					printf("INFO : Error : Enter ./a.out -e beautiful.bmp secret.txt <output>\n");
				}
			}
			else
			{

				printf("INFO : Error : Enter ./a.out -e beautiful.bmp secret.txt <output>\n");
			}

		}
		else if (ret==e_decode)
		{
			if (argc>=3)
			{
				printf("INFO : Selected decoding\n");
				if(read_and_validate_decode_args(argv, &decInfo)==e_success)
				{
					printf("INFO : Read and validate success\n");
					if (do_decoding(&decInfo)==e_success)
					{
						printf("INFO : ##Decoding success##\n");

					}
					else
					{
						printf("INFO : decoding is not success\n");
					}
				}
				else
				{
					//printf("INFO : Wrong files entered\n");
					printf("INFO : Error:  Enter ./a.out -d output.bmp <filename>\n");
				}
			}
			else
			{
				printf("INFO : Error:  Enter ./a.out -d output.bmp <filename>\n");
			}

		}
		else
		{
			printf("INFO : Error\n");
		}
	}
	else
	{
		printf(" Error  : Enter the minimum 2 arguments and max of 4 arguments\n");

		//printf("INFO : Error : Enter ./a.out -e beautiful.bmp secret.txt <output>\n");
		//printf("INFO : Error:  Enter ./a.out -d output.bmp <filename>\n");

	}

}
OperationType check_operation_type(char *argv[])
{
	if (strcmp(argv[1],"-e")==0)
	{
		return e_encode;
	}
	else if (strcmp (argv[1],"-d")==0)
	{
		return e_decode;
	}	

}
