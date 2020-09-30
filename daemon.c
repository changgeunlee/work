#include <stdio.h>
#include "include/hyper_header.h"

//---//

#define START  0
#define STOP   1
#define STATUS 2

#define PIDFILE "/var/run/cgleed.pid"

#define def_daemon_name "cgleed"

//---//

static void show_help(void);
list_t *init_linked_list(void);
int insert_linked_list(list_t *list, int data);

//---//

//---//
static void show_help(void)
{
	printf("./daemon [start|stop|status]\n");
	return;
}

list_t *init_linked_list(void)
{
	list_t *s_list;

	s_list = (list_t *)malloc(sizeof(list_t));
	if(s_list == (list_t *)NULL){
		printf("[INIT] list malloc failed\n");
		return(list_t *)NULL;
	}
	s_list->m_head = (node_t *)NULL;
	s_list->m_tail = (node_t *)NULL;
	s_list->m_count = 0;

	printf("[INIT] list\n");
	
	return(list_t *)s_list;
}

int insert_linked_list(list_t *list, int data)
{
	node_t *s_node;

	if(list == (list_t *)NULL){
		printf("[INSERT] list is NULL\n");
		return -1;
	}

	/* create node */		
	s_node = (node_t *)malloc(sizeof(node_t));
	if(s_node == (node_t *)NULL){
		printf("node malloc failed\n");
	}
	s_node->m_data = data;
	s_node->m_next = NULL;

	if((list->m_head == (node_t *)NULL) ||
			(list->m_tail == (node_t *)NULL)){

		list->m_head = list->m_tail = s_node;
		printf("[INSERT] first insert\n");
		printf("[INSERT] data[%d] node[%p] next[%p]\n",
				s_node->m_data,
				s_node,
				s_node->m_next);
	}
	else{
		printf("[INSERT] insert\n");
	}
	
	return 0;
}

int main(int argc, char **argv)
{
	int s_daemon_status = -1;
	int s_pid;
	int s_sid;
	int s_errno;
	int s_check;
	int s_num=0;
	FILE *fp;

	if(argc != 2){
		(void)show_help();
		return 0;
	}

	if(strcmp(argv[1], "start") == 0){
		s_daemon_status = START; /* 0 */
	}
	else if(strcmp(argv[1], "stop") == 0){
		s_daemon_status = STOP; /* 1 */
	}
	else if(strcmp(argv[1], "status") == 0){
		s_daemon_status = STATUS; /* 2 */
	}
	else{
		(void)show_help();
		return 0;
	}

	if(s_daemon_status == START){
		fp = fopen(PIDFILE, "r");
		if(fp != (FILE *)NULL){
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

		printf("*[%s] started\n", def_daemon_name);
	}
	else if(s_daemon_status == STOP){
		FILE *fp;
		int s_get_pid;

		fp = fopen(PIDFILE, "r");
		if(fp == NULL){
			printf("%s is not running\n", def_daemon_name);	
			exit(0);
		}
		s_check = fscanf(fp, "%d", &s_get_pid);
		if(s_check == 0){
			printf("fscanf failed\n");
			return 0;
		}
		fclose(fp);

		kill(s_get_pid, SIGKILL);

		do{
			s_check = remove(PIDFILE);

		}while(s_check == (-1));

		printf("*[%s] stopped\n", def_daemon_name);
	}
	else if(s_daemon_status == STATUS){
		FILE *fp;
		int s_get_pid;

		fp = fopen(PIDFILE, "r");
		if(fp == NULL){
			printf("%s is not running\n", def_daemon_name);	
			exit(0);
		}
		s_check = fscanf(fp, "%d", &s_get_pid);
		if(s_check == 0){
			printf("fscanf failed\n");
			return 0;
		}
		fclose(fp);
		printf("*[%s] is running\n", def_daemon_name);
	}


	if(s_daemon_status == START){
		list_t *s_list = NULL;

		/* init list */
		s_list = init_linked_list();

		/* main loop */
		while(1){
			/* insert list */
			++s_num;
			s_check = insert_linked_list((list_t *)s_list, s_num);
			if(s_check == (-1)){
				printf("[INSERT] insert linked list failed\n");
			}

			sleep(1);
		}
	}

	return -1;
}
