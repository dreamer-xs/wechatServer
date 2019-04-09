#ifndef __BO__H__
#define __BO__H__

#include <QThread>
//#include "src/SerialPort/myglobal.h"
//#include "src/commom/log.h"
#include "log.h"
#include "serialOperation.h"

#ifdef __cplusplus
extern "C"{
#endif

/****************血氧模块协议*****************/
#define BIT6  (1 <<6)
#define BIT5  (1 <<5)

////////////////////////////////////////////////////////
enum DefineHostCommand
{
	CMD_BYTE=0x80,
	CMD_SpO2INFOR, //0x81
	CMD_SpO2AVERAGESETTING,// 0x82
	CMD_VERSION,
	CMD_PARTNUM,
	CMD_SELFCHECK,
	CMD_MANUFACTURER,
	//CMD_REVERSE,
};
enum DefineSystemState
{
	PULSESEARCH=1, //1
	SENSORON,//normal 2
	//--------------
	SENSOROFF,// probe off 3
	SENSORDISCON,// no sensor 4
	INTMOTION,// artifact motion 5
	INTSUNLIGHT,//too much ambient light 6
	INTPOWER,// too much power noise 7
	//-------------
	SENORFAULT,//sensor fault 8
	SENSORSHORT, // sensor short 9
	OVERVOLTAGE, // over voltage 10
	MODULEERROR, //module error 11
	LOWBATTERY,// low battery 12
};
typedef struct
{
	unsigned char PulseBar; //
	unsigned char SensorSituation; //
	unsigned int PulseRate; // pulse rate [0-240]
	unsigned char SpO2Value; // SpO2 value [0~100]
	unsigned char Noise;//0,1,2,3,4
	unsigned char Perfusion; // perfusion [1-200]
	unsigned char PluseWave[10]; //pulse wave [0-255]
	char PartNum[8];// ascii
	char Manufacturer[16]; // ascii
	unsigned char Version;
}typedefSpO2Info;

/****************血氧模块协议*****************/

#ifdef __cplusplus
};
#endif

#define BUFF_LEN        1024

class Bo: public QThread
{
    Q_OBJECT
public:
    Bo(const char* dev, int rate);
    ~Bo();
    void run();
    void bo_protocol_deal(unsigned char rxbuf);

private:
    SerialOperation* serial;

    unsigned char RXDataBuf[20];
    unsigned char RXBufCnt;
};

#endif //__BO__H__

