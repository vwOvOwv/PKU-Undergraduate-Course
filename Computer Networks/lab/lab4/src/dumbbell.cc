#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/traffic-control-module.h"

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unordered_map>
#include <vector>
#include <filesystem>

using namespace ns3;

// - Network topology
//   - The dumbbell topology consists of 
//     - 4 servers (S0, S1, R0, R1)
//     - 2 routers (T0, T1) 
//   - The topology is as follows:
//
//                    S0                         R0
//     10 Mbps, 1 ms   |      1 Mbps, 10 ms       |   10 Mbps, 1 ms
//                    T0 ----------------------- T1
//     10 Mbps, 1 ms   |                          |   10 Mbps, 1 ms
//                    S1                         R1
//
// - Two TCP flows:
//   - TCP flow 0 from S0 to R0 using BulkSendApplication.
//   - TCP flow 1 from S1 to R1 using BulkSendApplication.

std::unordered_map<uint32_t, int64_t> finishTime;
const uint32_t N1 = 2;    // Number of nodes in left side
const uint32_t N2 = 2;    // Number of nodes in right side
uint32_t segmentSize = 1448;    // Segment size
Time startTime = Seconds(10.0);    // Start time for the simulation
Time stopTime = Seconds(60.0);    // Stop time for the simulation
std::vector<uint32_t> flowSize;

// Create directory recursively
bool CreateDirectoryRecursive(const std::string& path)
{
    size_t pos = 0;
    do {
        pos = path.find_first_of("/\\", pos + 1);
        std::string subdir = path.substr(0, pos);
        if (subdir.empty()) continue;
        if (mkdir(subdir.c_str(), 0755) != 0) {
            if (errno == EEXIST) {
                continue; // the directory already exists
            } else {
                std::cerr << "Create directory error: " << subdir << " errno: " << strerror(errno) << std::endl;
                return false;
            }
        }
    } while (pos != std::string::npos);
    return true;
}

// Trace sink function to trace congestion window
void TraceSinkWithContext(std::string context, uint32_t old_value, uint32_t new_value) 
{
    uint32_t nodeId = 0;
    int length = context.length();
    for(int i = 0; i < length; i++){
        if(context[i] >= '0' && context[i] <= '9'){
            while(i < length && context[i] >= '0' && context[i] <= '9'){
                nodeId = nodeId * 10 + context[i] - '0';
                i++;
            }
            break;
        }
    }
    std::ofstream cwndOut("lv1-results/cwnd/n" + std::to_string(nodeId) + ".dat", std::ios::out | std::ios::app);
    cwndOut << Simulator::Now().GetMicroSeconds() << " " << old_value / segmentSize << " "<< new_value / segmentSize << std::endl;
    cwndOut.close();
}

// Connect trace sink function to trace source (CongestionWindow)
void
TraceCwndWithContext(uint32_t nodeId, uint32_t cwndWindow)
{
    Config::Connect("/NodeList/" + std::to_string(nodeId) +
                               "/$ns3::TcpL4Protocol/SocketList/" +
                               std::to_string(cwndWindow) + "/CongestionWindow",
                               MakeCallback(&TraceSinkWithContext));
}

// Trace sink function to trace FCT
void TracePacketWithoutContext(uint32_t nodeId,
                               const Ptr<const Packet> packet,
                               const TcpHeader& header,
                               const Ptr<const TcpSocketBase> socket) {
    // if (header.GetFlags() == TcpHeader::ACK + TcpHeader::FIN) { // Check if the packet is the FINACK
    if (header.GetFlags() & TcpHeader::ACK) { // Check if the packet is the ACK
        finishTime[nodeId] = Simulator::Now().GetMicroSeconds();
    }
}

// Connect trace sink function to trace source (Rx)
void
TraceFCTWithContext(uint32_t nodeId, Ptr<BulkSendApplication> bulk_app)
{
    // use TraceConnectWithoutContext to avoid the context parameter
    bulk_app->GetSocket()->TraceConnectWithoutContext("Rx", MakeBoundCallback(TracePacketWithoutContext, nodeId));
}

// Function to install BulkSend application
void
InstallBulkSend(Ptr<Node> node,
                Ipv4Address address,
                uint16_t port,
                std::string socketFactory,
                uint32_t flowSize,
                uint32_t nodeId,
                uint32_t cwndWindow)
{
    BulkSendHelper source(socketFactory, InetSocketAddress(address, port));
    source.SetAttribute("MaxBytes", UintegerValue(flowSize));    // "0" means there is no limit. This line should be changed in Exercise 1.2
    ApplicationContainer sourceApps = source.Install(node);
    Ptr<BulkSendApplication> bulk_app = sourceApps.Get(0)->GetObject<BulkSendApplication>();
    sourceApps.Start(startTime);
    Simulator::Schedule(startTime + TimeStep(1), &TraceCwndWithContext, nodeId, cwndWindow);
    Simulator::Schedule(startTime + TimeStep(1), &TraceFCTWithContext, nodeId, bulk_app);
    sourceApps.Stop(stopTime);
}

// Function to install sink application
void
InstallPacketSink(Ptr<Node> node, uint16_t port, std::string socketFactory)
{
    PacketSinkHelper sink(socketFactory, InetSocketAddress(Ipv4Address::GetAny(), port));
    ApplicationContainer sinkApps = sink.Install(node);
    sinkApps.Start(startTime);
    sinkApps.Stop(stopTime);
}

int
main(int argc, char* argv[])
{
    std::vector<std::string> directories = {
        "lv1-results",
        "lv1-results/cwnd",
        "lv1-results/pcap",
        "lv1-results/fct"
    };

    // Check if the directories exist, if not create them
    for (const auto& dir : directories) {
        if (!CreateDirectoryRecursive(dir)) {
            std::cerr << "Create directory failed: " << dir << std::endl;
            return 1;
        }
    }

    // Clear the cwnd directory to ensure no old files are present
    std::string cwndDir = "lv1-results/cwnd";
    for (const auto& entry : std::filesystem::directory_iterator(cwndDir)) {
        if (entry.is_regular_file()) {
            std::filesystem::remove(entry.path());
        }
    }

    // Parse the command line arguments
    uint32_t flowSize0, flowSize1;
    CommandLine cmd;
    cmd.AddValue("flowSize0", "Number of Bytes contained in flow 0", flowSize0);
    cmd.AddValue("flowSize1", "Number of Bytes contained in flow 1", flowSize1);
    cmd.Parse (argc, argv);
    flowSize.push_back(flowSize0);
    flowSize.push_back(flowSize1);

    std::string socketFactory = "ns3::TcpSocketFactory";    // Socket factory to use
    std::string tcpTypeId = "ns3::TcpLinuxReno";    // TCP variant to use
    std::string qdiscTypeId = "ns3::FifoQueueDisc";    // Queue disc for gateway
    bool isSack = true;    // Flag to enable/disable sack in TCP
    uint32_t delAckCount = 1;    // Delayed ack count
    std::string recovery = "ns3::TcpClassicRecovery";    // Recovery algorithm type to use

    // Check if the qdiscTypeId and tcpTypeId are valid
    TypeId qdTid;
    NS_ABORT_MSG_UNLESS(TypeId::LookupByNameFailSafe(qdiscTypeId, &qdTid),
                        "TypeId " << qdiscTypeId << " not found");
    TypeId tcpTid;
    NS_ABORT_MSG_UNLESS(TypeId::LookupByNameFailSafe(tcpTypeId, &tcpTid),
                        "TypeId " << tcpTypeId << " not found");

    // Set recovery algorithm and TCP variant
    Config::SetDefault("ns3::TcpL4Protocol::RecoveryType",
                       TypeIdValue(TypeId::LookupByName(recovery)));
    Config::SetDefault("ns3::TcpL4Protocol::SocketType",
                       TypeIdValue(TypeId::LookupByName(tcpTypeId)));

    // Create nodes
    NodeContainer leftNodes;
    NodeContainer rightNodes;
    NodeContainer routers;
    routers.Create(2);
    leftNodes.Create(N1);
    rightNodes.Create(N2);

    // Create the point-to-point link helpers and connect two router nodes
    PointToPointHelper pointToPointRouter;
    pointToPointRouter.SetDeviceAttribute("DataRate", StringValue("1Mbps"));
    pointToPointRouter.SetChannelAttribute("Delay", StringValue("10ms"));

    NetDeviceContainer routerToRouter = pointToPointRouter.Install(routers.Get(0), routers.Get(1));

    // Create the point-to-point link helpers and connect leaf nodes to router
    PointToPointHelper pointToPointLeaf;
    pointToPointLeaf.SetDeviceAttribute("DataRate", StringValue("10Mbps"));
    pointToPointLeaf.SetChannelAttribute("Delay", StringValue("1ms"));

    std::vector<NetDeviceContainer> leftToRouter;
    std::vector<NetDeviceContainer> routerToRight;
    for (uint32_t i = 0; i < N1; i++)
    {
        leftToRouter.push_back(pointToPointLeaf.Install(leftNodes.Get(i), routers.Get(0)));
    }
    for (uint32_t i = 0; i < N2; i++)
    {
        routerToRight.push_back(pointToPointLeaf.Install(routers.Get(1), rightNodes.Get(i)));
    }

    // Enable pcap tracing on the devices on T0
    pointToPointLeaf.EnablePcap("lv1-results/pcap/lv1", leftToRouter[0].Get(1), true);
    pointToPointLeaf.EnablePcap("lv1-results/pcap/lv1", leftToRouter[1].Get(1), true);
    pointToPointLeaf.EnablePcap("lv1-results/pcap/lv1", routerToRouter.Get(0), true);

    // Install internet stack on all the nodes
    InternetStackHelper internetStack;

    internetStack.Install(leftNodes);
    internetStack.Install(rightNodes);
    internetStack.Install(routers);

    // Assign IP addresses to all the network devices
    Ipv4AddressHelper ipAddresses("10.0.0.0", "255.255.255.0");

    Ipv4InterfaceContainer routersIpAddress = ipAddresses.Assign(routerToRouter);
    ipAddresses.NewNetwork();

    std::vector<Ipv4InterfaceContainer> leftToRouterIPAddress;
    for (uint32_t i = 0; i < N1; i++)
    {
        leftToRouterIPAddress.push_back(ipAddresses.Assign(leftToRouter[i]));
        ipAddresses.NewNetwork();
    }

    std::vector<Ipv4InterfaceContainer> routerToRightIPAddress;
    for (uint32_t i = 0; i < N2; i++)
    {
        routerToRightIPAddress.push_back(ipAddresses.Assign(routerToRight[i]));
        ipAddresses.NewNetwork();
    }

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    // Set default sender and receiver buffer size as 1MB
    Config::SetDefault("ns3::TcpSocket::SndBufSize", UintegerValue(1 << 20));
    Config::SetDefault("ns3::TcpSocket::RcvBufSize", UintegerValue(1 << 20));

    // Set default initial congestion window as 10 segments
    Config::SetDefault("ns3::TcpSocket::InitialCwnd", UintegerValue(10));

    // Set default delayed ack count to a specified value
    Config::SetDefault("ns3::TcpSocket::DelAckCount", UintegerValue(delAckCount));

    // Set default segment size of TCP packet to a specified value
    Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue(segmentSize));

    // Enable/Disable SACK in TCP
    Config::SetDefault("ns3::TcpSocketBase::Sack", BooleanValue(isSack));

    // Set default parameters for queue discipline
    Config::SetDefault(qdiscTypeId + "::MaxSize", QueueSizeValue(QueueSize("100p")));

    // Install queue discipline on router
    TrafficControlHelper tch;
    tch.SetRootQueueDisc(qdiscTypeId);
    QueueDiscContainer qd;
    tch.Uninstall(routers.Get(0)->GetDevice(0));
    qd.Add(tch.Install(routers.Get(0)->GetDevice(0)).Get(0));

    // Enable BQL
    tch.SetQueueLimits("ns3::DynamicQueueLimits");

    // Install packet sink at receiver side
    for (uint32_t i = 0; i < N2; i++)
    {
        uint16_t port = 50000 + i;
        InstallPacketSink(rightNodes.Get(i), port, "ns3::TcpSocketFactory");
    }

    for (uint32_t i = 0; i < N1; i++)
    {
        uint16_t port = 50000 + i;
        InstallBulkSend(leftNodes.Get(i),
                        routerToRightIPAddress[i].GetAddress(1),
                        port,
                        socketFactory,
                        flowSize[i],
                        leftNodes.Get(i)->GetId(),
                        0);
    }

    // Set the stop time of the simulation
    Simulator::Stop(stopTime);

    // Start the simulation
    Simulator::Run();

    // Write the flow completion time (FCT) to a file
    std::ofstream fctOut("lv1-results/fct/fct.dat", std::ios::out | std::ios::trunc);   // clear the file if it exists
    for (uint32_t i = 0; i < N1; i++){
        fctOut << finishTime[leftNodes.Get(i)->GetId()] - startTime.GetMicroSeconds() << std::endl;
    }
    fctOut.close();

    // Cleanup and close the simulation
    Simulator::Destroy();

    return 0;
}
