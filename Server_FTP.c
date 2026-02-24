//Server Application

#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdbool.h>

////////////////////////////////////////////////////////////////////////////
//
//  Name        :     SendFileToClient
//  Description :     Send Data To Client
//  Input       :     Fd_Of_Client , FileName
//  Output      :     Nothing
//
////////////////////////////////////////////////////////////////////////////


void SendFileToClient(int ClientSocket,char *FileName)
{
    int fd = 0;
    struct stat sobj;
    char Buffer[1024];
    int ByteRead = 0;
    char Header[64]={'\0'};

    printf("FileName is : %s : %lu\n",FileName,strlen(FileName));

    fd = open(FileName,O_RDONLY);

    //Unable to open file
    if(fd < 0)
    {
        printf("Unable to open file \n");

        //Send error message to client
        write(ClientSocket,"ERR\n",4);

        return;
    }

    stat(FileName,&sobj);

    // Header : "ok 1700"
    snprintf(Header,sizeof(Header),"Ok %ld\n",(long)sobj.st_size);

    //Send Header to client
    write(ClientSocket,Header,strlen(Header));

    //Send actual file contents

    while((ByteRead = read(fd,Buffer,sizeof(Buffer)))>0)
    {

        //Send data to client
        write(ClientSocket,Buffer,ByteRead);
    }

    close(fd);
}

//////////////////////////////////////////////////////////////////////
//
//  Command Line Argument Application
//  1st Argument : Port Number
//  ./server    9000
//  argv[0]     argv[1]
//
//////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    int ServerSocket = 0;
    int ClientSocket = 0;
    int port = 0;
    int iRet = 0;
    pid_t pid = 0;

    char FileName [50] = {'\0'};

    struct sockaddr_in ServerAddr;
    struct sockaddr_in ClientAddr;

    socklen_t AddrLen = sizeof(ClientAddr);

    if(argc != 2)
    {
        printf("Unabel to processed as invalid number of arguments\n");
        printf("Please provide the port number\n");
        
        return -1;
    }


    //Port Number of server
    port = atoi(argv[1]);

    //////////////////////////////////////////////////////////////////////
    //  Step 1 : Create Tcp Socket
    //////////////////////////////////////////////////////////////////////

    ServerSocket = socket(AF_INET,SOCK_STREAM,0);

    if(ServerSocket<0)
    {
        printf("Unable to create server socket\n");
        return -1;
    }

    //////////////////////////////////////////////////////////////////////
    //  Step 2 : Bind socket to IP and Port
    //////////////////////////////////////////////////////////////////////

    memset(&ServerAddr,0,sizeof(ServerAddr));

    //Initialize the structure 
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port =  htons(port);
    ServerAddr.sin_addr.s_addr = INADDR_ANY;

    iRet = bind(ServerSocket,(struct sockaddr *)&ServerAddr,sizeof(ServerAddr));

    if(iRet == -1)
    {
        printf("Unable to bind\n");
        close(ServerSocket);
        return -1;
    }

    //////////////////////////////////////////////////////////////////////
    //  Step 3 : Listen for client connection
    //////////////////////////////////////////////////////////////////////

    iRet = listen(ServerSocket,11);

    if(iRet == -1)
    {
        printf("Server unable to listen the request\n");

        close(ServerSocket);

        return -1;
    }

    printf("Server is running on port : %d\n",port);

    //////////////////////////////////////////////////////////////////////
    //  Loop which accept client request Continuously
    //////////////////////////////////////////////////////////////////////

    while(1)
    {

        //////////////////////////////////////////////////////////////////////
        //  Step 4 : Accept the client request
        //////////////////////////////////////////////////////////////////////

        memset(&ClientAddr,0,sizeof(ClientAddr));

        printf("Server is waiting for client request \n");

        ClientSocket=accept(ServerSocket,(struct sockaddr *)&ClientAddr,&AddrLen);

        if(ClientSocket < 0)
        {
            printf("Unable to accept client request\n");
            continue;   // Used for while

        }

        printf("Client gets connected : %s \n",inet_ntoa(ClientAddr.sin_addr));

        //////////////////////////////////////////////////////////////////////
        //  Step 5 : Create new process to handle client request
        //////////////////////////////////////////////////////////////////////

        pid = fork();

        if(pid < 0)
        {
            printf("Unable to create a new client \n");
            close(ClientSocket);
            continue;
        }

        //New process gets created for client
        if(pid == 0)
        {
            printf("New process gets created for client request\n");
            close(ServerSocket);

            iRet = read(ClientSocket,FileName,sizeof(FileName));

            printf("Requested File by client : %s \n",FileName);

            FileName[strcspn(FileName,"\r\n")] = '\0';

            SendFileToClient(ClientSocket,FileName);

            close(ClientSocket);
            
            printf("File Transfer done & client disconnected \n");

            exit(0);    // kill the child process
        
        }// End Of If (fork)

        else    //Parent Process (Server)
        {
            close(ClientSocket);
        }   //End of else

        

    }//End of while
    close(ServerSocket);
    return 0;
    //End of main
}