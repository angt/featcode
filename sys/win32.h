#pragma once

#include <windows.h>
#include <setjmp.h>

static jmp_buf sys_jmp;
static PVOID windows_handler_handle = NULL;

static LONG WINAPI
sys_exception_handler(PEXCEPTION_POINTERS ExceptionInfo) {
    if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ILLEGAL_INSTRUCTION)
        longjmp(sys_jmp, 1);

    return EXCEPTION_CONTINUE_SEARCH;
}

static void
sys_init(void) {
    windows_handler_handle = AddVectoredExceptionHandler(1, sys_exception_handler);
}

static void
sys_exit(void) {
    if (windows_handler_handle) {
        RemoveVectoredExceptionHandler(windows_handler_handle);
        windows_handler_handle = 0;
    }
}

#define sys_setjmp() setjmp(sys_jmp)
