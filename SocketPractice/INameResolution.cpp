#include <WinSock2.h>
#include <atlsocket.h>
#include <stdlib.h>
#include <stdio.h>

void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

BOOL DomainToIP(WCHAR *szDomain, IN_ADDR *pAddr)
{
	ADDRINFOW	*pAddrInfo;
	SOCKADDR_IN *pSockAddr;

	if (GetAddrInfo(szDomain, L"0", NULL, &pAddrInfo) != 0)
	{
		return FALSE;
	}

	pSockAddr = (SOCKADDR_IN *)pAddrInfo->ai_addr;
	*pAddr = pSockAddr->sin_addr;
	FreeAddrInfo(pAddrInfo);

	return TRUE;
}

int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	IN_ADDR Addr;
	WCHAR Domain[100], szIP[16];
	char strDomain[100], strIP[16];

	printf("Domain : ");
	gets_s(strDomain, 100);

	wsprintf(Domain, TEXT("%S"), strDomain);
	DomainToIP(Domain, &Addr);
	InetNtop(AF_INET, &Addr, szIP, 16);

	wcstombs_s(0, strIP, sizeof(strIP), szIP, wcslen(szIP));
	printf("IP : %s\n", strIP);

	return 0;
}