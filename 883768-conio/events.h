#ifndef __CONSOLE_EVENTS_PROCESSOR_H__
#define __CONSOLE_EVENTS_PROCESSOR_H__

typedef VOID (*KeyEventProcCB)(KEY_EVENT_RECORD) ;
typedef VOID (*MouseEventProcCB)(MOUSE_EVENT_RECORD) ;
typedef VOID (*ResizeEventProcCB)(WINDOW_BUFFER_SIZE_RECORD) ;

typedef struct ConsoleEvents {
	HANDLE hStdin;
	KeyEventProcCB cbKey;
	MouseEventProcCB cbMouse;
	ResizeEventProcCB cbResize;
} ConsoleEvents;

ConsoleEvents* ConsoleEventsCreate(HANDLE hStdin);
VOID ConsoleEventsDestroy(ConsoleEvents* self);

VOID ConsoleSetKeyProc(ConsoleEvents* self, KeyEventProcCB cb);
VOID ConsoleSetMouseProc(ConsoleEvents* self, MouseEventProcCB cb);
VOID ConsoleSetResizeProc(ConsoleEvents* self, ResizeEventProcCB cb);
VOID ConsoleResetKeyProc(ConsoleEvents* self);
VOID ConsoleResetMouseProc(ConsoleEvents* self);
VOID ConsoleResetResizeMouseProc(ConsoleEvents* self);

int ConsoleEventLoop(ConsoleEvents* self);

#endif
