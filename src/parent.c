#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/mman.h>
#include <sys/stat.h>        
#include <fcntl.h>           
#include <limits.h>

#include "settings.h"

#define SUCCESS 0
#define FAIL -1
#define BUF_SIZE 256
#define MAX_STRING  256
#define NUMERAL_SYSTEM 10






int ChildWork(){
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

void ParentWork( int childFD ){
    float toPrint;
    char pr[BUF_SIZE];
    while( read( childFD, &toPrint, sizeof( float ) ) > 0 ){
        sprintf( pr,"Received from child %4.4f \n", toPrint );
        write( STDOUT_FILENO, pr , strlen( pr ) * sizeof( char ) );
    }
}

int main(){
    

    int id = fork();
    if ( id < 0 ){
        perror("Fork error");
		return FAIL;

    }else if( id == 0){ // Программа ребенка
        
        execv(  );



        exit( SUCCESS );

    }else{ //Программа родителя
        char fileName[ NAME_MAX ];
        scanf( "%s", fileName );


        fflush(stdout);    // и сбрасываем буффер вывода

        int transportName = shm_open( SHM_File_Name, O_WRONLY | O_CREAT | O_ACCMODE, S_IWUSR | S_IRUSR );
        if ( transportName == -1  ){
            perror("Cant create File_for_transport_Name\n");
            exit( FAIL );
        }
        

        char* addr = mmap(NULL, NAME_MAX, PROT_WRITE, MAP_SHARED, transportName, 0);
        if (addr == MAP_FAILED){
            perror("Mmap Parent error");
            exit(FAIL);
        }

        strcpy( addr, fileName );



        ParentWork(  );
        

        exit( SUCCESS );
    }
}