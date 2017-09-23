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

void CCommandController::MakeRoom(const LinkPtr & shared_clientInfo, const string & roomName)
{
	CLink* client = shared_clientInfo.get();
	if (nullptr != client && (false == client->IsRoomEnterState()))
	{
		// ���� �����
		int newRoomNumber = mRoomManager.MakeRoom(shared_clientInfo, roomName);
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
		mRoomManager.Broadcast(shared_clientInfo, Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "��� �÷��̾ �غ� �Ǿ����ϴ�."));
	}
	else
	{
		shared_clientInfo.get()->SendnMine(Packet(ProtocolInfo::ChattingMessage,ProtocolDetail::Message, ProtocolMessageTag::Text, "�ٸ� ��� �÷��̾ �غ� �Ǿ�� �մϴ�."));
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



void CCommandController::CommandHandling(const LinkPtr& shared_clientInfo, Packet& packet)
{
	try
	{
		if (ProtocolDetail::EnterRoom == packet.InfoProtocolDetail) // �濡 ����
		{
			EnterRoom(shared_clientInfo, stoi(packet.InfoValue/*commandString.at(1)*/));
		}
		else if (ProtocolDetail::EnterChanel == packet.InfoProtocolDetail)
		{
			ChangeChannel(shared_clientInfo, stoi(packet.InfoValue));
		}
		else if (ProtocolDetail::MakeRoom == packet.InfoProtocolDetail)
		{
			//int battingMoney = stoi(commandString.at(2));
			MakeRoom(shared_clientInfo, packet.InfoValue);
		}
		else if (ProtocolDetail::OutRoom == packet.InfoProtocolDetail)
		{
			if (true == mChannelManager.EnterMyChannel(shared_clientInfo)) // ä�ο� ����
			{
				OutRoom(shared_clientInfo);
			}
		}
		else if (ProtocolDetail::ReadyGame == packet.InfoProtocolDetail)
		{
			SendAllReadyGameNotice(shared_clientInfo);
		}
		else if(ProtocolDetail::Message == packet.InfoProtocolDetail)
		{
			if (shared_clientInfo.get()->IsRoomEnterState())
			{
				mRoomManager.Talk(shared_clientInfo, packet);
			}
			else
			{
				mChannelManager.Talk(shared_clientInfo, packet);
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

