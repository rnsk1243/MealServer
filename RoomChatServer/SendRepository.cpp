#include "SendRepository.h"


CSendRepository::CSendRepository()
{
}

CSendRepository * CSendRepository::GetInstance()
{
	if (nullptr == SendRepositoryStatic)
	{
		return new CSendRepository();
	}
	return SendRepositoryStatic;
}

CSendRepository::~CSendRepository()
{
}

void CSendRepository::PushPacket(const PacketPtr & packet)
{
	ScopeLock<MUTEX> MU(mRAII_SendRepositoryMUTEX);
	mPacketQueue.push(packet);
}

void CSendRepository::PopPacket(PacketPtr& packet)
{
	ScopeLock<MUTEX> MU(mRAII_SendRepositoryMUTEX);
	if (!mPacketQueue.empty())
	{
		packet = mPacketQueue.front();
		mPacketQueue.pop();
	}
	packet = nullptr;
}
