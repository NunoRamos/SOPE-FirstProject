#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

pid_t createProcess(char arg[]){
	pid_t pid;
	pid=fork();

	if(pid < 0){
		perror("fork ERROR");
		exit(3);
	}

	else if(pid == 0){
		execlp("listdir","listdir", arg, NULL);
		perror("execLp ERROR");
		exit(1);
	}

	wait(NULL);
	return pid;
}

int main(int argc, char	*argv[]) {
	DIR *dirp;
	struct	dirent *direntp;
	struct	stat stat_buf;
	char	*str;
	char	name[200];

	if	(argc != 2) {
			fprintf( stderr, "Usage: %s dir_name\n", argv[0]);
			exit(1);
		}
	if	((dirp = opendir( argv[1])) == NULL) {
			perror(argv[1]);
			exit(2);
		}
	while	((direntp = readdir( dirp)) != NULL) {
			sprintf(name,"%s/%s",argv[1],direntp->d_name);
			// alternativa a chdir(); ex: anterior
			if (  lstat (name, &stat_buf)==-1) {
	                     	 perror("lstat ERROR");
				 exit(3);                                     }
			//      printf("%10d - ",(int) stat_buf.st_ino);
			if(S_ISREG(stat_buf.st_mode)) str = "regular";
         	        else  if (S_ISDIR(stat_buf.st_mode)){
			       	str = "directory";
				if(strcmp( direntp->d_name, ".")  && strcmp( direntp->d_name, ".."))
					createProcess(direntp->d_name);
			}
			else str = "other";
			printf("%-25s - %s\n", direntp->d_name, str);
		}
	closedir(dirp);
	exit(0);      
}