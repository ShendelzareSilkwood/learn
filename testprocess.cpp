#include "testprocess.h"


extern testRecvData testDataBuff[3];
extern SOCKET serSocket;
extern sockaddr_in serAddr;
extern sockaddr_in sinAddr;
extern void transmit();

test::test(QObject *parent): QThread(parent)
{
}

test::~test()
{
}

void test::run()
{
	transmit();

	char* sendData = "debug request";
	sendto(serSocket, sendData, strlen(sendData), 0, (sockaddr*)&sinAddr, sizeof(sinAddr));

	int optVal = 1280 * 320 * 100;
	setsockopt(serSocket, SOL_SOCKET, SO_RCVBUF, (char*)&optVal, sizeof(int));
	int fromlen = sizeof(sinAddr);

	testRecvData* recvbuff = new testRecvData[3];				
	int cnt = 0;								

	while (1)
	{
		int ret = recvfrom(serSocket, (char*)&recvbuff[cnt], sizeof(testRecvData), 0, (sockaddr*)&sinAddr, &fromlen);
		cnt++;
		if (cnt == 1)
		{
			memcpy(&testDataBuff[0], &recvbuff[0], sizeof(testRecvData));
			emit resultReady(0);
		}
		if (cnt == 2)
		{
			memcpy(&testDataBuff[1], &recvbuff[1], sizeof(testRecvData));
			emit resultReady(1);
		}
		if (cnt == 3)
		{
			memcpy(&testDataBuff[2], &recvbuff[2], sizeof(testRecvData));
			emit resultReady(2);
			cnt = 0;
		}
		if (!dataShowFlag)
			break;
	}
}
