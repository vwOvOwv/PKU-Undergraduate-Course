#include <cstdlib>
#include <fcntl.h>
#include <gtest/gtest.h>
#include <signal.h>
#include <stdint.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <thread>

#include "../include/util.h"

#define ORIGIN SOURCE_DIR "/build/testdata"
#define RESULT SOURCE_DIR "/build/recvfile"

static int port = 49152;
static char origin[100];
static char result[100];

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

int diff_file(const char* f1, const char* f2) {
    FILE* fp1 = fopen(f1, "r");
    FILE* fp2 = fopen(f2, "r");
    if (!fp1 || !fp2) {
        LOG_DEBUG("original file: %s\n", f1);
        LOG_DEBUG("new file: %s\n", f2);
        if(!fp1)
            LOG_DEBUG("Autograder: original file error\n");
        if(!fp2)
            LOG_DEBUG("Autograder: new file error\n");
        return -1;
    }
    char f1_buffer[65536];
    char f2_buffer[65536];
    while (1) {
        int size1 = fread((void*)f1_buffer, sizeof(char), 65536, fp1);
        int size2 = fread((void*)f2_buffer, sizeof(char), 65536, fp2);
        if (size1 != size2){
            LOG_DEBUG("Autograder: wrong size\n");
            return -1;
        }
        for (int i = 0; i < size1; i++) {
            if (f1_buffer[i] != f2_buffer[i]){
                LOG_DEBUG("Autograder: wrong content\n");
                return -1;
            }
        }
        if (feof(fp1)) {
            if (!feof(fp2)){
                LOG_DEBUG("Autograder: wrong end\n");
                return -1;
            }
            else
                return 1;
        }
    }
}

int wait_or_timeout(pid_t sender, pid_t receiver) {
    int sleep_secs = 0;
    int interval = 3;
    int timeout = 45;
    int cnt = 0;
    int r = 0;
    int status;

    while (1) {
        int pid = waitpid(-1, &status, WNOHANG);
        if (pid == -1) {
            LOG_FATAL("waitpid(): unexpected error\n");
        } else if (pid == 0) {
            sleep(interval);
            sleep_secs += interval;
            if (sleep_secs >= timeout) {
                if (waitpid(sender, &status, WNOHANG) == 0) {
                    kill(sender, SIGTERM);
                }
                if (waitpid(receiver, &status, WNOHANG) == 0) {
                    kill(receiver, SIGTERM);
                }
                LOG_MSG("Timeout\n");
                return -1;
            }
        } else {
            if (!WIFEXITED(status)) {
                r = -1;
            }
            if (WEXITSTATUS(status) != 0) {
                r = -1;
            }

            cnt++;
            if (cnt == 2) {
                return r;
            }
        }
    }
}

void exe_name(char* name, size_t maxlen, bool is_sender, bool normal) {
    const char* prefix =
        normal ? SOURCE_DIR "/build/" : SOURCE_DIR "/test/test_";
    const char* body = is_sender ? "sender" : "receiver";

    std::snprintf(name, maxlen, "%s%s", prefix, body);
}

// issn: is sender the normal version or the test version?
int run_tests(const char* window_size, const char* err, const char* prob,
              bool issn, bool opt) {
    char sender_name[200];
    char receiver_name[200];
    char port_s[10];
    const char* mode = opt ? "1" : "0";
    pid_t sender, receiver;

    port++;

    exe_name(sender_name, 200, true, issn);
    exe_name(receiver_name, 200, false, !issn);
    std::snprintf(port_s, 10, "%d", port);

    if ((receiver = fork()) == 0) {
        if (issn) {
            if (execl(receiver_name, receiver_name, port_s, result, window_size,
                      mode, err, prob, nullptr) == -1) {
                LOG_FATAL("Failed to execute the receiver\n");
            }
        } else {
            if (execl(receiver_name, receiver_name, port_s, result, window_size,
                      mode, nullptr) == -1) {
                LOG_FATAL("Failed to execute the receiver\n");
            }
        }
    }
    if ((sender = fork()) == 0) {
        if (issn) {
            if (execl(sender_name, sender_name, "127.0.0.1", port_s, origin,
                      window_size, mode, nullptr) == -1) {
                LOG_FATAL("Failed to execute the sender\n");
            }
        } else {
            if (execl(sender_name, sender_name, "127.0.0.1", port_s, origin,
                      window_size, mode, err, prob, nullptr) == -1) {
                LOG_FATAL("Failed to execute the sender\n");
            }
        }
    }

    if (wait_or_timeout(sender, receiver) == -1) {
        return -1;
    }
    return diff_file(origin, result);
}

void create_random(int megabytes) {
    char cmd[200];
    std::snprintf(cmd, 200, "head -c %d < /dev/urandom > %s",
                  megabytes * 1024 * 1024, origin);
    system(cmd);
}

/* -------------------------- before & after tests -------------------------- */
class RTP : public ::testing::Test {
    void SetUp() override {
        std::snprintf(origin, 100, "%s%d", ORIGIN, rand() % 12345 + 123);
        std::snprintf(result, 100, "%s%d", RESULT, rand() % 12345 + 123);
        create_random(2);
    }

    void TearDown() override {
        std::remove(origin);
        std::remove(result);
    }
};

/* ---------------------------- no failure tests ---------------------------- */
TEST_F(RTP, NORMAL) {
    ASSERT_EQ(run_tests("16", "0", "0", true, false), 1);
    ASSERT_EQ(run_tests("16", "0", "0", false, false), 1);
}

TEST_F(RTP, NORMAL_SMALL_WINDOW) {
    ASSERT_EQ(run_tests("1", "0", "0", true, false), 1);
    ASSERT_EQ(run_tests("1", "0", "0", false, false), 1);
}

TEST_F(RTP, NORMAL_HUGE_WINDOW) {
    ASSERT_EQ(run_tests("20000", "0", "0", true, false), 1);
    ASSERT_EQ(run_tests("20000", "0", "0", false, false), 1);
}

TEST_F(RTP, NORMAL_OPT) {
    ASSERT_EQ(run_tests("16", "0", "0", true, true), 1);
    ASSERT_EQ(run_tests("16", "0", "0", false, true), 1);
}

TEST_F(RTP, NORMAL_OPT_SMALL_WINDOW) {
    ASSERT_EQ(run_tests("1", "0", "0", true, true), 1);
    ASSERT_EQ(run_tests("1", "0", "0", false, true), 1);
}

TEST_F(RTP, NORMAL_OPT_HUGE_WINDOW) {
    ASSERT_EQ(run_tests("20000", "0", "0", true, true), 1);
    ASSERT_EQ(run_tests("20000", "0", "0", false, true), 1);
}

/* -------------------------- normal receiver tests ------------------------- */
TEST_F(RTP, RECEIVER_SINGLE_1) {
    // lost or corrupted data
    // std::cerr << "Mark Begin\n" << std::endl;
    ASSERT_EQ(run_tests("16", "1", "10", false, false), 1);
    // std::cerr << "Mark End\n" << std::endl;
    ASSERT_EQ(run_tests("16", "2", "10", false, false), 1);
    // std::cerr << "Mark End\n" << std::endl;
}

TEST_F(RTP, RECEIVER_SINGLE_2) {
    // duplicate or reorder data
    ASSERT_EQ(run_tests("16", "4", "10", false, false), 1);
    ASSERT_EQ(run_tests("16", "8", "10", false, false), 1);
}

TEST_F(RTP, RECEIVER_MIXED_1) {
    ASSERT_EQ(run_tests("16", "15", "20", false, false), 1);
}

TEST_F(RTP, RECEIVER_MIXED_2) {
    ASSERT_EQ(run_tests("16", "15", "30", false, false), 1);
}

/* --------------------------- normal sender tests -------------------------- */
TEST_F(RTP, SENDER_SINGLE_1) {
    ASSERT_EQ(run_tests("16", "1", "10", true, false), 1);
    ASSERT_EQ(run_tests("16", "2", "10", true, false), 1);
}

TEST_F(RTP, SENDER_SINGLE_2) {
    ASSERT_EQ(run_tests("16", "4", "10", true, false), 1);
    ASSERT_EQ(run_tests("16", "8", "10", true, false), 1);
}

TEST_F(RTP, SENDER_MIXED_1) {
    ASSERT_EQ(run_tests("16", "15", "20", true, false), 1);
}

TEST_F(RTP, SENDER_MIXED_2) {
    ASSERT_EQ(run_tests("16", "15", "30", true, false), 1);
}

/* --------------------------- opt receiver tests --------------------------- */
TEST_F(RTP, OPT_RECEIVER_SINGLE_1) {
    ASSERT_EQ(run_tests("16", "1", "10", false, true), 1);
    ASSERT_EQ(run_tests("16", "2", "10", false, true), 1);
}

TEST_F(RTP, OPT_RECEIVER_SINGLE_2) {
    ASSERT_EQ(run_tests("16", "4", "10", false, true), 1);
    ASSERT_EQ(run_tests("16", "8", "10", false, true), 1);
}

TEST_F(RTP, OPT_RECEIVER_MIXED_1) {
    ASSERT_EQ(run_tests("16", "15", "20", false, true), 1);
}

TEST_F(RTP, OPT_RECEIVER_MIXED_2) {
    ASSERT_EQ(run_tests("16", "15", "30", false, true), 1);
}

TEST_F(RTP, OPT_RECEIVER_MIXED_3) {
    ASSERT_EQ(run_tests("32", "15", "60", false, true), 1);
}

TEST_F(RTP, OPT_RECEIVER_MIXED_4) {
    ASSERT_EQ(run_tests("32", "25", "50", false, true), 1);
}

/* ---------------------------- opt sender tests ---------------------------- */
TEST_F(RTP, OPT_SENDER_SINGLE_1) {
    ASSERT_EQ(run_tests("16", "1", "10", true, true), 1);
    ASSERT_EQ(run_tests("16", "2", "10", true, true), 1);
}

TEST_F(RTP, OPT_SENDER_SINGLE_2) {
    ASSERT_EQ(run_tests("16", "4", "10", true, true), 1);
    ASSERT_EQ(run_tests("16", "8", "10", true, true), 1);
}

TEST_F(RTP, OPT_SENDER_MIXED_1) {
    ASSERT_EQ(run_tests("16", "15", "20", true, true), 1);
}

TEST_F(RTP, OPT_SENDER_MIXED_2) {
    ASSERT_EQ(run_tests("16", "15", "30", true, true), 1);
}

TEST_F(RTP, OPT_SENDER_MIXED_3) {
    ASSERT_EQ(run_tests("32", "15", "60", true, true), 1);
}

TEST_F(RTP, OPT_SENDER_MIXED_4) {
    ASSERT_EQ(run_tests("32", "25", "50", true, true), 1);
}