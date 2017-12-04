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
	// Получаем размер блока для хранения зашифрованного сообщения
	DWORD cbEncodedBlob = CryptMsgCalculateEncodedLength(
		dwMsgEncodingType,      // вид кодирования
		0,                      // флаги (опции)
		CMSG_DATA,              // тип собщения
		NULL,                   // указатель на структуру
		NULL,                   // внутренний идентификатор контента
		cbContent);             // размер контента
	if (!cbEncodedBlob) {
		dwErrorCode = GetLastError();
		tcerr << TEXT("Getting cbEncodedBlob length failed: ") << dwErrorCode << endl;
		return dwErrorCode;
	}
	//-------------------------------------------------------------------
	// Выделяем память
	LPBYTE pbEncodedBlob = (LPBYTE)malloc(cbEncodedBlob);
	if (!pbEncodedBlob) {
		dwErrorCode = GetLastError();
		tcerr << TEXT("Memory allocation failed: ") << dwErrorCode << endl;
		return dwErrorCode;
	}
	//-------------------------------------------------------------------
	// Открываем дескриптор сообщения для шифрования
	HCRYPTMSG hMsg = CryptMsgOpenToEncode(
		dwMsgEncodingType,       // вид кодирования
		0,                       // флаги (опции)
		CMSG_DATA,               // тип собщения
		NULL,                    // указатель на дополнительную информацию
		NULL,                    // внутренний идентификатор контента
		NULL);                   // потоковая информация
	if (!hMsg) {
		dwErrorCode = GetLastError();
		tcerr << TEXT("OpenToEncode failed: ") << dwErrorCode << endl;
		free(pbEncodedBlob);
		return dwErrorCode;
	}
	//-------------------------------------------------------------------
	// Заполняем блок сообщения исходными (открытыми) данными
	if (!CryptMsgUpdate(
		hMsg,         // дескриптор сообщения
		pbContent,    // указатель на исходные данные (контент)
		cbContent,    // размер исходных данных
		TRUE))        // завершающий вызов
	{
		dwErrorCode = GetLastError();
		tcerr << TEXT("MsgUpdate failed: ") << dwErrorCode << endl;
		CryptMsgClose(hMsg);
		free(pbEncodedBlob);
		return dwErrorCode;
	}
	//-------------------------------------------------------------------
	// Получаем зашифрованное сообщение
	if (!CryptMsgGetParam(
		hMsg,                      // дескриптор сообщения
		CMSG_BARE_CONTENT_PARAM,   // тип параметра
		0,                         // индекс
		pbEncodedBlob,             // указатель на буфер для записи результата
		&cbEncodedBlob))           // размер буфера
	{
		dwErrorCode = GetLastError();
		tcerr << TEXT("MsgGetParam failed: ") << dwErrorCode << endl;
		CryptMsgClose(hMsg);
		free(pbEncodedBlob);
		return dwErrorCode;
	}

	//-------------------------------------------------------------------
	// pbEncodedBlob указывает на зашифрованный блок данных
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
	// Открываем дескриптор сообщения для декодирования
	HCRYPTMSG hMsg = CryptMsgOpenToDecode(
		dwMsgEncodingType,     // вид кодирования
		0,                     // флаги (опции)
		CMSG_DATA,             // зашифрованные данные
		NULL,                  // не используется (устар.)
		NULL,                  // зарезевированный параметр
		NULL);                 // потоковая ифноврмация (указатель на CMSG_STREAM_INFO)

	if (!hMsg) {
		dwErrorCode = GetLastError();
		tcerr << TEXT("OpenToDecode failed: ") << dwErrorCode << endl;
		return dwErrorCode;
	}

	//-------------------------------------------------------------------
	// Заполняем блок сообщения исходнми (закодированными) данными
	printf("\nThe length of the encoded message is %d.\n\n", cbEncodedBlob);

	if (!CryptMsgUpdate(
		hMsg,                 // дескриптор сообщения
		pbEncodedBlob,        // указатель на буфер с зашифрованными данными
		cbEncodedBlob,        // размер буфера
		TRUE))                // завершающий вызов
	{
		dwErrorCode = GetLastError();
		tcerr << TEXT("Decode MsgUpdate failed: ") << dwErrorCode << endl;
		CryptMsgClose(hMsg);
		return dwErrorCode;
	}
	//-------------------------------------------------------------------
	// Получаем размер блока для хранения раскодированного сообщения
	DWORD cbDecoded;
	if (!CryptMsgGetParam(
		hMsg,                  // дескриптор сообщения
		CMSG_CONTENT_PARAM,    // тип папаметра
		0,                     // индекс
		NULL,                  // указатель на буфер для записи результата
		&cbDecoded))           // размер буфера (в данном)
	{
		dwErrorCode = GetLastError();
		tcerr << TEXT("Decode CMSG_CONTENT_PARAM failed: ") << dwErrorCode << endl;
		CryptMsgClose(hMsg);
		return dwErrorCode;
	}
	//-------------------------------------------------------------------
	// Выделяем память
	LPBYTE pbDecoded = (LPBYTE)malloc(cbDecoded);
	if (!pbDecoded) {
		dwErrorCode = GetLastError();
		tcerr << TEXT("Decoding memory allocation failed: ") << dwErrorCode << endl;
		CryptMsgClose(hMsg);
		return dwErrorCode;
	}
	//-------------------------------------------------------------------
	// Получаем открытые данные
	if (!CryptMsgGetParam(
		hMsg,                  // дескриптор сообщения
		CMSG_CONTENT_PARAM,    // тип папаметра
		0,                     // индекс
		pbDecoded,             // указатель на буфер для записи результата
		&cbDecoded))           // размер буфера (в данном)
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
