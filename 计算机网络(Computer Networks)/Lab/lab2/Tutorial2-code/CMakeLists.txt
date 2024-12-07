cmake_minimum_required(VERSION 2.8)
project(tutorial2-RTPsniffer)

set(CMAKE_CXX_STANDARD 11)

include_directories(/usr/local/include)

link_directories(/usr/local/lib)

add_executable(rtp_sniffer
	src/rtp_sniffer.c)
target_link_libraries(rtp_sniffer pcap)
