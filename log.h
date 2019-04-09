#ifndef __LOG_H__
#define __LOG_H__

//#include <QObject>

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>

/************************************c语言通用日志框架************************************
*   定义显示的日志级别
*   #define LOG_LEVEL
*       1. LOG_LEVEL_VERBOSE
*       2. LOG_LEVEL_DEBUG
*       3. LOG_LEVEL_INFO
*       4. LOG_LEVEL_WARNING
*       5. LOG_LEVEL_ERROR
*       6. LOG_LEVEL_FATAL
*       7. LOG_LEVEL_SILENT
*   function：(使用printf输出格式带换行符)
*           LOGV(format,...)
*           LOGD(format,...)
*           LOGI(format,...)
*           LOGW(format,...)
*           LOGE(format,...)
*           LOGF(format,...)
*   eg. LOGV("hellowrd");
*       int a=1;
*       LOGE("a=%d",a);
*
*************************************日志模块用法简介****************************************
*
*   1.定义日志文件路径(59行) 和打印日志级别(62行), 没定义则打印到stdout
*       define DEBUG_FILE    "log.dat"
*       define LOG_LEVEL   LOG_LEVEL_INFO
*   2.代码中初始化日志文件
*       LOG_INIT;
*
*   3.代码中使用日志文件API记录日志,一共7个级别, 建议调试时最低级别设置为全部级别输出，生产时设置最低级别问INFO
*       LOGV("verbose");    
*       LOGD("debug");
*       LOGI("infomation");
*       LOGW("warning");
*       LOGE("error");
*       LOGF("fatal");
*
*   4.代码中关闭日志文件
*       LOG_EXIT;
*
*****************************************************************************************/

#pragma once
#ifndef __LOG_PRINT_H_
#define __LOG_PRINT_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*控制日志输出到控制台或者文件，注释此行日志输出到控制台*/
//#define DEBUG_FILE    "/var/log/debug_ibreezee.log"
#define DEBUG_FILE    "/var/log/ibreezee.log"

/*改变此行控制日志输出级别*/
#define LOG_LEVEL   LOG_LEVEL_VERBOSE

#define LOG_LEVEL_VERBOSE   1   /*所有*/
#define LOG_LEVEL_DEBUG     2   /*调试*/
#define LOG_LEVEL_INFO      3   /*信息*/
#define LOG_LEVEL_WARNING   4   /*警告*/
#define LOG_LEVEL_ERROR     5   /*错误*/
#define LOG_LEVEL_FATAL     6   /*致命错误*/
#define LOG_LEVEL_SILENT    7   /*静默*/

#define TITLE_VERBOSE       " V: "
#define TITLE_DEBUG         " D: "
#define TITLE_INFO          " I: "
#define TITLE_WARNING       " W: "
#define TITLE_ERROR         " E: "
#define TITLE_FATAL         " F: "

#ifndef LOG_LEVEL
#   define LOG_LEVEL    LOG_LEVEL_VERBOSE
#endif

#define LOG_NOOP    (void) 0

#ifndef _TIME_POINTER_
#define _TIME_POINTER_
     static time_t timep;
     static struct tm *p;
#endif

#ifdef DEBUG_FILE
#define LOG_INIT \
        MyLog log;  \
        do{ \
            time(&timep);   \
            p =localtime(&timep);   \
            MyLog::log_write( \
            "%04d-%02d-%02d %02d:%02d:%02d [In %s Func %s Line:%u] *** LOG INIT ***" "\n",  \
            p->tm_year+1900,1+p->tm_mon,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec,  \
            __FILE__,__func__,__LINE__);   \
        }while(0)

#define LOG_EXIT \
         do{ \
            time(&timep);   \
            p =localtime(&timep);   \
            MyLog::log_write(\
            "%04d-%02d-%02d %02d:%02d:%02d [In %s Func %s Line:%u] *** LOG EXIT ***" "\n",\
            p->tm_year+1900,1+p->tm_mon,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec,\
            __FILE__,__func__,__LINE__);   \
            MyLog::log_close(); \
         }while(0)
#else
#define LOG_INIT \
         do{ \
            time(&timep);   \
            p =localtime(&timep);   \
            fprintf(stdout,\
            "%04d-%02d-%02d %02d:%02d:%02d [In %s Func %s Line:%u] *** LOG INIT ***" "\n",  \
            p->tm_year+1900,1+p->tm_mon,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec,  \
            __FILE__,__func__,__LINE__);   \
        }while(0)

#define LOG_EXIT \
         do{ \
            time(&timep);   \
            p =localtime(&timep);   \
            fprintf(stdout,\
            "%04d-%02d-%02d %02d:%02d:%02d [In %s Func %s Line:%u] *** LOG EXIT ***" "\n",\
            p->tm_year+1900,1+p->tm_mon,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec,\
            __FILE__,__func__,__LINE__);   \
            MyLog::log_close(); \
         }while(0)
#endif

#ifdef DEBUG_FILE
#   define  LOG_PRINT(level,fmt,...)    \
        do{ \
            time(&timep);   \
            p =localtime(&timep);           \
            MyLog::log_write(\
            "%04d-%02d-%02d %02d:%02d:%02d [In %s Func %s Line:%u] " level fmt,\
            p->tm_year+1900,1+p->tm_mon,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec,\
            __FILE__,__func__,__LINE__,##__VA_ARGS__);   \
        }while(0)
#else
#   define  LOG_PRINT(level,fmt,...)    \
        do{ \
            time(&timep);   \
            p =localtime(&timep);           \
            fprintf(stderr, \
            "%04d-%02d-%02d %02d:%02d:%02d [In %s Func %s Line:%u] " level fmt,\
            p->tm_year+1900,1+p->tm_mon,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec,\
            __FILE__,__func__,__LINE__,##__VA_ARGS__);   \
        }while(0)
#endif

#if LOG_LEVEL_VERBOSE >= LOG_LEVEL
#   define LOG_VERBOSE(fmt,...) LOG_PRINT(TITLE_VERBOSE,fmt,##__VA_ARGS__)
#else
#   define LOG_VERBOSE(...) LOG_NOOP
#endif

#define LOGV(format,...)    LOG_VERBOSE(format,##__VA_ARGS__)

#if LOG_LEVEL_DEBUG >= LOG_LEVEL
#   define  LOG_DEBUG(fmt,...)  LOG_PRINT(TITLE_DEBUG,fmt,##__VA_ARGS__)
#else
#   define  LOG_DEBUG(...)  LOG_NOOP
#endif

#define LOGD(format,...)    LOG_DEBUG(format,##__VA_ARGS__)

#if LOG_LEVEL_INFO>= LOG_LEVEL
#   define  LOG_INFO(fmt,...)   LOG_PRINT(TITLE_INFO,fmt,##__VA_ARGS__)
#else
#   define  LOG_INFO(...)   LOG_NOOP
#endif

#define LOGI(format,...)    LOG_INFO(format,##__VA_ARGS__)

#if LOG_LEVEL_WARNING >= LOG_LEVEL
#   define  LOG_WARN(fmt,...)   LOG_PRINT(TITLE_WARNING,fmt,##__VA_ARGS__)
#else
#   define  LOG_WARN(...)   LOG_NOOP
#endif

#define LOGW(format,...)    LOG_WARN(format,##__VA_ARGS__)

#if LOG_LEVEL_ERROR >= LOG_LEVEL
#   define  LOG_ERROR(fmt,...)  LOG_PRINT(TITLE_ERROR,fmt,##__VA_ARGS__)
#else
#   define  LOG_ERROR(...)  LOG_NOOP
#endif

#define LOGE(format,...)    LOG_ERROR(format,##__VA_ARGS__)

#if LOG_LEVEL_FATAL >= LOG_LEVEL
#   define  LOG_FATAL(fmt,...)  LOG_PRINT(TITLE_FATAL,fmt,##__VA_ARGS__)
#else
#   define  LOG_FATAL(...)  LOG_NOOP
#endif

#define LOGF(format,...)    LOG_FATAL(format,##__VA_ARGS__)

#ifdef __cplusplus
}
#endif
#endif




#ifdef __cplusplus
};
#endif

#define MODE S_IRWXU
//#define LOG_INIT    do{}while(0)
//#define LOG_EXIT    do{}while(0)
//#define LOGD(format,...)    do{}while(0)
//#define LOGI(format,...)    do{}while(0)
//#define LOGE(format,...)    do{}while(0)

//class MyLog : public QObject
class MyLog
{
    //Q_OBJECT
public:
    //MyLog(QObject* parent = NULL);
    MyLog();
    ~MyLog(void);

    //静态方法,无需实例化
    static void log_write (const char *format, ...);
    static void log_close(void);

    void log_open(const char* log_path);


private:
    static FILE* log_fd;
    //char log_dir[50] = "/data/log";
    char log_dir[50];
    char log_file[50];
};


#endif



