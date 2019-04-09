#ifndef __SERIAL__H__
#define __SERIAL__H__

//#include <QDebug>
//#include <QObject>
//#include <QThread>

//#include "src/SerialPort/myglobal.h"
//#include "src/commom/log.h"
#include "log.h"

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include <errno.h>
#include <signal.h>


#ifdef __cplusplus
};
#endif

//读取串口超时时间
#define SELECT_WAIT_TIME 60

class SerialOperation
{
public:
    SerialOperation(const char* dev, int rate);
    ~SerialOperation();

    int serial_check();
    int serial_init();
    void setTermios(struct termios * pNewtio, int uBaudRate);
    int serial_read(unsigned char* buf, int len);
    int serial_read_block(unsigned char* buf, int len);
    int serial_write(const char* buf);
    int set_opt(int fd,int nSpeed,int nBits,char nEvent,int nStop);
    void bo_protocol_deal(unsigned char rxbuf);

private:
    char device[50];        //串口设备
    int brate;              //波特率
    int fd;                 //串口文件描述符
};

#endif //__SERIAL__H__
