#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>

#define node_t __node_ts
typedef struct node_ts __node_ts;
struct node_ts {
	int m_data;

	char *m_next;
};

#define list_t __list_ts
typedef struct list_ts __list_ts;
struct list_ts {
	node_t *m_head;
	node_t *m_tail;
	int m_count;
};

