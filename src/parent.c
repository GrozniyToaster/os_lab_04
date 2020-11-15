#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/mman.h>
#include <semaphore.h>
#include <sys/stat.h>        
#include <fcntl.h>           

#include "settings.h"
#include <errno.h>
#include <limits.h>

#define SUCCESS 0
#define FAIL -1
#define BUF_SIZE 256
#define MAX_STRING  256
#define NUMERAL_SYSTEM 10







/*
void ParentWork( int childFD ){
    float toPrint;
    char pr[BUF_SIZE];
    while( read( childFD, &toPrint, sizeof( float ) ) > 0 ){
        sprintf( pr,"Received from child %4.4f \n", toPrint );
        write( STDOUT_FILENO, pr , strlen( pr ) * sizeof( char ) );
    }
}
*/
int main(){

    sem_t *FileNameReadyToConnect;
    if ( (FileNameReadyToConnect = sem_open(SendFileNameSem, O_CREAT, S_IWUSR | S_IRUSR, 0) ) == SEM_FAILED ) {
        perror("Before fork sem does not create\n");
        exit( errno );
    }
    sem_t *FileNameConnected;
    if ( (FileNameConnected = sem_open(ReadFileNameSem, O_CREAT, S_IWUSR | S_IRUSR, 0) ) == SEM_FAILED ) {
        perror("Before fork sem does not create\n");
        exit( errno );
    }
    int id = fork();
    if ( id < 0 ){
        perror("Fork error");
		exit( errno );

    }else if( id == 0){ // Программа ребенка
        
        if ( execv( "./os_lab4_child", NULL ) == -1 ){
            exit( errno );
        }

    }else{ //Программа родителя

        char fileName[ NAME_MAX ];
        scanf( "%s", fileName );

        int transportName = shm_open( SHM_File_Name, O_RDWR | O_CREAT , S_IWUSR | S_IRUSR );
        if ( transportName == -1  ){
            perror("Cant create File_for_transport_Name\n");
            exit( errno );
        }
        
        
        if ( ftruncate(transportName, NAME_MAX) == -1){
            perror("Ftruncate error");
            exit ( errno );
        }

        char* addr = mmap(NULL, NAME_MAX, PROT_WRITE, MAP_SHARED, transportName, 0);
        if (addr == MAP_FAILED){
            perror("Mmap Parent error");
            exit(errno);
        }
        fflush(stdout);    // и сбрасываем буффер вывода
        strcpy( addr, fileName );
        sem_post( FileNameReadyToConnect );
        if  (sem_wait( FileNameConnected ) == -1){
            perror("Parent: sem_wait");
            exit( errno );
        }
        if (munmap( addr, NAME_MAX ) == -1 ){
            perror("Unmap file_name Parent error");
            exit(errno);
        }
        if ( shm_unlink(SHM_File_Name) == -1 ){
            perror("Unlink file_name Parent error");
            exit(errno);
        }
        exit( SUCCESS );
    }
    
}