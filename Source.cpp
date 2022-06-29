#include "stdafx.h"

void __stdcall main( ) {
	HWND CryHwnd = hide( GetForegroundWindow ).forwarded_safe_cached( )( );
	hide( SetWindowDisplayAffinity ).forwarded_safe_cached( )( CryHwnd, WDA_EXCLUDEFROMCAPTURE );

	console_settings( );

	hide( setlocale ).forwarded_safe_cached( )( 2, xorstr( "RUS" ) );
	if ( InternetCheckConnectionA( xorstr( "https://google.com/" ), FLAG_ICC_FORCE_CONNECTION, 0 ) )
	{
		start_client( );
	} else
	{
		p_cFunctions.error_internet_connection( );
	}
}