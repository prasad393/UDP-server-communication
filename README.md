# UDP-server-communication

# UDP Client-Server Simulation (Docker + Dev Container)

##  Project Summary
This project simulates a UDP-based client-server architecture inside Docker containers or a VS Code Dev Container.
It tests and detects:
- Packet loss
- Out-of-order packets
- Duplicate packets

The sender reads messages from `messages.txt`, while the receiver logs sequence checks with status output.

---

##  Folder Structure
```
udp-project/
├── client/
│   ├── udp_sender.cpp
│   ├── Dockerfile.sender
│
├── server/
│   ├── udp_receiver.cpp
│   ├── Dockerfile.receiver
│
├── messages.txt          # Input file for the sender
├── docker-compose.yml    # Runs both sender and receiver containers
├── README.md             # This file
```

---

##  messages.txt Format
Each line = One message to send  
Format:
```
<SEQ_NUM> | <DATA>
```

Example:
```
0 | A
1 | B
2 | C
4 | D
5 | E
5 | E    # duplicate
7 | F
8 | G
...
```

---

## How to Build & Run (Dev Container or Docker)

### 1. Open in VS Code
Open folder in VS Code and **Reopen in Container** if prompted.

### 2. Build Docker images
```bash
docker-compose build
```

### 3. Run sender and receiver
```bash
docker-compose up
```

---

## Clean Build Files
To remove cache/compiled files inside container:
```bash
rm -rf CMakeFiles CMakeCache.txt Makefile cmake_install.cmake udp_sender udp_receiver
```

---

## Save Logs While Running
Save terminal logs while still printing to screen:
```bash
./udp_receiver | tee receiver_log.txt
./udp_sender | tee sender_log.txt
```

---

## Notes
- Modify `messages.txt` for test cases (loss, duplicate, reordering).
- Receiver stops gracefully after receiving a `STOP` message.
- Sender waits 1 second between messages.