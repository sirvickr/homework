#pragma once

//   Регистрирует службу в реестре системы
void InstallService(PWSTR pszServiceName,
	PWSTR pszDisplayName,
	DWORD dwStartType,
	PWSTR pszDependencies,
	PWSTR pszAccount,
	PWSTR pszPassword);

//   Удаляет службу из реестра системы
void UninstallService(PWSTR pszServiceName);
