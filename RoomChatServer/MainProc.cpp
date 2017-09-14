//#pragma comment(lib,"version.lib")
#include<iostream>
#include"ReadyNetWork.h"
#include"CommandController.h"
#include"Link.h"
#include"RoomManager.h"
#include"ChannelManager.h"
#include"Channel.h"
#include"Listener.h"
#include"Lobby.h"
#include<process.h>
#include<thread>
#include"ErrorHandler.h"
#include"resource.h"
#include"BasicExcel.hpp"
#include<Windows.h>
#include"MinidumpHelp.h"
#include"ReadHandler.h"
#include"ConstEnumInfo.h"
using namespace std;
using namespace YExcel;

void printVersionInfo()
{
	// ���������� ���� ����
	char* buffer = NULL;
	// ������ Ȯ���� ����
	char* name = "RoomChatServer.exe";

	DWORD infoSize = 0;

	// ���Ϸκ��� ���������������� ũ�Ⱑ �������� ���մϴ�.
	infoSize = GetFileVersionInfoSize(name, 0);
	if (infoSize == 0) return;
	
	// �����Ҵ�
	buffer = new char[infoSize];
	if (buffer)
	{
		// �������������͸� �����ɴϴ�.
		if (GetFileVersionInfo(name, 0, infoSize, buffer) != 0)
		{
			VS_FIXEDFILEINFO* pFineInfo = NULL;
			UINT bufLen = 0;
			// buffer�� ���� VS_FIXEDFILEINFO ������ �����ɴϴ�.
			if (VerQueryValue(buffer, "\\", (LPVOID*)&pFineInfo, &bufLen) != 0)
			{
				WORD majorVer, minorVer, buildNum, revisionNum;
				majorVer = HIWORD(pFineInfo->dwFileVersionMS);
				minorVer = LOWORD(pFineInfo->dwFileVersionMS);
				buildNum = HIWORD(pFineInfo->dwFileVersionLS);
				revisionNum = LOWORD(pFineInfo->dwFileVersionLS);

				// ���Ϲ��� ���
				printf("version : %d,%d,%d,%d\n", majorVer, minorVer, buildNum, revisionNum);
			}
		}
		delete[] buffer;
	}
}


int thSendRecv(void* v_clientSocket)
{
	const SOCKET* clientSocket = ((SOCKET*)v_clientSocket);
	CGuestLink guest(clientSocket);
	int isLogin = 0;
	vector<string> userInfo;
	while (SUCCES_LOGIN != isLogin)
	{
		isLogin = LobbyStatic->ActionServiceLobby(&guest, userInfo);
	}
	LinkPtr shared_clientInfo(new CLink(clientSocket, userInfo[IndexUserPK], userInfo[IndexUserID].c_str()));

	// EnterChannelNum ä�ο� ����
	vector<string> commandChannel;
	CommandControllerStatic->SetEnterChannel(shared_clientInfo, StartEnterChannelNum);
	if (false == ReadHandlerStatic->ReadUserGoods(shared_clientInfo, NameMemberGoodsTxt))
		return 0;

	cout << "���� ��ȭ = " << shared_clientInfo->GetMyMoney() << endl;
	shared_clientInfo.get()->SendnMine("�α��� ����");
	ErrorHandStatic->ErrorHandler(SUCCES_LOGIN, shared_clientInfo);
	while (true)
	{
		string recvMessage;
		ListenerStatic->RecvnLink(shared_clientInfo, recvMessage);
		vector<string> commandMessage = ReadHandlerStatic->Parse(recvMessage, '/');
		CommandControllerStatic->CommandHandling(shared_clientInfo, commandMessage);
	}
}

void main()
{
	//MinidumpHelp dump;
	//dump.install_self_mini_dump();
	/////////// ���� ���� ��� ///////////
	printVersionInfo();
	//////////////////////////////////////
	BasicExcel excel;
	excel.ReadExcel("CharacterDB.xls");
	

	while (true)
	{
		SOCKET* clientSocket = new SOCKET();
		ReadyNetWorkStatic->Accept(clientSocket);
		
		thread clientThread(thSendRecv, clientSocket);
		clientThread.detach();
	}
	getchar();
}