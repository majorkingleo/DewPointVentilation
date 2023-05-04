#include "SimpleOutDebug.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <utf8_util.h>
#include <stdio.h>

using namespace Tools;

SimpleOutDebug::SimpleOutDebug()
: Debug(),
  prefix(),
  print_line_and_file_info( true )
{

}


void SimpleOutDebug::add( const char *file, unsigned line, const char *function, const std::string & s )
{

	if( print_line_and_file_info ) {

		// remove directory info from filename
		// ../Code/Src/main_task.cc becomes main_task.cc
		std::string f = file;
		auto pos = f.rfind('/');
		if( pos != std::string::npos ) {
			f = f.substr( pos + 1 );
		}

		printf( "%s:%u ", f.c_str(), line );
	}

	if( !prefix.empty() ) {
		printf( "%s ", prefix.c_str() );
	}

	printf( "%s\r\n", s.c_str() );

}

void SimpleOutDebug::add( const char *file, unsigned line, const char *function, const std::wstring & s )
{
	add( file, line, function,  Utf8Util::wStringToUtf8(s) );
}

