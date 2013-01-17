/*
 * dumpcalls.c: dump callsigns from a Super Check Partial file, such as MASTER.DTA
 * Anthony Willard
 * 2013-01-16
 *
 *
 *
 * */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>

int fgetsz( char *call, FILE *fp )
{
	int c = 0;
	int bytes = 0;
	while( 1 == 1 )
	{
		c = fgetc( fp );
		if( feof( fp ) )
		{
			call[0] = '\0';
			break;
		}

		call[0] = (char) c;
		call++;

		if( c == '\0' )
		{
			break;
		}

		bytes++;
	}

	return( bytes );
}

int main( int argc, char **argv )
{

	// variables
	FILE *input = NULL;

	// working variables
	int keys[((37 *37)+ 1) * sizeof(int)];
	char call[50] = {0};

	// parameter processing

	// file operation
	//
	// open input file
	if( !( input = fopen( "MASTER.DTA", "rb" ) ) )
	{
		perror( "Error opening input file" );
		return( errno );
	}	

	fread( (void *)keys, (37 * 37) + 1, sizeof(int), input );
	fprintf( stderr, "Current file location after reading keys: %ld, 0x%x\n", ftell( input ), ftell( input ) );

	while( !feof( input )  )
	{
		//fprintf( stderr, "Calling reader...\n" );
		call[0] = '\0';
		fgetsz( call, input );

		//fprintf( stderr, "Calling output...\n" );
		if( call[0] == '\0' ) break;

		printf( "%s\n", call );
	}

	fprintf( stderr, "Current file location after reading all data: %ld, 0x%x\n", ftell( input ), ftell( input ) );
	
	// clean up
	if( input )
	{
		fclose( input );
	}
}
