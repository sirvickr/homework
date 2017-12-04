#include "stdafx.h"

#include "CryptoAPI.h"

using namespace std;

DWORD Encrypt(LPBYTE pbContent, DWORD cbContent, LPBYTE *ppbEncodedBlob, LPDWORD pcbEncodedBlob)
{
	const DWORD dwMsgEncodingType = PKCS_7_ASN_ENCODING | X509_ASN_ENCODING;
	DWORD dwErrorCode = ERROR_SUCCESS;

	*ppbEncodedBlob = NULL;
	*pcbEncodedBlob = 0;

	//-------------------------------------------------------------------
	// �������� ������ ����� ��� �������� �������������� ���������
	DWORD cbEncodedBlob = CryptMsgCalculateEncodedLength(
		dwMsgEncodingType,      // ��� �����������
		0,                      // ����� (�����)
		CMSG_DATA,              // ��� ��������
		NULL,                   // ��������� �� ���������
		NULL,                   // ���������� ������������� ��������
		cbContent);             // ������ ��������
	if (!cbEncodedBlob) {
		dwErrorCode = GetLastError();
		tcerr << TEXT("Getting cbEncodedBlob length failed: ") << dwErrorCode << endl;
		return dwErrorCode;
	}
	//-------------------------------------------------------------------
	// �������� ������
	LPBYTE pbEncodedBlob = (LPBYTE)malloc(cbEncodedBlob);
	if (!pbEncodedBlob) {
		dwErrorCode = GetLastError();
		tcerr << TEXT("Memory allocation failed: ") << dwErrorCode << endl;
		return dwErrorCode;
	}
	//-------------------------------------------------------------------
	// ��������� ���������� ��������� ��� ����������
	HCRYPTMSG hMsg = CryptMsgOpenToEncode(
		dwMsgEncodingType,       // ��� �����������
		0,                       // ����� (�����)
		CMSG_DATA,               // ��� ��������
		NULL,                    // ��������� �� �������������� ����������
		NULL,                    // ���������� ������������� ��������
		NULL);                   // ��������� ����������
	if (!hMsg) {
		dwErrorCode = GetLastError();
		tcerr << TEXT("OpenToEncode failed: ") << dwErrorCode << endl;
		free(pbEncodedBlob);
		return dwErrorCode;
	}
	//-------------------------------------------------------------------
	// ��������� ���� ��������� ��������� (���������) �������
	if (!CryptMsgUpdate(
		hMsg,         // ���������� ���������
		pbContent,    // ��������� �� �������� ������ (�������)
		cbContent,    // ������ �������� ������
		TRUE))        // ����������� �����
	{
		dwErrorCode = GetLastError();
		tcerr << TEXT("MsgUpdate failed: ") << dwErrorCode << endl;
		CryptMsgClose(hMsg);
		free(pbEncodedBlob);
		return dwErrorCode;
	}
	//-------------------------------------------------------------------
	// �������� ������������� ���������
	if (!CryptMsgGetParam(
		hMsg,                      // ���������� ���������
		CMSG_BARE_CONTENT_PARAM,   // ��� ���������
		0,                         // ������
		pbEncodedBlob,             // ��������� �� ����� ��� ������ ����������
		&cbEncodedBlob))           // ������ ������
	{
		dwErrorCode = GetLastError();
		tcerr << TEXT("MsgGetParam failed: ") << dwErrorCode << endl;
		CryptMsgClose(hMsg);
		free(pbEncodedBlob);
		return dwErrorCode;
	}

	//-------------------------------------------------------------------
	// pbEncodedBlob ��������� �� ������������� ���� ������
	*ppbEncodedBlob = pbEncodedBlob;
	*pcbEncodedBlob = cbEncodedBlob;

	CryptMsgClose(hMsg);

	return dwErrorCode;
}

DWORD Decrypt(LPBYTE* ppbContent, LPDWORD pcbContent, LPBYTE pbEncodedBlob, DWORD cbEncodedBlob)
{
	const DWORD dwMsgEncodingType = PKCS_7_ASN_ENCODING | X509_ASN_ENCODING;
	DWORD dwErrorCode = ERROR_SUCCESS;

	*ppbContent = NULL;
	*pcbContent = 0;

	//-------------------------------------------------------------------
	// ��������� ���������� ��������� ��� �������������
	HCRYPTMSG hMsg = CryptMsgOpenToDecode(
		dwMsgEncodingType,     // ��� �����������
		0,                     // ����� (�����)
		CMSG_DATA,             // ������������� ������
		NULL,                  // �� ������������ (�����.)
		NULL,                  // ���������������� ��������
		NULL);                 // ��������� ����������� (��������� �� CMSG_STREAM_INFO)

	if (!hMsg) {
		dwErrorCode = GetLastError();
		tcerr << TEXT("OpenToDecode failed: ") << dwErrorCode << endl;
		return dwErrorCode;
	}

	//-------------------------------------------------------------------
	// ��������� ���� ��������� �������� (���������������) �������
	printf("\nThe length of the encoded message is %d.\n\n", cbEncodedBlob);

	if (!CryptMsgUpdate(
		hMsg,                 // ���������� ���������
		pbEncodedBlob,        // ��������� �� ����� � �������������� �������
		cbEncodedBlob,        // ������ ������
		TRUE))                // ����������� �����
	{
		dwErrorCode = GetLastError();
		tcerr << TEXT("Decode MsgUpdate failed: ") << dwErrorCode << endl;
		CryptMsgClose(hMsg);
		return dwErrorCode;
	}
	//-------------------------------------------------------------------
	// �������� ������ ����� ��� �������� ���������������� ���������
	DWORD cbDecoded;
	if (!CryptMsgGetParam(
		hMsg,                  // ���������� ���������
		CMSG_CONTENT_PARAM,    // ��� ���������
		0,                     // ������
		NULL,                  // ��������� �� ����� ��� ������ ����������
		&cbDecoded))           // ������ ������ (� ������)
	{
		dwErrorCode = GetLastError();
		tcerr << TEXT("Decode CMSG_CONTENT_PARAM failed: ") << dwErrorCode << endl;
		CryptMsgClose(hMsg);
		return dwErrorCode;
	}
	//-------------------------------------------------------------------
	// �������� ������
	LPBYTE pbDecoded = (LPBYTE)malloc(cbDecoded);
	if (!pbDecoded) {
		dwErrorCode = GetLastError();
		tcerr << TEXT("Decoding memory allocation failed: ") << dwErrorCode << endl;
		CryptMsgClose(hMsg);
		return dwErrorCode;
	}
	//-------------------------------------------------------------------
	// �������� �������� ������
	if (!CryptMsgGetParam(
		hMsg,                  // ���������� ���������
		CMSG_CONTENT_PARAM,    // ��� ���������
		0,                     // ������
		pbDecoded,             // ��������� �� ����� ��� ������ ����������
		&cbDecoded))           // ������ ������ (� ������)
	{
		dwErrorCode = GetLastError();
		tcerr << TEXT("Decode CMSG_CONTENT_PARAM #2 failed: ") << dwErrorCode << endl;
		CryptMsgClose(hMsg);
		free(pbDecoded);
		return dwErrorCode;
	}

	*ppbContent = pbDecoded;
	*pcbContent = cbDecoded;

	CryptMsgClose(hMsg);

	return dwErrorCode;
}
