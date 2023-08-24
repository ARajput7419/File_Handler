
// For more information use command  :  ./filehandler -help   


#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>


struct create_arguments{

	char * filename; // name of the file 
	char * permission; // permission for that file

}; // structure used to store command line arguments needed for creation of file


struct read_arguments{

	char * filename; // name of the file

	int * total_bytes; // total bytes need to be read 

}; // structure used to store command line arguments needed for reading a file


struct write_arguments{

	char * filename ; // name of the file

	int * total_bytes; // total bytes need to write into that file

	char * data;	// contain address of the data that need to be written in the file

}; // structure used to store command line arguments needed for writing into a  file

struct info_arguments{

	char * filename; // name of the file


}; // structure used to store command line arguments needed to get information of a particular file

struct copy_arguments{

	char * sourcefile; // file name from where the content will be copied

	char * targetfile; // file name where the content will be pasted

	int * buffer_size; // size of buffer which is used to fasten the process 

}; // structe used to store command line arguments needed to copy content of one file into another


void getCreateArguments(char * argv [] , struct  create_arguments * arguments){

	/*

	Taken arguments :
		argv - command line arguments 
		arguments - data structure used to hold the command line arguments value

	Return :
		None

	Description : 

		This function take command line arguments and after parsing them store corresponding value into the structure 

		for further processing  for create system call .

		Command line arguments must contain --filename , --permission attributes and their corresponding values 

	*/

	if (strcmp(argv[2],"--filename")==0)
	{
		arguments->filename = argv[3];

		if(strcmp(argv[4],"--permission")==0) arguments->permission = argv[5];

		else {

			printf("Invalid Arguments ...");
			exit(1);
		}

		
	}

	else if(strcmp(argv[4],"--filename")==0)
	{
		arguments->filename = argv[5];

		if(strcmp(argv[2],"--permission")==0) arguments->permission = argv[3];

		else {

			printf("Invalid Arguments ...");
			exit(1);
		}
	}

	else{

		printf("Invalid arguments ...\n");
		exit(1);
	}

	


}


void getReadArguments(char * argv [] , struct  read_arguments * arguments){

/*

	Taken arguments :
		argv - command line arguments 
		arguments - data structure used to hold the command line arguments value

	Return :
		None

	Description : 

		This function take command line arguments and after parsing them store corresponding value into the structure 

		for further processing of read system call.

	Command line arguments must contain --filename , --bytes attributes and their corresponding values 

	*/


if (strcmp(argv[2],"--filename")==0)
{
	arguments -> filename = argv[3];

	if(strcmp(argv[4],"--bytes")==0){

		int temp = atoi(argv[5]);

		arguments -> total_bytes = &temp;
	}
	else {

		printf("Invalid Arguments ... \n");
		exit(1);
	}
}
else if (strcmp(argv[4],"--filename")==0){

	arguments -> filename = argv[5];

	if(strcmp(argv[2],"--bytes")==0){

		int temp = atoi(argv[2]);

		arguments -> total_bytes = &temp;
	}
	else{

		printf("Invalid Arguments ... \n");
		exit(1);
	}

}
else{

		printf("Invalid Arguments ... \n");
		exit(1);

	}


}


void getWriteArguments(char * argv [] , struct  write_arguments  * arguments){

	/*

	Taken arguments :
		argv - command line arguments 
		arguments - data structure used to hold the command line arguments value

	Return :
		None

	Description : 

		This function take command line arguments and after parsing them store corresponding value into the structure 

		for further processing  for write system call .

		Command line arguments must contain --filename , --bytes , --data attributes and their corresponding values 

	*/

	for(int i = 2;i<=6;i+=2){

		if(strcmp(argv[i],"--filename")==0){

			if(arguments->filename != NULL){

				printf("Argument found multiple times ...\n");
				exit(1);
			}

			arguments->filename = argv[i+1];
		}
		else if(strcmp(argv[i],"--bytes")==0){

			if(arguments->total_bytes != NULL){

				printf("Argument found multiple times ...\n");
				exit(1);
			}
			int temp = atoi(argv[i+1]);
			arguments->total_bytes = &temp;

		}
		else if(strcmp(argv[i],"--data")==0){

			if(arguments->data != NULL){

				printf("Argument found multiple times ...\n");
				exit(1);
			}
			arguments->data = argv[i+1];
		}
		else{

			printf("Invalid Arguments ...\n");
			exit(1);
		}
	}

}


void getInfoArguments(char * argv[] , struct  info_arguments  * arguments){

	/*

	Taken arguments :
		argv - command line arguments 
		arguments - data structure used to hold the command line arguments value

	Return :
		None

	Description : 

		This function take command line arguments and after parsing them store corresponding value into the structure 

		for further processing of stat system call .

		Command line arguments must contain --filename it's  value 

	*/

	if (strcmp(argv[2],"--filename")==0)
	{
		arguments -> filename = argv[3];
	}
	else
	{
		printf("Invalid Arguments ...\n");
		exit(1);
	}

}

void getCopyArguments(char * argv [] , struct  copy_arguments * arguments){


	/*

	Taken arguments :
		argv - command line arguments 
		arguments - data structure used to hold the command line arguments value

	Return :
		None

	Description : 

		This function take command line arguments and after parsing them store corresponding value into the structure 

		for further processing of copying the data

		Command line arguments must contain --sourcefile , --targetfile, --buffer_size attributes and their corresponding values 

	*/


	for(int i = 2;i<=6;i+=2){

		if(strcmp(argv[i],"--sourcefile")==0){

			if(arguments->sourcefile != NULL){

				printf("Argument found multiple times ...\n");
				exit(1);
			}

			arguments->sourcefile = argv[i+1];
		}
		else if(strcmp(argv[i],"--targetfile")==0){

			if(arguments->targetfile != NULL){

				printf("Argument found multiple times ...\n");
				exit(1);
			}
			arguments->targetfile = argv[i+1];

		}
		else if(strcmp(argv[i],"--buffer_size")==0){

			if(arguments->buffer_size != NULL){

				printf("Argument found multiple times ...\n");
				exit(1);
			}
			int temp =  atoi(argv[i+1]);
			arguments->buffer_size = &temp;
		}
		else{

			printf("Invalid Arguments ...\n");
			exit(1);
		}
	}


}


int parsePermission(char * permission){

/*

Taken arguments :
		permission - string must have format rwxrwxrwx and use of _ to disable particular permission

	Return :
		integer value that represent permissions 

	Description : 

		This function takes permission and return corresponding integral value


*/

	int perm = 0;

	if(strlen(permission)!=9) {

		printf("Invalid permission format");

		exit(1);

	}
	for(int i = 0;i<9;i++){

		char p = permission[i];

		if(i/3 == 0 ){

			if ( p=='r') perm |= S_IRUSR;
			else if ( p=='w') perm |= S_IWUSR;
			else if ( p=='x') perm |= S_IXUSR;
			else if( p!='_'){

				printf("Invalid permission format\n");
				exit(1);
			}


		}
		else if(i/3 == 1){


			if ( p=='r') perm |= S_IRGRP;
			else if ( p=='w') perm |= S_IWGRP;
			else if ( p=='x') perm |= S_IXGRP;
			else if( p!='_') {

				printf("Invalid permission format\n");
				exit(1);

		}
		else{

			if ( p=='r') perm |= S_IROTH;
			else if ( p=='w') perm |= S_IWOTH;
			else if ( p=='x') perm |= S_IXOTH;
			else if( p!='_') {

				printf("Invalid permission format\n");
				exit(1);
			}

		}

	}


	}

	return perm;

}



void createFile(char * filename,char * permission){

	/*
	
	filename - name of the file 

	permission - permission for that file

	return - None

	This function create a file and report error if any..


	*/

	int perm = parsePermission(permission);

	int fd = creat(filename,perm);

	if (fd == -1)
	{
		
		perror("Error :\n");

		exit(1);

	}
	else
	{

		printf("File \"%s\" is created successfully ....",filename);
	}


}


void readFile(char * filename , int total_bytes){

	/*

	filename - name of the file

	total_bytes - number of bytes to read

	return - None

	This function read n number of bytes from  a file , It also report errors if any..

	*/


	if( total_bytes <= 0 ) {

		printf("Byte count should be positive integer\n");
		exit(1);

	}

	char * buffer = NULL;

	int fd = open(filename,O_RDONLY);


	if(fd == -1){

		perror("Error : \n");
		exit(1);
	}
	else{

		buffer = (char *)malloc(sizeof(char)*total_bytes);

		int total_read = read(fd,buffer,total_bytes);

		if (total_read < 0)
		{
			perror("Error : \n");
			close(fd);
			exit(1);
		}

		printf("Total %d bytes are read from file \"%s\"....\n",total_read,filename);

		printf("Data is : \n\n %s",buffer);

		close(fd);

	}

}


void writeFile(char * filename , int total_bytes , char * data){


	/*
	
	filename - name of the file

	total_bytes - count of bytes that need to be written in the file

	data - data that need to be written

	return None

	This function write n number of  bytes into a file and report errors if any ..


	*/

	if( total_bytes <= 0 ) {

		printf("Byte count should be positive integer\n");
		exit(1);

	}


	int fd = open(filename,O_WRONLY);

	if (fd == -1)
	{
			perror("Error : \n");
			exit(1);
	}
	else{

		int total_write = write(fd,data,total_bytes);

		if( total_write < 0) {

			perror("Error : \n");
			close(fd);
			exit(1);
		}


		printf("Total %d bytes are written in file \"%s\"....",total_write,filename);

		close(fd);
	}	


}


void information(char * filename){

	/*
	
	filename - name of the file

	return - None

	This function print information of a file like size , inode number , permissions etc and reports error if any.. 

	*/

	struct stat  info ;

 	int status = stat(filename,&info);

 	if ( status == -1 )
 	{
 		perror("Error : \n");
 		exit(1);
 	}
 	else{

 	  printf("File Permissions \n");
	  printf((info.st_mode & S_IRUSR)? "r":"-");
	  printf((info.st_mode & S_IWUSR)? "w":"-");
	  printf((info.st_mode & S_IXUSR)? "x":"-");
	  printf(" ");
	  printf((info.st_mode & S_IRGRP)? "r":"-");
	  printf((info.st_mode & S_IWGRP)? "w":"-");
	  printf((info.st_mode & S_IXGRP)? "x":"-");
	  printf(" ");
	  printf((info.st_mode & S_IROTH)? "r":"-");
	  printf((info.st_mode & S_IWOTH)? "w":"-");
	  printf((info.st_mode & S_IXOTH)? "x\n":"-\n");
	  printf("User ID %d \n",info.st_uid);
	  printf("Block Size  %ld \n",info.st_blksize);
	  printf("Group ID %d \n",info.st_gid);
	  printf("Number of Blocks  %ld \n",info.st_blocks);
	  printf("File Size %ld Bytes\n",info.st_size);
	  printf("Link Count %u \n",(unsigned int)info.st_nlink);
	  printf("Inode Number %d \n",info.st_ino);

	  struct tm date_time;

	  date_time = *(gmtime(&info.st_ctime));
      printf("Created on: %d-%d-%d %d:%d:%d \n", date_time.tm_mday, date_time.tm_mon, date_time.tm_year + 1900, 
                                              date_time.tm_hour, date_time.tm_min, date_time.tm_sec);
      date_time = *(gmtime(&info.st_mtime));
      printf("Modified on: %d-%d-%d %d:%d:%d \n", date_time.tm_mday, date_time.tm_mon, date_time.tm_year + 1900, 
                                              date_time.tm_hour, date_time.tm_min, date_time.tm_sec);
	  


 	}


}



void copy(char * sourcefile , char * targetfile , int buffer_size){


	/*
	
		sourcefile - name of the file from where data will be copied 

		targetfile - name of the file where data will be pasted 

		buffer_size - size of the buffer which is actually used to fasten the process

		return - None

		This function copy the content of one file into another file  and reports error if any ..

	*/

	if( buffer_size <= 0 ) {

		printf("Buffer size should be positive integer\n");
		exit(1);

	}

	int fd = open(sourcefile,O_RDONLY);

	if (fd == -1)
	{
		perror("Error : \n");
		exit(1);
	}

	int fd2 = creat(targetfile,0666);

	if (fd2 == -1)
	{
		perror("Error : \n");
		exit(1);
	}

	char * buffer = (char *)malloc(sizeof(char)*buffer_size);

	while(1) {

		int total_read = read(fd,buffer,buffer_size);

		if(total_read < 0){

			perror("Error :\n");
			close(fd);
			close(fd2);
			exit(1);
		}

		else if(total_read == 0){

			printf("File \"%s\" is copied successfully ....",sourcefile);
			close(fd);
			close(fd2);
			return;
		}

		int total_write = write(fd2,buffer,total_read);

		if ( total_write < 0)
		{
			perror("Error : \n");
			close(fd);
			close(fd2);
			exit(1);
		}

	}


}



int main(int argc, char  *argv[])
{
	
	/*
	
	argc - number of arguments 

	argv - command line arguments 

	This function invokes particular helper  function based on the arguments to done a specific task and it also reports error if any

	*/

	if ( argc == 1)
	{
		printf("Please provide arguments\n");

		exit(1);
	}

	else {


		char * option = argv[1];


		if(strcmp(option,"-create") == 0){

			// creation of a file

			if (argc == 6)
			{
				
				struct create_arguments * arguments = (struct create_arguments *)malloc(sizeof(struct create_arguments)); 

				getCreateArguments(argv,arguments);

				createFile(arguments->filename,arguments->permission);
			}

			else {

				printf("Invalid number of arguments ...\n");
				exit(1);
			}


		}
		else if(strcmp(option,"-read")==0){

			// reading a file 

			if(argc == 6)
			{


				struct  read_arguments * arguments = (struct read_arguments *)malloc(sizeof(struct read_arguments));

				getReadArguments(argv,arguments);

				readFile(arguments->filename,*(arguments->total_bytes));
			}
			else{

				printf("Invalid number of arguments ...\n");
				exit(1);
			}



		}
		else if(strcmp(option,"-write")==0){


			// writting into a file

			if(argc == 8)
			{

				struct  write_arguments * arguments = (struct write_arguments *)malloc(sizeof(struct write_arguments));

				getWriteArguments(argv,arguments);

				writeFile(arguments->filename,*(arguments->total_bytes),arguments->data);

			}
			else{

				printf("Invalid number of arguments ...\n");
				exit(1);
			}

		}
		else if(strcmp(option,"-info")==0){

			//fetching information of a file

			if( argc == 4){

				struct info_arguments * arguments = (struct info_arguments *) malloc(sizeof(struct info_arguments));

				getInfoArguments(argv,arguments);

				information(arguments->filename);
			}
			else{

				printf("Invalid number of arguments ...\n");
				exit(1);
			}



		}
		else if(strcmp(option,"-copy")==0){

			// copying the content of a file into another target file

			if(argc == 8 )
			{

				struct copy_arguments * arguments = (struct copy_arguments *) malloc(sizeof(struct copy_arguments));

				getCopyArguments(argv,arguments);

				copy(arguments->sourcefile,arguments->targetfile,*(arguments->buffer_size));
			}
			else{

				printf("Invalid number of arguments ...\n");
				exit(1);
			}
		}
		else if(strcmp(option,"-help")==0){

			// shows help page 

			printf("Format : filehandler [OPION]  [ARGUMENTS]... \n");
			printf("OPTIONS : \n");
			printf("	-read   : For Reading a File\n");
			printf("	-write  : For Writing into a File\n");
			printf("	-copy   : For Copying a File\n");
			printf("	-info   : For getting information of file\n");
			printf("	-create : For Creating a File \n");
			printf("	-help   : For Help Page\n");

			printf("\n\n");

			printf("ARGUMENTS : \n");
			printf("	--filename    : Name of The File\n");
			printf("	--sourcefile  : Source File Name ( In Case of Copy Operation ) \n");
			printf("	--targetfile  : Target File Name ( In Case of Copy Operation ) \n");
			printf("	--data        : Data to Write Into a File \n");
			printf("	--buffer_size : Buffer Size  ( In Case of Copy Operation ) \n");
			printf("	--bytes       : Total Byte to Read or Write \n");
			printf("	--permission  : Permission format \"rwxrwxrwx\"  , use _ to disable the particular permission \n");


			exit(0);

		}
		else{

			// invalid option

			printf("Invalid option \n");
			exit(1);
		}


	}

	return 0;
}
