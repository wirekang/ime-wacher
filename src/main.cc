#define UNICODE
#define SVCNAME TEXT("ime-watcher")

#include <strsafe.h>
#include <tchar.h>
#include <windows.h>
#include <winsvc.h>

#include <chrono>

#include "config.h"
#include "svc.h"
#include "util.h"

SERVICE_STATUS gSvcStatus;
SERVICE_STATUS_HANDLE gSvcStatusHandle;
HANDLE ghSvcStopEvent = NULL;

VOID WINAPI SvcCtrlHandler(DWORD);
VOID WINAPI SvcMain(DWORD, LPTSTR *);

VOID ReportSvcStatus(DWORD, DWORD, DWORD);
VOID SvcInit(DWORD, LPTSTR *);

int __cdecl _tmain(int argc, TCHAR *argv[]) {
#ifdef _DEBUG
  printf("debug main\n");
  printf("debug main\n");
  printf("debug main\n");
  svc_start();
  printf("svg_start failed\n");
#else
  SERVICE_TABLE_ENTRY DispatchTable[] = {
      {SVCNAME, (LPSERVICE_MAIN_FUNCTION)SvcMain}, {NULL, NULL}};

  // This call returns when the service has stopped.
  // The process should simply terminate when the call returns.

  if (!StartServiceCtrlDispatcher(DispatchTable)) {
    printf("StartServiceCtrlDispatcher error\n");
  }
#endif
}

//
// Purpose:
//   Entry point for the service
//
// Parameters:
//   dwArgc - Number of arguments in the lpszArgv array
//   lpszArgv - Array of strings. The first string is the name of
//     the service and subsequent strings are passed by the process
//     that called the StartService function to start the service.
//
// Return value:
//   None.
//
VOID WINAPI SvcMain(DWORD dwArgc, LPTSTR *lpszArgv) {
  // Register the handler function for the service
  gSvcStatusHandle = RegisterServiceCtrlHandler(SVCNAME, SvcCtrlHandler);
  if (!gSvcStatusHandle) {
    printf("RegisterServiceCtrlHandler error\n");
    return;
  }

  // These SERVICE_STATUS members remain as set here
  gSvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
  gSvcStatus.dwServiceSpecificExitCode = 0;

  // Report initial status to the SCM
  ReportSvcStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

  // Perform service-specific initialization and work.
  SvcInit(dwArgc, lpszArgv);
}

//
// Purpose:
//   The service code
//
// Parameters:
//   dwArgc - Number of arguments in the lpszArgv array
//   lpszArgv - Array of strings. The first string is the name of
//     the service and subsequent strings are passed by the process
//     that called the StartService function to start the service.
//
// Return value:
//   None
//
VOID SvcInit(DWORD dwArgc, LPTSTR *lpszArgv) {
  ghSvcStopEvent = CreateEvent(NULL,   // default security attributes
                               TRUE,   // manual reset event
                               FALSE,  // not signaled
                               NULL);  // no name

  if (ghSvcStopEvent == NULL) {
    ReportSvcStatus(SERVICE_STOPPED, GetLastError(), 0);
    return;
  }

  // Report running status when initialization is complete.
  ReportSvcStatus(SERVICE_RUNNING, NO_ERROR, 0);

  // TO_DO: Perform work until service stops.
  svc_start();
  ReportSvcStatus(SERVICE_STOPPED, ERROR_INVALID_FUNCTION, 0);
}

//
// Purpose:
//   Sets the current service status and reports it to the SCM.
//
// Parameters:
//   dwCurrentState - The current state (see SERVICE_STATUS)
//   dwWin32ExitCode - The system error code
//   dwWaitHint - Estimated time for pending operation,
//     in milliseconds
//
// Return value:
//   None
//
VOID ReportSvcStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode,
                     DWORD dwWaitHint) {
  static DWORD dwCheckPoint = 1;

  // Fill in the SERVICE_STATUS structure.

  gSvcStatus.dwCurrentState = dwCurrentState;
  gSvcStatus.dwWin32ExitCode = dwWin32ExitCode;
  gSvcStatus.dwWaitHint = dwWaitHint;

  if (dwCurrentState == SERVICE_START_PENDING)
    gSvcStatus.dwControlsAccepted = 0;
  else
    gSvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

  if ((dwCurrentState == SERVICE_RUNNING) ||
      (dwCurrentState == SERVICE_STOPPED))
    gSvcStatus.dwCheckPoint = 0;
  else
    gSvcStatus.dwCheckPoint = dwCheckPoint++;

  // Report the status of the service to the SCM.
  SetServiceStatus(gSvcStatusHandle, &gSvcStatus);
}

//
// Purpose:
//   Called by SCM whenever a control code is sent to the service
//   using the ControlService function.
//
// Parameters:
//   dwCtrl - control code
//
// Return value:
//   None
//
VOID WINAPI SvcCtrlHandler(DWORD dwCtrl) {
  // Handle the requested control code.

  switch (dwCtrl) {
    case SERVICE_CONTROL_STOP:
      ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);

      // Signal the service to stop.
      svc_stop();

      SetEvent(ghSvcStopEvent);
      ReportSvcStatus(gSvcStatus.dwCurrentState, NO_ERROR, 0);

      return;

    case SERVICE_CONTROL_INTERROGATE:
      break;

    default:
      break;
  }
}
