//Client Application

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
//  Name        :     ReadLine
//  Description :     Reads the Header send by server
//  Input       :     Fd , Header_Received_By_Server, bytes_to_read
//  Output      :     characters read from file
//
////////////////////////////////////////////////////////////////////////////

int ReadLine(int Sock, char *Line,int max)
{
    int i = 0;
    char ch = '\0';
    int n = 0;

    while(i < max - 1)
    {
        n = read(Sock,&ch,1);
        if(n<=0)
        {
            break;
        }
        Line[i++]=ch;
        if(ch == '\n')
        {
            break;
        }
    }//End of while

    Line[i]='\0';

    return i;

}//End of ReadLine;

//////////////////////////////////////////////////////////////////////
//
//  Command Line Argument Application
//  1st Argument : IP Address
//  2nd Argument : Port Number
//  3rd Argument : Target File Name 
//  4th Argument : New File Name
// 
//  ./client    127.0.0.1   9000      Demo.txt    A.txt
//  argv[0]     argv[1]     argv[2]   argv[3]     argv[4]
//
//////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{

    int port = 0 ;                  // argv[2]
    int Sock = 0;
    int iRet = 0;

    char * ip = NULL;               // argv[1]
    char * Filename = NULL;         // argv[3]
    char *OutFilename = NULL;       // argv[4]

    char Header [64] = {'\0'};

    struct sockaddr_in ServerAddr;

    if(argc != 5)
    {
        printf("Unable to proceed as invalid number of arguments\n");

        printf("Please provide below arguments : \n");

        printf("1st Argument : IP Address\n");
        printf("2nd Argument : Port number\n");
        printf("3rd Argument : Target file name\n");
        printf("4th Argument : New file name\n");

        return -1;
    }

    // Store Command Line Arguments into variables
    ip = argv[1];
    port = atoi(argv[2]);
    Filename = argv[3];
    OutFilename = argv[4];
    
    ///////////////////////////////////////////////////////////////////////////////////
    // Step 1 : Create TCP Socket
    ///////////////////////////////////////////////////////////////////////////////////

    Sock = socket(AF_INET,SOCK_STREAM,0);

    if(Sock<0)
    {
        printf("Unable to create client socket \n");
        return -1;
    }

    ///////////////////////////////////////////////////////////////////////////////////
    // Step 2 : Connect with Server
    ///////////////////////////////////////////////////////////////////////////////////

    memset(&ServerAddr,0,sizeof(ServerAddr));

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(port);

    // Convert the IP Address into binary format

    inet_pton(AF_INET,ip,&ServerAddr.sin_addr);

    iRet = connect(Sock,(struct sockaddr *)&ServerAddr,sizeof(ServerAddr));

    if(iRet == -1)
    {
        printf("Unable to connect with server\n");

        close(Sock);

        return -1;
    }

    ///////////////////////////////////////////////////////////////////////////////////
    // Step 3 : Sending FileName to server
    ///////////////////////////////////////////////////////////////////////////////////
    
    write(Sock,Filename,strlen(Filename));
    write(Sock,"\n",1);

    ///////////////////////////////////////////////////////////////////////////////////
    // Step 4 : Read the header
    ///////////////////////////////////////////////////////////////////////////////////

    iRet = ReadLine(Sock, Header,sizeof(Header));

    if(iRet <= 0)
    {
        printf("Server gets disconnected abnormally \n");
        close(Sock);
        return -1;
    }

    long FileSize = 0;
    printf("Header is %s \n",Header);
    sscanf(Header,"Ok %ld",&FileSize);

    ///////////////////////////////////////////////////////////////////////////////////
    // Step 5 : Create new file
    ///////////////////////////////////////////////////////////////////////////////////

    int outfd = 0;

    outfd = open(OutFilename,O_CREAT | O_WRONLY | O_TRUNC , 0777);

    if(outfd < 0)
    {
        printf("Unable to create downloaded file\n");
        return -1;
    }

    char Buffer[1024] = {'\0'};
    long received = 0;
    long remaining = 0;
    int n = 0;
    int toRead = 0;


    while(received < FileSize)
    {
        remaining = FileSize - received;

        if(remaining > 1024)
        {
            toRead = 1024;
        }
        else
        {
            toRead = remaining;
        }

        n = read(Sock,Buffer,toRead);

        write(outfd,Buffer,n);

        received = received + n;
    
    } // End Of While

    close(outfd);
    close(Sock);

    if(received == FileSize)
    {
        printf("Download Complete ...\n");
        return 0;
    }

    else
    {
        printf("Download Failed ...\n");
        return -1;
    }

    return 0;
    
}//End of main