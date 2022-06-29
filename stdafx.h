#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>
#include <ctime>
#include <TlHelp32.h>
#include <string>
#include <array>
#include <utility>
#include <cstdarg>
#include <winternl.h>
#include <wininet.h>
#include <fstream>
#include <Psapi.h>

#pragma comment (lib, "wininet.lib")
#pragma comment (lib, "user32.lib")
#pragma comment(lib, "ntdll.lib")

using namespace std;
#pragma comment (lib, "libcurl.lib")
#pragma comment (lib, "library_x64.lib")


#include "resource.h"
#include "main_utils.h"
#include "client_auth.h"

using namespace KeyAuth;

string name = xorstr( "" );
string ownerid = xorstr( "" );
string secret = xorstr( "" );
string version = xorstr( "" );
string url = xorstr( "https://keyauth.win/api/1.1/" );
string ssPin = xorstr( "ssl pin key (optional)" );

api KeyAuthApp( name, ownerid, secret, version, url, ssPin );


#include "errors_functions.h"
#include "main_security.h"
#include "main_client.h"
