#pragma once
#include<queue>
#include<memory>
#include"ConstEnumInfo.h"
#include"RecvRepository.h"
#include"RAII.h"

class CSendRepository
{
	PacketQueue mPacketQueue;
	MUTEX mRAII_SendRepositoryMUTEX;
	CSendRepository();
public:
	static CSendRepository* GetInstance();
	CSendRepository(const CSendRepository&) = delete;
	CSendRepository& operator=(const CSendRepository&) = delete;
	~CSendRepository();
	void PushPacket(const PacketPtr& packet);
	void PopPacket(PacketPtr& packet);
};
static CSendRepository* SendRepositoryStatic = CSendRepository::GetInstance();
