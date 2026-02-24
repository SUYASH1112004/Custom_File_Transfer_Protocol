# 🚀 Custom FTP Server & Client in C (Socket Programming)

A Custom File Transfer Protocol (FTP) system developed completely in **C using TCP sockets on Linux**.

This project demonstrates low-level networking, process management, and file handling in Unix-based systems.

---

## 📌 Project Overview

This project consists of:

- 🖥️ **Server Application**
- 💻 **Client Application**

The system allows a client to request a file from the server and download it using a structured protocol (Header + Data model).

---

## 🖥️ Server Application Features

- Creates a TCP socket using `socket()`
- Binds to a custom port using `bind()`
- Listens for client connections using `listen()`
- Accepts incoming connections via `accept()`
- Uses `fork()` for handling multiple clients concurrently
- Sends:
  - A structured header → `Ok <filesize>`
  - File data in chunks (1024 bytes)

---

## 💻 Client Application Features

- Connects to server using `connect()`
- Sends requested filename
- Reads header using custom `ReadLine()` function
- Extracts file size using `sscanf()`
- Downloads file in chunks
- Creates a new file locally with received content

---

## 🧠 Key Concepts Implemented

- ✅ TCP Socket Programming (`AF_INET`, `SOCK_STREAM`)
- ✅ Multi-client handling using `fork()`
- ✅ File handling (`open`, `read`, `write`, `stat`)
- ✅ Structured protocol design (Header + Data model)
- ✅ Chunk-based file transfer
- ✅ Error handling for invalid files
- ✅ IP conversion using `inet_pton()`
- ✅ Network byte order conversion using `htons()`

---

## ⚙️ How to Run

### 1️⃣ Compile

```bash
gcc server.c -o server
gcc client.c -o client
```

2️⃣ Run Server
./server 9000
3️⃣ Run Client
./client 127.0.0.1 9000 Demo.txt Downloaded.txt
🔄 Working Flow

Client connects to server

Client sends filename

Server checks file availability

Server sends header → Ok <filesize>

Server streams file data

Client reconstructs file

Download completes successfully ✅

---
## 📚 What I Learned

How FTP-like systems work internally

Control information vs data stream handling

Handling partial reads and writes

Process-based concurrency using fork()

Low-level Linux networking concepts

🚀 Future Enhancements

🔐 Authentication system

📂 Directory listing support

⬆️ File upload feature

🧵 Thread-based concurrency

🔒 SSL/TLS encryption

---

## 🛠️ Technologies Used

C Programming

Linux System Calls

TCP/IP Networking

Socket Programming

Process Management (fork())

---
## 📌 Author

Suyash Patil




---

