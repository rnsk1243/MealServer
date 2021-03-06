#include"Link.h"
#include"ErrorHandler.h"
#include<atlstr.h>
#include"Util.h"


CLink::CLink(const SOCKET* clientSocket, const string& strPKNumber, const char* name) :
	mClientSocket(clientSocket),
	mMyChannelNum(0),
	mMyRoomNum(NoneRoom),
	mMyPKNumber(stoi(strPKNumber)),
	mIsInitGoods(false),
	mIsGameOK(false),
	mMyGoods(stoi(strPKNumber)),
	mDebtMoney(0),
	mPayBackMoney(0),
	mName(name)
{
}


CLink::~CLink()
{
	cout << "~CLink() 소멸자 호출" << endl;
	SaveCalculateMoney();// 갈땐 가더라도 정산은 하고 가야지?
	cout << mName << "클라이언트 정보가 삭제 됩니다. = " << endl;
	closesocket(*mClientSocket);
	delete mClientSocket;
	cout << "클라이언트 삭제 완료" << endl;
}

bool CLink::IsRoomEnterState()
{
	if (NoneRoom != mMyRoomNum)
	{
		return true;
	}
	return false;
}


const SOCKET * CLink::GetClientSocket()
{
	return mClientSocket;
}

int CLink::GetMyRoomNum()
{
	return mMyRoomNum;
}

void CLink::SetMyRoomNum(const int & myRoomNum)
{
	mMyRoomNum = myRoomNum;
}

int CLink::GetMyChannelNum()
{
	return mMyChannelNum;
}

void CLink::SetMyChannelNum(const int& myChannelNum)
{
	mMyChannelNum = myChannelNum;
}

string CLink::GetMyName()
{
	return mName;
}

bool CLink::IsZeroMoney()
{
	return mMyGoods.IsZeroMoney();
}

void CLink::SetZeroMoney()
{
	mMyGoods.SetZeroMoney();
}

const int CLink::GetMyMoney()
{
	return mMyGoods.GetMyMoney();
}



const int CLink::GetMyPKNumber() const
{
	return mMyPKNumber;
}

void CLink::SetInitGoods()
{
	mIsInitGoods = true;
}

void CLink::SetReadyGame(const int& battingMoney)
{
	if (battingMoney > GetMyMoney())
	{
		mIsGameOK = false;
		SendnMine("베팅 머니가 부족하여 준비 되지 못 했습니다.");
	}
	else
	{
		mIsGameOK = true;
		SendnMine("당신은 준비 되셨습니다.");
	}
}
void CLink::SetNoReadyGame()
{
	mIsGameOK = false;
}
bool CLink::GetReadyGame()
{
	return mIsGameOK;
}
bool CLink::InitMoney(int money)
{
	if (false == mMyGoods.InitMoney(money))
	{
		ErrorHandStatic->ErrorHandler(ERROR_INIT_MONEY, LinkPtr(this));
		return false;
	}
	return true;
}

bool CLink::AddMoney(const int & addMoney)
{
	if (0 == addMoney)
	{
		ErrorHandStatic->ErrorHandler(ERROR_SAVE_MONEY_ZERO, LinkPtr(this));
		return false;
	}
	if (false == mMyGoods.AddMyMoney(addMoney))
	{
		ErrorHandStatic->ErrorHandler(ERROR_SAVE_MONEY, LinkPtr(this));
		return false;
	}
	return true;
}

bool CLink::MinusMyMoney(const int & minusMoney)
{
	if (0 == minusMoney)
	{
		ErrorHandStatic->ErrorHandler(ERROR_SAVE_MONEY_ZERO, LinkPtr(this));
		return false;
	}
	mMyGoods.MinusMyMoney(minusMoney);
	return true;
}

bool CLink::PayBackMoney(const int & payBack)
{
	// 받을 돈 증가 실제 .txt에 쓰지는 않음 그러나 나중에 이 만큼 증가
	mPayBackMoney += payBack;
	return true;
}

bool CLink::GetPrizeBattingMoney(const int& bettingMoney)
{
	int prizeMoney = EnterRoomPeopleLimit * bettingMoney; // 총 상금
	PayBackMoney(prizeMoney);
	return true;
}
bool CLink::SaveCalculateMoney()
{
	//정산하고 저장할 돈
	int calculateMoney = 0;
	if (mPayBackMoney == mDebtMoney)
	{
		mPayBackMoney = 0;
		mDebtMoney = 0;
		return true;
	}
	if (mPayBackMoney > mDebtMoney)
	{
		// 받을 돈 - 나갈 돈
		calculateMoney = (mPayBackMoney - mDebtMoney);
		if (true == AddMoney(calculateMoney))
		{
			mPayBackMoney = 0;
			mDebtMoney = 0;
			return true;
		}
	}
	else
	{
		calculateMoney = (mDebtMoney - mPayBackMoney);
		if (true == MinusMyMoney(calculateMoney))
		{
			mPayBackMoney = 0;
			mDebtMoney = 0;
			return true;
		}
	}
	return false;
}

bool CLink::InitGoods(int initMoney)
{
	if (true == mIsInitGoods)
	{
		ErrorHandStatic->ErrorHandler(ERROR_INIT_GOODS_TRUE, LinkPtr(this));
		return false;
	}
	return InitMoney(initMoney);
}

bool CLink::InitBetting()
{
	mIsGameOK = false;
	mDebtMoney = 0;
	return true;
}

void CLink::LostWillMoney(const int& fine)
{
	// 돈 감소 (일단 빚으로) 실제 .txt에 쓰지는 않음 그러나 나중에 이 만큼 차감함.
	mDebtMoney += fine;
}



void CLink::SendnMine(const string & message, int flags)
{
	int isSuccess = 0;

	/// UTF8 인코딩
	
	//wstring strUni = CA2W(message.c_str());
	//string strUTF8(CW2A(strUni.c_str(), CP_UTF8));
	///

	const char* chMessage = UTF8ToANSI(message.c_str());
	size_t size = strlen(chMessage);
	isSuccess = send(*mClientSocket, (char*)&size, IntSize, flags); // 사이즈 보내기
	if (isSuccess == SOCKET_ERROR)
	{
		//ErrorHandStatic->ErrorHandler(ERROR_NULL_LINK_SEND, LinkPtr(this));
		return;
	}
	int temp = 0;
	while (true)
	{
		temp += send(*mClientSocket, chMessage, (int)size, flags);
		if (temp == SOCKET_ERROR)
		{
			//ErrorHandStatic->ErrorHandler(ERROR_NULL_LINK_SEND, LinkPtr(this));
			return;
		}
		if (temp >= (int)size)
			break;
	}
}