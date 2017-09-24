#include "RoomManager.h"
#include"ErrorHandler.h"

CRoomManager::CRoomManager():newRoomNumber(StartRoomNum)
{
}


CRoomManager::~CRoomManager()
{
}

RoomListIt CRoomManager::GetMyRoomIter(int ChannelNum, int roomNum)
{
	RoomListIt iterBegin = mRooms.begin();
	RoomListIt iterEnd = mRooms.end();

	for (; iterBegin != iterEnd; ++iterBegin)
	{
		// ä���� ������ Ȯ��
		if (ChannelNum == (*iterBegin)->GetChannelNum())
		{
			// ä���� ���� ���ȣ ���� ������ Ȯ��
			if (roomNum == (*iterBegin)->GetRoomNum())
			{
				return iterBegin;
			}
		}
	}
	cout << roomNum << "�� ���� �����ϴ�." << endl;
	ErrorHandStatic->ErrorHandler(ERROR_GET_ROOM);
	//RoomListIt* error = nullptr;
	return iterBegin; // iterBegin == iterEnd �̸� ���̾���.
}


void CRoomManager::PushRoom(const RoomPtr & shared_newRoom)
{
	ScopeLock<MUTEX> MU(mRAII_RoomManagerMUTEX);
	mRooms.push_back(shared_newRoom);
	++newRoomNumber;
}

int CRoomManager::SearchRoom()
{
	RoomListIt roomBegin = mRooms.begin();
	for (; roomBegin != mRooms.end(); ++roomBegin)
	{
		if (EnterRoomPeopleLimit > (*roomBegin).get()->GetAmountPeople())
		{
			return (*roomBegin).get()->GetRoomNum();
		}
	}
	return NoneRoom;
}

RoomListIt CRoomManager::EraseRoom(RoomListIt deleteTargetRoomIter)
{
	ScopeLock<MUTEX> MU(mRAII_RoomManagerMUTEX);
	RoomListIt delRoom = mRooms.erase(deleteTargetRoomIter);
	return delRoom;
}


RoomListIt CRoomManager::ExitRoom(const LinkPtr & shared_clientInfo)
{
	CLink* client = shared_clientInfo.get();
	if (nullptr == client)
		return mRooms.end();
	RoomListIt myRoomIter = GetMyRoomIter(client->GetMyChannelNum(), client->GetMyRoomNum());
	if (mRooms.end() != myRoomIter)
	{
		string outClientName(shared_clientInfo.get()->GetMyName() + " ���� �濡�� �����̽��ϴ�.");
		(*myRoomIter).get()->Talk(shared_clientInfo, Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, outClientName.c_str()));
		client->SetMyRoomNum(NoneRoom);
		client->InitBetting(); // ���� �ʱ�ȭ ��Ŵ
		//if (true == (*myRoomIter)->IsGame())					// �����߿� ������?
		//{
		//	client->LostWillMoney((*myRoomIter)->GetBattingMoney());	// ���� �ΰ�
		//	client->SaveCalculateMoney(); // ���� ������ ������..�ؾ���
		//	(*myRoomIter)->AllInitBetting();					// �뿡 ����ִ� ��� �غ� �ʱ�ȭ
		//}
		(*myRoomIter).get()->EraseClient(shared_clientInfo);
		return myRoomIter;
	}
	return mRooms.end();
}

int CRoomManager::MakeRoom(const LinkPtr & shared_clientInfo, const string& roomName)
{
	CLink* client = shared_clientInfo.get();
	RoomPtr newRoom(new CRoom(newRoomNumber, client->GetMyChannelNum(), roomName, 0));
	int makedRoomNumber = newRoomNumber; // PushRoom�� ȣ���ϸ� newRoomNumber�� 1 �����ϱ⶧����
	PushRoom(newRoom);
	return makedRoomNumber;

	//if (client->GetMyMoney() >= battingMoney && battingMoney >= 0)
	//{
	//	RoomPtr newRoom(new CRoom(newRoomNumber, client->GetMyChannelNum(), roomName, battingMoney));
	//	int makedRoomNumber = newRoomNumber; // PushRoom�� ȣ���ϸ� newRoomNumber�� 1 �����ϱ⶧����
	//	PushRoom(newRoom);
	//	return makedRoomNumber;
	//}
	//else
	//{
	//	client->SendnMine("���ÿ� �ʿ��� ���� �����Ͽ� ���� ���� �� �����ϴ�.");
	//	ErrorHandStatic->ErrorHandler(ERROR_ROOM_ENTRER_BATTING_MONEY);
	//	return -1;
	//}
}

bool CRoomManager::EnterRoom(const LinkPtr & shared_clientInfo, int targetRoomNumBer)
{
	CLink* client = shared_clientInfo.get(); 
	if (nullptr == client)
		return false;
	if (true == client->IsRoomEnterState()) // �̹� �濡 �ִ��� Ȯ��
		return false;
	RoomListIt targetRoomIter = GetMyRoomIter(client->GetMyChannelNum(), targetRoomNumBer);
	
	if (mRooms.end() != targetRoomIter)
	{
		if (EnterRoomPeopleLimit <= (*targetRoomIter).get()->GetAmountPeople())
		{
			//shared_clientInfo.get()->SendnMine(Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, DialogEnterRoomPeopleLimit.c_str()));
			cout << "�� �� ���� �� ��" << endl;
			return false;
		}
		(*targetRoomIter)->PushClient(shared_clientInfo, targetRoomNumBer);
		return true;
		//int BattingMoney = (*targetRoomIter)->GetBattingMoney();
		//if (BattingMoney <= client->GetMyMoney())
		//{
		//	(*targetRoomIter)->PushClient(shared_clientInfo, targetRoomNumBer);
		//	return true;
		//}
		//else
		//{
		//	shared_clientInfo.get()->SendnMine(EnterRoomMoneyLack);
		//	ErrorHandStatic->ErrorHandler(ERROR_ROOM_ENTRER_BATTING_MONEY, shared_clientInfo);
		//}
	}
	return false;
}

bool CRoomManager::IsAllReadyGame(const LinkPtr & shared_clientInfo)
{
	CLink* client = shared_clientInfo.get();
	if (nullptr == client)
		return false;
	RoomListIt targetRoomIter = GetMyRoomIter(client->GetMyChannelNum(), client->GetMyRoomNum());
	if (mRooms.end() != targetRoomIter)
	{
		//client->SetReadyGame((*targetRoomIter).get()->GetBattingMoney());
		return ((*targetRoomIter)->IsAllReady());
	}
	return false;
}

void CRoomManager::Broadcast(const LinkPtr & shared_clientInfo, const Packet & packet, int flags)
{
	CLink* client = shared_clientInfo.get();
	if (nullptr == client)
		return;
	RoomListIt myRoomIter = GetMyRoomIter(client->GetMyChannelNum(), client->GetMyRoomNum());
	if (mRooms.end() != myRoomIter)
	{
		(*myRoomIter).get()->Broadcast(packet, flags);
	}
}

void CRoomManager::Talk(const LinkPtr & shared_clientInfo, const Packet & packet, int flags)
{
	CLink* client = shared_clientInfo.get();
	if (nullptr == client)
		return;
	RoomListIt myRoomIter = GetMyRoomIter(client->GetMyChannelNum(), client->GetMyRoomNum());
	if (mRooms.end() != myRoomIter)
	{
		(*myRoomIter).get()->Talk(shared_clientInfo, packet, flags);
	}
}



