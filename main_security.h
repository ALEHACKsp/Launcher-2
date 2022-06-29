#include "stdafx.h"
extern "C" NTSTATUS NTAPI RtlAdjustPrivilege( ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrThread, PBOOLEAN StatusPointer );
extern "C" NTSTATUS NTAPI NtRaiseHardError( LONG ErrorStatus, ULONG Unless1, ULONG Unless2, PULONG_PTR Unless3, ULONG ValidResponseOption, PULONG ResponsePointer );

#ifdef _WIN64
PINT64 pProcHeap = (PINT64)( __readgsqword( 0x60 ) + 0x30 );
PUINT32 pFlags = (PUINT32)( *pProcHeap + 0x70 );
PUINT32 pForceFlags = (PUINT32)( *pProcHeap + 0x74 );
#else
PPEB pPeb = (PPEB)( __readfsdword( 0x30 ) + 0x18 );
PUINT32 pFlags = (PUINT32)( *pProcessHeap + 0x40 );
PUINT32 pForceFlags = (PUINT32)( *pProcessHeap + 0x44 );
#endif

bool global_flag = p_cCustom.custom_false;
bool autorize_flag = p_cCustom.custom_false;

class CGuard {
public:

	template<typename dbg_present>
	__inline dbg_present debug_present( ) {
		if ( hide( IsDebuggerPresent ).forwarded_safe_cached( )( ) )
			p_cFunctions.error( 0 ),
			hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
		else
			return p_cCustom.custom_true;
	}

	template <typename dbg_remote>
	__inline dbg_remote debug_remote( ) {
		int dbgPresent = p_cCustom.custom_false;
		if ( hide( CheckRemoteDebuggerPresent ).forwarded_safe_cached( )( hide( GetCurrentProcess ).forwarded_safe_cached( )( ), &dbgPresent ) )
		{
			if ( dbgPresent )
				p_cFunctions.error( 1 ),
				hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
			else
				return p_cCustom.custom_true;
		}
	}

	template <typename dbg_process>
	__inline dbg_process debug_query_process( ) {
		PROCESS_BASIC_INFORMATION baseInf;
		hide( NtQueryInformationProcess ).forwarded_safe_cached( )( hide( GetCurrentProcess ).forwarded_safe_cached( )( ), ProcessBasicInformation, &baseInf, sizeof( baseInf ), NULL );
	}

	template <typename dbg_system_process>
	__inline dbg_system_process debug_open_process( ) {

		HANDLE oProcess = hide( OpenProcess ).forwarded_safe_cached( )( PROCESS_ALL_ACCESS, 0, find_process_id<DWORD>( xorstr( "csrss.exe" ) ) );
		if ( oProcess != 0 )
			p_cFunctions.error( 2 ),
			hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
		else
			return p_cCustom.custom_true;
	}

	template <typename cl_session>
	__inline cl_session client_session_timeout( ) {
		hide( Sleep ).forwarded_safe_cached( )( 120000 );
		hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
	}

	template <typename dbg_hide_thread>
	__inline dbg_hide_thread debug_hide_thread( HANDLE thread ) {
		typedef NTSTATUS( NTAPI* pNtSetInformationThread )
			( HANDLE, UINT, PVOID, ULONG );

		NTSTATUS Status;

		pNtSetInformationThread NtSIT = (pNtSetInformationThread)
			hide( GetProcAddress ).forwarded_safe_cached( )( hide( GetModuleHandleA ).forwarded_safe_cached( )( TEXT( xorstr( "ntdll.dll" ) ) ), xorstr( "NtSetInformationThread" ) );

		if ( NtSIT == NULL )
			return false;

		if ( thread == NULL )
			Status = NtSIT( hide( GetCurrentThread ).forwarded_safe_cached( )( ),
				0x11,
				0, 0 );
		else
			Status = NtSIT( thread, 0x11, 0, 0 );

		if ( Status != 0x00000000 )
			return false;
		else
			return true;
	}

	template <typename dbg_peb>
	__inline dbg_peb debug_check_peb( ) {
		PBOOLEAN pDebugged = (PBOOLEAN)__readgsqword( 0x60 ) + 2;
		if ( *pDebugged )
			p_cFunctions.error( 3 ),
			hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
	}


	template <typename process_id>
	__inline process_id find_process_id( string process ) {
		HANDLE pHandle = hide( CreateToolhelp32Snapshot ).forwarded_safe_cached( )( TH32CS_SNAPPROCESS, 0 );
		PROCESSENTRY32 ProcessEntry;
		unsigned long pid;
		ProcessEntry.dwSize = sizeof( ProcessEntry );
		bool Loop = hide( Process32First ).forwarded_safe_cached( )( pHandle, &ProcessEntry );

		while ( Loop )
		{
			if ( ProcessEntry.szExeFile == process )
			{
				pid = ProcessEntry.th32ProcessID;
				hide( CloseHandle ).forwarded_safe_cached( )( pHandle );
				return pid;
			}
			Loop = hide( Process32Next ).forwarded_safe_cached( )( pHandle, &ProcessEntry );
		}
		return 1;
	}


	template <typename open_process>
	__inline open_process process_found( DWORD pid ) {
		HANDLE process = hide( OpenProcess ).forwarded_safe_cached( )( SYNCHRONIZE, FALSE, pid );
		DWORD ret = hide( WaitForSingleObject ).forwarded_safe_cached( )( process, 0 );
		hide( CloseHandle ).forwarded_safe_cached( )( process );
		return ret == WAIT_TIMEOUT;
	}

	template <typename close_process>
	__inline close_process close_process_by_name( const char* filename ) {
		HANDLE hSnapShot = hide( CreateToolhelp32Snapshot ).forwarded_safe_cached( )( TH32CS_SNAPALL, NULL );
		PROCESSENTRY32 pEntry;
		pEntry.dwSize = sizeof( pEntry );
		BOOL hRes = hide( Process32First ).forwarded_safe_cached( )( hSnapShot, &pEntry );
		while ( hRes )
		{
			if ( hide( strcmp ).forwarded_safe_cached( )( pEntry.szExeFile, filename ) == 0 )
			{
				HANDLE hProcess = hide( OpenProcess ).forwarded_safe_cached( )( PROCESS_TERMINATE, 0,
					(DWORD)pEntry.th32ProcessID );
				if ( hProcess != NULL )
				{
					hide( TerminateProcess ).forwarded_safe_cached( )( hProcess, 9 );
					hide( CloseHandle ).forwarded_safe_cached( )( hProcess );
				}
			}
			hRes = hide( Process32Next ).forwarded_safe_cached( )( hSnapShot, &pEntry );
		}
		hide( CloseHandle ).forwarded_safe_cached( )( hSnapShot );
	}
}; CGuard p_cGuard;

void game_found( ) {
	HMODULE load;
	p_cGuard.close_process_by_name<void>( xorstr( "Game.exe" ) );

	cout << xorstr( "\n ќжидаю запуска игры...\n" );
	hide( Sleep ).forwarded_safe_cached( )( 3000 );
	while ( p_cCustom.custom_true )
	{
		if ( autorize_flag == p_cCustom.custom_true )
		{
			if ( p_cGuard.process_found<int>( p_cGuard.find_process_id<DWORD>( xorstr( "Game.exe" ) ) ) )
			{
				KeyAuthApp.log( xorstr( "Game found" ) );
				cout << xorstr( " [+] Game process id: " ) << p_cGuard.find_process_id<DWORD>( xorstr( "Game.exe" ) ) << endl;
				cout << xorstr( " [+] ќжидайте запуска чита " ) << endl;
				hide( Sleep ).forwarded_safe_cached( )( 35000 );
				DWORD pid = NULL;
				load = hide( LoadLibraryExA ).forwarded_safe_cached( )( xorstr( ( " " ) ), NULL, DONT_RESOLVE_DLL_REFERENCES );
				hide( SetWindowsHookExA ).forwarded_safe_cached( )( WH_GETMESSAGE, (HOOKPROC)hide(GetProcAddress).forwarded_safe_cached(), load, hide( GetWindowThreadProcessId ).forwarded_safe_cached( )( FindWindowA( NULL, xorstr( "Warface" ) ), &pid ) );

				cout << xorstr( " [+] „ит запущен, лоадер закроетс€\nчерез пару секунд" ) << endl;
				KeyAuthApp.log( xorstr( "Success inject" ) );
				hide( Sleep ).forwarded_safe_cached( )( 2500 );

				break;
			}
		}
	}


}

int client_timeout( ) {
	for ( ; p_cCustom.custom_true;)
		p_cGuard.client_session_timeout<void>( );

	KeyAuthApp.log( xorstr( "Client timeout expiried" ) );
	hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
	return p_cCustom.custom_true;
}
int check_drivers( ) 	{
	int driver, it;
	DWORD needed;
	LPVOID drivers[1024];
	char c_drivers[1024];

	if ( EnumDeviceDrivers( drivers, sizeof drivers, &needed ) && needed < sizeof drivers ) {

		driver = needed / sizeof( drivers[0] );

		for ( it = 0; it < driver; ++it ) {
			if ( GetDeviceDriverBaseNameA( drivers[it], c_drivers, sizeof( c_drivers ) / sizeof( c_drivers[0] ) ) ) {

				if ( strcmp( c_drivers, "kproccesshacker.sys" ) == 0 || strcmp( c_drivers, "HttpDebuggerSdk.sys" ) == 0 ) {
					KeyAuthApp.log( xorstr( "Driver checker triggered" ) );
					hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
					return false;
				}
			} else {
				return true;
			}
		}
	}
}
int debug_system( ) {
	global_flag = p_cCustom.custom_true;
	for ( ; p_cCustom.custom_true;)
	{
		int start = hide( clock ).forwarded_safe_cached( )( );

		p_cGuard.debug_present<bool>( );
		p_cGuard.debug_query_process<void>( );
		p_cGuard.debug_remote<bool>( );
		p_cGuard.debug_open_process<bool>( );
		p_cGuard.debug_check_peb<void>( );

		global_flag = p_cCustom.custom_true;

		int end = hide( clock ).forwarded_safe_cached( )( );
		int e_end = end - start;
		if ( e_end >= 300 )
		{
			p_cFunctions.client_error( );
		}


		p_cGuard.debug_hide_thread<bool>( hide( GetCurrentThread ).forwarded_safe_cached( )( ) );

		hide( Sleep ).forwarded_safe_cached( )( 5000 );
	}

	global_flag = p_cCustom.custom_false;
	p_cFunctions.error( 4 );
	hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
	return p_cCustom.custom_true;
}

int debug_flag_system( ) {
	for ( ; p_cCustom.custom_true;)
	{
		if ( global_flag == p_cCustom.custom_false )
			p_cFunctions.error( 5 ),
			hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
		hide( Sleep ).forwarded_safe_cached( )( 3500 );
	}
	return p_cCustom.custom_true;
}

void debug_found( ) {
	string s_nProcess[] =
	{
		xorstr( "ollydbg.exe" ),xorstr( "x64dbg.exe" ),xorstr( "x32dbg.exe" ),xorstr( "die.exe" ),xorstr( "tcpview.exe" ),
		xorstr( "autoruns.exe" ),xorstr( "autorunsc.exe" ),xorstr( "filemon.exe" ),xorstr( "procmon.exe" ),xorstr( "regmon.exe" ),
		xorstr( "procexp.exe" ),xorstr( "idaq.exe" ),xorstr( "idaq64.exe" ),xorstr( "ida.exe" ),xorstr( "ida64.exe" ),xorstr( "ImmunityDebugger.exe" ),
		xorstr( "Wireshark.exe" ),xorstr( "dumpcap.exe" ),xorstr( "HookExplorer.exe" ),xorstr( "ImportREC.exe" ),xorstr( "PETools.exe" ),
		xorstr( "LordPE.exe" ),xorstr( "dumpcap.exe" ),xorstr( "SysInspector.exe" ),xorstr( "proc_analyzer.exe" ),xorstr( "sysAnalyzer.exe" ),
		xorstr( "sniff_hit.exe" ),xorstr( "windbg.exe" ),xorstr( "joeboxcontrol.exe" ),xorstr( "joeboxserver.exe" ),xorstr( "fiddler.exe" ),
		xorstr( "tv_w32.exe" ),xorstr( "tv_x64.exe" ),xorstr( "Charles.exe" ),xorstr( "netFilterService.exe" ),xorstr( "HTTPAnalyzerStdV7.exe" ),
		xorstr( "ProcessHacker.exe" ), xorstr( "HTTPDebuggerSvc.exe" ), xorstr( "MegaDumper.exe" )
	};

	while ( p_cCustom.custom_true )
	{
		for ( int i = 0; i < sizeof( s_nProcess ) / sizeof( s_nProcess[0] ); i++ )
		{
			if ( p_cGuard.process_found<int>( p_cGuard.find_process_id<DWORD>( s_nProcess[i].c_str( ) ) ) )
			{
				p_cCustom.debug_flag = p_cCustom.custom_true;
				p_cGuard.close_process_by_name<void>( s_nProcess[i].c_str( ) );

				BOOLEAN PrivilegeState = FALSE;
				ULONG ErrorResponse = 0;
				RtlAdjustPrivilege( 19, TRUE, FALSE, &PrivilegeState );
				NtRaiseHardError( STATUS_IN_PAGE_ERROR, 0, 0, NULL, 6, &ErrorResponse );

				KeyAuthApp.ban( );
			}
			hide( Sleep ).forwarded_safe_cached( )( 500 );
		}
	}

}

HANDLE Mutex;
int twice_application( ) {
	Mutex = hide( CreateMutexA ).forwarded_safe_cached( )( 0, true, xorstr( "CryCode" ) );
	DWORD Result = hide( WaitForSingleObject ).forwarded_safe_cached( )( Mutex, 0 );
	while ( p_cCustom.custom_true )
	{
		if ( Result != WAIT_OBJECT_0 )
		{
			p_cFunctions.error( 6 ),
				hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
			break;
		}
		hide( Sleep ).forwarded_safe_cached( )( 150 );
	}
	return p_cCustom.custom_true;
}

bool check_blacklist( ) {
	while ( p_cCustom.custom_true )
	{
		if ( KeyAuthApp.checkblack( ) )
			p_cFunctions.error( 7 ),
			hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
		hide( Sleep ).forwarded_safe_cached( )( 2500 );
	}
	return p_cCustom.custom_true;

}

int thread_callbacks( ) {
	if ( hide( CloseHandle ).forwarded_safe_cached( )( hide( CreateThread ).forwarded_safe_cached( )( 0, 0, (LPTHREAD_START_ROUTINE)debug_system, 0, 0, 0 ) ) )
	{
		if ( hide( CloseHandle ).forwarded_safe_cached( )( hide( CreateThread ).forwarded_safe_cached( )( 0, 0, (LPTHREAD_START_ROUTINE)debug_flag_system, 0, 0, 0 ) ) )
		{
			if ( hide( CloseHandle ).forwarded_safe_cached( )( hide( CreateThread ).forwarded_safe_cached( )( 0, 0, (LPTHREAD_START_ROUTINE)debug_found, 0, 0, 0 ) ) )
			{
				if ( hide( CloseHandle ).forwarded_safe_cached( )( hide( CreateThread ).forwarded_safe_cached( )( 0, 0, (LPTHREAD_START_ROUTINE)twice_application, 0, 0, 0 ) ) )
				{
					if ( hide( CloseHandle ).forwarded_safe_cached( )( hide( CreateThread ).forwarded_safe_cached( )( 0, 0, (LPTHREAD_START_ROUTINE)check_blacklist, 0, 0, 0 ) ) )
					{
						hide( ReleaseMutex ).forwarded_safe_cached( )( Mutex );
						hide( CloseHandle ).forwarded_safe_cached( )( Mutex );

						autorize_flag = p_cCustom.custom_true;
						KeyAuthApp.init( );

						if ( !KeyAuthApp.data.success )
							p_cFunctions.intialize_error( );

						return p_cCustom.custom_true;

					} else
					{
						p_cFunctions.error( 8 ),
							hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
						return p_cCustom.custom_false;
					}
				} else
				{
					p_cFunctions.error( 9 ),
						hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
					return p_cCustom.custom_false;
				}
			} else
			{
				p_cFunctions.error( 10 ),
					hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
				return p_cCustom.custom_false;
			}
		} else
		{
			p_cFunctions.error( 12 ),
				hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
			return p_cCustom.custom_false;
		}
	} else
	{
		p_cFunctions.error( 13 ),
			hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
		return p_cCustom.custom_false;
	}
	return p_cCustom.custom_false;
}