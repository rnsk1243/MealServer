#include "WriteHandler.h"
#include"ReadHandler.h"
#include"ConstEnumInfo.h"
#include"ErrorHandler.h"
#include"Util.h"

CWriteHandler::CWriteHandler()
{
}

CWriteHandler * CWriteHandler::GetInstance()
{
	if (nullptr == WriteHandlerStatic)
	{
		WriteHandlerStatic = new CWriteHandler();
	}
	return WriteHandlerStatic;
}


CWriteHandler::~CWriteHandler()
{
}

bool CWriteHandler::Write(const char * textFileName, int count, ...)
{
	ofstream outFile(textFileName, ios::app);
	if (!outFile)
	{
	//	cout << "������ �����ϴ�." << endl;
		return false;
	}
	va_list Marker;
	va_start(Marker, count);

	for (int i = 0; i < count; i++)
	{
		outFile << va_arg(Marker, string).c_str() << '|';
	}
	outFile << '\n';
	outFile.close();
	va_end(Marker);
	return true;
}

bool CWriteHandler::Write(const char * textFileName, const vector<string>& strWrite)
{
	ofstream outFile(textFileName, ios::app);
	if (!outFile)
	{
	//	cout << "������ �����ϴ�." << endl;
		return false;
	}
	vector<string>::const_iterator iterBeginWrite = strWrite.begin();
	for (; iterBeginWrite != strWrite.end(); ++iterBeginWrite)
	{
		outFile << (*iterBeginWrite).c_str() << '|';
	}
	outFile << '\n';
	outFile.close();
	return true;
}



//int CWriteHandler::writeGacharResult(const char * textFileName, const char * userName)
//{
//	string userNameString = userName;
//	// ã�� ���̵� ���� ���
//	int targetLine = ReadHandlerStatic->Search(textFileName, 1, userNameString);
//
//
//
//	return 0;
//}

//������������ �ִ� ���� ���� Ŀ�� �̵��ؾ��� ũ�� ��ȯ(text�̸�, �ش� ������ ��ϵ� ���� ��, ������ ��ϵ� ������ ������ ����)
//int CWriteHandler::BeginToTargetUserLineCurserMoveSize(const char* textName, int targetUserLineNum, char* targetLine)
//{
//	ifstream input(textName);
//	input.seekg(0, ios::beg);
//	char checkLine;
//	int offset = 0;
//	int line = 1;
//	while (true)
//	{
//		input.seekg(1, ios::cur);
//		offset++;
//		checkLine = (char)input.peek();
//		if (checkLine == '\n')
//		{
//			input.seekg(1, ios::cur);
//			line++;
//			offset++;
//		}
//		if (targetUserLineNum == line)
//		{
//			input.seekg(1, ios::cur);
//			offset++;
//			break;
//		}
//	}
//	input.getline(targetLine, BufSize);
//	input.close();
//
//	return offset;
//}
//
//// ������ ������ �ִ� ���κ��� ������ �ִ� ������ Ŀ���� �̵��ؾ��� ũ�� ��ȯ(��������ϵȶ���, ã��ī�����, ���ο�ī������Ȯ�ο뺯��, ã��ī�� ������������)
//int CWriteHandler::TargetLineToUserCurserMoveSize(const char* targetSouce, string& searchCardNum, bool& isNewCard, int& cardAmount)
//{
//	int offset = 0;
//	string targetLine = targetSouce;
//	// �ش� ���̵� ������ '|'�� �������� �ڸ�
//	vector<string> tempVec = ReadHandlerStatic->Parse(targetLine, '|');
//
//	vector<string>::iterator iterBegin = tempVec.begin();
//	vector<string>::iterator iterEnd = tempVec.end();
//	// �� �� id �κ��� ����  
//	offset += (strlen((*iterBegin).c_str()) + 1/*'|'������ 1�� ����*/);
//	++iterBegin; // ���̵� �κ� �ѱ�
//	for (; iterBegin != iterEnd; ++iterBegin) // ī�� �κ�
//	{
//		// (ī���ȣ/ī�尹��)�� '/'�� �������� �ڸ�
//		vector<string> tempVec2 = ReadHandlerStatic->Parse((*iterBegin), '/');
//		vector<string>::iterator iterBegin2 = tempVec2.begin(); // ī�� ��ȣ �κ�
//																// ī�� ��ȣ�� ã������ ī�� ��ġ ���� Ȯ��
//		int compareResult = (*iterBegin2).compare(searchCardNum);
//		if (0 == compareResult)
//		{	// ��ġ�ϸ� offset����(������ �ִ� ī����)
//			isNewCard = false;
//			++iterBegin2; // ī�� ���� �κ�
//			cardAmount = atoi((*iterBegin2).c_str());
//			return offset;
//		}
//		// ��ġ ���� ������ "ī���ȣ/ī�尹��|" <-�� ��ŭ�� offset�� ����
//		offset += (strlen((*iterBegin).c_str()) + 1);
//	}
//	// �ű� ī��
//	isNewCard = true;
//	cardAmount = 0;
//	return offset;
//}

// ����ó�� �Ϸ�� �Լ�
// ī�� ���(������ؽ�Ʈ�̸�, �����Ұ����� �̵��ؾ��ϴ� ��, ����������ī��, ����Ҽ���, �ڸ���)
bool CWriteHandler::WriteObj(const string& textName, int offset, int recordedNumber, int cipher)
{
	char charObj[20];
	if (false == IntToAlphabet(recordedNumber, cipher, charObj))
	{
		return false;
	}
	string strObj(charObj);

	ofstream output(textName, ios::out | ios::in);

	output.seekp(offset, ios::beg);

	const char* doWrite = strObj.c_str();
	//cout << "��� �� ���ڿ� = " << doWrite << endl;
	output.write(doWrite, strlen(doWrite));
	output.close();
	return true;
	/*
	�߰��� ������ �Ұ��� �ϴٰ�.. ���� ���� ����� ��� �����͸� �����ϴ� ���߿� �����ؾ� �մϴ�.
	You cannot insert in the middle of the file.
	You have to copy the old file to a new file and insert whatever you want in the middle during copying to the new file.
	Otherwise, if you intend to overwrite data/lines in the existing file, that is possible by using
	std::ostream::seekp() to identify the position within the file.

	https://stackoverflow.com/questions/30642827/how-to-write-to-middle-of-a-file-in-c
	*/

	//if (isNewCard)
	//{// ���ο� ī��
	//	output.seekp(-7, ios::cur); // "xxxxxx" �κ� ��ŭ �ڷ�(������ �κ�)
	//	string nextSpace = "xxxxxx"; // ���� ���ο� ī�� ���� Ȯ����
	//	result += nextSpace;
	//	const char* doWrite = result.c_str();
	//	output.write(doWrite, strlen(doWrite));
	//}
	//else
	//{// ���ο� ī�尡 �ƴ�
	//	const char* doWrite = result.c_str();
	//	output.write(doWrite, strlen(doWrite));
	//}
}

bool CWriteHandler::WriteNextJoinUserNum(const string& textFileName,const int& nextUserNum)
{
	ofstream outFile(textFileName, ios::beg);
	if (!outFile)
	{
		//cout << "������ �����ϴ�." << endl;
		return false;
	}

	int lastUserNum = nextUserNum;
	++lastUserNum;
	string userNum = IntToString(lastUserNum);
	outFile << userNum;
	outFile.close();
	return true;
}