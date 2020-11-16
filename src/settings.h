#pragma once
#define BUF_SIZE 400
#define POSSIBLE_FLOATS 85

const char SHM_File_Name[] = "/file_in_name";
const char SHM_Pipe[] = "/file_pipe";

const char SendFileNameSem[] = "/send_creating_SHM";
const char ReadFileNameSem[] = "/read_creating_SHM";

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/mman.h>
#include <semaphore.h>
#include <sys/stat.h>        
#include <fcntl.h>           

#include <errno.h>
#include <limits.h>
#include <math.h>

#define SUCCESS 0
#define FAIL -1
