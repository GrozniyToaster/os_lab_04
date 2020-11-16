
#include "settings.h"

    sem_t *Send;
    sem_t *Received;
    void* addr;

void ParentWork(){
    int signals = 0;
    float len;
    float floats[ POSSIBLE_FLOATS ];
    while ( signals == 0 ){
        if  (sem_wait( Received ) == -1){
            perror("Parent: sem_wait");
            exit( errno );
        }
        memcpy( &len, addr, sizeof( float ) );
        if ( len <= 0){
            signals = len;
            break;
        }
        float* data = (float*)addr + 1;
        int strlen = len;
        memcpy( floats, data, sizeof( float ) * strlen );
        for ( int i = 0; i< strlen ; i++){
            if ( floats[i] == INFINITY ){
                printf("Deviation by zero\n");
                return;
            }
            printf("%f ", floats[i]);
        }
        printf("\n");
        sem_post( Send );
    }
    if ( signals == 0 ){
        sem_post( Send );
        return;
    }else if ( signals == -1 ){
        sem_post( Send );
        perror("Wrong format ( one float in string )\n");
        return;
    }else if ( signals == -2 ){
        sem_post( Send );
        printf( "File does not exist\n" );
        return;
    }
}

int main(){

    if ( (Send = sem_open(SendFileNameSem, O_CREAT, S_IWUSR | S_IRUSR, 0) ) == SEM_FAILED ) {
        perror("Before fork sem does not create\n");
        exit( errno );
    }
    if ( (Received = sem_open(ReadFileNameSem, O_CREAT, S_IWUSR | S_IRUSR, 0) ) == SEM_FAILED ) {
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
        
        if ( ftruncate(transportName, BUF_SIZE) == -1){
            perror("Ftruncate error");
            exit ( errno );
        }

        addr = mmap(NULL, NAME_MAX, PROT_WRITE | PROT_READ , MAP_SHARED, transportName, 0);
        if (addr == MAP_FAILED){
            perror("Mmap Parent error");
            exit(errno);
        }

        strcpy( (char*) addr, fileName );
        sem_post( Send );

        if  (sem_wait( Received ) == -1){
            perror("Parent: sem_wait");
            exit( errno );
        }
        
        sem_post( Send );

        ParentWork();
        if (munmap( addr, NAME_MAX ) == -1 ){
            perror("Unmap file_name Parent error");
            exit(errno);
        }
        if ( shm_unlink(SHM_File_Name) == -1 ){
            perror("Unlink file_name Parent error");
            exit(errno);
        }
    }
        sem_close( Send );
        sem_unlink( SendFileNameSem );
        sem_close( Received );
        sem_unlink( ReadFileNameSem );
        exit( SUCCESS );
    
}