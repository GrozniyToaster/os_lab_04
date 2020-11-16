
#include "settings.h"

    void* addr;
    sem_t* Send;
    sem_t* Receive;

void writeInFile( long len, float* mas ){
    
    float count = len ;
    float* ad = (float*) addr + 1;
    if ( len < 0 ){
        memcpy( (float*) addr, &( count ), sizeof( float ) );
        sem_post( Receive );
        return;
    }
    if ( sem_wait( Send ) == -1 ){
        perror( "Wait Receive in Child Write\n" );
        exit( errno );
    }
    memcpy( (float*) addr, &( count ), sizeof( float ) );
    if ( len != 0 ){
        memcpy( ad, (void*) mas, sizeof( float ) * len );
    }
    sem_post( Receive );
}

int ChildWork(){
    float floats[ POSSIBLE_FLOATS ];
    char* string = NULL;
    size_t len;
    
    while( getline( &string, &len, stdin ) > 0 ){
        char* currTok = strtok( string, " ");
        if ( *currTok == '\n'  ){
            continue;
        } 
        if ( currTok == NULL ){// exep
            printf("Child eof\n");
            writeInFile( 0, NULL );
            break;
        }
        float piv = strtof( currTok, NULL );
        int TokCounter = 0;
        while( currTok != NULL ){
            currTok = strtok( NULL, " ");
            if ( currTok == NULL ){
                break;
            }
            floats[ TokCounter ] = strtof( currTok, NULL );
            TokCounter++;
        }
        if ( TokCounter == 0 ){ // exep
            writeInFile( -1, NULL );
            free( string );
            return FAIL;
        }
        for ( int i = 0; i < TokCounter; i++ ){
            if ( floats[i] == 0 ){
                floats[i] = INFINITY;
                writeInFile( i + 1, floats );
                free( string );
                return( FAIL );
            }
            floats[i] = piv / floats[i];
        }
        writeInFile( TokCounter, floats );

    }
    writeInFile( 0, NULL );
    free( string );
    return SUCCESS;
}

int main(){


    if ( ( Send = sem_open(SendFileNameSem, 0)) == SEM_FAILED ) { // семафоры
        perror("Child: sem_open\n");
        exit (errno);
    }
    if ( ( Receive = sem_open(ReadFileNameSem, 0)) == SEM_FAILED ) {
        perror("Child: sem_open\n");
        exit (errno);
    }

    if ( sem_wait( Send ) == -1 ){ // ждем создания файла ждя имени
        perror("Child: sem_wait\n");
        exit( errno );
    }
    int transportName = shm_open( SHM_File_Name, O_RDWR, S_IWUSR | S_IRUSR ); // коннектимся 
    if ( transportName == -1  ){
        perror("Child: Cant open File_for_transport_Name\n");
        exit( errno );
    }
    
    addr = mmap(NULL, BUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, transportName, 0);
    if (addr == MAP_FAILED){
        perror("Mmap Parent error\n");
        exit( errno );
    }

    char fileName[ NAME_MAX ];
    strcpy( fileName, (char*) addr );
    sem_post( Receive );
    int input = open( fileName, O_RDONLY );
    if ( input == -1 ){
        perror("Child: Cant open input file\n");
        writeInFile( -2, NULL );
        exit( errno );
    }

    dup2( input, STDIN_FILENO );

    if ( ChildWork() < 0 ){
        exit( FAIL );
    }
    exit( SUCCESS );
}