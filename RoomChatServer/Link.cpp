#include"Link.h"
#include"ErrorHandler.h"

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
	cout << "~CLink() �Ҹ��� ȣ��" << endl;
	SaveCalculateMoney();// ���� ������ ������ �ϰ� ������?
	cout << mName << "Ŭ���̾�Ʈ ������ ���� �˴ϴ�. = " << endl;
	closesocket(*mClientSocket);
	delete mClientSocket;
	cout << "Ŭ���̾�Ʈ ���� �Ϸ�" << endl;
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
		SendnMine("���� �Ӵϰ� �����Ͽ� �غ� ���� �� �߽��ϴ�.");
	}
	else
	{
		mIsGameOK = true;
		SendnMine("����� �غ� �Ǽ̽��ϴ�.");
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
	// ���� �� ���� ���� .txt�� ������ ���� �׷��� ���߿� �� ��ŭ ����
	mPayBackMoney += payBack;
	return true;
}

bool CLink::GetPrizeBattingMoney(const int& bettingMoney)
{
	int prizeMoney = EnterRoomPeopleLimit * bettingMoney; // �� ���
	PayBackMoney(prizeMoney);
	return true;
}
bool CLink::SaveCalculateMoney()
{
	//�����ϰ� ������ ��
	int calculateMoney = 0;
	if (mPayBackMoney == mDebtMoney)
	{
		mPayBackMoney = 0;
		mDebtMoney = 0;
		return true;
	}
	if (mPayBackMoney > mDebtMoney)
	{
		// ���� �� - ���� ��
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
	// �� ���� (�ϴ� ������) ���� .txt�� ������ ���� �׷��� ���߿� �� ��ŭ ������.
	mDebtMoney += fine;
}



void CLink::SendnMine(const string & message, int flags)
{
	int isSuccess = 0;
	const char* chMessage = message.c_str();
	size_t size = strlen(chMessage);
	isSuccess = send(*mClientSocket, (char*)&size, IntSize, flags); // ������ ������
	if (isSuccess == SOCKET_ERROR)
	{
		//return ErrorHandStatic->ErrorHandler(ERROR_NULL_LINK_SEND);
	}
	int temp = 0;
	while (true)
	{
		temp += send(*mClientSocket, chMessage, (int)size, flags);
		if (temp == SOCKET_ERROR)
		{
			//return ErrorHandStatic->ErrorHandler(ERROR_NULL_LINK_SEND);
		}
		if (temp >= (int)size)
			break;
	}
}