#include <QCoreApplication>
#include <sys/time.h>
#include "application.h"
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "QDebug"

#define LOG_FILE "/usr/local/wiklosoft/log"
#define RUNNING_DIR "/tmp/wiklosoft/"
#define LOCK_FILE "/tmp/wiklosoft/ocf_device_lock"

void signal_handler(int sig)
{
    switch(sig) {
    case SIGHUP:
        qDebug() << "hangup signal catched";
        exit(0);
        break;
    case SIGTERM:
        qDebug() << "terminate signal catched";
        exit(0);
        break;
    }
}
void init_daemon()
{
    qDebug() << "init_deamon";
    int i,lfp;
    char str[10];
    if(getppid()==1)
        return; /* already a daemon */
    i=fork();
    if (i<0)
    {
        qDebug() << "Fork error";
        exit(1); /* fork error */
    }
    if (i>0)
    {
        qDebug() << "Parent exists";
        exit(0); /* parent exits */

    }
    qDebug() << "forked";
    /* child (daemon) continues */
    setsid(); /* obtain a new process group */

    for (i=getdtablesize();i>=0;--i)
        close(i); /* close all descriptors */
    i=open("/dev/null",O_RDWR); dup(i); dup(i); /* handle standart I/O */

    umask(027); /* set newly created file permissions */

    chdir(RUNNING_DIR); /* change running directory */
    lfp=open(LOCK_FILE,O_RDWR|O_CREAT,0640);
    if (lfp<0)
        exit(1); /* can not open */
    if (lockf(lfp,F_TLOCK,0)<0)
        exit(0); /* can not lock */
    /* first instance continues */
    sprintf(str,"%d\n",getpid());

    qDebug() << "process" << getpid();

    write(lfp,str,strlen(str)); /* record pid to lockfile */
    signal(SIGCHLD,SIG_IGN); /* ignore child */
    signal(SIGTSTP,SIG_IGN); /* ignore tty signals */
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTTIN,SIG_IGN);
    signal(SIGHUP,signal_handler); /* catch hangup signal */
    signal(SIGTERM,signal_handler); /* catch kill signal */
}
uint64_t get_current_ms(){
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // caculate milliseconds
    return milliseconds;
}



int main(int argc, char *argv[])
{
    for (int i=0; i<argc; i++){
        if (QString(argv[i]) == "-d") init_daemon();
    }
    Application a(argc, argv);

    return a.exec();
}

