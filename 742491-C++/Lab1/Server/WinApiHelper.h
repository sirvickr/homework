#pragma once

PSID ConvertNameToBinarySid(LPTSTR pAccountName);
void DisplayAccessMask(ACCESS_MASK Mask);
void GetAccess(AUTHZ_CLIENT_CONTEXT_HANDLE hAuthzClient, PSECURITY_DESCRIPTOR psd);
BOOL GetEffectiveRightsForUser(
	AUTHZ_RESOURCE_MANAGER_HANDLE hManager,
	PSECURITY_DESCRIPTOR psd,
	LPTSTR lpszUserName);
void UseAuthzSolution(PSECURITY_DESCRIPTOR psd, LPTSTR lpszUserName);
void DisplayError(char* pszAPI, DWORD dwError);

LPTSTR GetObjectOwner(
	LPTSTR pObjectName,
	SE_OBJECT_TYPE objType = SE_FILE_OBJECT,
	LPDWORD lpdwSize = NULL);
