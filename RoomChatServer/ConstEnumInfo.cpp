#include"ConstEnumInfo.h"
#include"Util.h"

Packet::Packet()
{
}

void Packet::AddName(string name)
{
	char resultTemp[BufSizeValue];
	resultTemp[0] = '\0'; // ù��° ��Ҹ� null���ڷ� ���־�� ��.
	strcat_s(resultTemp, BufSizeValue, name.c_str());
	strcat_s(resultTemp, BufSizeValue, " : ");
	strcat_s(resultTemp, BufSizeValue, InfoValue);

	strcpy_s(InfoValue, BufSizeValue, resultTemp);
}
