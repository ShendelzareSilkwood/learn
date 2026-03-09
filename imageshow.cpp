#include "imageshow.h"


extern uint8_t showImageBuff[320 * 1280];
extern SOCKET serSocket;
extern sockaddr_in serAddr;
extern sockaddr_in sinAddr;
extern void transmit();

imageshow::imageshow(QObject *parent): QThread(parent)
{
}

imageshow::~imageshow()
{
}


void imageshow::run()
{
	transmit();
	char* sendData = "image request";
	sendto(serSocket, sendData, strlen(sendData), 0, (sockaddr*)&sinAddr, sizeof(sinAddr));
	int optVal = 1280 * 320 * 100;
	setsockopt(serSocket, SOL_SOCKET, SO_RCVBUF, (char*)&optVal, sizeof(int));
	int fromlen = sizeof(sinAddr);
	char oneImageFlag[6] = { 0 };

	while (1)
	{
		recvfrom(serSocket, oneImageFlag, sizeof(oneImageFlag), 0, (sockaddr*)&sinAddr, &fromlen);
		if (!strcmp(oneImageFlag, "start"))
		{
			for (int i = 0; i < 320; i++)
			{
				int ret = recvfrom(serSocket, (char*)&showImageBuff[i*1280], sizeof(char) * 1280, 0, (sockaddr*)&sinAddr, &fromlen);
			}
			emit imageReady();
		}
		if (!imageShowFlag)
			break;
	}
	
}
