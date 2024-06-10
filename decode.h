
#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * decoding output file to secret text file
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
//#define MAX_FILE_SUFFIX 5

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;
 

    /* Output File Info */
    char output_fname[50];
	int output_extn_size;
    FILE *fptr_output;
    char extn_output_file[5];
    char output_data[MAX_SECRET_BUF_SIZE];
    int size_output_file;
    int size_extn;

    /* Magic string info */
    char magic_string[3];

} DecodeInfo;


/* decoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);


//uint _get_file_size(FILE *fptr);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status _open_files(DecodeInfo *decInfo);

/* Store Magic String */
Status decode_magic_string( char *magic_string, DecodeInfo *decInfo);

/* decode secret file extension size*/

Status decode_secret_file_extn_size(int *size, FILE *fptr_src_image,FILE *output_fname);

/* decode secret file extenstion */
Status decode_secret_file_extn( char *file_extn, DecodeInfo *decInfo);

/*open the output file*/
Status open_output_file(DecodeInfo *decInfo);

/* decode secret file size */
Status decode_secret_file_size(int *file_size, DecodeInfo *decInfo);

/* decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* decode function, which does the real encoding */
Status decode_image_to_data(char *data, int size, FILE *fptr_src_image, FILE *fptr_output);

/* decode a byte into LSB of image data array */
Status decode_lsb_of_byte(char *data, char *image_buffer);

/* decode a size into LSB of image data array */
Status decode_size_to_lsb(int *size,char *image_buffer);

//Status decode_string(char *string, int size,FILE * fptr_output);
#endif

