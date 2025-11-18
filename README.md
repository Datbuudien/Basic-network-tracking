# 💻 C++ LAN USER ACTIVITY MONITORING PROJECT

## 🌟 Introduction

This project is a study and practical application of **Network Programming** using C++ TCP Sockets. The primary goal is to establish a robust Client-Server system for monitoring basic application activity on a Client machine within the same Local Area Network (LAN).

An auxiliary purpose is to provide a simple utility tool for guardians or parents to monitor their ward's computer usage (e.g., active web browser tab titles, newly opened applications).

---

## ✨ Key Features

| Feature | Detailed Description |
| :--- | :--- |
| **Real-time Activity Tracking** | Uses Windows API (`GetForegroundWindow`) to accurately track active applications and tab switching, not just process creation. |
| **Server-Side Timestamping** | The Server automatically adds a precise timestamp (`YYYY-MM-DD HH:MM:SS`) upon data receipt and logs it to **`client_log.txt`**. |
| **Content Monitoring** | Logs the full window title (e.g., "YouTube - Google Chrome") rather than just the executable name. |
| **Broad Compatibility** | The `client.exe` is compiled using **Static Linking** (`-static`) allowing it to run on any standard Windows machine **without requiring** additional C++ runtime dependencies (DLLs). |
| **Process Filtering** | Automatically filters out common system noise processes (e.g., `cmd.exe`, `rundll32.exe`, etc.) from the log. |

---

## 🛠️ System Requirements and Building

### Requirements

* **Operating System:** Windows (Client and Server).
* **Network:** Both machines must be connected to the same Wi-Fi/LAN.
* **Compiler:** G++ (MinGW) or Visual Studio.

### Build Instructions (For Developers)

To ensure the final executable runs on a clean machine without runtime errors, **Static Linking is mandatory**.

| File             | Compilation Command                                |
| :--------------- | :------------------------------------------------- |
| **`server.exe`** | `g++ server.cpp -o server.exe -lws2_32`            |
| **`client.exe`** | `g++ client.cpp -o client.exe -lws2_32 -static`    |

---

## 👨‍👩‍👧‍👦 USAGE GUIDE (For Parents/Guardians)

The system is designed with a two-part structure: **The Server Machine (Guardian) listens for messages from the Client Machine (Ward).**

### Step 1: File Preparation

1.  **Server Machine (Guardian):** Keep the **`server.exe`** executable.
2.  **Client Machine (Ward):** Copy the **`client.exe`** executable to this machine.

### Step 2: Start the Server (Guardian Machine)

1.  Execute **`server.exe`**.
2.  **CRITICAL:** If the Windows Firewall prompts an alert, you **must** select **Allow Access** for the program to receive connections.
3.  The console screen will display the waiting status:

   ![IMAGE: SERVER CONSOLE WAITING FOR CONNECTION](Assets/Server.png)

### Step 3: Run the Client (Ward's Machine)

1.  Execute **`client.exe`** on the ward's machine.
2.  This file is designed to **run silently in the background** and will not display any working window or console output upon successful connection.

### Step 4: Monitoring and Log Review

1.  Return to the **Server Machine**.
2.  As soon as the Client machine opens an application or switches tabs, the log will appear instantly on the Server Console:

   ![IMAGE: SERVER CONSOLE RECEIVING TIMESTAMPS LOG](Assets/Client.png)

3.  All received data is saved to the **`client_log.txt`** file (located in the same directory as `server.exe`), preserving historical activity:

    ![IMAGE: CONTENT OF client_log.txt FILE WITH TIMESTAMPS](Assets/Res.png)

---

## 🔒 Security and Notes

* This project only functions within the same Local Area Network (LAN/Wi-Fi).
* The behavior of `client.exe` (monitoring foreground activity and sending data over the network) is similar to spyware. Consequently, **Windows Defender** or third-party antivirus software may **quarantine or delete the file**. If this happens, you must add the file to the security exception list.
* Data is transmitted in **plain text** (unencrypted).
