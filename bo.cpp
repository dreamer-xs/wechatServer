#include "bo.h"

//获取串口设备,并打开串口
Bo::Bo(const char* dev, int rate)
{
    LOGI("dev: %s rate: %d\n", dev, rate);
    serial = new SerialOperation(dev, rate);
}

Bo::~Bo()
{
    if(serial)
        delete(serial);
}

/*血氧模块协议解析
 *
 */
void Bo::bo_protocol_deal(unsigned char rxbuf)
{
    //fprintf(stderr, "%u\n", rxbuf);
    typedefSpO2Info CeChipInfor;
	unsigned char i,sumcrc,temp;
	//--------------------------------------------------
	if (rxbuf >= CMD_BYTE)// command and reset
	{
		RXBufCnt = 0;
		RXDataBuf[RXBufCnt++] = rxbuf;
	}
	else // disposal datum
	{
		if(RXDataBuf[0] == CMD_SpO2INFOR)//SpO2 measuring information
		{
			RXDataBuf[RXBufCnt++] = rxbuf;
			if (RXBufCnt >= 20)
			{
				sumcrc=0;
				for(i = 0;i<19; i++)sumcrc+=RXDataBuf[i];
				sumcrc=sumcrc&0x7F;
				RXDataBuf[19] = RXDataBuf[19]&0x7F;
				if(sumcrc==RXDataBuf[19])
				{
					//==============================
					CeChipInfor.PluseWave[0] = RXDataBuf[1];
					if(RXDataBuf[11] & 0x01) 
                        CeChipInfor.PluseWave[0] += 0x80;
					CeChipInfor.PluseWave[1] = RXDataBuf[2];
					if(RXDataBuf[11] & 0x02) 
                        CeChipInfor.PluseWave[1] += 0x80;
					CeChipInfor.PluseWave[2] = RXDataBuf[3];
					if(RXDataBuf[11] & 0x04) 
                        CeChipInfor.PluseWave[2] += 0x80;
					CeChipInfor.PluseWave[3] = RXDataBuf[4];
					if(RXDataBuf[11] & 0x08) 
                        CeChipInfor.PluseWave[3] += 0x80;
					CeChipInfor.PluseWave[4] = RXDataBuf[5];
					if(RXDataBuf[11] & 0x10) 
                        CeChipInfor.PluseWave[4] += 0x80;
					CeChipInfor.PluseWave[5] = RXDataBuf[6];
					if(RXDataBuf[11] & 0x20) 
                        CeChipInfor.PluseWave[5] += 0x80;
					CeChipInfor.PluseWave[6] = RXDataBuf[7];
					if(RXDataBuf[11] & 0x40) 
                        CeChipInfor.PluseWave[6] += 0x80;
					CeChipInfor.PluseWave[7] = RXDataBuf[8];
					if(RXDataBuf[12] & 0x01) 
                        CeChipInfor.PluseWave[7] += 0x80;
					CeChipInfor.PluseWave[8] = RXDataBuf[9];
					if(RXDataBuf[12] & 0x02) 
                        CeChipInfor.PluseWave[8] += 0x80;
					CeChipInfor.PluseWave[9] = RXDataBuf[10];
					if(RXDataBuf[12] & 0x04) 
                        CeChipInfor.PluseWave[9] += 0x80;

					//for(i=0;i<10;i++)
					//{
					//    CeChipInfor.PluseWave[i]=255-CeChipInfor.PluseWave[i];
					//}

					//-------------
					// update pulse wave
					//------------
					temp = RXDataBuf[15]&0x7F;
					if(temp>100)temp=100;
					if(temp<30)temp=0; // SpO2
					CeChipInfor.SpO2Value=temp;
					//-------------
					// update SpO2 value
					//------------
					temp = RXDataBuf[14]&0x7F;
					if(RXDataBuf[13]&BIT5){temp=temp|0x80;}
					if (temp < 30)temp = 0;
					if (temp > 240)temp = 240;
					CeChipInfor.PulseRate=temp;
					//-------------
					// update pulse rate
					//------------
					temp = 0;
					if(RXDataBuf[16]&0x01)temp = PULSESEARCH;
					else if(RXDataBuf[16]&0x02)temp = SENSORON;
					else if(RXDataBuf[16]&0x04)temp = SENSOROFF;
					else if(RXDataBuf[16]&0x08)temp = SENSORDISCON;
					else if(RXDataBuf[16]&0x10)temp = INTMOTION;
					else if(RXDataBuf[16]&0x20)temp = INTSUNLIGHT;
					else if(RXDataBuf[16]&0x40)temp = INTPOWER;
					else if(RXDataBuf[17]&0x04)temp = SENORFAULT;
					else if(RXDataBuf[17]&0x08)temp = SENSORSHORT;
					else if(RXDataBuf[17]&0x10)temp = OVERVOLTAGE;
					else if(RXDataBuf[17]&0x20)temp = MODULEERROR;
					else if(RXDataBuf[13]&0x10)temp = LOWBATTERY;
					CeChipInfor.SensorSituation=temp;
					// update sensor state
					CeChipInfor.Noise = RXDataBuf[17]&0x03;
					// update noise
					CeChipInfor.PulseBar = RXDataBuf[13]&0x0F; // BAR
					// update pulse bar
					if (RXDataBuf[17]&BIT6)CeChipInfor.Perfusion = 128 + RXDataBuf[18]; //
					else CeChipInfor.Perfusion = RXDataBuf[18]; //
					// update perfusion

                    //发送脉率值到前端
                    //qmlData.sendData(PR,CeChipInfor.PulseRate);
                    //发送血氧值到前端
                    //qmlData.sendData(BLOOD_OXYGEN,CeChipInfor.SpO2Value);

                    /*脉率波数据类型转换，累计８组发给前端绘图(１秒)
                     * 血氧模块每125ms发送一帧，每帧包含10个脉率波值
                     */
                    int len;
#if 0
                    for(len=0;len<10;len++)
                    {
                        boList<<((int)(CeChipInfor.PluseWave[len]));
                    }

                    if(boList.length() >= 80)
                    {
                        //qDebug()<<boList;
                        //qmlData.sendData(BLOOD, boList);
                        boList.clear();
                        LOGD("send PluseWave value to qml\n");
                    }
#endif
#if 1
                    fprintf(stderr, "PulseRate: %03u SpO2Value: %03d |",    \
                            CeChipInfor.PulseRate, \
                            CeChipInfor.SpO2Value);
                    for(len=0;len<10;len++)
                        fprintf(stderr, "[%d]: %03u |", len, CeChipInfor.PluseWave[len]);

                    fprintf(stderr, "\n");
#endif
				}
				else
				{
					// report error
				}
			}
		}
		else if(RXDataBuf[0] == CMD_SpO2AVERAGESETTING)
		{
			RXDataBuf[RXBufCnt++] = rxbuf;
			if (RXBufCnt >= 3)
			{
				sumcrc=(RXDataBuf[0]+RXDataBuf[1])&0x7F;
				RXDataBuf[2] = RXDataBuf[2] &0x7F;
				if(sumcrc==RXDataBuf[2])
				{
					// report the SpO2 average setting on eChip
				}
				else
				{
					// report error
				}
			}
		}
		else if(RXDataBuf[0] == CMD_VERSION)// version
		{
			RXDataBuf[RXBufCnt++] = rxbuf;
			if (RXBufCnt >= 3)
			{
				sumcrc=(RXDataBuf[0]+RXDataBuf[1])&0x7F;
				RXDataBuf[2] = RXDataBuf[2] &0x7F;
				if(sumcrc==RXDataBuf[2])
				{
					CeChipInfor.Version=RXDataBuf[1];// report version
				}
				else
				{
					// report error
				}
			}
		}
		else if(RXDataBuf[0] == CMD_PARTNUM)//part number
		{
			RXDataBuf[RXBufCnt++] = rxbuf;
			if (RXBufCnt >= 10)
			{
				sumcrc=0;
				for(i = 0;i<9; i++)sumcrc+=RXDataBuf[i];
				sumcrc=sumcrc&0x7F;
				RXDataBuf[9] = RXDataBuf[9] &0x7F;
				if(sumcrc==RXDataBuf[9])
				{
					for(i = 0;i<8; i++)
						CeChipInfor. PartNum [i]=RXDataBuf[i+1];// report part number
				}
				else
				{
					// report error Part number
				}
			}
		}
		else if(RXDataBuf[0] == CMD_SELFCHECK)// self check
		{
			RXDataBuf[RXBufCnt++] = rxbuf;
			if (RXBufCnt >= 3)
			{
				sumcrc=(RXDataBuf[0]+RXDataBuf[1])&0x7F;
				RXDataBuf[2] = RXDataBuf[2] &0x7F;
				if(sumcrc==RXDataBuf[2])
				{
					// report self-check information
				}
				else
				{
					// report error
				}
			}
		}
		else if(RXDataBuf[0] == CMD_MANUFACTURER)// manufacturer深圳安维森实业有限公司
		{
			RXDataBuf[RXBufCnt++] = rxbuf;
			if (RXBufCnt >= 18)
			{
				sumcrc=0;
				for(i = 0;i<17; i++)sumcrc+=RXDataBuf[i];
				sumcrc=sumcrc&0x7F;
				RXDataBuf[17] = RXDataBuf[17] &0x7F;
				if(sumcrc==RXDataBuf[17])
				{
					for(i = 0;i<16; i++)
						CeChipInfor. Manufacturer [i]=RXDataBuf[i+1];// report Manufacturer
				}
				else
				{
					// report error Manufacturer
				}
			}
		}
		else // error command or CMD_REVERSE
		{
			RXBufCnt = 0;// error
		}

	}
}


void Bo::run()
{

    LOGI("bo thread begin to read data\n");

    fd_set rfds;
    struct timeval tv;
    int retval, nread, nwrite;
    unsigned char buf[BUFF_LEN];

    for (;;) 
    { 
        nread = serial->serial_read_block(buf, BUFF_LEN);

        //LOGI("nread: %d\n", nread);
        if(nread)
        {
            int i;
            for(i=0;i<nread; i++)
            {
#if 0
                if(buf[i] == 0x81)
                    fprintf(stderr, "\n");
                fprintf(stderr, "%02x ", buf[i]);
#else
                bo_protocol_deal(buf[i]);
#endif
            }
        }
    } 

    LOGI("bo thread end to read data\n");
}

