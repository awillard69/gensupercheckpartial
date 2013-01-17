/*
 * gensupercheckpartial.c: generate a super check file, typically MASTER.DTA, for logging programs
 *
 * Anthoy J. Willard
 * 2013-01-16
 *
 *
 */

#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>

#define INDEXER_SIZE ((37*37)+1)

int call_compare(const void *s1, const void *s2)
{
	//fprintf( stderr, "%s =? %s\n", (char *)s1, *(char **)s2);
	return( strcmp( (char *)s1, *(char **)s2) );
}


int main( int argc, char **argv )
{
	// variables
	FILE *input = NULL; // arbitrary input file pointer
	FILE *scp = NULL; // supercheckparital output file

	char *output_filename = NULL; // the name of the output file to create

	// input call variables
	char *call_data = NULL; // block of calls as loaded from file
	char **call_ptrs = NULL; // block of pointers for accessing calls
	unsigned int call_cnt = 0; // number of calls in list
	char *call_filename = NULL; // pointer to file name containing calls to use

	// team variables	
	char *team_name = NULL; // name to associate when loading a call
	char *team_data = NULL; // block of calls as loaded from team file
	char **team_ptrs = NULL; // block of pointers for accessing team calls
	unsigned int team_cnt = 0; // number of calls in team list
	char *team_filename = NULL; // pointer to the file for team association

	// index variables
	int index[INDEXER_SIZE] = {0};
	const char *charvals = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789/";

	// parameter variables
	int opt;

	// parameter processing
	while( (opt = getopt( argc, argv, "i:o:t:n:" ) ) != -1 )
	{
		switch( opt )
		{
			case 'i':
				call_filename = strdup( optarg );
				break;
			case 'o':
				output_filename = strdup( optarg );
				break;
			case 't':
				team_filename = strdup( optarg );
				break;
			case 'n':
				team_name = strdup( optarg );
				break;

			case 'h': /* '?' */
				fprintf( stderr, "%s -i <input file> -o <output file> [-t <team file> -n <team name>]\n", argv[0] );
				break;
			default: /* '?' */
				fprintf( stderr, "%s -i <input file> -o <output file> [-t <team file> -n <team name>]\n", argv[0] );
				return( 99 );
		}
	}

	// debugging information
	fprintf( stderr, "Input filename : %s\n", call_filename );
	fprintf( stderr, "Out filename   : %s\n", output_filename );
	fprintf( stderr, "Team filename  : %s\n", team_filename );
	fprintf( stderr, "Team name      : %s\n", team_name);

	if( call_filename == NULL || output_filename == NULL )
	{
		fprintf( stderr, "Missing required file names, use -h for parameters\n" );
		exit( 98 );
	}

	// read in list of calls to use for the generation of the file
	long filesize = 0;

	// we open this here to make sure we can.  don't want to wait until after
	// any intensive processing to find out we cannot output any data...	
	if( !( scp = fopen( output_filename, "wb" ) ) )
	{
		perror( "Error opening output file" );
		exit( errno );
	}

	if( !( input = fopen( call_filename, "r" ) ) )
	{
		perror( "Error opening input file" );
		exit( errno );
	}

	// get the file size of the input
	fseek( input, 0, SEEK_END );
	filesize = ftell( input ) + 10; // plus a few bytes of boundary

	fseek( input, 0, SEEK_SET );

	fprintf( stderr, "Input file contains %ld bytes of callsign data\n", filesize );

	// now assign the data area needed for the calls.
	if( !( call_data = (char *)malloc( filesize ) ) )
	{
		perror( "Error allocating call storage" );
		exit( errno );
	}

	// read the calls into the buffer
	char buff_call[50];
	char *callptr = call_data;

	while( fgets( buff_call, 50, input ) != NULL )
	{
		//fprintf( stderr, "%s ", buff_call );

		strcpy( callptr, buff_call ); // copy the call into the buffer space
		callptr += strlen( buff_call ) ; // move the indexer 
		call_cnt++; // increment the number of calls encountered
	}

	// the process loads the end-of-line values, so swap them with nulls
	int setidx = 0;
	if( !( call_ptrs = (char **)malloc( call_cnt * sizeof(char *) ) ) )
	{
		perror( "Error allocating call pointer index storage" );
		exit( errno );
	}

	call_ptrs[ setidx ] = call_data;
	for( int i = 0; i < filesize; i ++ )
	{
		if( call_data[i] == '\n' )
		{
			call_data[i] = '\0';
			call_ptrs[ setidx++ ] = &call_data[i + 1];
		}
	}

	fprintf( stderr, "Load process loaded %d calls\n", call_cnt );
	fprintf( stderr, "Last pointer loaded %d\n", setidx );
	
	// done loading input calls...
	fclose( input );

	// team data processing
	if( team_filename != NULL && team_name != NULL )
	{
		if( !( input = fopen( team_filename, "r" ) ) )
		{
			perror( "Error opening team input file" );
			exit( errno );
		}
	
		// get the file size of the input
		fseek( input, 0, SEEK_END );
		filesize = ftell( input ) + 10; // plus a few bytes of boundary
	
		fseek( input, 0, SEEK_SET );
	
		fprintf( stderr, "Team input file contains %ld bytes of callsign data\n", filesize );
	
		// now assign the data area needed for the calls.
		if( !( team_data = (char *)malloc( filesize ) ) )
		{
			perror( "Error allocating team call storage" );
			exit( errno );
		}
	
		// read the calls into the buffer
		callptr = team_data;
	
		while( fgets( buff_call, 50, input ) != NULL )
		{
			//fprintf( stderr, "%s ", buff_call );
	
			strcpy( callptr, buff_call ); // copy the call into the buffer space
			callptr += strlen( buff_call ) ; // move the indexer 
			team_cnt++; // increment the number of calls encountered
		}
	
		// the process loads the end-of-line values, so swap them with nulls
		int setidx = 0;
		if( !( team_ptrs = (char **)malloc( team_cnt * sizeof(char *) ) ) )
		{
			perror( "Error allocating team call pointer index storage" );
			exit( errno );
		}
	
		team_ptrs[ setidx ] = team_data;
		for( int i = 0; i < filesize; i ++ )
		{
			if( team_data[i] == '\n' )
			{
				team_data[i] = '\0';
				team_ptrs[ setidx++ ] = &team_data[i + 1];
			}
		}
	
		fprintf( stderr, "Load process loaded %d team calls\n", team_cnt );
		fprintf( stderr, "Last pointer loaded %d\n", setidx );
		
//		// dump the calls, just for reference...
//		for( int x = 0; x < team_cnt; x++ )
//		{
//			fprintf( stderr, "%s ", team_ptrs[ x ] );
//		}
//		fprintf( stderr, "\n" ); fflush( stderr );
		
		// done loading input calls...
		fclose( input );
	}
	
	// start outputting the file
	// output the index portion
	size_t bytes = 0;

	bytes = fwrite( index, sizeof(int), INDEXER_SIZE, scp );

	fflush( scp );

	fprintf( stderr, "Wrote index header: %d units, %d bytes\n", bytes, bytes * sizeof(int) ); fflush( stderr );

	int indexidx = 0;
	int charidx1 = 0;
	int charidx2 = 0;
	char prefix[3] = {0}; // a prefix protion of finding claims

	for( charidx1 = 0; charidx1 < 37; charidx1++ )
	{
		prefix[0] = charvals[charidx1];
	
		for( charidx2 = 0; charidx2 < 37; charidx2++ )
		{
			// set the pointer location
			index[ indexidx++ ] = ftell( scp );

			prefix[1] = charvals[charidx2];

			//fprintf( stderr, "Building %s, %d, %d\n", prefix, charidx1, charidx2 );
			//fprintf( stderr, "Building %s\n", prefix ); fflush( stderr );


			// now look for any callsign containing the prefix, or any portion of the 
			// call and output it to the output file pointer
			for( int curcall = 0; curcall < call_cnt; curcall++ )
			{
				//fprintf( stderr, "%s ", call_ptrs[ curcall ] );
				//fflush( stderr );
				if( strstr( call_ptrs[curcall], prefix ) != NULL )
				{
					//fprintf( stderr, "%s: %s\n", prefix, call_ptrs[ curcall ] ); fflush( stderr );

					fwrite( call_ptrs[ curcall ], strlen( call_ptrs[ curcall ] ), 1, scp );
					
					// look for a team affiliation, if it's available
					if( team_ptrs != NULL )
					{
						//fprintf( stderr, "Looking for [%s]\n", call_ptrs[ curcall ] );
						
						//if( bsearch( call_ptrs[ curcall ], team_ptrs, team_cnt, sizeof(char *), (int(*)(const void*, const void*))strcmp) != NULL )
						if( bsearch( call_ptrs[ curcall ], team_ptrs, team_cnt, sizeof(char *), call_compare) != NULL )
						{
							//fprintf( stderr, "Found team member %s\n", call_ptrs[ curcall ] );
							fwrite( team_name, strlen( team_name ), 1, scp );
						}
						else
						{
							//fprintf( stderr, "x" );
						}
					}
					fwrite( "", 1, 1, scp ); // write the NULL byte
				}
			}
			fflush( scp );
		}
	}

	// set the last-est pointer, basically to EOF
	index[ indexidx ] = ftell( scp );

	fseek( scp, 0, SEEK_SET );

	bytes = fwrite( &index[0], sizeof(int), INDEXER_SIZE, scp );

	fflush( scp );

	fprintf( stderr, "\n" );
	fprintf( stderr, "Wrote index header: %d units, %d bytes\n", bytes, bytes * sizeof(int) ); fflush( stderr );

	fclose( scp );





}
