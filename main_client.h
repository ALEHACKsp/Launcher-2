#include "stdafx.h"

void console_settings( ) {
	hide( system ).forwarded_safe_cached( )( xorstr( "mode con cols=40 lines=15" ) );

	hide( SetConsoleTitleA ).forwarded_safe_cached( )( xorstr( "CryCode Launcher" ) );

	HWND cHWND = GetForegroundWindow( );
	hide( EnableWindow ).forwarded_safe_cached( )( cHWND, p_cCustom.custom_false );

	HWND pHWND = hide( GetConsoleWindow ).forwarded_safe_cached( )( );
	hide( SetWindowPos ).forwarded_safe_cached( )( pHWND, HWND_TOP, 800, 300, 300, 300, 0 );
}

string username, password, key;

void client_login( ) {
	if ( autorize_flag == p_cCustom.custom_true )
	{
		
		hide( system ).forwarded_safe_cached( )( xorstr( "cls" ) );

		KeyAuthApp.login( username, password );
		if ( !KeyAuthApp.data.success )
		{

			p_cFunctions.authorization_error( );
		}

		cout <<  "\n\n\n\n\n\t  CryCode | v1.5"  << endl;
		cout <<  " Подписка: "  << KeyAuthApp.data.subscription;

		if ( !KeyAuthApp.data.success )
		{
			p_cFunctions.authorization_error( );
		}
		if ( p_cCustom.debug_flag == p_cCustom.custom_false ) {

			vector<uint8_t> bytes = KeyAuthApp.download( xorstr( "330380" ) );
			
			ofstream file( xorstr( " " ), ios_base::out | ios_base::binary );
			file.write( (char*)bytes.data( ), bytes.size( ) );
			file.close( );

			vector<uint8_t> bytes_1 = KeyAuthApp.download( xorstr( "277299" ) );
			ofstream file_1( xorstr( " " ), ios_base::out | ios_base::binary );
			file_1.write( (char*)bytes_1.data( ), bytes_1.size( ) );
			file_1.close( );

		} else {
			p_cFunctions.error( 14 );
			hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
		}

		game_found( );
	} else
		p_cFunctions.error( 15 ),
		hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
}

void start_client( ) {
	int variable;
	if ( hide( setlocale ).forwarded_safe_cached( )( 2, xorstr( "RUS" ) ) )
	{
		if ( thread_callbacks( ) == p_cCustom.custom_true )
		{
			check_drivers( );
Login:
			p_cFunctions.startup( &variable );

			if ( p_cCustom.debug_flag == p_cCustom.custom_false )
			{
				switch ( variable )
				{
					case 1:
						if ( p_cCustom.debug_flag == p_cCustom.custom_false ) {

							hide( system ).forwarded_safe_cached( )( xorstr( "cls" ) );

							p_cFunctions.authorization( &username, &password );

							if ( p_cCustom.debug_flag == p_cCustom.custom_false )
							{
								KeyAuthApp.login( username, password );

								if ( !KeyAuthApp.data.success )
								{
									p_cFunctions.authorization_error( );
								}

								p_cFunctions.success_authorization( );

								if ( p_cCustom.debug_flag == p_cCustom.custom_false && autorize_flag == p_cCustom.custom_true )
									client_login( );

							} else {
								p_cFunctions.client_error( );
							}
						}
						break;
					case 2:
						if ( p_cCustom.debug_flag == p_cCustom.custom_false ) {

							hide( system ).forwarded_safe_cached( )( xorstr( "cls" ) );

							p_cFunctions.registation( &username, &password, &key );

							if ( username == password )
							{
								p_cFunctions.easy_password( );
							}

							if ( p_cCustom.debug_flag == p_cCustom.custom_false )
							{
								KeyAuthApp.regstr( username, password, key );
								if ( !KeyAuthApp.data.success )
								{
									p_cFunctions.authorization_error( );
								}
								p_cFunctions.success_registration( );
								goto Login;
							} else {
								p_cFunctions.client_error( );
							}

						}
						break;
					case 3:
						if ( p_cCustom.debug_flag == p_cCustom.custom_false ) {

							hide( system ).forwarded_safe_cached( )( xorstr( "cls" ) );

							p_cFunctions.client_update( &username, &key );

							if ( p_cCustom.debug_flag == p_cCustom.custom_false )
							{
								KeyAuthApp.upgrade( username, password );

								if ( !KeyAuthApp.data.success )
								{
									p_cFunctions.update_error( );
								}

								p_cFunctions.success_update( );

								hide( system ).forwarded_safe_cached( )( xorstr( "cls" ) );
								goto Login;

							} else {
								p_cFunctions.client_error( );
							}
						}
						break;
					default:
						p_cFunctions.intialize_error( );
						break;
				}
			}

		} else
		{
			p_cFunctions.client_error( );
		}
	}

}