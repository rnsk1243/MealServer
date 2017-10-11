#include "CommandController.h"
#include"ErrorHandler.h"
#include"ReadHandler.h"
class CErrorHandler;

CCommandController::CCommandController()
{
}


CCommandController::~CCommandController()
{
	//cout << "CNetWork ��ü �Ҹ��� ȣ��" << endl;
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

void CCommandController::EnterRoom(const LinkPtr & shared_clientInfo)
{
	bool isSpecialRoom = false;
	int roomNumber = AutoMakeORSearchRoom(shared_clientInfo, isSpecialRoom);

	if (isSpecialRoom)
	{
<<<<<<< HEAD
		//cout << "room ���� ����" << endl;
		enterRoomNumber = MakeRoom(shared_clientInfo, RoomInitName, EnterAutoMatchingRoomPeopleLimit);
=======
		EnterSpecialRoomFunc(shared_clientInfo, roomNumber);
>>>>>>> origin/WOOHEE
	}
	else
	{
		//cout << enterRoomNumber << " �� ������ ����" << endl;
		if (mRoomManager.EnterRoom(shared_clientInfo, roomNumber))
		{
			mChannelManager.ExitChannel(shared_clientInfo);
			return;
		}
	}

}

void CCommandController::EnterSpecialRoomFunc(const LinkPtr& shared_clientInfo, const int & roomNumber, const string & pw)
{
	if (NoneRoom != roomNumber)
	{
		//cout << enterRoomNumber << " �� ������ ����" << endl
		if (mRoomManager.EnterRoomSpecial(shared_clientInfo, roomNumber, pw))
		{
			mChannelManager.ExitChannel(shared_clientInfo);
		}
	}
}

void CCommandController::EnterSpecialRoomFunc(const LinkPtr& shared_clientInfo, const int & roomNumber, const string & pw)
{
	if (NoneRoom != roomNumber)
	{
		//cout << enterRoomNumber << " �� ������ ����" << endl
		if (mRoomManager.EnterRoomSpecial(shared_clientInfo, roomNumber, pw))
		{
			mChannelManager.ExitChannel(shared_clientInfo);
		}
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

<<<<<<< HEAD
int CCommandController::MakeRoom(const LinkPtr & shared_clientInfo, const string & roomName, const ProtocolTeamAmount& teamAmount, const string & roomPW)
=======
int CCommandController::MakeRoom(const LinkPtr & shared_clientInfo, const string & roomName, const ProtocolTeamAmount& teamAmount, const bool & isSpecialRoom, const string & roomPW)
>>>>>>> origin/WOOHEE
{
	CLink* client = shared_clientInfo.get();
	if (nullptr != client && (false == client->IsRoomEnterState()))
	{
		// ���� �����
<<<<<<< HEAD
		int newRoomNumber = mRoomManager.MakeRoom(shared_clientInfo, roomName, teamAmount, roomPW);
=======
		int newRoomNumber = mRoomManager.MakeRoom(shared_clientInfo, roomName, teamAmount, isSpecialRoom, roomPW);
>>>>>>> origin/WOOHEE
		if (NoneRoom != newRoomNumber)
		{
			return newRoomNumber;
		}
	}
	shared_clientInfo.get()->SendnMine(Packet(ProtocolInfo::RequestResult, ProtocolDetail::FailRequest, State::ClientMakeRoom, nullptr)); // makeRoom �⺻ ���·� ���ư���
	return NoneRoom;
}

void CCommandController::OutRoom(const LinkPtr & shared_clientInfo)
{
	//cout << "outRoom ȣ��" << endl;
	ProtocolCharacterTagIndex targetPos = shared_clientInfo.get()->GetMyPosition();
	ProtocolSceneName oldSceneState = shared_clientInfo.get()->GetMySceneState();
	RoomListIt roomIter;
	bool isSucces = false;
	{
		// ���α׷��� �ѹ��� ���̸� �����尡 ���̴���.. �ݺ��ڸ� �߸� �������ԵǾ� ���α׷� ���� .. ��¿ �� ���� ���ɾ����.
		ScopeLock<MUTEX> MU(mRAII_CommandMUTEX);
		roomIter = mRoomManager.ExitRoom(shared_clientInfo, isSucces);	// �뿡�� ������
	}
	if (!isSucces)
		return;
	try
	{
		if (true == (*roomIter).get()->IsRoomEmpty())							// �뿡 �ƹ��� ���� Ȯ��
		{
			mRoomManager.EraseRoom(roomIter);								// �ƹ��� ������ �� ����
		}
		else
		{
			if (ProtocolSceneName::RoomScene == oldSceneState)	// RoomScene���� �������� ��ο��� �������� �˸��� �гο��� �ڽ��� ����� ��Ŵ
			{
				cout << "���� ����� ��ο��� �˸�" << endl;
				Packet packet(ProtocolInfo::ClientCommend, ProtocolDetail::RemovePanel, targetPos, nullptr);
				(*roomIter)->Broadcast(packet);
				(*roomIter)->NotReadyTogether(); // ������ �ѻ���̶� ������ Ready Ǯ��
			}
		}
	}
	catch (const std::exception&)
	{
		return;
	}
}
void CCommandController::DeleteClientSocket(const LinkPtr & shared_clientInfo)
{
	int myChannelNum = shared_clientInfo.get()->GetMyChannelNum();
	int myRoomNum = shared_clientInfo.get()->GetMyRoomNum();
	//cout << "DeleteClientSocket���� myRoomNum = " << myRoomNum << endl;
	//�濡 �ֳ� ä�ο� �ֳ� Ȯ��
	if (NoneRoom == myRoomNum)
	{
		// ä���϶�
		if (mChannelManager.ExitChannel(shared_clientInfo))
		{
			//cout << "channel���� �������ϴ�." << endl;
		}
	}
	else
	{
		// ���϶�
		OutRoom(shared_clientInfo);
		//cout << "room���� �����ϴ�." << endl;
	}
}

void CCommandController::SendAllReadyGameNotice(const LinkPtr & shared_clientInfo)
{
	if (mRoomManager.IsAllReadyGame(shared_clientInfo))
	{
		cout << "��� ���� ��" << endl;
		// ��޴����� ���� �� ��� �Լ� ȣ�� �� ��.
		//mRoomManager.Broadcast(shared_clientInfo, Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "��� �÷��̾ �غ� �Ǿ����ϴ�."));
		//mRoomManager.Broadcast(shared_clientInfo, Packet(ProtocolInfo::SceneChange, ProtocolDetail::NoneDetail, ProtocolSceneName::MainScene, nullptr));
	}
}

void CCommandController::ChangeCharacter(const LinkPtr & shared_clientInfo, Packet& packet)
{
	mRoomManager.ChangeMyCharacter(shared_clientInfo, packet);
}

void CCommandController::GetHostIP(const LinkPtr & shared_clientInfo)
{
	mRoomManager.GetHostIP(shared_clientInfo);
}

int CCommandController::AutoMakeORSearchRoom(const LinkPtr & shared_clientInfo, bool & isSpecialRoom)
{
	int enterRoomNumber = mRoomManager.SearchRoom(isSpecialRoom);
	// ����� ���� ��� ����
	if (NoneRoom == enterRoomNumber)
	{
		//cout << "room ���� ����" << endl;
		enterRoomNumber = MakeRoom(shared_clientInfo, RoomInitName, EnterAutoMatchingRoomPeopleLimit, false); 
		isSpecialRoom = false;
	}
	return enterRoomNumber;
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
			//cout << "���� ��� ����" << endl;
			EnterRoom(shared_clientInfo); // �ڵ���Ī ����
		}
		else if (ProtocolDetail::EnterSpecialRoom == packet.InfoProtocolDetail)
		{
			vector<string> roomNumberPWVector = ReadHandlerStatic->Parse(packet.InfoValue, '/');
			EnterSpecialRoomFunc(shared_clientInfo, stoi(roomNumberPWVector.at(0)), roomNumberPWVector.at(1));
		}
		else if (ProtocolDetail::EnterChanel == packet.InfoProtocolDetail)
		{
			ChangeChannel(shared_clientInfo, stoi(packet.InfoValue));
		}
		else if (ProtocolDetail::MakeRoom == packet.InfoProtocolDetail)
		{
<<<<<<< HEAD
			int roomNumber = MakeRoom(shared_clientInfo, RoomInitName, (ProtocolTeamAmount)packet.InfoTagIndex, packet.InfoValue/*pw*/);
=======
			int roomNumber = MakeRoom(shared_clientInfo, RoomInitName, (ProtocolTeamAmount)packet.InfoTagIndex, true, packet.InfoValue/*pw*/);
>>>>>>> origin/WOOHEE
			EnterSpecialRoomFunc(shared_clientInfo, roomNumber, packet.InfoValue);
			//int battingMoney = stoi(commandString.at(2));
			//MakeRoom(shared_clientInfo, packet.InfoValue);
		}
		else if (ProtocolDetail::OutRoom == packet.InfoProtocolDetail)
		{
			if (true == mChannelManager.EnterMyChannel(shared_clientInfo)) // ä�ο� ����
			{
				OutRoom(shared_clientInfo);
			}
		}
		else if (ProtocolDetail::SetReadyGame == packet.InfoProtocolDetail)
		{
			shared_clientInfo.get()->SetReadyGame();
			SendAllReadyGameNotice(shared_clientInfo);
		}
		else if (ProtocolDetail::NotReadyGame == packet.InfoProtocolDetail)
		{
			shared_clientInfo.get()->SetNoReadyGame();
		}
		else if (ProtocolDetail::ChangeCharacter == packet.InfoProtocolDetail)
		{
			ChangeCharacter(shared_clientInfo, packet);
		}
		else if (ProtocolDetail::GetHostIP == packet.InfoProtocolDetail)
		{
			GetHostIP(shared_clientInfo);
		}
	}
	catch (const std::exception&)
	{
		int channelNum = 0;
		//cout << "���ó�� ����" << endl;
		ErrorHandStatic->ErrorHandler(ERROR_COMMAND, shared_clientInfo);
	}
}

void CCommandController::ChattingMessage(const LinkPtr & shared_clientInfo, Packet & packet)
{
	if (ProtocolDetail::Message != packet.InfoProtocolDetail || ProtocolMessageTag::Text != packet.InfoTagIndex)
	{
		return;
	}

	try
	{
		packet.AddName(shared_clientInfo.get()->GetMyName());
		if (shared_clientInfo.get()->IsRoomEnterState())
		{
			mRoomManager.Talk(shared_clientInfo, packet);
		}
		else
		{
			//mChannelManager.Talk(shared_clientInfo, packet);
		}
	}
	catch (const std::exception&)
	{
		int channelNum = 0;
		//cout << "���ó�� ����" << endl;
		ErrorHandStatic->ErrorHandler(ERROR_COMMAND, shared_clientInfo);
	}
}

