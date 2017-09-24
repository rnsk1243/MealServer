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
}


CRoom::~CRoom()
{
	cout << mRoomNum << " 번 방이 삭제 됩니다." << endl;
}

void CRoom::PushClient(const LinkPtr& shared_client, const int& enterRoomNumber)
{
	ScopeLock<MUTEX> MU(mRAII_RoomMUTEX);
	mClientInfos.push_back(shared_client);
	shared_client.get()->SetMyRoomNum(enterRoomNumber);
	IncreasePeople();
	cout << "방에 들어왔음 현재 인원 = " << mAmountPeople << endl;
	
	if (mIsNewRoom && mAmountPeople >= EnterRoomPeopleLimit)
	{
		cout << "새로 만들어진 방에 모두 들어왔음" << endl;
		ProtocolCharacterTagIndex tagIndex = ProtocolCharacterTagIndex::Red01;
		LinkListIt clientBegin = mClientInfos.begin();
		for (; clientBegin != mClientInfos.end(); ++clientBegin)
		{
			Packet matchingSuccessPacket(ProtocolInfo::ClientCommend, ProtocolDetail::MatchingSuccess, State::ClientRoomIn, nullptr);
			(*clientBegin).get()->SendnMine(matchingSuccessPacket);
		}
		clientBegin = mClientInfos.begin();
		cout << "///////////// 보내는 위치 = " << tagIndex << endl;
		for (; clientBegin != mClientInfos.end(); ++clientBegin)
		{
			EnterBroadcast((*clientBegin), tagIndex);
			tagIndex = (ProtocolCharacterTagIndex)(tagIndex + 1);
		}
		mIsNewRoom = false;
	}
	else if (!mIsNewRoom && mAmountPeople <= EnterRoomPeopleLimit)
	{
		//EnterBroadcast(shared_client);
	}
	/*string message(shared_client.get()->GetMyName() + "님이 방에 입장 하셨습니다.");
	Packet p(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, message.c_str());
	Broadcast(p);*/
	//Broadcast(shared_client.get()->GetMyName() + "님이 방에 입장 하셨습니다.");
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
//															  // 실제 옮기기 전에 준비작업으로 room정보 수정
//			LinkListIt linkBegin = targetRoom->GetIterMyInfoBegin();
//			LinkListIt linkEnd = targetRoom->GetIterMyInfoEnd();
//			for (; linkBegin != linkEnd; ++linkBegin)
//			{
//				CLink* targetClient = (*linkBegin).get();
//				targetClient->SetMyRoomNum(mRoomNum);
//				IncreasePeople(); // 방 인원수 갱신
//			}
//			mClientInfos.merge(targetRoom->mClientInfos); // 실제 옮김
//		} // rock1 unlock
//	} // rock0 unlock
//	return true;
//}

bool CRoom::IsAllReady()
{
	// 인원수가 모자르면 무조건 false;
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
	bool isSaveResult = true; // 모두 .txt에 저장 되었나? 한 명이라도 안되면 false 반환
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

void CRoom::EnterBroadcast(const LinkPtr& shared_client, ProtocolCharacterTagIndex tagIndex)
{
	Packet imagePacket(ProtocolInfo::ClientCommend, ProtocolDetail::ImageChange, tagIndex, ProtocolCharacterImageName[ProtocolCharacterImage::Tofu].c_str());
	Packet namePacket(ProtocolInfo::ClientCommend, ProtocolDetail::NameChange, tagIndex, shared_client.get()->GetMyName().c_str());

	string message(shared_client.get()->GetMyName() + "님이 방에 입장 하셨습니다.");
	Packet welcomePacket(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, message.c_str());
	Broadcast(imagePacket);
	Broadcast(namePacket);
	Broadcast(welcomePacket);
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
//			(*clientIterBegin).get()->SendnMine("당신은 졌습니다.", flags);
//		}
//		else
//		{
//			(*clientIterBegin).get()->SendnMine("당신은 이겼습니다.", flags);
//		}
//	}
//}
//
//
