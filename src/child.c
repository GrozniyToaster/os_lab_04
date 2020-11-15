#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/mman.h>
#include <sys/stat.h> 
#include <semaphore.h>       
#include <fcntl.h>           
#include <limits.h>
#include <errno.h>

#include "settings.h"

#define SUCCESS 0
#define FAIL -1
#define BUF_SIZE 256
#define MAX_STRING  256
#define NUMERAL_SYSTEM 10






/*int ChildWork(){
    float floats[3];
    char command[BUF_SIZE];
    while( ReadString( STDIN_FILENO, command ) > 0 ){
        if ( StrToFloatTok( command, floats, 3 ) < 3 ){
            perror("Nonvalid command");
            perror(command);
            return FAIL;
        }
        for( int i = 1; i < 3; ++i ){
            float res = floats[0] / floats[i];
            if ( isinff( res ) ){
                perror("Division by zero");
                return FAIL;
            }
            write( STDOUT_FILENO, &res, sizeof( float )  );
        }
    }
    return SUCCESS;
}
*/

int main(){

    sem_t* CreationFSM;
    if ( ( CreationFSM = sem_open(SendFileNameSem, 0)) == SEM_FAILED ) {
            perror("Child: sem_open");
            exit (errno);
    }
    sem_t* ReceiveFSM;
    if ( ( ReceiveFSM = sem_open(ReadFileNameSem, 0)) == SEM_FAILED ) {
            perror("Child: sem_open");
            exit (errno);
    }
    printf( "Child alive\n" );

    if ( sem_wait( CreationFSM ) == -1 ){
        perror("Child: sem_wait");
        exit( errno );
    }
    int transportName = shm_open( SHM_File_Name, O_RDONLY , S_IWUSR | S_IRUSR );
    if ( transportName == -1  ){
        perror("Child: Cant open File_for_transport_Name\n");
        exit( errno );
    }
    
    char* addr = mmap(NULL, NAME_MAX, PROT_READ, MAP_SHARED, transportName, 0);
    if (addr == MAP_FAILED){
        perror("Mmap Parent error");
        exit( errno );
    }
    
    sem_post( ReceiveFSM );

    char fileName[ NAME_MAX ];
    strcpy( fileName, addr );
    
    printf( "Child: %s", fileName );
}