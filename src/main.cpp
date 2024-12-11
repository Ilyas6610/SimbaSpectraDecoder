#include "PcapFileParser.h"
#include "SimbaPacketDecoder.h"

int main(){
    std::vector<std::string> paths = {  
        "/home/ilyas/src/data/2023-10-09.1849-1906.pcap"
        // "/home/ilyas/proj/data/2023-10-09.2349-2355.pcap",
        // "/home/ilyas/proj/data/2023-10-10.0439-0450.pcap",
        // "/home/ilyas/proj/data/2023-10-10.0845-0905.pcap",
        // "/home/ilyas/proj/data/2023-10-10.0959-1005.pcap",
        // "/home/ilyas/proj/data/2023-10-10.1359-1406.pcap",
        // "/home/ilyas/proj/data/2023-10-10.1849-1906.pcap"
    };
    for(auto path: paths){
        PcapFile<SimbaPacket> pcapFile(path.c_str(), std::make_unique<SimbaPacketDecoder>());
        std::ofstream ofs(path+".json");
        ofs << '[';
        while(!pcapFile.eof()){
            if(auto record = pcapFile.getNextRecord())
                for(const auto& message: record->messages)
                    std::cout << message.second->getMessageAsString() << ",\n";
        }
        if(auto pos = ofs.tellp(); pos != 1)
            ofs.seekp(ofs.tellp()-2);
        ofs << "]\n";
        ofs.close();
    }
    return 0;
}