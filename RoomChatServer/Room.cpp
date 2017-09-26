#include "Room.h"
#include"ErrorHandler.h"
#include"ConstEnumInfo.h"

CRoom::CRoom(int roomNum, int channelNum, const string& roomName, const int& battingMoney) :
	mRoomNum(roomNum),
	mChannelNum(channelNum),
	mRoomName(roomName),
	mAmountPeople(0),
	//mBettingMoney(battingMoney),
	//mPlayingGame(false),
	mIsNewRoom(true)
{
	//InitializeCriticalSection(&CS_MyInfoList);
	//cout << "sizeof(ProtocolCharacterTagIndex) = " << sizeof(ProtocolCharacterTagIndex) << endl;
	mUsePosition.reserve(7);
	mUsePosition.assign(mUsePosition.capacity(), NotUsed);
}


CRoom::~CRoom()
{
	cout << mRoomNum << " �� ���� ���� �˴ϴ�." << endl;
}


LinkListIt CRoom::EraseClient(const LinkPtr& shared_client)
{
	LinkListIt delLinkIter = find(mClientInfos.begin(), mClientInfos.end(), shared_client);
	{
		ScopeLock<MUTEX> MU(mRAII_RoomMUTEX);
		delLinkIter = mClientInfos.erase(delLinkIter);
		DecreasePeople();
	}
	return delLinkIter;
}

int CRoom::GetRoomNum()
{
	return mRoomNum;
}

int CRoom::GetChannelNum()
{
	return mChannelNum;
}

const string CRoom::GetRoomName()
{
	return mRoomName;
}

int CRoom::GetAmountPeople()
{
	return mAmountPeople;
}

bool CRoom::IsRoomEmpty()
{
	if (0 >= mAmountPeople)
	{
		return true;
	}
	else
	{
		return false;
	}
	return false;
}

//int CRoom::GetBattingMoney()
//{
//	return mBettingMoney;
//}

//bool CRoom::MergeRoom(CRoom * targetRoom)
//{
//	{
//		ScopeLock<MUTEX> MU(mRAII_RoomMUTEX); // rock0
//		{
//			ScopeLock<MUTEX> MU(targetRoom->mRAII_RoomMUTEX); // rock1
//															  // ���� �ű�� ���� �غ��۾����� room���� ����
//			LinkListIt linkBegin = targetRoom->GetIterMyInfoBegin();
//			LinkListIt linkEnd = targetRoom->GetIterMyInfoEnd();
//			for (; linkBegin != linkEnd; ++linkBegin)
//			{
//				CLink* targetClient = (*linkBegin).get();
//				targetClient->SetMyRoomNum(mRoomNum);
//				IncreasePeople(); // �� �ο��� ����
//			}
//			mClientInfos.merge(targetRoom->mClientInfos); // ���� �ű�
//		} // rock1 unlock
//	} // rock0 unlock
//	return true;
//}

bool CRoom::IsAllReady()
{
	// �ο����� ���ڸ��� ������ false;
	if (EnterRoomPeopleLimit > mAmountPeople)
	{
		return false;
	}
	LinkListIt iterBegin = mClientInfos.begin();
	for (; iterBegin != mClientInfos.end(); ++iterBegin)
	{
		CLink* client = (*iterBegin).get();
		if (false == client->GetReadyGame())
		{
			return false;
		}
	}
	//SetGame();
	return true;
}


bool CRoom::AllCalculateMoney()
{
	LinkListIt linkBegin = mClientInfos.begin();
	bool isSaveResult = true; // ��� .txt�� ���� �Ǿ���? �� ���̶� �ȵǸ� false ��ȯ
	for (; linkBegin != mClientInfos.end(); ++linkBegin)
	{
		if (false == (*linkBegin).get()->SaveCalculateMoney())
		{
			ErrorHandStatic->ErrorHandler(ERROR_SAVE_MONEY, (*linkBegin));
			isSaveResult = false;
			continue;
		}
	}
	return isSaveResult;
}

void CRoom::PushClient(const LinkPtr& shared_client, const int& enterRoomNumber)
{
	ScopeLock<MUTEX> MU(mRAII_RoomMUTEX);
	mClientInfos.push_back(shared_client);
	shared_client.get()->SetMyRoomNum(enterRoomNumber);
	IncreasePeople();
	cout << "�濡 ������ ���� �ο� = " << mAmountPeople << endl;
	
	if (mIsNewRoom && mAmountPeople >= EnterRoomPeopleLimit)
	{
		ProtocolCharacterTagIndex tagIndex = ProtocolCharacterTagIndex::Red01;
		for (LinkListIt clientBegin = mClientInfos.begin(); clientBegin != mClientInfos.end(); ++clientBegin)
		{
			EnterBroadcast(*clientBegin, tagIndex);
			tagIndex = (ProtocolCharacterTagIndex)(tagIndex + 1);
		}
		for (LinkListIt clientBegin = mClientInfos.begin(); clientBegin != mClientInfos.end(); ++clientBegin)
		{
			NoticRoomIn(*clientBegin);
		}
		mIsNewRoom = false;
		//cout << "���� ������� �濡 ��� ������" << endl;
		//ProtocolCharacterTagIndex tagIndex = ProtocolCharacterTagIndex::Red01;
		//LinkListIt clientBegin = mClientInfos.begin();
		//for (; clientBegin != mClientInfos.end(); ++clientBegin)
		//{
		//	Packet matchingSuccessPacket(ProtocolInfo::ClientCommend, ProtocolDetail::MatchingSuccess, State::ClientRoomIn, nullptr);
		//	(*clientBegin).get()->SendnMine(matchingSuccessPacket);
		//}
		//clientBegin = mClientInfos.begin();
		//cout << "///////////// ������ ��ġ = " << tagIndex << endl;
		//for (; clientBegin != mClientInfos.end(); ++clientBegin)
		//{
		//	EnterBroadcast((*clientBegin), tagIndex);
		//	tagIndex = (ProtocolCharacterTagIndex)(tagIndex + 1);
		//}
		//mIsNewRoom = false;
	}
	else if (!mIsNewRoom && mAmountPeople <= EnterRoomPeopleLimit)
	{
		vector<int>::iterator useBegin = mUsePosition.begin();
		for (; useBegin != mUsePosition.end(); ++useBegin)
		{
			if (NotUsed == (*useBegin))
			{
				EnterBroadcast(shared_client, ProtocolCharacterTagIndex(*useBegin));
				NoticRoomIn(shared_client);
			}
		}
	}
	/*string message(shared_client.get()->GetMyName() + "���� �濡 ���� �ϼ̽��ϴ�.");
	Packet p(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, message.c_str());
	Broadcast(p);*/
	//Broadcast(shared_client.get()->GetMyName() + "���� �濡 ���� �ϼ̽��ϴ�.");
}

void CRoom::NoticRoomIn(const LinkPtr & shared_client)
{
	Packet matchingSuccessPacket(ProtocolInfo::ClientCommend, ProtocolDetail::MatchingSuccess, State::ClientRoomIn, nullptr);
	shared_client.get()->SendnMine(matchingSuccessPacket);
	string message(shared_client.get()->GetMyName() + "���� �濡 ���� �ϼ̽��ϴ�.");
	Packet welcomePacket(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, message.c_str());
	Talk(shared_client, welcomePacket);
}

// ���, ��ġ
void CRoom::EnterBroadcast(const LinkPtr& shared_client, ProtocolCharacterTagIndex tagIndex)
{
	if (Used == mUsePosition[tagIndex])
	{
		ErrorHandStatic->ErrorHandler(ERROR_ENTER_ROOM, shared_client);
		return;
	}
	mUsePosition[tagIndex] = Used; // ����� ǥ��
	Packet packetName(ProtocolInfo::ClientCommend, ProtocolDetail::NameChange, tagIndex, shared_client.get()->GetMyName().c_str());
	Packet packetImage(ProtocolInfo::ClientCommend, ProtocolDetail::ImageChange, tagIndex, ProtocolCharacterImageName[InitCharacter].c_str());
	shared_client.get()->SetMyPosition(tagIndex);
	Broadcast(packetName);
	Broadcast(packetImage);
}

void CRoom::ChangetCharacterBroadcast(const LinkPtr & shared_client, const ProtocolCharacterImageNameIndex& characterImageIndex)
{
	CLink* client = shared_client.get();
	if (client->GetReadyGame())
	{
		Packet packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "�غ� �����̹Ƿ� ĳ���͸� ������ �� �����ϴ�.");
		client->SendnMine(packet);
		return;
	}
	if (mUsePosition[client->GetMyPosition()] == Used)
	{
		Packet packet(ProtocolInfo::ClientCommend, ProtocolDetail::ImageChange, client->GetMyPosition(), ProtocolCharacterImageName[characterImageIndex].c_str());
		Broadcast(packet);
		client->SetMyCharacter(characterImageIndex);
	}
	else
	{
		ErrorHandStatic->ErrorHandler(ERROR_ROOM_CHANGET_CHARACTER, shared_client);
		return;
	}
}

//bool CRoom::AllInitBetting()
//{
//	LinkListIt linkBegin = mClientInfos.begin();
//	for (; linkBegin != mClientInfos.end(); ++linkBegin)
//	{
//		(*linkBegin).get()->InitBetting();
//	}
//	SetGameOver();
//	return true;
//}

//bool CRoom::IsGame()
//{
//	return mPlayingGame;
//}

void CRoom::Broadcast(const Packet& packet, int flags)
{
	LinkListIt clientIterBegin = mClientInfos.begin();
	for (; clientIterBegin != mClientInfos.end(); ++clientIterBegin)
	{
		(*clientIterBegin).get()->SendnMine(packet, flags);
	}
}

void CRoom::Talk(const LinkPtr& myClient, const Packet & packet, int flags)
{
	LinkListIt clientIterBegin = mClientInfos.begin();
	LinkListIt myIter = find(mClientInfos.begin(), mClientInfos.end(), myClient);
	if (mClientInfos.end() == myIter)
	{
		Broadcast(packet, flags);
		return;
	}
	for (; clientIterBegin != mClientInfos.end(); ++clientIterBegin)
	{
		if (clientIterBegin != myIter)
		{
			(*clientIterBegin).get()->SendnMine(packet, flags);
		}
	}
}

void CRoom::GetHostIP()
{
	LinkListIt linkBegin = mClientInfos.begin();
	for (; linkBegin != mClientInfos.end(); ++linkBegin)
	{
		if ((*linkBegin).get()->GetMyPosition() == ProtocolCharacterTagIndex::Red01)
		{
			Packet packet(ProtocolInfo::ClientCommend, ProtocolDetail::GetHostIP, 0, (*linkBegin).get()->GetMyIP().c_str());
			(*linkBegin).get()->SendnMine(packet);
		}
	}
}

void CRoom::IncreasePeople()
{
	mAmountPeople++;
}

void CRoom::DecreasePeople()
{
	if (mAmountPeople > 0) mAmountPeople--;
}

//void CRoom::SetGame()
//{
//	mPlayingGame = true;
//}
//
//void CRoom::SetGameOver()
//{
//	mPlayingGame = false;
//}
//
//void CRoom::SendBattingResult(const LinkPtr& winner, int flags)
//{
//	LinkListIt clientIterBegin = mClientInfos.begin();
//	LinkListIt winnerIter = find(mClientInfos.begin(), mClientInfos.end(), winner);
//	for (; clientIterBegin != mClientInfos.end(); ++clientIterBegin)
//	{
//		if (clientIterBegin != winnerIter)
//		{
//			(*clientIterBegin).get()->SendnMine("����� �����ϴ�.", flags);
//		}
//		else
//		{
//			(*clientIterBegin).get()->SendnMine("����� �̰���ϴ�.", flags);
//		}
//	}
//}
//
//
