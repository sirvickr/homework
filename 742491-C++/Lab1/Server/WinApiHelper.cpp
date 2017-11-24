#include "stdafx.h"
#include "WinApiHelper.h"

typedef std::basic_string<TCHAR> tstring;
typedef std::basic_ostream<TCHAR> tostream;
typedef std::basic_istream<TCHAR> tistream;
typedef std::basic_ostringstream<TCHAR> tostringstream;
typedef std::basic_istringstream<TCHAR> tistringstream;

#if defined(UNICODE) || defined(_UNICODE)
#define tcout std::wcout
#define tcerr std::wcerr
#else
#define tcout std::cout
#define tcerr std::cerr
#endif

using namespace std;

PSID ConvertNameToBinarySid(LPTSTR pAccountName)
{
	LPTSTR lpServerName = NULL;
	LPTSTR pDomainName = NULL;
	DWORD dwDomainNameSize = 0;
	PSID pSid = NULL;
	DWORD dwSidSize = 0;
	SID_NAME_USE sidType;
	BOOL fSuccess = FALSE;
	HRESULT hr = S_OK;

	DWORD dwCode = 0;
	for (int i = 0; i < 2; i++) {
		dwCode = LookupAccountName(
			lpServerName,      // look up on local system
			pAccountName,
			pSid,              // buffer to receive name
			&dwSidSize,
			pDomainName,
			&dwDomainNameSize,
			&sidType);
		if (!dwCode) {
			dwCode = GetLastError();
			cerr << "LookupAccountName failed with error " << dwCode << ": ";
			if (dwCode == ERROR_NONE_MAPPED) {
				cerr << "ERROR_NONE_MAPPED";
				break;
			}
			else if (dwCode == ERROR_INSUFFICIENT_BUFFER) {
				cerr << "ERROR_INSUFFICIENT_BUFFER (dwSidSize " << dwSidSize << " dwDomainNameSize " << dwDomainNameSize << ")" << endl;
				pSid = (LPTSTR)LocalAlloc(LPTR, dwSidSize * sizeof(TCHAR));
				if (pSid == NULL) {
					cerr << "LocalAlloc failed with error " << GetLastError() << endl;
					break;
				}
				pDomainName = (LPTSTR)LocalAlloc(LPTR, dwDomainNameSize * sizeof(TCHAR));
				if (pDomainName == NULL) {
					cerr << "LocalAlloc failed with error " << GetLastError() << endl;
					break;
				}
			}
			else {
				cerr << "unexpected" << endl;
			}
		}
		else {
			tcerr << TEXT("SUCCESS: pDomainName ") << pDomainName << endl;
			fSuccess = TRUE;
			break;
		}
	} // for(i)
	if (pDomainName != NULL) {
		LocalFree(pDomainName);
		pDomainName = NULL;
	}
	if (pSid != NULL && fSuccess == FALSE) {
		LocalFree(pSid);
		pSid = NULL;
	}
	return pSid;
}

void DisplayError(char* pszAPI, DWORD dwError)
{
	LPVOID lpvMessageBuffer;

	if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_FROM_SYSTEM,
		GetModuleHandle(L"Kernel32.dll"), dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  // the user default language
		(LPTSTR)&lpvMessageBuffer, 0, NULL))
	{
		wprintf_s(L"FormatMessage failed with %d\n", GetLastError());
		ExitProcess(GetLastError());
	}

	//  ...now display this string.
	wprintf_s(L"ERROR: API        = %s.\n", (char *)pszAPI);
	wprintf_s(L"       error code = %08X.\n", dwError);
	wprintf_s(L"       message    = %s.\n", (char *)lpvMessageBuffer);

	//  Free the buffer allocated by the system.
	LocalFree(lpvMessageBuffer);
}

void DisplayAccessMask(ACCESS_MASK Mask)
{
	wprintf_s(L"Effective Allowed Access Mask : %8X\n", Mask);
	if (((Mask & GENERIC_ALL) == GENERIC_ALL)
		|| ((Mask & FILE_ALL_ACCESS) == FILE_ALL_ACCESS))
	{
		wprintf_s(L"Full Control\n");
		return;
	}
	if (((Mask & GENERIC_READ) == GENERIC_READ)
		|| ((Mask & FILE_GENERIC_READ) == FILE_GENERIC_READ))
		wprintf_s(L"Read\n");
	if (((Mask & GENERIC_WRITE) == GENERIC_WRITE)
		|| ((Mask & FILE_GENERIC_WRITE) == FILE_GENERIC_WRITE))
		wprintf_s(L"Write\n");
	if (((Mask & GENERIC_EXECUTE) == GENERIC_EXECUTE)
		|| ((Mask & FILE_GENERIC_EXECUTE) == FILE_GENERIC_EXECUTE))
		wprintf_s(L"Execute\n");

}

void GetAccess(AUTHZ_CLIENT_CONTEXT_HANDLE hAuthzClient, PSECURITY_DESCRIPTOR psd)
{
	AUTHZ_ACCESS_REQUEST AccessRequest = { 0 };
	AUTHZ_ACCESS_REPLY AccessReply = { 0 };
	BYTE     Buffer[1024];
	BOOL bRes = FALSE;  // assume error

						//  Do AccessCheck.
	AccessRequest.DesiredAccess = MAXIMUM_ALLOWED;
	AccessRequest.PrincipalSelfSid = NULL;
	AccessRequest.ObjectTypeList = NULL;
	AccessRequest.ObjectTypeListLength = 0;
	AccessRequest.OptionalArguments = NULL;

	RtlZeroMemory(Buffer, sizeof(Buffer));
	AccessReply.ResultListLength = 1;
	AccessReply.GrantedAccessMask = (PACCESS_MASK)(Buffer);
	AccessReply.Error = (PDWORD)(Buffer + sizeof(ACCESS_MASK));


	if (!AuthzAccessCheck(0,
		hAuthzClient,
		&AccessRequest,
		NULL,
		psd,
		NULL,
		0,
		&AccessReply,
		NULL)) {
		wprintf_s(_T("AuthzAccessCheck failed with %d\n"), GetLastError());
	}
	else
		DisplayAccessMask(*(PACCESS_MASK)(AccessReply.GrantedAccessMask));

	return;
}

BOOL GetEffectiveRightsForUser(AUTHZ_RESOURCE_MANAGER_HANDLE hManager,
	PSECURITY_DESCRIPTOR psd,
	LPTSTR lpszUserName)
{
	PSID pSid = NULL;
	BOOL bResult = FALSE;
	LUID unusedId = { 0 };
	AUTHZ_CLIENT_CONTEXT_HANDLE hAuthzClientContext = NULL;

	pSid = ConvertNameToBinarySid(lpszUserName);
	if (pSid) {
		bResult = AuthzInitializeContextFromSid(
			0,
			pSid,
			hManager,
			NULL,
			unusedId,
			NULL,
			&hAuthzClientContext);
		if (bResult) {
			GetAccess(hAuthzClientContext, psd);
			AuthzFreeContext(hAuthzClientContext);
		}
		else {
			wprintf_s(_T("AuthzInitializeContextFromSid failed with %d\n"), GetLastError());
		}
	}
	if (pSid != NULL) {
		LocalFree(pSid);
		pSid = NULL;
	}

	return bResult;
}

void UseAuthzSolution(PSECURITY_DESCRIPTOR psd, LPTSTR lpszUserName)
{
	AUTHZ_RESOURCE_MANAGER_HANDLE hManager;
	BOOL bResult = FALSE;

	bResult = AuthzInitializeResourceManager(AUTHZ_RM_FLAG_NO_AUDIT,
		NULL, NULL, NULL, NULL, &hManager);
	if (bResult)
	{
		bResult = GetEffectiveRightsForUser(hManager, psd, lpszUserName);
		AuthzFreeResourceManager(hManager);
	}
	else
		wprintf_s(_T("AuthzInitializeResourceManager failed with %d\n"), GetLastError());
}

LPTSTR GetObjectOwner(LPTSTR pObjectName, SE_OBJECT_TYPE objType, LPDWORD lpdwSize)
{
	DWORD dwRtnCode = 0;
	PSID pSidOwner = NULL;
	BOOL bRtnBool = TRUE;
	LPTSTR AcctName = NULL;
	LPTSTR DomainName = NULL;
	LPTSTR OwnerName = NULL;
	DWORD dwAcctName = 1, dwDomainName = 1, dwOwnerName = 0;
	SID_NAME_USE eUse = SidTypeUnknown;
	HANDLE hFile;
	PSECURITY_DESCRIPTOR pSD = NULL;
	DWORD dwErrorCode = 0;

	// Открыть дескриптор интересующего объекта
	hFile = CreateFile(
		pObjectName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		dwErrorCode = GetLastError();
		tcerr << TEXT("CreateFile error = ") << dwErrorCode << endl;
		return NULL;
	}

	// Получить SID владельца объекта
	dwRtnCode = GetSecurityInfo(
		hFile,
		objType,
		OWNER_SECURITY_INFORMATION,
		&pSidOwner,
		NULL,
		NULL,
		NULL,
		&pSD);

	if (dwRtnCode != ERROR_SUCCESS) {
		dwErrorCode = GetLastError();
		tcerr << TEXT("GetSecurityInfo error = ") << dwErrorCode << endl;
		return NULL;
	}

	// Первый вызов LookupAccountSid для получения размеров буферов
	bRtnBool = LookupAccountSid(
		NULL,                   // LPCWSTR       lpSystemName            _In_opt_ (локальная машина)
		pSidOwner,              // PSID          Sid                     _In_
		AcctName,               // LPWSTR        Name                    _Out_opt_
		(LPDWORD)&dwAcctName,   // LPDWORD       cchName                 _Inout_
		DomainName,             // LPWSTR        ReferencedDomainName    _Out_opt_
		(LPDWORD)&dwDomainName, // LPDWORD       cchReferencedDomainName _Inout_
		&eUse);                 // PSID_NAME_USE peUse                   _Out_

								// Выделить память для буферов
	AcctName = (LPTSTR)GlobalAlloc(GMEM_FIXED, sizeof(TCHAR) * (dwAcctName + 1));

	if (AcctName == NULL) {
		dwErrorCode = GetLastError();
		tcerr << TEXT("GlobalAlloc error = ") << dwErrorCode << endl;
		return NULL;
	}

	DomainName = (LPTSTR)GlobalAlloc(GMEM_FIXED, sizeof(TCHAR) * (dwDomainName + 1));

	if (DomainName == NULL) {
		dwErrorCode = GetLastError();
		tcerr << TEXT("GlobalAlloc error = ") << dwErrorCode << endl;
		return NULL;
	}

	// Второй вызов LookupAccountSid для получения имени учетной записи
	bRtnBool = LookupAccountSid(
		NULL,                   // имя локального или удаленного компьютера
		pSidOwner,              // идентификатор безопасности
		AcctName,               // буфер для имени учетной записи
		(LPDWORD)&dwAcctName,   // размер, требуемый для хранения имени
		DomainName,             // буфер для имени домена
		(LPDWORD)&dwDomainName, // размер, требуемый для хранения имени
		&eUse);                 // тип идентификатора безопасности

	if (bRtnBool == FALSE) {
		dwErrorCode = GetLastError();
		if (dwErrorCode == ERROR_NONE_MAPPED)
			tcerr << TEXT("Account owner not found for specified SID.") << endl;
		else
			tcerr << TEXT("Error in LookupAccountSid.") << endl;
		return NULL;

	}
	else if (bRtnBool == TRUE) {
		// Print the account name.
		dwOwnerName = dwDomainName + dwAcctName + 1;
		tcerr << TEXT("dwAcctName = ") << dwAcctName << TEXT(" dwDomainName = ") << dwDomainName
			<< TEXT(" dwOwnerName = ") << dwOwnerName << endl;
		tcerr << TEXT("Account domain = ") << DomainName << endl;
		tcerr << TEXT("Account owner = ") << AcctName << endl;

		OwnerName = (LPTSTR)GlobalAlloc(GMEM_FIXED, sizeof(TCHAR) * (dwOwnerName + 1));

		if (OwnerName == NULL) {
			dwErrorCode = GetLastError();
			tcerr << TEXT("GlobalAlloc error = ") << dwErrorCode << endl;
			return NULL;
		}

		wcscpy(OwnerName, DomainName);
		wcscat(OwnerName, TEXT("/"));
		wcscat(OwnerName, AcctName);
		tcerr << TEXT("OwnerName = ") << OwnerName << endl;
	}

	if (lpdwSize)
		*lpdwSize = dwOwnerName;
	return OwnerName;
}
