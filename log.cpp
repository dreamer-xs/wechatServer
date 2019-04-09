#include "log.h"

//用static成员变量必须要初始化
FILE*  MyLog::log_fd = NULL;

//初始化文件描述符
//MyLog::MyLog(QObject *parent):QObject(parent)
MyLog::MyLog()

{
    //strcpy(log_dir,"/data/log");
    //strcpy(log_dir,"/var/log");
    strcpy(log_dir,".");

    if(log_fd)
    {
        fflush(log_fd);
        fclose(log_fd);
        log_fd = NULL;
    }

    log_open(log_dir);
}

MyLog::~MyLog()
{
    log_close();
}

void MyLog::log_write (const char *format, ...) 
{
    va_list arg;
    int done;

    va_start (arg, format);

#if 0
    time_t time_seconds = time(0);
    struct tm* tm_log = localtime(&time_seconds);
    fprintf(log_fd, "%04d-%02d-%02d %02d:%02d:%02d ", tm_log->tm_year + 1900, tm_log->tm_mon + 1, tm_log->tm_mday, tm_log->tm_hour, tm_log->tm_min, tm_log->tm_sec);
#endif

    if(!log_fd)
    {
        /*
        openlog("ShowData", LOG_CONS | LOG_PID, 0); 
        syslog(LOG_INFO, format, arg);  
        //emit log_open_failed();
        closelog();
        */
        printf("ERR:loger module is not open!\n");
    }
    else
    {
        done = vfprintf (log_fd, format, arg);
        fflush(log_fd);
    }

    va_end (arg);
}

void MyLog::log_close(void)
{
    if(log_fd)
    {
#if 0
        log_write( "-----------------------------------------------------------------------------\n");
        log_write( "-------------------------   process stop  -----------------------------------\n");
        log_write( "-----------------------------------------------------------------------------\n");
#endif
        fflush(log_fd);
        fclose(log_fd);
        log_fd = NULL;
    }
}

void MyLog::log_open(const char* log_path)
{
    //create log dir and open log file
    mkdir(log_path, MODE);

#ifdef DEBUG_FILE
    sprintf(log_file, "%s", DEBUG_FILE);
#else
    sprintf(log_file, "%s%s", log_path, "/receive.log");
#endif

    log_fd = fopen(log_file, "a");
    if(NULL == log_fd)
    {
        printf("err: can't open log file %s!\n", log_path);
        //emit log_open_failed();
    }

    printf("log file: %s\n",log_file);

#if 0
    log_write( "-----------------------------------------------------------------------------\n");
    log_write( "-------------------------   process start   ---------------------------------\n");
    log_write( "-----------------------------------------------------------------------------\n");
    log_write("log file: %s fd: %d\n",log_file, fileno(log_fd));
#endif

}

