/*
5.4 connect()
Модифицируйте программу из п.5.1: добавьте два обязательных параметра командной строки: 
имя и номер порта, с которых принимать сообщения. После создания сокета установите для 
него указанный адрес удаленного сокета с помощью функции connect(). Покажите, что после 
этого сообщения с других адресов не принимаются.
*/

#include <iostream>
#include <unistd.h> // close()
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "../tasklog.h"

using namespace std;

const size_t DataBufSize = 8192;
char data_buf[DataBufSize];

int main(int argc, char* argv[])
{
	if(argc != 4) {
		cout << "usage: " << argv[0] << " server_port client_host client_port" << endl;
		cout << "e.g. " << argv[0] << " 2222 127.0.0.1 3333" << endl;
		return 0;
	}
	uint32_t server_port = strtol(argv[1], NULL, 0);
	const char* client_host = argv[2];
	uint32_t client_port = strtol(argv[3], NULL, 0);

	int sock; // Дескриптор сокета
	char line[1000]; // Массив для принятой и отсылаемой строки
	struct sockaddr_in servaddr, cliaddr; // Структуры для адресов сервера и клиента
	/* Заполняем структуру для адреса сервера: семейство протоколов TCP/IP, сетевой интерфейс - любой, 
	номер порта - из параметра командной строки. Поскольку в структуре содержится дополнительное не
	нужное нам поле, которое должно быть нулевым, перед заполнением обнуляем ее всю */
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(server_port);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	// Создаем UDP сокет
	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror(NULL); // Печатаем сообщение об ошибке
		exit(1);
	}
	// Настраиваем адрес сокета
	if(bind(sock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
		perror(NULL);
		close(sock);
		exit(1);
	}
	/* Устанавливаем для клиента возможные значения порта и адреса
	Для проверки можно запустить эту программу со следующими параметрами (пример) 2222 127.0.0.1 3333 на том же компьютере netcat:
	netcat -u -s localhost -p 3333 localhost 2222
	netcat -u -s localhost -p 3331 localhost 2222
	*/
	struct sockaddr_in host_addr;
	bzero(&host_addr, sizeof(host_addr));
	host_addr.sin_family = AF_INET;
	host_addr.sin_port = htons(client_port);
	int status = inet_pton(host_addr.sin_family, client_host, &host_addr.sin_addr);
	if (status <= 0) {
		if (status == 0)
			fprintf(stderr, "Not in presentation format");
		else
			perror("inet_pton");
		exit(EXIT_FAILURE);
	}
	if(connect(sock, (const sockaddr*) &host_addr, sizeof(host_addr)) == -1) {
		perror("inet_pton");
		exit(EXIT_FAILURE);
	}
	// Основной цикл обслуживания
	while(1) {
		// Максимальная длина для хранения ожидаемого адрcpеса клиента
		socklen_t clilen = sizeof(cliaddr);
		/* Ожидаем прихода запроса от клиента и читаем его. Максимально возможная длина датаграммы – 999 символов,
		адрес отправителя помещаем в структуру cliaddr, его реальная длина будет занесена в переменную clilen */
		int n; // Лоличество принятых символов
		if((n = recvfrom(sock, line, 999, 0, (struct sockaddr *) &cliaddr, &clilen)) < 0) {
			perror(NULL);
			close(sock);
			exit(1);
		}
		line[n] = '\0'; // Клиент нам может и не прислать символ завершения строки
		// Печатаем принятый текст и сведения о клиенском хосте (getnameinfo() вынесена в отдельную функцию)
		cout << line << " (from " << getHostName(&cliaddr, clilen) << ")" << endl;
		// Принятый текст отправляем обратно по адресу отправителя
		if(sendto(sock, line, strlen(line), 0, (struct sockaddr *) &cliaddr, clilen) < 0) {
			perror(NULL);
			close(sock);
			exit(1);
		}
		// Уходим ожидать новую датаграмму
	}
	return 0;
}
