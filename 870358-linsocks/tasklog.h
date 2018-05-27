#include <iostream>
#include <iomanip>
#include <string.h> // memset
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

void showBytes32(uint32_t n, const char* prefix)
{
	using namespace std;
	
	cout << setfill('0') << hex << uppercase << prefix << " byte order:\ns_addr " << setw(8) << n << ",";
	for(int i = 0; i < 4; i++) {
		cout << " " << setw(2) << static_cast<uint32_t>(reinterpret_cast<uint8_t*>(&n)[i]);
	//	cout << " " << setw(2) << ((n >> (i << 3)) & 0xFF); // то же самое
	}
	cout << endl;
}

/** 
 * Печать флагов
 * @param ai указатель на addrinfo
 */
void printFlags(const struct addrinfo *ai)
{
	using namespace std;

	// Флагов нет
    if (0 == ai->ai_flags) {
        printf("(no flags) ");
        return;
    }
    // Какие-то флаги есть
    printf("(flags:");
    if (ai->ai_flags & AI_PASSIVE)
        printf(" passive");
    if (ai->ai_flags & AI_CANONNAME)
        printf(" canon");
    if (ai->ai_flags & AI_NUMERICHOST)
        printf(" numerichost");
#if defined(AI_NUMERICSERV)
    if (ai->ai_flags & AI_NUMERICSERV)
        printf(" numericserv");
#endif
#if defined(AI_V4MAPPED)
    if (ai->ai_flags & AI_V4MAPPED)
        printf(" v4mapped");
#endif
#if defined(AI_ALL)
    if (ai->ai_flags & AI_ALL)
        printf(" all");
#endif

    // расширения IDN (Internationalized Domain Names)
#if defined(AI_IDN)
    if (ai->ai_flags & AI_IDN)
        printf(" idn");
#endif
#if defined(AI_CANONIDN)
    if (ai->ai_flags & AI_CANONIDN)
        printf(" canonidn");
#endif
#if defined(AI_IDN_ALLOW_UNASSIGNED)
    if (ai->ai_flags & AI_IDN_ALLOW_UNASSIGNED)
        printf(" idn_allow_unassigned");
#endif
#if defined(AI_IDN_USE_STD3_ASCII_RULES)
    if (ai->ai_flags & AI_IDN_USE_STD3_ASCII_RULES)
        printf(" idn_use_std3_ascii_rules");
#endif
	printf(") ");
}

/**
 * Печать семейства протоколов (inet, inet6, unix, ...)
 * @param ai указатель на addrinfo
 */
void printFamily(const struct addrinfo *ai)
{
    switch (ai->ai_family) {
    case AF_INET:
        printf("IPv4");
        break;
    case AF_INET6:
        printf("IPv6");
        break;
    case AF_UNIX:
        printf("Unix");
        break;
    case AF_UNSPEC:
        printf("unspec address family ");
        break;
    default:
        printf("unknown address family value %d", ai->ai_family);
    }
    printf(" ");
}
/**
 * Тип сокета (stream, datagram, ...)
 * @param ai указатель на addrinfo
 */
void printSocktype(const struct addrinfo *ai)
{
    printf("socktype: ");
    switch (ai->ai_socktype) {
    case SOCK_STREAM:
        printf("stream");
        break;
    case SOCK_DGRAM:
        printf("datagram");
        break;
    case SOCK_SEQPACKET:
        printf("seqpacket");
        break;
    case SOCK_RAW:
        printf("raw");
        break;
    default:
        printf("unknown type %d", ai->ai_socktype);
    }
    printf(" ");
}

/**
 * Печать протокола (tcp, udp, ,...)
 * @param ai указатель на addrinfo
 */
void printProtocol(const struct addrinfo *ai)
{
	printf("protocol: ");
	switch (ai->ai_protocol) {
	case 0:
		printf("default");
		break;
	case IPPROTO_TCP:
		printf("tcp");
		break;
	case IPPROTO_UDP:
		printf("udp");
		break;
	case IPPROTO_RAW:
		printf("raw");
		break;
	default:
		printf("unknown protocol %d", ai->ai_protocol);
	}
	printf(" ");
}

void printAddrInfo(const addrinfo* ai)
{
	using namespace std;

	printFlags(ai);
	printFamily(ai);
	printSocktype(ai);
	printProtocol(ai);

	char ipstr[INET6_ADDRSTRLEN];
	void *addr;
	// получаем указатель на адрес, по разному в разных протоколах
	if (ai->ai_family == AF_INET) {
		struct sockaddr_in *ipv4 = (struct sockaddr_in *)ai->ai_addr;
		addr = &(ipv4->sin_addr);
	} else if (ai->ai_family == AF_INET6) {
		struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)ai->ai_addr;
		addr = &(ipv6->sin6_addr);
	} else {
		return;
	}
	// преобразуем IP в строку и выводим его:
	inet_ntop(ai->ai_family, addr, ipstr, sizeof ipstr);
	cout << "canonname \"" << (ai->ai_canonname ? ai->ai_canonname : "") << "\" len " << (unsigned int) ai->ai_addrlen << ": " << ipstr << endl;
}

// Получение информации об удалённом хосте: имя и номер порта
std::string getHostName(struct sockaddr_in* sock, socklen_t size) {
    char host[NI_MAXHOST] = "", port[NI_MAXSERV] = "";
    getnameinfo((struct sockaddr *) sock, size, host, NI_MAXHOST, port, NI_MAXSERV, NI_NUMERICSERV);
    return std::string(host) + ":" + port;
}
