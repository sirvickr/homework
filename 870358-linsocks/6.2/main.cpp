/*
6.1. strtol() или getaddrinfo(), socket(), bind(), listen(), accept().
Написать программу, которая получает один обязательный аргумент командной 
строки - номер порта. Далее нужно создать TCP-сокет, назначить ему полученный 
номер порта с помощью функции bind(), включить прием входящих соединений с помощью 
функции listen() и принять одно входящее соединение с помощью accept().
*/

#include <iostream>
#include <unistd.h> // close()
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "../tasklog.h"

using namespace std;

int main(int argc, char* argv[])
{
	if(argc < 2) {
		cout << "usage: " << argv[0] << " port_number" << endl;
		cout << "e.g " << argv[0] << " 2110" << endl;
		return 0;
	}
	
	uint32_t port_number = strtol(argv[1], NULL, 0);

	int server_sock; // Дескриптор сервернго сокета
	short domain = AF_INET;

	struct sockaddr_in server_addr, client_addr;

	// Создаем TCP сокет
	server_sock = socket(domain, SOCK_STREAM, 0);
	if(server_sock < 0){
		perror(NULL); // Печатаем сообщение об ошибке
		exit(1);
	}

	/* Заполняем структуру для адреса сервера: семейство протоколов TCP/IP, сетевой интерфейс - любой, 
	номер порта - из параметра командной строки. Поскольку в структуре содержится дополнительное не
	нужное нам поле, которое должно быть нулевым, перед заполнением обнуляем ее всю */
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = domain;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port_number);

	// Настраиваем адрес сокета
	if(bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		perror("bind");
		close(server_sock);
		exit(1);
	}
	cout << "bound socket" << endl;

	// Переводим созданный TCP-сокет в пассивное (слушающее) состояние. 
	// Глубину очереди для входящих соединений устанавливаем равной 3
	listen(server_sock , 3);

	int client_sock;
	cout << "waiting for incoming connections..." << endl;
	// В переменную size заносим максимальную длину ожидаемого адреса клиента
	socklen_t size = sizeof(struct sockaddr_in);
	/* Ожидаем полностью установленного соединения на слушающем сокете. При нормальном 
	завершении, в структуре client_addr будет лежать полный адрес клиента, установившего 
	соединение, а в переменной size – его фактическая длина. Вызов же вернет дескриптор 
	присоединенного сокета, через который будет происходить общение с клиентом. */
	client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &size);
	if (client_sock < 0)
	{
		perror("accept");
		close(server_sock);
		exit(1);
	}
	// Печатаем сведения о клиенском хосте (getnameinfo() вынесена в отдельную функцию)
	cout << "connection from " << getHostName(&client_addr, sizeof(client_addr)) << " accepted" << endl;

	return 0;
}
