#include "stdafx.h"
string cheatname_autorization = xorstr( "\n\n\n\n\tCryCode | Авторизация" );
string cheatname_registration = xorstr( "\n\n\n\n\tCryCode | Регистрация" );
string cheatname_update = xorstr( "\n\n\n\n\tCryCode | Обновление" );
class CCustom {
public:
	const int custom_true = 0 < 1;
	const int custom_false = 0 > 1;
	bool debug_flag = custom_false;

}; CCustom p_cCustom;


class CFunctions {
public:
	void startup( int* switch_num ) {
		KeyAuthApp.log( xorstr( "User started loader" ) );
		cout << cheatname_autorization << endl;
		cout << xorstr( "- - - - - - - - - - - - - - - - - -\n" );
		cout << xorstr( " [1] Вход\n [2] Регистрация\n [3] Обновить лицензию" ) << endl;
		cout << xorstr( "- - - - - - - - - - - - - - - - - -\n" );
		cout << xorstr( " Выбор: " ); cin >> *switch_num;
	}

	void intialize_error( ) {
		KeyAuthApp.log( xorstr( "Intialize error" ) );
		cout << xorstr( "[auth] Ошибка инициализации: " ) << KeyAuthApp.data.message;
		hide( Sleep ).forwarded_safe_cached( )( 1500 );
		hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
	}
	void client_error( ) {
		KeyAuthApp.log( xorstr( "User error" ) );
		cout << xorstr( "[security] Ошибка клиента" );
		hide( Sleep ).forwarded_safe_cached( )( 1500 );
		hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
	}


	void authorization( string* login, string* password ) {
		KeyAuthApp.log( xorstr( "User started authorization state" ) );
		cout << cheatname_autorization << endl;
		cout << xorstr( "- - - - - - - - - - - - - - - - - -\n" );
		cout << xorstr( " Логин: " ); cin >> *login;
		cout << xorstr( " Пароль: " ); cin >> *password;
		cout << xorstr( "- - - - - - - - - - - - - - - - - -\n" );
	}
	void authorization_error( ) {
		KeyAuthApp.log( xorstr( "User autorization error" ) );
		cout << xorstr( "[auth] Ошибка авторизации\n" );
		hide( Sleep ).forwarded_safe_cached( )( 1500 );
		hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
	}
	void success_authorization( ) {
		KeyAuthApp.log( xorstr( "Success autorization" ) );
		cout << xorstr( "[auth] Вход выполнен успешно " );
		hide( Sleep ).forwarded_safe_cached( )( 1500 );
	}

	void easy_password( ) {
		KeyAuthApp.log( xorstr( "User have easy password" ) );
		cout << xorstr( "[auth] Придумайте более сложный пароль" );
		hide( Sleep ).forwarded_safe_cached( )( 1500 );
		hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
	}
	void success_registration( ) {
		KeyAuthApp.log( xorstr( "Success registration" ) );
		cout << xorstr( "[auth] Аккаунт зарегестрирован " );
		hide( Sleep ).forwarded_safe_cached( )( 1500 );
		hide( system ).forwarded_safe_cached( )( xorstr( "cls" ) );
	}


	void client_update( string* login, string* key ) {
		KeyAuthApp.log( xorstr( "User started update state" ) );
		cout << cheatname_update << endl;
		cout << xorstr( "- - - - - - - - - - - - - - - - - -\n" );
		cout << xorstr( " Логин: " ); cin >> *login;
		cout << xorstr( " Ключ: " ); cin >> *key;
		cout << xorstr( "- - - - - - - - - - - - - - - - - -\n" );
	}
	void update_error( ) {
		KeyAuthApp.log( xorstr( "Key update error" ) );
		cout << xorstr( "[auth] Ошибка обновления\n" );
		hide( Sleep ).forwarded_safe_cached( )( 1500 );
		hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
	}
	void success_update( ) {
		KeyAuthApp.log( xorstr( "Key update success" ) );
		cout << xorstr( "[auth] Ключ обновлен " );
		hide( Sleep ).forwarded_safe_cached( )( 1500 );
	}

	void registation( string* login, string* password, string* key ) {
		KeyAuthApp.log( xorstr( "User started registration state" ) );
		cout << cheatname_registration << endl;
		cout << xorstr( "- - - - - - - - - - - - - - - - - -\n" );
		cout << xorstr( " Логин: " ); cin >> *login;
		cout << xorstr( " Пароль: " ); cin >> *password;
		cout << xorstr( " Ключ: " ); cin >> *key;
		cout << xorstr( "- - - - - - - - - - - - - - - - - -\n" );
	}

	void game_found_error( ) {
		KeyAuthApp.log( xorstr( "Error game found" ) );
		cout << xorstr( "[launcher] Ошибка определения" );
		hide( Sleep ).forwarded_safe_cached( )( 1500 );
		hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
	}

	void error( int number ) {
		KeyAuthApp.log( xorstr( "Launcher error" ) );
		cout << xorstr( "\nLauncher error: " ) << number;
		hide( Sleep ).forwarded_safe_cached( )( 1500 );
		hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
	}

	void error_internet_connection( ) {
		KeyAuthApp.log( xorstr( "Internet connection error" ) );
		cout << xorstr( "Нет подключения к интернету" );
		hide( Sleep ).forwarded_safe_cached( )( 1500 );
		hide( ExitProcess ).forwarded_safe_cached( )( p_cCustom.custom_false );
	}
}; CFunctions p_cFunctions;

