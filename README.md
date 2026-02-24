🚀 Built a Custom FTP Server & Client in C using Socket Programming

Excited to share my recent project — a Custom File Transfer Protocol (FTP) system developed completely in C using TCP sockets on Linux.

This project helped me deeply understand low-level networking, process management, and file handling in Unix systems.

🔹 What I Built
🖥️ Server Application

Creates a TCP socket using socket()

Binds to a custom port using bind()

Listens for multiple clients using listen()

Accepts connections via accept()

Uses fork() to handle multiple clients concurrently

Sends:

A structured header → Ok <filesize>

Then streams file content in chunks (1024 bytes)

💻 Client Application

Connects to server using connect()

Sends requested filename

Reads structured header using custom ReadLine() function

Extracts file size using sscanf()

Downloads file safely in chunks

Creates a new file locally with received content

🔹 Key Concepts Implemented

✅ TCP Socket Programming (AF_INET, SOCK_STREAM)
✅ Multi-client handling using fork()
✅ File handling (open, read, write, stat)
✅ Structured protocol design (Header + Data model)
✅ Chunk-based file transfer
✅ Error handling for invalid files
✅ IP conversion using inet_pton()
✅ Network byte order using htons()

🔹 How It Works

Client Command:

./client 127.0.0.1 9000 Demo.txt Downloaded.txt

Server Command:

./server 9000

Flow:

Client connects

Sends filename

Server checks file

Server sends header → Ok 1700

File data transferred

Client reconstructs file

Download completed ✅

🔹 What I Learned

How real FTP-like systems work internally

Difference between control information (header) and data stream

Importance of handling partial reads/writes

Process-based concurrency

Low-level Linux networking internals

This project strengthened my understanding of:

Operating Systems

Computer Networks

System Programming

Linux Internals

Next Step:
Planning to extend this with:

Authentication system

Directory listing support

File upload feature

Thread-based concurrency

Encryption (SSL/TLS)
