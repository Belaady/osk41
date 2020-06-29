/*
// Tambahkan di sini nama para kontributor (akunGitHub)
// Harap UPDATE tanggal revisi!
// Tue Jun  9 17:46:47 WIB 2020
 */

#include     "share.h"
char*        progs[]={P01, P02, P03, P04, P05, P06, P07};
char         tmpStr[256]={};
extern char* akunGitHub;
extern int   delay;
extern int   boss;
myshare*     mymap;

// Tambahkan di sini nama para kontributor (akunGitHub)
// Harap UPDATE tanggal revisi!
// Tue Jun  9 17:46:47 WIB 2020
void init(int isboss, int argc, char* argv[]) {
   if (isboss == BOSS) {
        int ssize=sizeof(myshare);
        int fd   =open(SHAREMEM, MYFLAGS, CHMOD);
        fchmod   (fd, CHMOD);
        ftruncate(fd, ssize);
        mymap=mmap(NULL, ssize, MYPROTECTION, MYVISIBILITY, fd, 0);
        close(fd);
        if (argc >= 2) {
	    mymap->state = OPEN;
            printf("ShareMemory is OPEN, BYE BYE ==== ====\n");
            exit(0);
        }
        mymap->state = OPEN;
    	mymap->mutexctr = 0;
    	mymap->entry = 0;
    	sem_init(&mymap->mutex, 0, 1);
    	for(int i = 0; i < MAXPROGS + 1; i++) {
        	strcpy(mymap->progs[i].akun, "");
        	mymap->progs[i].stamp = 0;
    	}
    } else {
        sleep(delay);
        if( access(SHAREMEM, F_OK ) == -1 ) {
            printf("No \"%s\" file.\n", SHAREMEM);
            exit(-1);
        }
        int fd=open(SHAREMEM, O_RDWR, CHMOD);
        int ssize=sizeof(myshare);
        mymap=mmap(NULL, ssize, MYPROTECTION, MYVISIBILITY, fd, 0);
        close(fd);
    }
}

// Tambahkan di sini nama para kontributor (akunGitHub)
// Harap UPDATE tanggal revisi!
// Tue Jun  9 17:46:47 WIB 2020
void myPrint(char* str1, char* str2) {
    printf("%s[%s]\n", str1, str2);
    fflush(NULL);
}

// Tambahkan di sini nama para kontributor (akunGitHub)
// Harap UPDATE tanggal revisi!
// Tue Jun  9 17:46:47 WIB 2020
int getEntry(char* akunGitHub) {
    sem_wait (&(mymap->mutex));
    int entry = mymap->entry++;
    mymap->progs[entry].stamp++;
    mymap->mutexctr++;
    sem_post (&(mymap->mutex));
    return entry;
}

// Tambahkan di sini nama para kontributor (akunGitHub)
// Harap UPDATE tanggal revisi!
// Tue Jun  9 17:46:47 WIB 2020
void display(int entry) {
    sem_wait (&(mymap->mutex));
    mymap->progs[entry].stamp++;
    mymap->mutexctr++;
    char status[30];
    if(mymap->state == CLOSED) {
        strcpy(status, "CLOSED");
    } else {
        strcpy(status, "OPEN");
    }
    printf("%s[progs[%02d] TIME[%02d] MUTEX[%02d] MMAP[%s] ", akunGitHub, entry, mymap->mutexctr, mymap->progs[entry].stamp, status);
    fflush(NULL);
    
    for(int i = 0; i < mymap->entry; i++) {
        printf("[%s]", mymap->progs[i].akun);
	fflush(NULL);
    } 
    printf("]\n");
    fflush(NULL);
    sem_post (&(mymap->mutex));
}

// Tambahkan di sini nama para kontributor (akunGitHub)
// Harap UPDATE tanggal revisi!
// Tue Jun  9 17:46:47 WIB 2020
void putInfo(char* akun, int entry) {
    sem_wait (&(mymap->mutex));
    mymap->progs[entry].stamp++;
    mymap->mutexctr++;
    strcpy(mymap->progs[entry].akun, akun);
    sem_post (&(mymap->mutex));
}

// Tambahkan di sini nama para kontributor (akunGitHub)
// Harap UPDATE tanggal revisi!
// Tue Jun  9 17:46:47 WIB 2020
void checkOpen(void) {
    if (mymap->state == CLOSED) {
        printf("ShareMemory is NOT OPEN, BYE BYE ==== ====\n");
        exit(0);
    }
}

// Tambahkan di sini nama para kontributor (akunGitHub)
// Harap UPDATE tanggal revisi!
// Tue Jun  9 17:46:47 WIB 2020
void myWait(int boss, int entry) {
    if (boss == BOSS) {
	while(wait(NULL) > 0);
	mymap->state = CLOSED;
    }
}

// Tambahkan di sini nama para kontributor (akunGitHub)
// Harap UPDATE tanggal revisi!
// Tue Jun  9 17:46:47 WIB 2020
int main(int argc, char* argv[]) {
    sprintf(tmpStr, "START PID[%d] PPID[%d]", getpid(), getppid());
    myPrint(akunGitHub, tmpStr);
    init(boss, argc, argv);
    checkOpen();
    if (boss == BOSS) {
        for(int temp=0;temp<sizeof(progs) / sizeof(progs[0]);temp++) {
            if(fork() == 0) {
                execl(progs[temp], NULL);
                break;
            }
        }
    }
    sleep  (delay);
    int entry = getEntry(akunGitHub);
    sleep  (delay);
    display(entry);
    sleep  (delay);
    putInfo(akunGitHub, entry);
    sleep  (delay);
    display(entry);
    myWait (boss, entry);
    myPrint(akunGitHub, "BYEBYE =====  ===== =====");
}

