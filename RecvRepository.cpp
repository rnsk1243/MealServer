#include "RecvRepository.h"



CRecvRepository::CRecvRepository()
{
	
}

CRecvRepository * CRecvRepository::GetInstance()
{
	if (nullptr == RecvRepositoryStatic)
	{
		return new CRecvRepository();
	}
	return RecvRepositoryStatic;
}


CRecvRepository::~CRecvRepository()
{
}

void CRecvRepository::PushPacket(const OrderStructSocketPtr & packet)
{
	ScopeLock<MUTEX> MU(mRAII_RecvRepositoryMUTEX);
	mQueueOtherSocket.push(packet);
}

void CRecvRepository::PopPacket(OrderStructSocketPtr& packet)
{
	ScopeLock<MUTEX> MU(mRAII_RecvRepositoryMUTEX);
	if (!mQueueOtherSocket.empty())
	{
		packet = mQueueOtherSocket.front();
		mQueueOtherSocket.pop();
	}
	packet = nullptr;
}

void CRecvRepository::PushPacket(const OrderStructLinkPtr & packet)
{
	ScopeLock<MUTEX> MU(mRAII_RecvRepositoryMUTEX);
	mQueueOtherLink.push(packet);
}

void CRecvRepository::PopPacket(OrderStructLinkPtr& packet)
{
	ScopeLock<MUTEX> MU(mRAII_RecvRepositoryMUTEX);
	if (!mQueueOtherLink.empty())
	{
		packet = mQueueOtherLink.front();
		mQueueOtherLink.pop();
	}
	packet = nullptr;
}
