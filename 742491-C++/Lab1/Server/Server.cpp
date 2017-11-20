#include "stdafx.h"

using namespace std;

char* OpenURL(char *url);

int main()
{
	WSADATA lpWSAData;
	// Инициализация библиотеки Ws2_32.dll.
	if (WSAStartup(MAKEWORD(1, 1), &lpWSAData) != 0) 
		return WSAGetLastError();
	
	char *result = OpenURL("http://www.codenet.ru/");
	if (result) {
		printf("%s", result);
		free(result);
	}
	else {
		printf("Error # WSABASEERR+%d\n", WSAGetLastError() - WSABASEERR);
	}

	// Деинициализация библиотеки Ws2_32.dll
	WSACleanup();
	return 0;
}
#if 1
char* OpenURL(char *url) {
	SOCKET s;

	// Проверим на правильность введенный адрес.
	// Он должен начинаться с "http://"
	if (memcmp(url, "HTTP://", 7) != 0 && memcmp(url, "http://", 7) != 0) return(NULL);
	url += 7;


	// Получим имя хоста, номер порта и путь ----------------------------

	char *http_host = _strdup(url); // Имя хоста (HTTP_HOST)
	int port_num = 80;             // Номер порта по умолчанию (HTTP_PORT)
	char *http_path = NULL;        // Путь (REQUEST_URI)

	char *pch = strchr(http_host, ':');
	if (!pch) {
		pch = strchr(http_host, '/');
		if (pch) {
			*pch = 0;
			http_path = _strdup(pch + 1);
		}
		else http_path = _strdup("");
	}
	else {
		*pch = 0; pch++;
		char *pch1 = strchr(pch, '/');
		if (pch1) {
			*pch1 = 0;
			http_path = _strdup(pch1 + 1);
		}
		else http_path = _strdup("");

		port_num = atoi(pch);

		if (port_num == 0) port_num = 80;
	}

	// Получаем IP адрес по имени хоста
	struct hostent* hp;
	//if (!(hp = getaddrinfo(http_host))) {
	if (!(hp = gethostbyname(http_host))) {
		free(http_host);
		free(http_path);
		return(NULL);
	}

	// Открываем сокет
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) {
		free(http_host);
		free(http_path);
		return(NULL);
	}

	// Заполняем структуру sockaddr_in
	struct sockaddr_in ssin;
	memset((char *)&ssin, 0, sizeof(ssin));
	ssin.sin_family = AF_INET;
	ssin.sin_addr.S_un.S_un_b.s_b1 = hp->h_addr[0];
	ssin.sin_addr.S_un.S_un_b.s_b2 = hp->h_addr[1];
	ssin.sin_addr.S_un.S_un_b.s_b3 = hp->h_addr[2];
	ssin.sin_addr.S_un.S_un_b.s_b4 = hp->h_addr[3];
	ssin.sin_port = htons(port_num);

	// Выводим IP адрес хоста, с которым будем соединятся
	printf("Conecting to %d.%d.%d.%d...", (unsigned char)hp->h_addr[0],
		(unsigned char)hp->h_addr[1],
		(unsigned char)hp->h_addr[2],
		(unsigned char)hp->h_addr[3]);

	// Соединяемся с хостом
	if (connect(s, (sockaddr *)&ssin, sizeof(ssin)) == -1) {
		free(http_host);
		free(http_path);
		printf("Error\n");
		return(NULL);
	}
	printf("Ok\n");

	// Формируем HTTP запрос
	size_t QuerySize = 2048;
	char *query = (char*)malloc(QuerySize);

	strcpy_s(query, QuerySize, "GET /");
	strcat_s(query, QuerySize, http_path);
	strcat_s(query, QuerySize, " HTTP/1.0\nHost: ");
	strcat_s(query, QuerySize, http_host);
	strcat_s(query, QuerySize, "\nUser-agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)");
	strcat_s(query, QuerySize, "\nAccept: */*\n\n");


	// Выводим HTTP запрос
	printf("%s", query);

	// Отправляем запрос серверу
	int cnt = send(s, query, strlen(query), 0);

	// Освобождаем память
	free(http_host);
	free(http_path);
	free(query);

	// Проверяем, не произошло ли ошибки при отправке запроса на сервер
	if (cnt == SOCKET_ERROR) return(NULL);

	cnt = 1;

	// Получаем ответ с сервера ---------------------------------

	int size = 1024 * 1024; // 1Mb
	char *result = (char*)malloc(size);
	strcpy_s(result, size, "");
	char *result_ptr = result;

	while (cnt != 0 && size>0) {
		cnt = recv(s, result_ptr, sizeof(size), 0);
		if (cnt>0) {
			result_ptr += cnt;
			size -= cnt;
		}
	}
	*result_ptr = 0;

	return(result);
}
#endif
