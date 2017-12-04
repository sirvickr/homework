#include "stdafx.h"
#include "WinApiHelper.h"

using namespace std;

PSID ConvertNameToBinarySid(LPCTSTR pAccountName, LPDWORD lpdwRetCode)
{
	LPTSTR lpServerName = NULL;
	LPTSTR pDomainName = NULL;
	DWORD dwDomainNameSize = 0;
	PSID pSid = NULL;
	DWORD dwSidSize = 0;
	SID_NAME_USE sidType;
	BOOL fSuccess = FALSE;
	HRESULT hr = S_OK;

	DWORD dwCode = ERROR_SUCCESS;
	for (int i = 0; i < 2; i++) {
		if (!LookupAccountName(
			lpServerName,      // NULL - поиск на локальной машине
			pAccountName,
			pSid,              // буфер для резултата (первый раз NULL - запрос размеров)
			&dwSidSize,
			pDomainName,
			&dwDomainNameSize,
			&sidType)) 
		{
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
					dwCode = GetLastError();
					cerr << "LocalAlloc failed with error " << dwCode << endl;
					break;
				}
				pDomainName = (LPTSTR)LocalAlloc(LPTR, dwDomainNameSize * sizeof(TCHAR));
				if (pDomainName == NULL) {
					dwCode = GetLastError();
					cerr << "LocalAlloc failed with error " << dwCode << endl;
					break;
				}
			} else {
				cerr << "unexpected" << endl;
			}
		} else {
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
	*lpdwRetCode = dwCode;
	return pSid;
}

void DisplayAccessReply(const AUTHZ_ACCESS_REPLY& accessReply)
{
	cout << "Effective Allowed Access Mask: ResultListLength " << accessReply.ResultListLength << endl;
	for (DWORD i = 0; i < accessReply.ResultListLength; i++) {
		ACCESS_MASK Mask = accessReply.GrantedAccessMask[i];
		cout << " [" << i << "]: "
			<< hex << uppercase << setfill('0') << setw(8) << Mask << dec;
		if (((Mask & GENERIC_ALL) == GENERIC_ALL)
			|| ((Mask & FILE_ALL_ACCESS) == FILE_ALL_ACCESS))
		{
			cout << " ( Full Control )" << endl;
			return;
		}
		cout << " (";
		if (((Mask & GENERIC_READ) == GENERIC_READ)
			|| ((Mask & FILE_GENERIC_READ) == FILE_GENERIC_READ))
			cout << " Read";
		if (((Mask & GENERIC_WRITE) == GENERIC_WRITE)
			|| ((Mask & FILE_GENERIC_WRITE) == FILE_GENERIC_WRITE))
			cout << " Write";
		if (((Mask & GENERIC_EXECUTE) == GENERIC_EXECUTE)
			|| ((Mask & FILE_GENERIC_EXECUTE) == FILE_GENERIC_EXECUTE))
			cout << " Execute";
		cout << " )" << endl;
	}
}

DWORD GetAccess(AUTHZ_CLIENT_CONTEXT_HANDLE hAuthzClient, PSECURITY_DESCRIPTOR psd, PAUTHZ_ACCESS_REPLY pAccessReply)
{
	AUTHZ_ACCESS_REQUEST AccessRequest = { 0 };
	AUTHZ_ACCESS_REPLY AccessReply = { 0 };
	BYTE Buffer[1024];
	DWORD dwRetCode = ERROR_SUCCESS;

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
		NULL)) 
	{
		dwRetCode = GetLastError();
		tcerr << _T("AuthzAccessCheck failed with code ") << dwRetCode << endl;
	}
	else {
		*pAccessReply = AccessReply;
		//DisplayAccessMask(*(PACCESS_MASK)(AccessReply.GrantedAccessMask));
	}
	return dwRetCode;
}

DWORD GetEffectiveRightsForUser(
	AUTHZ_RESOURCE_MANAGER_HANDLE hManager,
	PSECURITY_DESCRIPTOR psd,
	LPCTSTR lpszUserName,
	PAUTHZ_ACCESS_REPLY pAccessReply)
{
	DWORD dwRetCode = ERROR_SUCCESS;
	PSID pSid = NULL;
	BOOL bResult = FALSE;
	LUID unusedId = { 0 };
	AUTHZ_CLIENT_CONTEXT_HANDLE hCtx = NULL;

	pSid = ConvertNameToBinarySid(lpszUserName, &dwRetCode);
	if (pSid) {
		if (AuthzInitializeContextFromSid(0, pSid, hManager, NULL, unusedId, NULL, &hCtx)) {
			dwRetCode = GetAccess(hCtx, psd, pAccessReply);
			AuthzFreeContext(hCtx);
		}
		else {
			dwRetCode = GetLastError();
			tcerr << _T("AuthzInitializeContextFromSid failed with code ") << dwRetCode << endl;
		}
		LocalFree(pSid);
		pSid = NULL;
	}

	return dwRetCode;
}

DWORD UseAuthzSolution(PSECURITY_DESCRIPTOR psd, LPCTSTR lpszUserName, PAUTHZ_ACCESS_REPLY pAccessReply)
{
	DWORD dwRetCode = ERROR_SUCCESS;
	AUTHZ_RESOURCE_MANAGER_HANDLE hManager;

	if (AuthzInitializeResourceManager(AUTHZ_RM_FLAG_NO_AUDIT, NULL, NULL, NULL, NULL, &hManager))
	{
		dwRetCode = GetEffectiveRightsForUser(hManager, psd, lpszUserName, pAccessReply);
		AuthzFreeResourceManager(hManager);
	}
	else {
		dwRetCode = GetLastError();
		tcerr << _T("AuthzInitializeResourceManager failed with code ") << dwRetCode << endl;
	}

	return dwRetCode;
}

LPTSTR GetObjectOwner(LPCTSTR pObjectName, SE_OBJECT_TYPE objType, LPDWORD lpdwSize)
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

		_tcscpy(OwnerName, DomainName);
		_tcscat(OwnerName, TEXT("/"));
		_tcscat(OwnerName, AcctName);
		tcerr << TEXT("OwnerName = ") << OwnerName << endl;
	}

	if (lpdwSize)
		*lpdwSize = dwOwnerName;
	return OwnerName;
}
