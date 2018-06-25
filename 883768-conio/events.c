#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#pragma hdrstop
#include "events.h"

ConsoleEvents* ConsoleEventsCreate(HANDLE hStdin, void* cbParam) {
	ConsoleEvents* self;
	if(INVALID_HANDLE_VALUE == hStdin)
		return NULL;
	self = (ConsoleEvents*)malloc(sizeof(ConsoleEvents));
	if(!self)
		return NULL;
	memset(self, 0x00, sizeof(ConsoleEvents));
	self->hStdin = hStdin;
	self->cbParam = cbParam;
	return self;
}

VOID ConsoleEventsDestroy(ConsoleEvents* self) {
	if(self) {
		free(self);
	}
}

VOID ConsoleSetKeyProc(ConsoleEvents* self, KeyEventProcCB cb) {
	self->cbKey = cb;
}

VOID ConsoleSetMouseProc(ConsoleEvents* self, MouseEventProcCB cb) {
	self->cbMouse = cb;
}

VOID ConsoleSetResizeProc(ConsoleEvents* self, ResizeEventProcCB cb) {
	self->cbResize = cb;
}

VOID ConsoleResetKeyProc(ConsoleEvents* self) {
	self->cbKey = NULL;
}

VOID ConsoleResetMouseProc(ConsoleEvents* self) {
	self->cbMouse = NULL;
}

VOID ConsoleResetResizeMouseProc(ConsoleEvents* self) {
	self->cbResize = NULL;
}

#define EVENT_BUF_SIZE 128

int ConsoleEventLoop(ConsoleEvents* self) {
	DWORD fdwSaveOldMode;
	DWORD cNumRead, i;
	DWORD fdwMode = 0;
	int result = 0;
	INPUT_RECORD irInBuf[EVENT_BUF_SIZE];
	if (! GetConsoleMode(self->hStdin, &fdwSaveOldMode) )
		return -1;
	if(self->cbKey)
		fdwMode |= ENABLE_WINDOW_INPUT;
	if(self->cbMouse)
		fdwMode |= ENABLE_MOUSE_INPUT;
	// è ò.ä.
    if (! SetConsoleMode(self->hStdin, fdwMode) ) 
		return -1;

	while (0 == result)
    { 
        // Wait for the events. 

        if (! ReadConsoleInput( 
				self->hStdin,   // input buffer hStdin
				irInBuf,        // buffer to read into
				EVENT_BUF_SIZE, // size of read buffer
				&cNumRead) ) {  // number of records read
			result = -1;
			break;
		}

        // Dispatch the events to the appropriate handler. 

        for (i = 0; i < cNumRead; i++) 
        {
            switch(irInBuf[i].EventType) 
            { 
                case KEY_EVENT: // keyboard input 
					result = self->cbKey(irInBuf[i].Event.KeyEvent, self->cbParam);
					break;

                case MOUSE_EVENT: // mouse input 
					result = self->cbMouse(irInBuf[i].Event.MouseEvent, self->cbParam);
                    break; 

                case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing 
					result = self->cbResize(irInBuf[i].Event.WindowBufferSizeEvent, self->cbParam);
                    break; 

				case FOCUS_EVENT:  // disregard focus events
					break;

                case MENU_EVENT:   // disregard menu events 
                    break; 

                default: 
                    //ErrorExit("Unknown event type"); 
                    break; 
            } 
        }
    } 

    // Restore input mode on exit.

    SetConsoleMode(self->hStdin, fdwSaveOldMode);

	return result;
}

