#include "serialOperation.h"

//获取串口设备,并打开串口
SerialOperation::SerialOperation(const char* dev, int rate)
{
    LOGI("dev: %s rate: %d\n", dev, rate);

    brate = rate;
    memset(device, 0, sizeof(device));
    memcpy(device, dev, strlen(dev));

    serial_init();
}

SerialOperation::~SerialOperation()
{
    //关闭串口，释放FIFO
    if(fd)
        close(fd);
}

//初始化串口选项：  
void SerialOperation::setTermios(struct termios * pNewtio, int uBaudRate)
{
    //struct termios oldtio, newtio;
    bzero(pNewtio, sizeof(struct termios)); /* clear struct for new port settings */

    //8N1
    //pNewtio->c_cflag = uBaudRate | CS8 | CREAD | CLOCAL;
    //奇校验
    pNewtio->c_cflag = uBaudRate | CS8 | PARODD | CLOCAL;
    pNewtio->c_iflag = IGNPAR;

    pNewtio->c_oflag = 0;
    pNewtio->c_lflag = 0; //non ICANON

    /* set stop & start mark */
    pNewtio->c_cc[VSTOP]    = 0x22;  
    pNewtio->c_cc[VSTART]   = 0x33;  
    pNewtio->c_iflag |= IXON|IXOFF; //使用软件流控制

    /*
       initialize all control characters
       default values can be found in /usr/include/termios.h, and
       are given in the comments, but we don't need them here
       */
    pNewtio->c_cc[VINTR] = 0; /* Ctrl-c */
    pNewtio->c_cc[VQUIT] = 0; /* Ctrl-\ */
    pNewtio->c_cc[VERASE] = 0; /* del */
    pNewtio->c_cc[VKILL] = 0; /* @ */
    pNewtio->c_cc[VEOF] = 4; /* Ctrl-d */
    pNewtio->c_cc[VTIME] = 5; /* inter-character timer, timeout VTIME*0.1 */
    pNewtio->c_cc[VMIN] = 0; /* blocking read until VMIN character arrives */
    pNewtio->c_cc[VSWTC] = 0; /* '\0' */
    pNewtio->c_cc[VSTART] = 0; /* Ctrl-q */
    pNewtio->c_cc[VSTOP] = 0; /* Ctrl-s */
    pNewtio->c_cc[VSUSP] = 0; /* Ctrl-z */
    pNewtio->c_cc[VEOL] = 0; /* '\0' */
    pNewtio->c_cc[VREPRINT] = 0; /* Ctrl-r */
    pNewtio->c_cc[VDISCARD] = 0; /* Ctrl-u */
    pNewtio->c_cc[VWERASE] = 0; /* Ctrl-w */
    pNewtio->c_cc[VLNEXT] = 0; /* Ctrl-v */
    pNewtio->c_cc[VEOL2] = 0; /* '\0' */
}

int serial_check(const char* device)
{
    int ret;

    ret = access(device, R_OK);
    if(0 == ret)
    {
        LOGI("devices : %s is access\n", device);
    }
    else
    {
        LOGE("err: devices : %s is not exist\n", device);
    }
    return ret;
}

//串口初始化
int SerialOperation::serial_init()
{
    if(serial_check() < 0)
    {
        LOGE("Failed to access  serial\n");
        return -1;
    }

    if ((fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY)) < 0)
    {
        LOGE( "can't open serial port!\n");
        return -1;
    }

    LOGI( "success to open %s , fd : %d\n", device, fd);

    /*配置串口*/
    set_opt(fd,brate,8,'N',1);
    return 0;
}


int SerialOperation::serial_read(unsigned char* buf, int len)
{
    if(!fd)
    {
        LOGE("serial is not open!\n");
        return -1;
    }

    unsigned int nread = 0;
    //printf("Begin to read serial ...\n");

    memset(buf, 0, len);
    nread = read(fd, buf, len);

    if(nread <= 0)
    {
        LOGD("No data read\n");
    }
    else
    {
        ;
    }

    return nread;
}

int SerialOperation::serial_write(const char* buf)
{
    if(!fd)
        return -1;

    int nwrite = 0;
    nwrite = write(fd, buf, sizeof(buf));
    fsync(fd);

    return nwrite;
}

//串口设备检测
int SerialOperation::serial_check()
{
    int ret;

    ret = access(device, R_OK);
    if(0 == ret)
    {
        LOGI("Discover Devices : %s\n", device);
    }
    else
    {
        //printf("Waiting for device ...\n");
        //sleep(3);
    }
    return ret;
}


int SerialOperation::serial_read_block(unsigned char* buf, int len)
{
    fd_set rfds;
    struct timeval tv;
    int retval, nread, nwrite;

    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    tv.tv_sec = SELECT_WAIT_TIME;
    tv.tv_usec = 0;

    retval = select(1+fd, &rfds, NULL, NULL, &tv);

    if (retval == -1)
    {
        perror("select()");
        LOGE( "select err\n");
        return -1;
    }
    else if (retval)
    {
        if (FD_ISSET(fd, &rfds))
        {
            nread = serial_read(buf, len);
#if 0
            int i;
            for(i=0;i<nread; i++)
            {
                if(buf[i] == 0x81)
                    fprintf(stderr, "\n");
                fprintf(stderr, "%02x ", buf[i]);
                //bo_protocol_deal(buf[i]);
            }
#endif
        }
    }
    else
    {
        LOGD("no data receive\n");
    }
}

/************************************************************
 * 功能: 串口参数配置
 * 输入参数: fd:open打开的文件描述符 nspeed:波特率 nBits:数据位数 
 * nEvent:奇偶校验 nStop:停止位

 ***********************************************************/
int SerialOperation::set_opt(int fd,int nSpeed,int nBits,char nEvent,int nStop)
{
    struct termios newttys1,oldttys1;

    if(tcgetattr(fd,&oldttys1)!=0)     /*保存原先串口配置*/
    {
        LOGE("Setupserial 1");
        return -1;
    }

    bzero(&newttys1,sizeof(newttys1));       /*将一段内存区域的内容全清为零*/
    newttys1.c_cflag|=(CLOCAL|CREAD );       /*CREAD 开启串行数据接收，CLOCAL并打开本地连接模式*/   

    newttys1.c_cflag &=~CSIZE;              /*设置数据位数*/
    switch(nBits)     /*选择数据位*/  
    {
        case 7:
            newttys1.c_cflag |=CS7;
            break;
        case 8:
            newttys1.c_cflag |=CS8;
            break;
    }
    switch( nEvent )    /*设置校验位 */ 
    {
        case '0':       /*奇校验  */
            newttys1.c_cflag |= PARENB;             /*开启奇偶校验 */ 
            newttys1.c_iflag |= (INPCK | ISTRIP);   /*INPCK打开输入奇偶校验；ISTRIP去除字符的第八个比特*/  
            newttys1.c_cflag |= PARODD;             /*启用奇校验(默认为偶校验)*/  
            break;
        case 'E' :       /*偶校验  */
            newttys1.c_cflag |= PARENB;             /*开启奇偶校验 */ 
            newttys1.c_iflag |= ( INPCK | ISTRIP);  /*打开输入奇偶校验并去除字符第八个比特  */
            newttys1.c_cflag &= ~PARODD;            /*启用偶校验；  */
            break;
        case 'N':     /*关闭奇偶校验*/
            newttys1.c_cflag &= ~PARENB;
            break;
    }

    switch( nSpeed )        /*设置波特率 */ 
    {
        case 2400:
            cfsetispeed(&newttys1, B2400);    /*设置输入速度*/
            cfsetospeed(&newttys1, B2400);    /*设置输出速度*/
            break;
        case 4800:
            cfsetispeed(&newttys1, B4800);
            cfsetospeed(&newttys1, B4800);
            break;
        case 9600:
            cfsetispeed(&newttys1, B9600);
            cfsetospeed(&newttys1, B9600);
            break;
        case 115200:
            cfsetispeed(&newttys1, B115200);
            cfsetospeed(&newttys1, B115200);
            break;
        default:
            cfsetispeed(&newttys1, B9600);
            cfsetospeed(&newttys1, B9600);
            break;
    }

    if( nStop == 1)   /*设置停止位；若停止位为1，则清除CSTOPB，若停止位为2，则激活CSTOPB*/ 
    {
        newttys1.c_cflag &= ~CSTOPB;      /*默认为送一位停止位； */ 
    }
    else if( nStop == 2)
    {
        newttys1.c_cflag |= CSTOPB;       /*CSTOPB表示送两位停止位；*/  
    }

    /*设置最少字符和等待时间，对于接收字符和等待时间没有特别的要求时*/
    newttys1.c_cc[VTIME] = 0; /*非规范模式读取时的超时时间； */ 
    newttys1.c_cc[VMIN]  = 0;  /*非规范模式读取时的最小字符数；*/  

    tcflush(fd ,TCIFLUSH); /*tcflush清空终端未完成的输入/输出请求及数据；TCIFLUSH表示清空正收到的数据，且不读取出来 */
    /* 在完成配置后，需要激活配置使其生效*/
    if((tcsetattr( fd, TCSANOW,&newttys1))!=0) /*TCSANOW不等数据传输完毕就立即改变属性*/  
    {
        LOGE("serial set error");
        return -1;
    }
    return 0;
}

