#include "stdafx.h"
#include "TcpSrv.h"
#include "../CryptoAPI/CryptoAPI.h"

using namespace std;

BOOL ValidOptions(int argc, char *argv[]);
BOOL WINAPI CtrlHandler(DWORD dwEvent);

char *g_Port = DEFAULT_PORT;
BOOL g_bVerbose = FALSE;
TcpSrv* server = NULL;

int main(int argc, char *argv[])
{
	if (!SetConsoleCtrlHandler(CtrlHandler, TRUE)) {
		int err = GetLastError();
		cerr << "SetConsoleCtrlHandler() failed to install console handler: " << err << endl;
		return err;
	}
	if (ValidOptions(argc, argv)) {
		server = new TcpSrv(g_Port, g_bVerbose);
		if (server) {
			server->Run();
		}
	}
	SetConsoleCtrlHandler(CtrlHandler, FALSE);
	return 0;
}

BOOL ValidOptions(int argc, char *argv[])
{
	BOOL bRet = TRUE;

	for (int i = 1; i < argc; i++) {
		if ((argv[i][0] == '-') || (argv[i][0] == '/')) {
			switch (tolower(argv[i][1])) {
			case 'e':
				if (strlen(argv[i]) > 3)
					g_Port = &argv[i][3];
				break;

			case 'v':
				g_bVerbose = TRUE;
				break;

			case '?':
				cout << "Usage:\n  iocpserver [-p:port] [-v] [-?]\n";
				cout << "  -e:port\tSpecify echoing port number\n";
				cout << "  -v\t\tVerbose\n";
				cout << "  -?\t\tDisplay this help" << endl;
				bRet = FALSE;
				break;

			default:
				cout << "Unknown options flag " << argv[i] << endl;
				bRet = FALSE;
				break;
			}
		}
	}
	cout << "accepting on " << g_Port << endl;

	return bRet;
}

BOOL WINAPI CtrlHandler(DWORD dwEvent)
{
	SOCKET sockTemp = INVALID_SOCKET;

	switch (dwEvent) {
	case CTRL_BREAK_EVENT:
		if (server) {
			server->Restart();
		}
	case CTRL_C_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
	case CTRL_CLOSE_EVENT:
		if (server) {
			server->Shutdown();
		}
		break;

	default:
		// unknown type--better pass it on.
		return(FALSE);
	}
	return TRUE;
}
