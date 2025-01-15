#!/bin/bash

ns3_path="/home/student/workspace/ns-allinone-3.38/ns-3.38"

# 检查是否提供了必须的参数
if [ $# -lt 2 ]; then
  echo "Usage: $0 <ns3_path> <test_name>"
  exit 1
fi

# 读取传入参数
ns3_path=$1
test_name=$2

# 获取脚本所在目录的绝对路径
test_path=$(cd "$(dirname "$0")" && pwd)

# 打印信息
echo "ns-3.38_path: $ns3_path"
echo "test_path: $test_path"
echo "test_name: $test_name"

cd "$(dirname "$0")"

# 根据 test_name 执行对应的测试脚本
case $test_name in
  "fct")
    echo "Testing Exericse 5: fct"
    python3 fct-test/run_fct_test "$ns3_path" "$test_path"
    ;;
  "pcap")
    echo "Testing Exericse 3: pcap"
    python3 pcap-test/run_pcap_test "$ns3_path" "$test_path"
    ;;
  "cwnd")
    echo "Testing Exericse 4: cwnd"
    python3 cwnd-test/run_cwnd_test "$ns3_path" "$test_path"
    ;;
  "all")
    echo "Testing Exericse 3: pcap"
    python3 pcap-test/run_pcap_test "$ns3_path" "$test_path"
    echo ""
    echo "Testing Exericse 4: cwnd"
    python3 cwnd-test/run_cwnd_test "$ns3_path" "$test_path"
    echo ""
    echo "Testing Exericse 5: fct"
    python3 fct-test/run_fct_test "$ns3_path" "$test_path"
    ;;
  *)
    echo "Invalid test_name. Allowed values: fct, pcap, cwnd, all"
    exit 1
    ;;
esac
