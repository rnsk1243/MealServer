#pragma once
#include<random>
#include<iostream>
#include"ErrorHandler.h"
using namespace std;

//static bool IntToAlphabet(const int num, char* chResult)
//{
//	if(num < 0 || num >= 100)
//	{
//		ErrorHandStatic->ErrorHandler(ERROR_INT_TO_ALPHABET_OUT_RANGE);
//		return false;
//	}
//	char temp[10];
//	if (10 > num && 0 <= num)
//	{
//		chResult[0] = '0';
//		_itoa_s(num, temp, 10);
//		chResult[1] = temp[0];
//		chResult[2] = '\0';
//	}
//	else if(10 <= num && 100 > num)
//	{
//		_itoa_s(num, temp, 10);
//		chResult[0] = temp[0];
//		chResult[1] = temp[1];
//		chResult[2] = '\0';
//	}
//	return true;
//}
static bool CalcCipher(const int& number, int& resultCipher);

// char���� ��ȯ�� ����, txt�� ����Ǵ� �ִ��ڸ��� ����, ��ȯ�Ǿ� ����� ����
static bool IntToAlphabet(const int& number, const int& maxCipher, char* chResult)
{
	int numberCipher = 0;
	if (false == CalcCipher(number, numberCipher))
	{
		return false;
	}
	if (maxCipher < numberCipher)
	{
		ErrorHandStatic->ErrorHandler(ERROR_MAXCIPHER_OVER);
		return false;
	}
	char temp[10];
	int zeroCount = maxCipher - numberCipher; // '0'�� ä������ ����
	int validCount = maxCipher - zeroCount; // ��ȿ ���ڰ� ä������ ����
	for (int cipher = 0; cipher < zeroCount; ++cipher)
	{
		chResult[cipher] = '0';
	}
	_itoa_s(number, temp, 10);
	if (validCount >(int)strlen(temp))
	{
		ErrorHandStatic->ErrorHandler(ERROR_VALID_NUMBER_OUT_OF_RANGE);
		return false;
	}
	int tempIndex = 0;
	for (int cipher = zeroCount; cipher < maxCipher; ++cipher)
	{
		chResult[cipher] = temp[tempIndex];
		++tempIndex;
	}
	chResult[maxCipher] = '\0';
	return true;
}
// ���ڸ� �������� ���ϱ�
static bool CalcCipher(const int& number, int& resultCipher)
{
	if (0 > number)
	{
		ErrorHandStatic->ErrorHandler(ERROR_CALC_CIPHER_MINUS_NUMBER);
		return false;
	}

	int targetNumber = number;
	while (targetNumber > 0)
	{
		targetNumber /= 10;
		++resultCipher;
	}
	return true;
}

static const string IntToString(const int& targetInt)
{
	string result;
	char tempUserNum[NameSize];
	_itoa_s(targetInt, tempUserNum, 10);
	result = tempUserNum;
	return result;
}

// �ڸ��� ����
static const bool AddCipHer(const int number, int& cipHerResult)
{
	if (number < 1 || number > 1000)
	{
		ErrorHandStatic->ErrorHandler(ERROR_CIPHER_OUT_RANGE);
		return false;
	}
	//int result = 0;
	int curNumber = 0;
	while (curNumber != number)
	{
		curNumber++;
		if (curNumber < 10)
		{
			++cipHerResult;
		}
		else if (curNumber >= 10 && curNumber < 100)
		{
			cipHerResult += 2;
		}
		else if (curNumber >= 100 && curNumber < 1000)
		{
			cipHerResult += 3;
		}
	}
//	cipHerResult = result;

	return true;
}

static const int RandNumber(int max = 100)
{
	// �⺻ 1 �̻� 100 ���� �� ����
	random_device rn;
	mt19937_64 rnd(rn());

	uniform_int_distribution<int> range(1, max);
	return range(rnd);
}

