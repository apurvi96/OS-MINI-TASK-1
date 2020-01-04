#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>//exit
#include<sys/types.h>//open call
#include<sys/stat.h>//open call
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include<dirent.h>
#include<chrono>
#include<ctime>
#include<iomanip>
#include<unistd.h>


using namespace std;
using namespace std::chrono; 
#define buf 4096
char files[1005][1005];

void copy(char *fs,char *fd)
{
	 auto start=high_resolution_clock::now();
	    int f1,f2;
	ssize_t rf,wf;
	char buffer[buf];
	mode_t mode=S_IRWXU|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH;
	f1=open(fs,O_RDONLY);

	if(f1==-1)
		{perror("cannot open source file");
			exit(0);
		}

	f2=open(fd,O_WRONLY|O_CREAT|O_TRUNC,mode);

	if(f2==-1)

	{
		perror("cannot open dest file");
		exit(0);
	}
	
	while((rf=read(f1,&buffer,buf))>0)
	{wf=write(f2,&buffer,(ssize_t)rf);}
		 
	auto end=high_resolution_clock::now();
	double total=duration_cast<nanoseconds>(end-start).count();
	cout<<"total time:"<<total<<" nsec\n";
	close(f1);
	close(f2);
}  

void list_files(char *path1,char *path2)
{
	int index=0;
	struct dirent *fop;
	int count=0;
	DIR *dirs=opendir(path1);
	DIR *dird=opendir(path2);

	if(dirs==NULL)
	{return;}
	
	if(dird==NULL)
	{return;}

	while((fop=readdir(dirs))!=NULL)
	{
		if(strcmp(fop->d_name,".")!=0 && strcmp(fop->d_name,"..")!=0)
		{
		string paths=string(path1);
		char *source=&*paths.begin();

		strcat(strcat(source,"/"),fop->d_name);
		string pathd=string(path2);
		char *dest=&*pathd.begin();

		strcat(strcat(dest,"/"),fop->d_name);
		copy(source,dest);
		
		}

	}
closedir(dirs);
closedir(dird);
}


int main(int argc, char* argf[])
{
	if(argc!=3)
	{
		cout<<"invalid";
		exit(1);
	}
	//checking if given argument is a file or directory
	struct stat bu;
	stat(argf[1],&bu);
	if(S_ISDIR(bu.st_mode))
	{list_files(argf[1],argf[2]);}//if directory call list_files function which will copy by iterating through each file
	
	else
	{copy(argf[1],argf[2]);}//if file then directly copy the contents

return 0;
}