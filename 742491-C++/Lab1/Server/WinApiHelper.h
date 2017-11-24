#pragma once

PSID ConvertNameToBinarySid(LPTSTR pAccountName, LPDWORD lpdwRetCode);
void DisplayAccessReply(const AUTHZ_ACCESS_REPLY& accessReply);
DWORD GetAccess(
	AUTHZ_CLIENT_CONTEXT_HANDLE hAuthzClient, 
	PSECURITY_DESCRIPTOR psd, 
	PAUTHZ_ACCESS_REPLY pAccessReply);
DWORD GetEffectiveRightsForUser(
	AUTHZ_RESOURCE_MANAGER_HANDLE hManager,
	PSECURITY_DESCRIPTOR psd,
	LPTSTR lpszUserName, 
	PAUTHZ_ACCESS_REPLY pAccessReply);
DWORD UseAuthzSolution(PSECURITY_DESCRIPTOR psd, LPTSTR lpszUserName, PAUTHZ_ACCESS_REPLY pAccessReply);

LPTSTR GetObjectOwner(
	LPTSTR pObjectName,
	SE_OBJECT_TYPE objType = SE_FILE_OBJECT,
	LPDWORD lpdwSize = NULL);
