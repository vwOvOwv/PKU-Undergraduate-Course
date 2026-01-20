/*
 * Copyright (C) 2017 CAMELab
 *
 * This file is part of SimpleSSD.
 *
 * SimpleSSD is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SimpleSSD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SimpleSSD.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * This file contains some codes from original gem5 project.
 * See src/sim/backtrace_glib.cc and src/sim/init_signals.cc of gem5.
 * Followings are copyright from gem5.
 */

/*
 * Copyright (c) 2015 ARM Limited
 * All rights reserved
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
 *
 * Copyright (c) 2000-2005 The Regents of The University of Michigan
 * Copyright (c) 2008 The Hewlett-Packard Development Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Andreas Sandberg, Nathan Binkert
 */

#include "sim/signal.hh"

#include <cinttypes>
#include <csignal>
#include <cstring>
#include <iostream>

#include <signal.h>

#ifdef _MSC_VER

#pragma comment(lib, "dbghelp.lib")

#include <Windows.h>
// Windows.h should be included before DbgHelp.h
#include <DbgHelp.h>

#ifndef _M_X64
#error "Only x86_64 (AMD64) is supported."
#endif

void (*closeHandler)(int) = nullptr;

void print_backtrace();

BOOL consoleHandler(DWORD type) {
  if (closeHandler) {
    closeHandler(0);
  }

  return TRUE;
}

LONG WINAPI exceptionHandler(LPEXCEPTION_POINTERS pExceptionInfo) {
  switch (pExceptionInfo->ExceptionRecord->ExceptionCode) {
    case EXCEPTION_ACCESS_VIOLATION:
    case EXCEPTION_IN_PAGE_ERROR:
      std::cerr << "Program has encountered a segmentation fault." << std::endl;
      break;
    case EXCEPTION_INT_OVERFLOW:
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
      std::cerr << "Program has encountered a floating point exception."
                << std::endl;
      break;
    default:
      std::cerr << "Program aborted." << std::endl;
  }

  print_backtrace();

  return EXCEPTION_EXECUTE_HANDLER;
}

#else

#include <execinfo.h>
#include <unistd.h>

#define FRAMECOUNT 32

static uint8_t stack[65536 * 2];

void print_backtrace();

static bool setupStack() {
  stack_t st;

  st.ss_sp = stack;
  st.ss_size = sizeof(stack);
  st.ss_flags = 0;

  return sigaltstack(&st, nullptr) == 0;
}

static void raiseSignal(int sig) {
  pthread_kill(pthread_self(), sig);

  exit(0);
}

static void installHandler(int sig, void (*handler)(int),
                           int flags = SA_RESTART) {
  struct sigaction sa;

  memset(&sa, 0, sizeof(sa));
  sigemptyset(&sa.sa_mask);
  sa.sa_handler = handler;
  sa.sa_flags = flags;

  sigaction(sig, &sa, nullptr);
}

void abortHandler(int sig) {
  std::cerr << "Program aborted." << std::endl;

  print_backtrace();
  raiseSignal(sig);
}

void segfaultHandler(int sig) {
  std::cerr << "Program has encountered a segmentation fault." << std::endl;

  print_backtrace();
  raiseSignal(sig);
}

void fpeHandler(int sig) {
  std::cerr << "Program has encountered a floating point exception."
            << std::endl;

  print_backtrace();
  raiseSignal(sig);
}

#endif

void print_backtrace() {
  std::cerr << "--- BEGIN LIBC BACKTRACE ---" << std::endl;

#ifdef _MSC_VER
  CONTEXT context;
  STACKFRAME64 frame;
  DWORD type;
  HANDLE hProcess = GetCurrentProcess();
  HANDLE hThread = GetCurrentThread();
  char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(char)];
  PSYMBOL_INFO symInfo = (PSYMBOL_INFO)buffer;
  IMAGEHLP_MODULE modInfo;
  DWORD64 displacement;

  RtlCaptureContext(&context);

  memset(&frame, 0, sizeof(STACKFRAME64));
  type = IMAGE_FILE_MACHINE_AMD64;
  frame.AddrPC.Offset = context.Rip;
  frame.AddrPC.Mode = AddrModeFlat;
  frame.AddrFrame.Offset = context.Rsp;
  frame.AddrFrame.Mode = AddrModeFlat;
  frame.AddrStack.Offset = context.Rsp;
  frame.AddrStack.Mode = AddrModeFlat;

  symInfo->SizeOfStruct = sizeof(SYMBOL_INFO);
  symInfo->MaxNameLen = MAX_SYM_NAME;
  modInfo.SizeOfStruct = sizeof(IMAGEHLP_MODULE);

  if (!SymInitialize(hProcess, nullptr, true)) {
    std::cerr << "Failed to initialize symbol table." << std::endl;
    std::cerr << "--- END LIBC BACKTRACE ---" << std::endl;

    return;
  }

  while (true) {
    if (!StackWalk64(type, hProcess, hThread, &frame, &context, nullptr,
                     SymFunctionTableAccess64, SymGetModuleBase64, nullptr)) {
      break;
    }

    if (frame.AddrPC.Offset != 0) {
      // Get Module Name
      if (SymGetModuleInfo(hProcess, frame.AddrPC.Offset, &modInfo)) {
        std::cerr << modInfo.ModuleName;
      }
      else {
        std::cerr << "<Unknown Module>";
      }

      // Get Symbole Name
      if (SymFromAddr(hProcess, frame.AddrPC.Offset, &displacement, symInfo)) {
        std::cerr << "(" << symInfo->Name << "+0x" << std::hex << displacement
                  << ")";
      }

      std::cerr << "[0x" << std::hex << frame.AddrPC.Offset << "]" << std::endl;
    }
    else {
      break;
    }
  }

  SymCleanup(hProcess);
  CloseHandle(hThread);
  CloseHandle(hProcess);
#else
  void *buffer[FRAMECOUNT];
  int size;

  size = backtrace(buffer, sizeof(buffer) / sizeof(*buffer));

  backtrace_symbols_fd(buffer, size, STDERR_FILENO);

  if (size == sizeof(buffer))
    std::cerr << "Warning: Backtrace may have been truncated." << std::endl;
#endif

  std::cerr << "--- END LIBC BACKTRACE ---" << std::endl;
}

void installSignalHandler(void (*handler)(int)) {
#ifdef _MSC_VER
  closeHandler = handler;
  SetConsoleCtrlHandler((PHANDLER_ROUTINE)consoleHandler, TRUE);
  SetUnhandledExceptionFilter(exceptionHandler);
#else
  installHandler(SIGINT, handler);
  installHandler(SIGABRT, abortHandler, SA_RESETHAND | SA_NODEFER);

  setupStack();

  installHandler(SIGSEGV, segfaultHandler,
                 SA_RESETHAND | SA_NODEFER | SA_ONSTACK);
  installHandler(SIGFPE, fpeHandler, SA_RESETHAND | SA_NODEFER | SA_ONSTACK);
#endif
}
