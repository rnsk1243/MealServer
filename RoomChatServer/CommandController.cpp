#include "CommandController.h"
#include"ErrorHandler.h"
class CErrorHandler;

CCommandController::CCommandController()
{
}


CCommandController::~CCommandController()
{
	cout << "CNetWork ��ü �Ҹ��� ȣ��" << endl;
}

void CCommandController::SetEnterChannel(const LinkPtr & shared_clientInfo, const int & moveChannelNumber)
{
	if (nullptr != shared_clientInfo.get())
	{
		if (ChannelAmount >= moveChannelNumber && StartEnterChannelNum <= moveChannelNumber)
		{
			if (0 == shared_clientInfo.get()->GetMyChannelNum())
			{
				mChannelManager.MoveChannel(shared_clientInfo, moveChannelNumber);
			}
		}
	}
}

void CCommandController::EnterRoom(const LinkPtr & shared_clientInfo, const int& roomNumber)
{
	if (mRoomManager.EnterRoom(shared_clientInfo, roomNumber))
	{
		mChannelManager.ExitChannel(shared_clientInfo);
	}
}

void CCommandController::ChangeChannel(const LinkPtr& shared_clientInfo, const int & moveChannelNumber)
{
	if (nullptr != shared_clientInfo.get())
	{
		if (false == shared_clientInfo.get()->IsRoomEnterState())
		{
			if (ChannelAmount >= moveChannelNumber && StartEnterChannelNum <= moveChannelNumber)
			{
				if (mChannelManager.ExitChannel(shared_clientInfo))
				{
					mChannelManager.MoveChannel(shared_clientInfo, moveChannelNumber);
				}
			}
		}	
	}
}

void CCommandController::MakeRoom(const LinkPtr & shared_clientInfo, const string & roomName, const int & battingMoney)
{
	CLink* client = shared_clientInfo.get();
	if (nullptr != client && (false == client->IsRoomEnterState()))
	{
		// ���� �����
		int newRoomNumber = mRoomManager.MakeRoom(shared_clientInfo, roomName, battingMoney);
		if (-1 != newRoomNumber)
		{
			// �뿡 ����
			EnterRoom(shared_clientInfo, newRoomNumber);
		}
	}
}

void CCommandController::OutRoom(const LinkPtr & shared_clientInfo)
{
	 RoomListIt roomIter = mRoomManager.ExitRoom(shared_clientInfo);	// �뿡�� ������
	 if (true == (*roomIter)->IsRoomEmpty())			// �뿡 �ƹ��� ���� Ȯ��
	 {
		 mRoomManager.EraseRoom(roomIter);			// �ƹ��� ������ �� ����
	 }
}
void CCommandController::DeleteClientSocket(const LinkPtr & shared_clientInfo)
{

	int myChannelNum = shared_clientInfo.get()->GetMyChannelNum();
	int myRoomNum = shared_clientInfo.get()->GetMyRoomNum();
	//�濡 �ֳ� ä�ο� �ֳ� Ȯ��
	if (NoneRoom == myRoomNum)
	{
		// ä���϶�
		if (mChannelManager.ExitChannel(shared_clientInfo))
		{
			cout << "channel���� �������ϴ�." << endl;
		}
	}
	else
	{
		// ���϶�
		OutRoom(shared_clientInfo);
		cout << "room���� �����ϴ�." << endl;
	}
}

void CCommandController::SendAllReadyGameNotice(const LinkPtr & shared_clientInfo)
{
	if (mRoomManager.IsAllReadyGame(shared_clientInfo))
	{
		// ��޴����� ���� �� ��� �Լ� ȣ�� �� ��.
		mRoomManager.Broadcast(shared_clientInfo, "��� �÷��̾ �غ� �Ǿ����ϴ�.");
	}
	else
	{
		shared_clientInfo.get()->SendnMine("�ٸ� ��� �÷��̾ �غ� �Ǿ�� �մϴ�.");
	}
	
}

CCommandController * CCommandController::GetInstance()
{
	if (nullptr == CommandControllerStatic)
	{
		CommandControllerStatic = new CCommandController();
	}
	return CommandControllerStatic;
}



void CCommandController::CommandHandling(const LinkPtr& shared_clientInfo, vector<string>& commandString)
{
	try
	{
		if (0 == commandString.at(0).compare(CommandEnter)) // �濡 ����
		{
			EnterRoom(shared_clientInfo, stoi(commandString.at(1)));
		}
		else if (0 == commandString.at(0).compare(CommandChannal))
		{
			ChangeChannel(shared_clientInfo, stoi(commandString.at(1)));
		}
		else if (0 == commandString.at(0).compare(CommandMakeRoom))
		{
			int battingMoney = stoi(commandString.at(2));
			MakeRoom(shared_clientInfo, commandString.at(1), battingMoney);
		}
		else if (0 == commandString.at(0).compare(CommandOutRoom))
		{
			if (true == mChannelManager.EnterMyChannel(shared_clientInfo)) // ä�ο� ����
			{
				OutRoom(shared_clientInfo);
			}
		}
		else if (0 == commandString.at(0).compare(CommandGameStart))
		{
			SendAllReadyGameNotice(shared_clientInfo);
		}
		else
		{
			if (shared_clientInfo.get()->IsRoomEnterState())
			{
				mRoomManager.Talk(shared_clientInfo, commandString.at(0));
			}
			else
			{
				mChannelManager.Talk(shared_clientInfo, commandString.at(0));
			}
		}
	}
	catch (const std::exception&)
	{
		int channelNum = 0;
		cout << "���ó�� ����" << endl;
		ErrorHandStatic->ErrorHandler(ERROR_COMMAND, shared_clientInfo);
	}
}

