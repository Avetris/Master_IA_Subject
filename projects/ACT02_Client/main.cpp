#include "net/socketutils.h"
#include "net/socketaddress.h"
#include "net/commands.h"
#include <thread>
#include <vector>
#include <string>


std::vector<std::string> options;
int result[2] = { 0 };

void printOptions() {
    std::cout << "Select an option (write 'exit' to end): " << std::endl;
    for (int i = 0; i < options.size(); i++) {
        std::cout << i << " - " << options.at(i) << std::endl;
    }
}

std::string checkInput(bool randomInteraction) {
    std::string selectedOption;
    if (randomInteraction) {
        selectedOption = options.at(rand() % options.size());
    }
    else {
        std::string input;
        std::cin >> input;
        if (input == "exit") {
            selectedOption = COMUNICATION_COMMAND::FINISH;
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

void checkResponse(std::pair<COMUNICATION_COMMAND, std::string> response) {
    switch (response.first)
    {
    case OPTIONS:
        options.push_back(response.second);
        break;
    case SELECTION:
    {
        std::string word = "";
        for (char c : response.second) {
            if (c == '-') {
                std::cout << "You: " << word << std::endl;
                word = "";
            }
            else {
                word += c;
            }
        }
        std::cout << "Other: " << word << std::endl;
        break;

    }
    case STAGE:
    {
        std::cout << "Stage: " << response.second << std::endl;
    }
        break;
    case WINNER:
    {
        int res = std::atoi(response.second.c_str());
        if (res == 1) {
            std::cout << "You have won the stage!" << std::endl;
        }
        else if (res == -1) {
            std::cout << "You have lost the stage!" << std::endl;
        }
        else {
            std::cout << "There has been a draw in the stage!" << std::endl;
        }
    }
        break;
    case RESULT:
    {
        std::cout << "Current Result: " << response.second.c_str() << std::endl;
    }
        break;
    }
}

int main() {
    SocketUtils::init();

    {
        srand(time(NULL));
        std::string response;

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
        COMUNICATION_COMMAND command = COMUNICATION_COMMAND::NONE;

        while (command != COMUNICATION_COMMAND::FINISH && result[0] >= 0 && result[1] >= 0) {

            if (options.size() == 0 && command != COMUNICATION_COMMAND::OPTIONS) {
                sprintf(buffer, Command::setCommand(COMUNICATION_COMMAND::START, "").c_str());
                result[0] = socket->sendTo(buffer, std::string(buffer).size() + 1);

                std::cout << "Starting the Game" << std::endl;
            }
            result[1] = socket->receiveFrom(buffer, bufferLength);

            std::string response = std::string(buffer);

            std::pair<COMUNICATION_COMMAND, std::string> current_command = Command::getCommand(response);

            if (!current_command.second.empty() && current_command.first != COMUNICATION_COMMAND::FINISH) {
                checkResponse(current_command);            }
            else {
                bool finish = false;

                switch (current_command.first)
                {
                    case NEXT_STAGE:
                    {
                        std::cout << std::endl << "------------------------------" << std::endl << std::endl;

                        if (!randomInteraction) {
                            printOptions();
                        }
                        sprintf(buffer, checkInput(randomInteraction).c_str());
                        result[0] = socket->sendTo(buffer, std::string(buffer).size() + 1);
                        command = COMUNICATION_COMMAND::NONE;
                        break;
                    }
                    case FINISH:
                    {
                        command = current_command.first;
                        std::cout << std::endl << "The match has finished!" << std::endl;
                        int res = std::atoi(current_command.second.c_str());
                        if (res > 0) {
                            std::cout << "You have won the match!" << std::endl;
                        }
                        else if (res < 0) {
                            std::cout << "You have lost the match!" << std::endl;
                        }
                        else {
                            std::cout << "There has been a drawn in the match!" << std::endl;
                        }
                        finish = true;
                        break;
                    }
                    default:
                    {
                        command = current_command.first;
                        break;
                    }
                }
                if (finish) {
                    break;
                }
            }
        }
    }

    SocketUtils::shutdown();
}

