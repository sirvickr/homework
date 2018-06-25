#ifndef __CONSOLE_EVENTS_PROCESSOR_H__
#define __CONSOLE_EVENTS_PROCESSOR_H__

// продолжают работу только в случае, если вернули 0
typedef int (*KeyEventProcCB)(KEY_EVENT_RECORD, void* cbParam) ;
typedef int (*MouseEventProcCB)(MOUSE_EVENT_RECORD, void* cbParam) ;
typedef int (*ResizeEventProcCB)(WINDOW_BUFFER_SIZE_RECORD, void* cbParam) ;

typedef struct ConsoleEvents {
	HANDLE hStdin;
	void* cbParam;
	KeyEventProcCB cbKey;
	MouseEventProcCB cbMouse;
	ResizeEventProcCB cbResize;
} ConsoleEvents;

ConsoleEvents* ConsoleEventsCreate(HANDLE hStdin, void* cbParam);
VOID ConsoleEventsDestroy(ConsoleEvents* self);

VOID ConsoleSetKeyProc(ConsoleEvents* self, KeyEventProcCB cb);
VOID ConsoleSetMouseProc(ConsoleEvents* self, MouseEventProcCB cb);
VOID ConsoleSetResizeProc(ConsoleEvents* self, ResizeEventProcCB cb);
VOID ConsoleResetKeyProc(ConsoleEvents* self);
VOID ConsoleResetMouseProc(ConsoleEvents* self);
VOID ConsoleResetResizeMouseProc(ConsoleEvents* self);

int ConsoleEventLoop(ConsoleEvents* self);

#endif
