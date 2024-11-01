#include <gtest/gtest.h>
#include <string>
#include <filesystem>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <fstream>
#include <cstdlib>
#include <fcntl.h>
#include <sys/wait.h>

pid_t startSubProcess(int *writefd, std::string exe, std::vector<std::string> &&args, std::filesystem::path &working_directory, int need_kill=1) {
    std::filesystem::remove_all(working_directory);
    std::filesystem::create_directory(working_directory);
    int write_fds[2];
    if (writefd)
        if(pipe(write_fds) != 0)
            return -1;
    pid_t status = fork();
    if (status == -1)
        return -2;
    if (status == 0) {
        // child process
        chdir(working_directory.c_str());
        int empty_fd = open("/dev/null", O_RDWR);
        if (writefd) {
            dup2(write_fds[0], STDIN_FILENO);
            close(write_fds[0]);
            close(write_fds[1]);
        } else {
            dup2(empty_fd, STDIN_FILENO);
        }
        dup2(empty_fd, STDOUT_FILENO);
        dup2(empty_fd, STDERR_FILENO);
        close(empty_fd);
        const char** _args = new const char*[args.size() + 1];
        for (int i = 0 ; i < args.size(); i ++) {
            _args[i] = args[i].c_str();
        }
        _args[args.size()] = nullptr;
        if (need_kill) {
            system(("killall " + exe).c_str());
        }
        if (execv(exe.c_str(), (char* const*)_args) == -1) {
            std::cerr << "GTest: Failed to start subprocess" << std::endl;
            exit(-1);
        }
    } else {
        if (writefd) {
            *writefd = write_fds[1];
            close(write_fds[0]);
        }
        usleep(500000); // Sleep 10 ms till STDIN/OUT_FILENO Correct
        pid_t child_pid = waitpid(status, NULL, WNOHANG);
        EXPECT_EQ(child_pid, 0);
    }
    return status;
}

/**
 * @brief Wait process to exit and return the retval or -1
 * 
 * @param pid 
 * @return int -1 when process not exit
 */
int waitProcessExit(pid_t pid) {
    usleep(500000);
    int status;
    auto retval = waitpid(pid, &status, WNOHANG);
    //std::cerr << "retval: " <<retval << " pid:" << pid << " " << WEXITSTATUS(status) << " " << WIFSTOPPED(status) << std::endl;
    if(retval != pid) {
        return -1;
    }
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    if (WIFSTOPPED(status))
        std::cerr << "Signal\n";
    return -1;
}

void clearProcess(pid_t pid) {
    kill(pid, SIGKILL);
    usleep(500000);
}

static pthread_once_t once_init = PTHREAD_ONCE_INIT;
static int last_port;
void initRandom() {
    std::ifstream fin("last_port", std::ios::in);
    if (fin) {
        fin >> last_port;
        fin.close();
    } else {
        last_port = 30000;
    }
}

int randPort() {
    pthread_once(&once_init, initRandom);
    last_port = rand() % 100 + last_port + 1;

    std::ofstream fout("last_port", std::ios::out);
    fout << last_port;
    fout.close();

    return last_port;
}

static std::filesystem::path current_dir;
static std::filesystem::path tmp_dir_ser;
static std::filesystem::path tmp_dir_cli;
static std::filesystem::path tmp_dir_clis[16];

int prepare(int& client_fd, int& server_port, pid_t& server_pid, pid_t& client_pid, int test_id, int std_client) {
    current_dir = std::filesystem::current_path();
    tmp_dir_ser = current_dir / "tmp_dir_server";
    tmp_dir_cli = current_dir / "tmp_dir_client";

    client_fd = 0;
    server_port = randPort();
    
    if (!std_client)
        server_pid = startSubProcess(nullptr, current_dir / "ftp_server_std", {"", "127.0.0.1", std::to_string(server_port), std::to_string(test_id)}, tmp_dir_ser);
    else
        server_pid = startSubProcess(nullptr, current_dir / "ftp_server", {"", "127.0.0.1", std::to_string(server_port)}, tmp_dir_ser);
    EXPECT_GE(server_pid, 0);

    if (std_client)
        client_pid = startSubProcess(&client_fd, current_dir / "ftp_client_std", {"", std::to_string(test_id)}, tmp_dir_cli);
    else
        client_pid = startSubProcess(&client_fd, current_dir / "ftp_client", {""}, tmp_dir_cli);
    EXPECT_GE(client_pid, 0);

    if (server_pid <= 0 || client_pid <= 0) {
        clearProcess(server_pid);
        clearProcess(client_pid);
        return -1;
    }
    
    usleep(500000);

    return 0;
}

int prepareMulti(int* client_fd, int& server_port, pid_t& server_pid, pid_t* client_pid, int test_id, int std_client) {
    current_dir = std::filesystem::current_path();
    tmp_dir_ser = current_dir / "tmp_dir_server";
    
    for (int i = 0; i < 16; i ++)
        tmp_dir_clis[i] = current_dir / ("tmp_dir_client" + std::to_string(i));

    server_port = randPort();
    
    if (!std_client)
        server_pid = startSubProcess(nullptr, current_dir / "ftp_server_std", {"", "127.0.0.1", std::to_string(server_port), std::to_string(test_id)}, tmp_dir_ser);
    else
        server_pid = startSubProcess(nullptr, current_dir / "ftp_server", {"", "127.0.0.1", std::to_string(server_port)}, tmp_dir_ser);
    EXPECT_GE(server_pid, 0);

    for (int i = 0; i < 16; i ++) {
        client_pid[i] = 0;
    }
    for (int i = 0; i < 16; i ++) {
        client_fd[i] = 0;
        if (std_client)
            client_pid[i] = startSubProcess(&client_fd[i], current_dir / "ftp_client_std", {"", std::to_string(test_id)}, tmp_dir_clis[i], i == 0);
        else
            client_pid[i] = startSubProcess(&client_fd[i], current_dir / "ftp_client", {""}, tmp_dir_clis[i], i == 0);
        EXPECT_GE(client_pid[i], 0);
    }
    

    if (server_pid <= 0 || client_pid[0] <= 0 || client_pid[1] <= 0 || client_pid[2] <= 0 || client_pid[3] <= 0) {
        clearProcess(server_pid);
        for (int i = 0; i < 16; i ++)
            clearProcess(client_pid[i]);
        return -1;
    }
    
    usleep(500000);

    return 0;
}

TEST(FTPServer, Open) {
    pid_t server_pid, client_pid;
    int server_port, client_fd;
    std::string cmd_str;

    if (prepare(client_fd, server_port, server_pid, client_pid, 1, 1) != 0)
        return ;
    
    cmd_str = "open 127.0.0.1 " + std::to_string(server_port) + "\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(500000);

    cmd_str = "quit\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(500000);
    cmd_str = "quit\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(500000);
    

    //int code = waitProcessExit(client_pid);
    //EXPECT_EQ(code, 1);
    FILE* fin = fopen((tmp_dir_cli / ".client_open_ok").string().c_str(), "r");
    EXPECT_NE(fin, nullptr);
    fclose(fin);

    clearProcess(client_pid);
    clearProcess(server_pid);
}

TEST(FTPServer, Get) {
    pid_t server_pid, client_pid;
    int server_port, client_fd;
    std::string cmd_str;

    if (prepare(client_fd, server_port, server_pid, client_pid, 3, 1) != 0)
        return ;

    cmd_str = "open 127.0.0.1 " + std::to_string(server_port) + "\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(500000);

    /** Generate Content **/
    uint32_t random_name = rand() % 1000;
    uint32_t random_content = rand() % 1000;
    std::ofstream fout((tmp_dir_ser / (std::to_string(random_name) + ".txt")).string(), std::ios::out);
    fout << random_content;
    fout.close();
    usleep(500000);
    /** Generate Content **/

    cmd_str = "get " + std::to_string(random_name) + ".txt\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(1000000);

    std::ifstream fin((tmp_dir_cli / (std::to_string(random_name) + ".txt")).string(), std::ios::in);
    uint32_t get_content;
    if (fin) {
        fin >> get_content;
        fin.close();
    } else get_content = ~0;
    EXPECT_EQ(get_content, random_content);

    clearProcess(client_pid);
    clearProcess(server_pid);
}

TEST(FTPServer, Put) {
    pid_t server_pid, client_pid;
    int server_port, client_fd;
    std::string cmd_str;

    if (prepare(client_fd, server_port, server_pid, client_pid, 4, 1) != 0)
        return ;

    cmd_str = "open 127.0.0.1 " + std::to_string(server_port) + "\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(500000);

    /** Generate Content **/
    uint32_t random_name = rand() % 1000;
    uint32_t random_content = rand() % 1000;
    std::ofstream fout((tmp_dir_cli / (std::to_string(random_name) + ".txt")).string(), std::ios::out);
    fout << random_content;
    fout.close();
    usleep(500000);
    /** Generate Content **/

    cmd_str = "put " + std::to_string(random_name) + ".txt\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(1000000);

    std::ifstream fin((tmp_dir_ser / (std::to_string(random_name) + ".txt")).string(), std::ios::in);
    uint32_t get_content;
    if (fin) {
        fin >> get_content;
        fin.close();
    } else EXPECT_EQ(0, 1);
    EXPECT_EQ(get_content, random_content);

    clearProcess(client_pid);
    clearProcess(server_pid);
}

TEST(FTPServer, SHA256) {
    pid_t server_pid, client_pid;
    int server_port, client_fd;
    std::string cmd_str;

    if (prepare(client_fd, server_port, server_pid, client_pid, 5, 1) != 0)
        return ;

    cmd_str = "open 127.0.0.1 " + std::to_string(server_port) + "\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(500000);

    /** Generate Content **/
    uint32_t random_name = rand() % 1000;
    uint32_t random_content = rand() % 10;
    std::ofstream fout((tmp_dir_ser / (std::to_string(random_name) + ".txt")).string(), std::ios::out);
    for (int i = 0; i < 1000000; i ++)
        fout << (random_content + i) % 10;
    fout.close();
    usleep(500000);
    /** Generate Content **/

    cmd_str = "sha256 " + std::to_string(random_name) + ".txt\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(1000000);

    //int exitCode = waitProcessExit(client_pid);
    //EXPECT_EQ(exitCode, 5);

    FILE* fin = fopen((tmp_dir_cli / "tmp.out").string().c_str(), "r");
    EXPECT_NE(fin, nullptr);
    char buff_list[2048];
    int n = ::fread(buff_list, 1, 2048, fin);
    fclose(fin);
    char checksum[65] = {0};
    memcpy(checksum, buff_list, 64);

    chdir(tmp_dir_ser.string().c_str());
    cmd_str = "sha256sum " + std::to_string(random_name) + ".txt\n";
    FILE* fin_std = popen(cmd_str.c_str(), "r");
    char buff_list_std[2048];
    int m = ::fread(buff_list_std, 1, 2048, fin_std);
    chdir(current_dir.string().c_str());
    char checksum_std[65] = {0};
    memcpy(checksum_std, buff_list_std, 64);

    EXPECT_EQ(memcmp(checksum, checksum_std, 64), 0);

    clearProcess(client_pid);
    clearProcess(server_pid);
}

TEST(FTPServer, List) {
    pid_t server_pid, client_pid;
    int server_port, client_fd;
    std::string cmd_str;

    if (prepare(client_fd, server_port, server_pid, client_pid, 5, 1) != 0)
        return ;

    cmd_str = "open 127.0.0.1 " + std::to_string(server_port) + "\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(500000);

    /** Generate Content **/
    int num_files = rand() % 10;

    while (num_files --) {
        uint32_t random_name = rand() % 1000;
        uint32_t random_content = rand() % 1000;
        std::ofstream fout((tmp_dir_ser / (std::to_string(random_name) + ".txt")).string(), std::ios::out);
        fout << random_content;
        fout.close();
    }
    usleep(500000);
    /** Generate Content **/

    cmd_str = "ls\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(1000000);

    //int exitCode = waitProcessExit(client_pid);
    //EXPECT_EQ(exitCode, 5);

    FILE* fin = fopen((tmp_dir_cli / "tmp.out").string().c_str(), "r");
    EXPECT_NE(fin, nullptr);
    char buff_list[2048];
    int n = ::fread(buff_list, 1, 2048, fin);
    fclose(fin);

    chdir(tmp_dir_ser.string().c_str());
    FILE* fin_std = popen("ls", "r");
    char buff_list_std[2048];
    int m = ::fread(buff_list_std, 1, 2048, fin_std);
    chdir(current_dir.string().c_str());

    EXPECT_EQ(n, m);
    if (n == m) EXPECT_EQ(memcmp(buff_list_std, buff_list, n), 0);

    clearProcess(client_pid);
    clearProcess(server_pid);
}

TEST(FTPServer, CdGet) {
    pid_t server_pid, client_pid;
    int server_port, client_fd;
    std::string cmd_str;

    if (prepare(client_fd, server_port, server_pid, client_pid, 3, 1) != 0)
        return ;

    cmd_str = "open 127.0.0.1 " + std::to_string(server_port) + "\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(500000);

    std::filesystem::path old_dir = tmp_dir_ser;

    /** Generate Directory **/
    uint32_t random_dir = rand() % 1000;
    std::filesystem::create_directory(tmp_dir_ser / std::to_string(random_dir));
    tmp_dir_ser = tmp_dir_ser / std::to_string(random_dir);
    /** Generate Directory **/

    cmd_str = "cd " + std::to_string(random_dir) + "\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(500000);

    /** Generate Content **/
    uint32_t random_name = rand() % 1000;
    uint32_t random_content = rand() % 1000;
    std::ofstream fout((tmp_dir_ser / (std::to_string(random_name) + ".txt")).string(), std::ios::out);
    fout << random_content;
    fout.close();
    usleep(500000);
    /** Generate Content **/

    cmd_str = "get " + std::to_string(random_name) + ".txt\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(1000000);

    std::ifstream fin((tmp_dir_cli / (std::to_string(random_name) + ".txt")).string(), std::ios::in);
    uint32_t get_content;
    if (fin) {
        fin >> get_content;
        fin.close();
    } else get_content = ~0;
    EXPECT_EQ(get_content, random_content);

    tmp_dir_ser = old_dir;

    clearProcess(client_pid);
    clearProcess(server_pid);
}

TEST(FTPServer, CdPut) {
    pid_t server_pid, client_pid;
    int server_port, client_fd;
    std::string cmd_str;

    if (prepare(client_fd, server_port, server_pid, client_pid, 4, 1) != 0)
        return ;

    cmd_str = "open 127.0.0.1 " + std::to_string(server_port) + "\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(500000);

    std::filesystem::path old_dir = tmp_dir_ser;

    /** Generate Directory **/
    uint32_t random_dir = rand() % 1000;
    std::filesystem::create_directory(tmp_dir_ser / std::to_string(random_dir));
    tmp_dir_ser = tmp_dir_ser / std::to_string(random_dir);
    /** Generate Directory **/

    cmd_str = "cd " + std::to_string(random_dir) + "\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(500000);

    /** Generate Content **/
    uint32_t random_name = rand() % 1000;
    uint32_t random_content = rand() % 1000;
    std::ofstream fout((tmp_dir_cli / (std::to_string(random_name) + ".txt")).string(), std::ios::out);
    fout << random_content;
    fout.close();
    usleep(500000);
    /** Generate Content **/

    cmd_str = "put " + std::to_string(random_name) + ".txt\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(1000000);

    std::ifstream fin((tmp_dir_ser / (std::to_string(random_name) + ".txt")).string(), std::ios::in);
    uint32_t get_content;
    if (fin) {
        fin >> get_content;
        fin.close();
    } else EXPECT_EQ(0, 1);
    EXPECT_EQ(get_content, random_content);

    tmp_dir_ser = old_dir;

    clearProcess(client_pid);
    clearProcess(server_pid);
}

TEST(FTPServer, MultiGet) {
    pid_t server_pid, client_pid[16];
    int server_port, client_fd[16];
    std::string cmd_str;

    if (prepareMulti(client_fd, server_port, server_pid, client_pid, 3, 1) != 0)
        return ;

    cmd_str = "open 127.0.0.1 " + std::to_string(server_port) + "\n";
    for (int i = 0; i < 16; i ++) {
        printf("%d\n", client_fd[i]);
        write(client_fd[i], cmd_str.c_str(), cmd_str.length());
    }
    usleep(500000);

    /** Generate Content **/
    uint32_t random_name = rand() % 1000;
    uint32_t random_content = rand() % 1000;
    std::ofstream fout((tmp_dir_ser / (std::to_string(random_name) + ".txt")).string(), std::ios::out);
    fout << random_content;
    fout.close();
    usleep(500000);
    /** Generate Content **/

    cmd_str = "get " + std::to_string(random_name) + ".txt\n";
    for (int i = 0; i < 16; i ++)
        write(client_fd[i], cmd_str.c_str(), cmd_str.length());
    usleep(1000000);

    for (int i = 0; i < 16; i ++) {
        std::ifstream fin((tmp_dir_clis[i] / (std::to_string(random_name) + ".txt")).string(), std::ios::in);
        uint32_t get_content;
        if (fin) {
            fin >> get_content;
            fin.close();
        } else get_content = ~0;
        EXPECT_EQ(get_content, random_content);
    }

    for (int i = 0; i < 16; i ++) {
        clearProcess(client_pid[i]);
    }
    clearProcess(server_pid);
}

TEST(FTPServer, MultiPut) {
    pid_t server_pid, client_pid[16];
    int server_port, client_fd[16];
    std::string cmd_str;

    if (prepareMulti(client_fd, server_port, server_pid, client_pid, 4, 1) != 0)
        return ;

    cmd_str = "open 127.0.0.1 " + std::to_string(server_port) + "\n";
    for (int i = 0; i < 16; i ++)
        write(client_fd[i], cmd_str.c_str(), cmd_str.length());
    usleep(500000);

    /** Generate Content **/
    uint32_t random_names[16];
    uint32_t random_contents[16];

    for (int i = 0; i < 16; i ++) {
        random_names[i] = i == 0 ? (rand() % 900) : (random_names[i-1] + 1 + (rand() % 10));
        random_contents[i] = rand() % 1000;
        std::ofstream fout((tmp_dir_clis[i] / (std::to_string(random_names[i]) + ".txt")).string(), std::ios::out);
        fout << random_contents[i];
        fout.close();
    }
    usleep(500000);
    /** Generate Content **/

    for (int i = 0; i < 16; i ++) {
        cmd_str = "put " + std::to_string(random_names[i]) + ".txt\n";
        write(client_fd[i], cmd_str.c_str(), cmd_str.length());
    }
    usleep(1000000);

    for (int i = 0; i < 16; i ++) {
        std::ifstream fin((tmp_dir_ser / (std::to_string(random_names[i]) + ".txt")).string(), std::ios::in);
        uint32_t get_content;
        if (fin) {
            fin >> get_content;
            fin.close();
        } else get_content = ~0;
        EXPECT_EQ(get_content, random_contents[i]);
    }

    for (int i = 0; i < 16; i ++)
        clearProcess(client_pid[i]);
    clearProcess(server_pid);
}

/* Additional tests (added by student)*/
TEST(FTPServer, PutBig) {
    pid_t server_pid, client_pid;
    int server_port, client_fd;
    std::string cmd_str;

    if (prepare(client_fd, server_port, server_pid, client_pid, 4, 1) != 0)
        return ;

    cmd_str = "open 127.0.0.1 " + std::to_string(server_port) + "\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(500000);

    /** Generate Content **/
    std::string random_name = "";
    for (int i = 0; i < 8; ++i) {
        random_name += 'a' + (rand() % 26);
    }
    std::string random_content(1024 * 1024, ' ');
    for (char &c : random_content) {
        c = 'a' + (rand() % 26);  // random alphabetical characters
    }
    std::ofstream fout((tmp_dir_cli / (random_name + ".txt")).string(), std::ios::out);
    fout << random_content;
    fout.close();
    usleep(500000);
    /** Generate Content **/

    cmd_str = "put " + random_name + ".txt\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(1000000);

    std::ifstream fin((tmp_dir_ser / (random_name + ".txt")).string(), std::ios::in);
    std::string get_content;
    if (fin) {
        std::ostringstream ss;
        ss << fin.rdbuf(); // read the file contents into a string
        get_content = ss.str();
        fin.close();
    } else EXPECT_EQ(0, 1);

    if(get_content != random_content)
        EXPECT_EQ(0, 1);

    clearProcess(client_pid);
    clearProcess(server_pid);
}

TEST(FTPServer, GetBig) {
    pid_t server_pid, client_pid;
    int server_port, client_fd;
    std::string cmd_str;

    if (prepare(client_fd, server_port, server_pid, client_pid, 3, 1) != 0)
        return ;

    cmd_str = "open 127.0.0.1 " + std::to_string(server_port) + "\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(500000);

    /** Generate Content **/
    std::string random_name = "";
    for (int i = 0; i < 8; ++i) {
        random_name += 'a' + (rand() % 26);
    }
    std::string random_content(1024 * 1024, ' ');
    for (char &c : random_content) {
        c = 'a' + (rand() % 26);  // random alphabetical characters
    }
    std::ofstream fout((tmp_dir_ser / (random_name + ".txt")).string(), std::ios::out);
    fout << random_content;
    fout.close();
    usleep(500000);
    /** Generate Content **/

    cmd_str = "get " + random_name + ".txt\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(1000000);

    std::ifstream fin((tmp_dir_cli / (random_name + ".txt")).string(), std::ios::in);
    std::string get_content;
    if (fin) {
        std::ostringstream ss;
        ss << fin.rdbuf(); // read the file contents into a string
        get_content = ss.str();
        fin.close();
    } else EXPECT_EQ(0, 1);
    
    if(get_content != random_content)
        EXPECT_EQ(0, 1);

    clearProcess(client_pid);
    clearProcess(server_pid);
}

TEST(FTPServer, MultiCdList) {
    pid_t server_pid, client_pid[16];
    int server_port, client_fd[16];
    std::string cmd_str;

    if (prepareMulti(client_fd, server_port, server_pid, client_pid, 5, 1) != 0)
        return ;

    cmd_str = "open 127.0.0.1 " + std::to_string(server_port) + "\n";
    for (int i = 0; i < 16; i ++){
        write(client_fd[i], cmd_str.c_str(), cmd_str.length());
    }
    usleep(500000);

    uint32_t random_dir[16];

    for(int i = 0; i < 16; i++){
        // std::cout << i << std::endl;
        random_dir[i] = rand() % 1000;
        std::filesystem::create_directory(tmp_dir_ser / std::to_string(random_dir[i]));
        cmd_str = "cd " + std::to_string(random_dir[i]) + "\n";
        write(client_fd[i], cmd_str.c_str(), cmd_str.length());
        int num_files = rand() % 10;
        while (num_files--) {
            uint32_t random_name = rand() % 1000;
            uint32_t random_content = rand() % 1000;
            std::ofstream fout((tmp_dir_ser / std::to_string(random_dir[i]) / (std::to_string(random_name) + ".txt")).string(), std::ios::out);
            fout << random_content;
            fout.close();
        }
        cmd_str = "ls\n";
        write(client_fd[i], cmd_str.c_str(), cmd_str.length());
        usleep(500000);
        
        FILE* fin = fopen((tmp_dir_clis[i] / "tmp.out").string().c_str(), "r");
        EXPECT_NE(fin, nullptr);
        char buff_list[2048];
        int n = ::fread(buff_list, 1, 2048, fin);
        fclose(fin);

        chdir((tmp_dir_ser / std::to_string(random_dir[i])).string().c_str());
        FILE* fin_std = popen("ls", "r");
        char buff_list_std[2048];
        int m = ::fread(buff_list_std, 1, 2048, fin_std);
        chdir(current_dir.string().c_str());

        EXPECT_EQ(n, m);
        if (n == m) EXPECT_EQ(memcmp(buff_list_std, buff_list, n), 0);
        
    }
    usleep(1000000);


    for (int i = 0; i < 16; i ++)
        clearProcess(client_pid[i]);
    clearProcess(server_pid);
}
/*********** FTP_SERVER ***********/
/*********** FTP_SERVER ***********/























/*********** FTP_CLIENT ***********/
/*********** FTP_CLIENT ***********/

TEST(FTPClient, Open) {
    pid_t server_pid, client_pid;
    int server_port, client_fd;
    std::string cmd_str;

    if (prepare(client_fd, server_port, server_pid, client_pid, 1, 0) != 0)
        return ;

    cmd_str = "open 127.0.0.1 " + std::to_string(server_port) + "\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(500000);

    //int code = waitProcessExit(server_pid);
    //EXPECT_EQ(code, 1);
    FILE* fin = fopen((tmp_dir_ser / ".server_open_ok").string().c_str(), "r");
    EXPECT_NE(fin, nullptr);
    fclose(fin);

    clearProcess(client_pid);
    clearProcess(server_pid);
}

TEST(FTPClient, Get) {
    pid_t server_pid, client_pid;
    int server_port, client_fd;
    std::string cmd_str;

    if (prepare(client_fd, server_port, server_pid, client_pid, 3, 0) != 0)
        return ;

    cmd_str = "open 127.0.0.1 " + std::to_string(server_port) + "\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(500000);

    /** Generate Content **/
    uint32_t random_name = rand() % 1000;
    uint32_t random_content = rand() % 1000;
    std::ofstream fout((tmp_dir_ser / (std::to_string(random_name) + ".txt")).string(), std::ios::out);
    fout << random_content;
    fout.close();
    usleep(500000);
    /** Generate Content **/

    cmd_str = "get " + std::to_string(random_name) + ".txt\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(1000000);

    std::ifstream fin((tmp_dir_cli / (std::to_string(random_name) + ".txt")).string(), std::ios::in);
    uint32_t get_content;
    if (fin) {
        fin >> get_content;
        fin.close();
    } else get_content = ~0;
    EXPECT_EQ(get_content, random_content);

    clearProcess(client_pid);
    clearProcess(server_pid);
}


TEST(FTPClient, Put) {
    pid_t server_pid, client_pid;
    int server_port, client_fd;
    std::string cmd_str;

    if (prepare(client_fd, server_port, server_pid, client_pid, 4, 0) != 0)
        return ;

    cmd_str = "open 127.0.0.1 " + std::to_string(server_port) + "\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(500000);

    /** Generate Content **/
    uint32_t random_name = rand() % 1000;
    uint32_t random_content = rand() % 1000;
    std::ofstream fout((tmp_dir_cli / (std::to_string(random_name) + ".txt")).string(), std::ios::out);
    fout << random_content;
    fout.close();
    usleep(500000);
    /** Generate Content **/

    cmd_str = "put " + std::to_string(random_name) + ".txt\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(1000000);

    std::ifstream fin((tmp_dir_ser / (std::to_string(random_name) + ".txt")).string(), std::ios::in);
    uint32_t get_content;
    if (fin) {
        fin >> get_content;
        fin.close();
    } else get_content = ~0;
    EXPECT_EQ(get_content, random_content);

    clearProcess(client_pid);
    clearProcess(server_pid);
}

TEST(FTPClient, GetBig) {
    pid_t server_pid, client_pid;
    int server_port, client_fd;
    std::string cmd_str;

    if (prepare(client_fd, server_port, server_pid, client_pid, 3, 0) != 0)
        return ;

    cmd_str = "open 127.0.0.1 " + std::to_string(server_port) + "\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(500000);

    /** Generate Content **/
    std::string random_name = "";
    for (int i = 0; i < 8; ++i) {
        random_name += 'a' + (rand() % 26);
    }
    std::string random_content(1024 * 1024, ' ');
    for (char &c : random_content) {
        c = 'a' + (rand() % 26);  // random alphabetical characters
    }
    std::ofstream fout((tmp_dir_ser / (random_name + ".txt")).string(), std::ios::out);
    fout << random_content;
    fout.close();
    usleep(500000);
    /** Generate Content **/

    cmd_str = "get " + random_name + ".txt\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(1000000);

    std::ifstream fin((tmp_dir_cli / (random_name + ".txt")).string(), std::ios::in);
    std::string get_content;
    if (fin) {
        fin >> get_content;
        fin.close();
    } else EXPECT_EQ(0, 1);

    if(get_content != random_content)
        EXPECT_EQ(0, 1);

    clearProcess(client_pid);
    clearProcess(server_pid);
}

TEST(FTPClient, PutBig) {
    pid_t server_pid, client_pid;
    int server_port, client_fd;
    std::string cmd_str;

    if (prepare(client_fd, server_port, server_pid, client_pid, 4, 0) != 0)
        return ;

    cmd_str = "open 127.0.0.1 " + std::to_string(server_port) + "\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(500000);

    /** Generate Content **/
    std::string random_name = "";
    for (int i = 0; i < 8; ++i) {
        random_name += 'a' + (rand() % 26);
    }
    std::string random_content(1024 * 1024, ' ');
    for (char &c : random_content) {
        c = 'a' + (rand() % 26);  // random alphabetical characters
    }
    std::ofstream fout((tmp_dir_cli / (random_name + ".txt")).string(), std::ios::out);
    fout << random_content;
    fout.close();
    usleep(500000);
    /** Generate Content **/

    cmd_str = "put " + random_name + ".txt\n";
    write(client_fd, cmd_str.c_str(), cmd_str.length());
    usleep(1000000);

    std::ifstream fin((tmp_dir_ser / (random_name + ".txt")).string(), std::ios::in);
    std::string get_content;
    if (fin) {
        fin >> get_content;
        fin.close();
    } else EXPECT_EQ(0, 1);

    if(get_content != random_content)
        EXPECT_EQ(0, 1);

    clearProcess(client_pid);
    clearProcess(server_pid);
}

int _tmain(int argc, wchar_t* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
