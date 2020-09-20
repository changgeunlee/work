#include <stdio.h>
#include "include/hyper_header.h"

#define START  0
#define STOP   1
#define STATUS 2

#define PIDFILE "/var/run/cgleed.pid"

#define def_daemon_name "cgleed"

void show_help(void);

void show_help(void)
{
	printf("./daemon [start|stop|status]\n");
	return;
}

int main(int argc, char **argv)
{
	int s_daemon_status = -1;
	int s_pid;
	int s_sid;
	int s_errno;
	FILE *fp;

	if(argc != 2){
		(void)show_help();
		return 0;
	}

	if(strcmp(argv[1], "start") == 0){
		s_daemon_status = 0;
	}
	else if(strcmp(argv[1], "stop") == 0){
		s_daemon_status = 1;
	}
	else if(strcmp(argv[1], "status") == 0){
		s_daemon_status = 2;
	}
	else{
		(void)show_help();
		return 0;
	}

	if(s_daemon_status == START){
		fp = fopen(PIDFILE, "r");
		if(fp != NULL){
			printf("%s is already running\n", def_daemon_name);	
			fclose(fp);
			exit(0);
		}

		s_pid = fork();
		if(s_pid < 0){
			exit(1);
		}
		else if(s_pid > 0){
			exit(0);
		}

		s_pid = fork();
		if(s_pid < 0){
			exit(1);
		}
		else if(s_pid > 0){
			exit(0);
		}
		umask(0);
		s_sid = setsid();
		if(s_sid < 0){
			exit(1);
		}
		if(chdir("/") == (-1)){
			printf("cchdir failed\n");
		}

		/* write pid file */
		fp = fopen(PIDFILE, "w");
		s_errno = errno;
		if(fp == NULL){
			printf("%s fopen failed(%d - %s)\n", PIDFILE, s_errno, strerror(s_errno));
			exit(1);
		}
		fprintf(fp, "%d", getpid());
		fclose(fp);

		while(1){
			
		}	
		printf("%s started\n", def_daemon_name);
	}
	else if(s_daemon_status == STOP){
		FILE *fp;
		int s_get_pid;

		fp = fopen(PIDFILE, "r");
		if(fp == NULL){
			printf("%s is not running\n", def_daemon_name);	
			exit(0);
		}
		fscanf(fp, "%d", &s_get_pid);
		fclose(fp);

		kill(s_get_pid, SIGKILL);

		if(remove(PIDFILE) == -1){
			printf("%s remove failed\n", PIDFILE);
			exit(1);
		}

		printf("%s stopped\n", def_daemon_name);
	}
	else if(s_daemon_status == STATUS){
		FILE *fp;
		int s_get_pid;

		fp = fopen(PIDFILE, "r");
		if(fp == NULL){
			printf("%s is not running\n", def_daemon_name);	
			exit(0);
		}
		fscanf(fp, "%d", &s_get_pid);
		fclose(fp);
		printf("%s is running\n", def_daemon_name);
	}

	/* remove pid fild */

	return 0;
}
