#include "../include/ftp_client.hh"

int connect_flag = 0;         // Flag to indicate connection status (0: disconnected, 1: connected)
extern char ip[MAXLINE], port[MAXLINE]; // IP and Port for connection, defined in another file

int main(int argc, char* argv[]) {
    string cmdline;           // String to store the command line input
    
    /* Main command loop */
    while (true) {
        /* Display prompt based on connection status */
        if (!connect_flag)
            cout << "MyFTP(None)> ";
        else
            cout << "MyFTP(" << ip << ":" << port << ")> ";
        
        /* Read command line input from the user */
        getline(cin, cmdline);
        
        /* If command line input is empty, continue to the next iteration */
        if (cmdline.empty())
            continue;
        
        /* Process the command line input */
        eval(cmdline);
    }
    exit(0);
}
