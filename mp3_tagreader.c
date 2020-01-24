/*****************************************************HEADER FILES***********************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include"types.h"
#include"mp3.h"

/****************************************************************************************************************************************/


/*****************************************************MACROS*****************************************************************************/

#define TAG         "ID3"
#define OFFSET       128
#define SONG          10
#define ARTIST        67
#define ALBUM        110
#define YEAR         129
#define COMMENT      185

/****************************************************************************************************************************************/


/*****************************************************FUNCTION PROTOTYPES****************************************************************/

OperationType check_operation_status(char *argv[]);
Status read_and_validate_args(char *argv[], Mp3Info *mp3Info);
Status open_file(Mp3Info *mp3Info);
Status start_operation(Mp3Info *mp3Info);
Status edit_info(Mp3Info *mp3Info);
Status version_id(Mp3Info *mp3Info);
Status get_details(Mp3Info *mp3Info);
uint verify_flag(Mp3Info *mp3Info);
void move_ptr(Mp3Info *mp3Info);
void print_tag(Mp3Info *mp3Info);
void close_file(Mp3Info *mp3Info);
void song(Mp3Info *mp3Info);

/****************************************************************************************************************************************/

int main(int argc,char *argv[])
{ 
	Mp3Info mp3Info;
	switch(argc)
	{
		case 1:
			printf("NOT ENOUGH ARGUMENTS \n");
			printf("Format : ./a.out [flag] [file.mp3]\n");
			printf("INFO   : ./a.out -h for help\n");
			exit(222);
		case 2:
			printf(".mp3 FILE NOT MENTIONED	\n");
			if(strcmp(argv[1],"-h") == 0)
				goto here;
			else
			{
				printf("Format : ./a.out [flag] [file.mp3]\n");
				printf("INFO   : ./a.out -h for help\n");
				exit(111);
			}
		case 3 ... 5:
			goto here;
		default:
			printf("You have entered more arguments \n");
			printf("Format : ./a.out [flag] [file.mp3]\n");
			printf("INFO   : ./a.out -h for help\n");
			exit(333);
	}

here: switch(check_operation_status(argv))
      {
	      case mp3_display:
		      mp3Info.fname = argv[2];
		      if(read_and_validate_args(argv , &mp3Info) == mp3_success)
		      {
			      if(open_file(&mp3Info) == mp3_success)
			      {
				     if(start_operation(&mp3Info) == mp3_success)
				     {
					close_file(&mp3Info);
				     } 
				     else
				     {
					printf("Opeartion failed \n");
					exit(1);
				     }
			      }
			      else
			      {
				      printf("Opeartion failed \n");
				      exit(2);
			      }
		      }
		      else
		      {
			      printf("Opeartion failed \n");
			      exit(3);
		      }
		      break;

	      case mp3_edit:
		      mp3Info.fname = argv[2];
		      mp3Info.flag = argv[3];
		      mp3Info.data = argv[4];
		      if(read_and_validate_args(argv,&mp3Info) == mp3_success)
		      {
			if(open_file(&mp3Info) == mp3_success)
			{
				if(edit_info(&mp3Info) == mp3_success)
				{
					close_file(&mp3Info);
				}
				else
				{
					printf("operation failed\n");
					exit(4);
				}
			}
			else
			{
				printf("operation failed\n");
				exit(5);
			}
		      }
		      else
		      {
			      printf("operation failed\n");
			      exit(6);
		      }
		      break;

	      case mp3_help:
		      printf("Format : ./a.out [flag] [file.mp3]\n");
		      printf("use the following flags :\n-t  --------> change title\n-a  --------> change artist\n-A  --------> change album\n-y  --------> change year\n-c  --------> change comment\n");
		      break;
      } 
}

OperationType check_operation_status(char *argv[])
{
	if((strcmp(argv[1],"-v")) == mp3_success)
		return mp3_display;
	else if(strcmp(argv[1],"-e") == mp3_success)
		return mp3_edit;
	else if(strcmp(argv[1],"-h") == mp3_success)
		return mp3_help;
	else
		return mp3_unsupported;
}

Status read_and_validate_args(char *argv[] , Mp3Info *mp3Info)
{
	char ext[5];
	strcpy(ext,strstr(mp3Info->fname,".mp3"));
	if(strcmp(ext,".mp3") == mp3_success)
		return mp3_success;
	else
		return mp3_failure;
}

Status open_file(Mp3Info *mp3Info)
{
	mp3Info->fptr = fopen(mp3Info->fname,"rw+");

	if(mp3Info->fptr == NULL)
	{
		perror("fopen");
	        fprintf(stderr, "ERROR: Unable to open file %s\n", mp3Info->fname);

                return mp3_failure;
	}

	return mp3_success;
}

Status start_operation(Mp3Info *mp3Info)
{
	if(version_id(mp3Info) == mp3_success)
	{
		if(get_details(mp3Info) == mp3_success)
		{
			print_tag(mp3Info);
			return mp3_success;
		}
		else
		{
			return mp3_failure;
			exit(4);
		}
	}
	else
	{
		return mp3_failure;
		exit(5);
	}
}

Status version_id(Mp3Info *mp3Info)
{
	fseek(mp3Info->fptr,0,SEEK_SET);
	fread(mp3Info->ver_id,3,1,mp3Info->fptr);
	mp3Info->ver_id[4] = '\0';
	if(strcmp(mp3Info->ver_id,TAG) == mp3_success)
	{
		printf("Version: %s\n",mp3Info->ver_id);
		return mp3_success;
	}
	else
		return mp3_failure;
}

Status get_details(Mp3Info *mp3Info)
{
	fseek(mp3Info->fptr,7,SEEK_CUR);
	move_ptr(mp3Info);
	fread(mp3Info->song_title,mp3Info->buffer-1,1,mp3Info->fptr);
	mp3Info->song_title[mp3Info->buffer-1] = '\0';
	song(mp3Info);

	move_ptr(mp3Info);
	fread(mp3Info->artist,mp3Info->buffer-1,1,mp3Info->fptr);
	mp3Info->artist[mp3Info->buffer-1] = '\0';

	move_ptr(mp3Info);
	fread(mp3Info->album,mp3Info->buffer-1,1,mp3Info->fptr);
	mp3Info->album[mp3Info->buffer-1] = '\0';
	
	move_ptr(mp3Info);
	fread(mp3Info->year,mp3Info->buffer-1,1,mp3Info->fptr);
	mp3Info->year[mp3Info->buffer-1] = '\0';
	
	fseek(mp3Info->fptr,41,SEEK_CUR);
	
	move_ptr(mp3Info);
	fseek(mp3Info->fptr,4,SEEK_CUR);
	fread(mp3Info->comment,mp3Info->buffer-1,1,mp3Info->fptr);
	mp3Info->comment[mp3Info->buffer-1] = '\0';
	
	return mp3_success;
}

void move_ptr(Mp3Info *mp3Info)
{
	fseek(mp3Info->fptr,4,SEEK_CUR);
	fseek(mp3Info->fptr,3,SEEK_CUR);
	fread(&mp3Info->buffer,1,1,mp3Info->fptr);
	fseek(mp3Info->fptr,3,SEEK_CUR);
}

void song(Mp3Info *mp3Info)
{
	printf("Title  : %s\n",mp3Info->song_title);
}

void print_tag(Mp3Info *mp3Info)
{
	printf("Artist : %s\n",mp3Info->artist);
	printf("Album  : %s\n",mp3Info->album);
	printf("Year   : %s\n",mp3Info->year);
	printf("Comment: %s\n",mp3Info->comment);
}

uint verify_flag(Mp3Info *mp3Info)
{
	if(strcmp(mp3Info->flag,"-t") == mp3_success)
		return 1;
	else if(strcmp(mp3Info->flag,"-a") == mp3_success)
		return 2;
	else if(strcmp(mp3Info->flag,"-A") == mp3_success)
		return 3;
	else if(strcmp(mp3Info->flag,"-y") == mp3_success)
		return 4;
	else if(strcmp(mp3Info->flag,"-c") == mp3_success)
		return 5;
	else
		return 0;
}

Status edit_info(Mp3Info *mp3Info)
{
	rewind(mp3Info->fptr);
	if(version_id(mp3Info) == mp3_success)
	{
		switch(verify_flag(mp3Info))
		{
			case 1:
				fseek(mp3Info->fptr,SONG,SEEK_SET);
				move_ptr(mp3Info);
				strcpy(mp3Info->song_title,mp3Info->data);
				mp3Info->song_title[mp3Info->buffer] = '\0';
				fwrite(mp3Info->song_title,mp3Info->buffer,1,mp3Info->fptr);
				return mp3_success;	
			case 2:
				fseek(mp3Info->fptr,ARTIST,SEEK_SET);
				move_ptr(mp3Info);
				strcpy(mp3Info->artist,mp3Info->data);
				mp3Info->artist[mp3Info->buffer] = '\0';
				fwrite(mp3Info->artist,mp3Info->buffer,1,mp3Info->fptr);	
				return mp3_success;	
			case 3:
				fseek(mp3Info->fptr,ALBUM,SEEK_SET);
				move_ptr(mp3Info);
				strcpy(mp3Info->album,mp3Info->data);
				mp3Info->album[mp3Info->buffer] = '\0';
				fwrite(mp3Info->album,mp3Info->buffer,1,mp3Info->fptr);	
				return mp3_success;	
			case 4:
				fseek(mp3Info->fptr,YEAR,SEEK_SET);
				move_ptr(mp3Info);
				strcpy(mp3Info->year,mp3Info->data);
				mp3Info->year[mp3Info->buffer] = '\0';
				fwrite(mp3Info->year,mp3Info->buffer,1,mp3Info->fptr);	
				return mp3_success;	
			case 5:
				fseek(mp3Info->fptr,COMMENT,SEEK_SET);
				move_ptr(mp3Info);
				fseek(mp3Info->fptr,4,SEEK_CUR);
				strcpy(mp3Info->comment,mp3Info->data);
				mp3Info->comment[mp3Info->buffer] = '\0';
				fwrite(mp3Info->comment,mp3Info->buffer,1,mp3Info->fptr);	
				return mp3_success;	
			default:
				printf("Format : ./a.out [flag] [file.mp3]\n");
				printf("INFO   : ./a.out -h for help\n");
				return mp3_failure;
		}
	}
	else
	{
		return mp3_failure;
	}

}

void close_file(Mp3Info *mp3Info)
{
	fclose(mp3Info->fptr);
	mp3Info->fptr = NULL;
	exit(0);
}

