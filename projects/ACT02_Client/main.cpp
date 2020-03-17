#include "net/socketutils.h"
#include "net/socketaddress.h"
#include <thread>
#include <vector>
#include <string>

std::vector<std::string> options;


void getOptions(std::string response) {
    std::string opt = "";
    uint8_t wordPos = 0;
    for (char c : response) {
        if (c == ';' || wordPos == response.size() - 1) {
            if (wordPos == response.size() - 1) {
                opt += c;
            }
            options.push_back(opt);
            opt = "";
        }
        else {
            opt += c;
        }
        wordPos++;
    }
}

void printOptions() {
    std::cout << "Select an option (write 'exit' to end): " << std::endl;
    for (int i = 0; i < options.size(); i++) {
        std::cout << i << " - " << options.at(i) << std::endl;
    }
}

std::string checkInput(bool randomInteraction) {
    std::string selectedOption;
    if (randomInteraction) {
        srand(time(NULL));
        selectedOption = options.at(rand() % options.size());
    }
    else {
        std::string input;
        std::cin >> input;
        if (input == "exit") {
            selectedOption = "exit";
        }
        for (int i = 0; i < options.size() && selectedOption.empty(); i++) {
            if (input == std::to_string(i) || input == options.at(i)) {
                selectedOption = options.at(i);
            }
        }
        if (selectedOption.empty()) {
            std::cout << "You have not selected a valid option." << std::endl;
            printOptions();
            selectedOption = checkInput(randomInteraction);
        }
    }
    return selectedOption;
}

void printStageResult(std::string value) {
    std::string word;
    uint8_t pos = 0;
    uint8_t wordPos = 0;
    for (char c : value) {
        if (c == '/' || wordPos == value.size() - 1) {
            if (wordPos == value.size() - 1) {
                word += c;
            }
            if (pos == 0) {
                if (std::atoi(word.c_str()) == 1) {
                    std::cout << "You have won the stage!" << std::endl;
                }
                else if (std::atoi(word.c_str()) == -1) {
                    std::cout << "You have lost the stage!" << std::endl;
                }
                else {
                    std::cout << "There has been a draw in the stage!" << std::endl;
                }
            }
            else if (pos == 1) {
                std::cout << "You: " << word << std::endl;
            }
            else if (pos == 2) {
                std::cout << "Other: " << word << std::endl;
            }
            pos++;
            word = "";
        }
        else {
            word += c;
        }
        wordPos++;
    }
    std::cout << std::endl << "------------------------------" << std::endl << std::endl;
}

int checkResponse(int* result, std::string response) {
    std::string val = "";
    uint8_t part = 0;
    uint8_t wordPos = 0;
    int stage = 0;
    for (char c : response) {
        if (c == ';' || wordPos == response.size() - 1) {
            if (wordPos == response.size() - 1) {
                val += c;
            }
            switch (part)
            {
            case 0:
                stage = std::atoi(val.c_str());
                break;
            case 1:
                printStageResult(val);
                break;
            case 2:
            {
                std::string res = "";
                uint8_t resPos = 0;
                uint8_t resWordPos = 0;
                for (char v : val) {
                    if (v == '/' || resWordPos == val.size() - 1) {
                        if (resWordPos == val.size() - 1) {
                            res += v;
                        }
                        result[resPos] = std::atoi(res.c_str());
                        resPos++;
                        res = "";
                    }
                    else {
                        res += v;
                    }
                    resWordPos++;
                }
                break;
            }
            case 3:
                if (result[0] > result[1]) {
                    std::cout << "You have won the match" << std::endl;
                }
                else if (result[0] < result[1]) {
                    std::cout << "You have lost the match" << std::endl;
                }
                else {
                    std::cout << "There has been a drawn in the match" << std::endl;
                }
                stage = -1;
                break;
            }

            part++;
            val = "";
        }
        else {
            val += c;
        }
        wordPos++;
    }
    return stage;
}

int main() {
    SocketUtils::init();

    {
        std::string response;

        int stage = 0;
        int result[2] = { 0 };

        std::cout << "Do you want to play? (In caso of saying no, there will be a match between machines)" << std::endl;
        std::string res ;
        while (!res._Equal("yes") && !res._Equal("no")) {
            std::cout << "(yes/no)";
            std::cin >> res;
        }
        bool randomInteraction = res._Equal("no");


        TCPSocketPtr socket = SocketUtils::createTCPSocket(SocketUtils::INET);

        SocketAddressPtr address = SocketUtils::createIPv4FromString("127.0.0.1:9090");

        socket->connectTo(*address.get());

        constexpr size_t bufferLength = 255;
        char buffer[bufferLength];

        while (!response._Equal("end") && (result[0] >= 0 || result[1] >= 0)) {
            if (options.size() == 0) {
                sprintf(buffer, "start");
                result[0] = socket->sendTo(buffer, std::string(buffer).size() + 1);
                result[1] = socket->receiveFrom(buffer, bufferLength);

                getOptions(std::string(buffer));

                std::cout << "Starting the Game" << std::endl;
            }

            std::cout << "Stage: " << stage << " - Result: " << result[0] << " - " << result[1] << std::endl;


            std::string selectedOption;
            if (!randomInteraction) {
                printOptions();
            }
            selectedOption = checkInput(randomInteraction);

            sprintf(buffer, selectedOption.c_str());
            result[0] = socket->sendTo(buffer, std::string(buffer).size() + 1);

            result[1] = socket->receiveFrom(buffer, bufferLength);
            
            response = std::string(buffer);

            if (!response._Equal("end")) {
                stage = checkResponse(result, response);

                if (stage < 0) {
                    break;
                }
            }

            //std::this_thread::sleep_for(std::chrono::milliseconds(10000));
        }
    }

    SocketUtils::shutdown();
}

