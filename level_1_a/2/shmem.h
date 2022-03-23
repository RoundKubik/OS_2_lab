#define MAXBUFF 80

#define PERM 0666 
typedef struct mem 
{
int segment;
char buff[MAXBUFF];

}Message;

static struct sembuf proc_wait[1] = {1, -1, 0};

static struct sembuf proc_start[1] = {1, 1, 0};

static struct sembuf mem_lock[2] = {0, 0, 0, 0, 1, 0};

static struct sembuf mem_unlock[1] = {0, -1, 0}; 
