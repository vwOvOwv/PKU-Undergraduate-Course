# 2024-Lab4-ns3-Test

## 目录结构

```bash
student@327fb651b54a:~/workspace/code/2024-Lab4-ns3-Test$ tree -L 3
.
|-- README.md
`-- test-utils
    |-- cwnd-test
    |   |-- run_cwnd_test
    |   |-- std_case1_n2.dat
    |   |-- std_case1_n3.dat
    |   |-- std_case2_n2.dat
    |   |-- std_case2_n3.dat
    |   |-- std_case3_n2.dat
    |   |-- std_case3_n3.dat
    |   |-- std_case4_n2.dat
    |   |-- std_case4_n3.dat
    |   |-- std_case5_n2.dat
    |   |-- std_case5_n3.dat
    |   `-- test_cases.json
    |-- fct-test
    |   |-- run_fct_test
    |   |-- std_case1.dat
    |   |-- std_case2.dat
    |   |-- std_case3.dat
    |   |-- std_case4.dat
    |   |-- std_case5.dat
    |   `-- test_cases.json
    |-- pcap-test
    |   |-- run_pcap_test
    |   |-- std_case1-0-0.pcap
    |   |-- std_case1-0-1.pcap
    |   |-- std_case1-0-2.pcap
    |   |-- std_case2-0-0.pcap
    |   |-- std_case2-0-1.pcap
    |   |-- std_case2-0-2.pcap
    |   |-- std_case3-0-0.pcap
    |   |-- std_case3-0-1.pcap
    |   |-- std_case3-0-2.pcap
    |   `-- test_cases.json
    `-- run_test.sh

4 directories, 32 files
```

`test-utils` 目录下存放了用于评测的脚本和部分测试用例，其中：
- `run_test.sh` 是用来运行测试的脚本
- `pcap-test/` 存放了 Exercise 3 的 3 个测试点
- `cwnd-test/` 存放了 Exercise 4 的 5 个测试点
- `fct-test/` 存放了 Exercise 5 的 5 个测试点

## 本地测试

你可以运行 `test-utils/run_test.sh <ns3_path> <test_name>` 来进行本地的测试，其中
- `ns3_path` 表示 `ns-3.38` 目录在你的机器上的 **绝对路径** (不是相对路径！！)
- `test_name` 表示你要进行的测试类型，支持的值有 fct、pcap、cwnd、all

举例而言，`test-utils/run_test.sh /home/student/workspace/ns-allinone-3.38 cwnd` 可用于单独测试 Exercise 4 (cwnd)，`test-utils/run_test.sh /home/student/workspace/ns-allinone-3.38 all` 可用于测试所有需要测试的三个 Exercise (pcap, cwnd, fct) 。

`run_test.sh`必须被放在原位置下运行，不能被移动到其他位置运行。不过你可以在任意工作目录运行它，比如直接用绝对路径运行 `/home/student/workspace/code/lab4-starter/test/run_test.sh /home/student/workspace/ns-allinone-3.38 all` 就是没问题的。

## 评测脚本的逻辑

下面特别介绍一下我们使用的评测脚本的逻辑。

我们通过比对【同学的程序输出的模拟结果】和【我们的标准例程输出的结果】来进行评测，`test-utils` 目录下以 "std" 开头的那些文件就是用标准例程输出的结果。考虑到不同机器上运行模拟的结果有可能会存在一些误差，所以我们在进行评测时采取了一些宽松的允许误差的比对方式：

* Exercise 3 (PCAP)：
  我们只会比对 `lv1-0-1.pcap` 和 `lv1-0-2.pcap` 这两个文件与标准答案，比对时，我们会提取出每行的 ack 字段的值进行比对，如果都一致，那么我们就认为你的输出结果是正确的。
* Exercise 4 (cwnd)：
  在判断两个 cwnd trace 文件是否相同时，我们只会比对其中记录的 cwnd 最大值以及第一次达到 cwnd 最大值时的时间戳，并且允许 10% 的误差。
* Exercise 5 (fct)：
  在判断两个 fct 输出是否相同时，我们会比对你的输出与标准答案输出的 fct 数值，并且允许 5% 的误差。
