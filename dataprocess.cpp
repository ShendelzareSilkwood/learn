#include "dataprocess.h"
#include <vector>

using namespace std;
extern SOCKET serSocket;
extern sockaddr_in serAddr;
extern sockaddr_in sinAddr;
extern void transmit();
extern recvData showBuff1[320];
extern recvData showBuff2[320];
extern recvData showBuff3[320];

dataprocess::dataprocess(QObject *parent): QThread(parent)
{
}

dataprocess::~dataprocess()
{
}



void dataprocess::run() 
{
	transmit();

	char sendData[] = "pointcloud request";
	sendto(serSocket, sendData, strlen(sendData), 0, (sockaddr*)&sinAddr, sizeof(sinAddr));

	//设定socket接收buff的大小
	int optval = sizeof(recvData) * 3000;
	int optLEn = sizeof(int);
	setsockopt(serSocket, SOL_SOCKET, SO_RCVBUF, (char*)&optval, optLEn);

	int len = sizeof(sinAddr);			//发送端套接字的长度

	recvData* recvbuff = new recvData[960];				//本地接收数据buff
	int addressNum = 0;									//数据接收buff环形位置记录

	while (1)
	{
		int ret = recvfrom(serSocket, (char*)&recvbuff[addressNum], sizeof(recvData), 0, (sockaddr*)&sinAddr, &len);
		if (recvbuff[addressNum].Num != addressNum)
//			emit recvError(addressNum);
		addressNum++;
		if (addressNum == 320)
		{
			memcpy(showBuff1, &recvbuff[0], sizeof(recvData) * 320);
			emit resultReady(1);
		}
		if (addressNum == 640)
		{
			memcpy(showBuff2, &recvbuff[320], sizeof(recvData) * 320);
			emit resultReady(2);
		}
		if (addressNum == 960)
		{
			memcpy(showBuff3, &recvbuff[640], sizeof(recvData) * 320);
			emit resultReady(3);
			addressNum = 0;
		}
	}

}




