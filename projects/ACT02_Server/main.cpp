#include "net/socketutils.h"
#include "net/socketaddress.h"
#include <string>
#include <thread>
#include <vector>

const std::vector<std::string> options = {
    "Rock", "Paper", "Scissors", "Lizard", "Spock"
};

class Game {
public:
    void init() {

    }

private:
    SocketAddress _clientAddress;
};

std::vector<Game> listGames;

const enum RESULT {
    WIN = 1,
    LOSE = -1,
    DRAW = 0
};

int chooseWinner(std::string player, std::string decission) {
    int result = RESULT::DRAW;
    if (player._Equal(decission)) {
        result = RESULT::DRAW;
    }else if (player._Equal(options[0])) {
        if (decission._Equal(options[1])) {
            result = RESULT::LOSE;
        }
        else if (decission._Equal(options[2])) {
            result = RESULT::WIN;
        }
        else if (decission._Equal(options[3])) {
            result = RESULT::WIN;
        }
        else if (decission._Equal(options[4])) {
            result = RESULT::LOSE;
        }
    }
    else if (player._Equal(options[1])) {
        if (decission._Equal(options[0])) {
            result = RESULT::WIN;
        }
        else if (decission._Equal(options[2])) {
            result = RESULT::LOSE;
        }
        else if (decission._Equal(options[3])) {
            result = RESULT::LOSE;
        }
        else if (decission._Equal(options[4])) {
            result = RESULT::WIN;
        }
    }
    else if (player._Equal(options[2])) {
        if (decission._Equal(options[1])) {
            result = RESULT::WIN;
        }
        else if (decission._Equal(options[0])) {
            result = RESULT::LOSE;
        }
        else if (decission._Equal(options[3])) {
            result = RESULT::WIN;
        }
        else if (decission._Equal(options[4])) {
            result = RESULT::LOSE;
        }
    }
    else if (player._Equal(options[3])) {
        if (decission._Equal(options[1])) {
            result = RESULT::WIN;
        }
        else if (decission._Equal(options[2])) {
            result = RESULT::LOSE;
        }
        else if (decission._Equal(options[0])) {
            result = RESULT::LOSE;
        }
        else if (decission._Equal(options[4])) {
            result = RESULT::WIN;
        }
    }
    else if (player._Equal(options[4])) {
        if (decission._Equal(options[1])) {
            result = RESULT::LOSE;
        }
        else if (decission._Equal(options[2])) {
            result = RESULT::WIN;
        }
        else if (decission._Equal(options[3])) {
            result = RESULT::LOSE;
        }
        else if (decission._Equal(options[0])) {
            result = RESULT::WIN;
        }
    }
    return result;
}

std::string generateResponse(int stage, int result, int* globalResult, std::string player, std::string decission) {
    std::string returnResult = std::to_string(stage) + ";";
    returnResult += std::to_string(result) + "/";
    returnResult += player + "/";
    returnResult += decission + ";";
    returnResult += std::to_string(globalResult[0]) + "/";
    returnResult += std::to_string(globalResult[1]);
    return returnResult;
}

int _stage = 0;
int _result[2] = { 0 };

int main() {
    SocketUtils::init();

    {
        TCPSocketPtr socket = SocketUtils::createTCPSocket(SocketUtils::INET);

        SocketAddressPtr address = SocketUtils::createIPv4FromString("127.0.0.1:9090");

        socket->bindTo(*address.get());
        socket->listenTo();

        SocketAddress addressIn;
        TCPSocketPtr connSocket = socket->acceptCon(addressIn);

        constexpr size_t bufferLength = 255;
        char buffer[bufferLength];
        
        for (;;) {
            connSocket->receiveFrom(buffer, bufferLength);
            std::cout << "Received = " << buffer << std::endl;

            if (std::string(buffer) == "exit") {
                std::cout << "Exiting = " << buffer << std::endl;
                break;
            }

            //std::this_thread::sleep_for(std::chrono::milliseconds(500));

            std::string response = std::string(buffer);

            if (response._Equal("start")) {
                std::string optionsSring = "";
                for (std::string op : options) {
                    if (!optionsSring.empty()) {
                        optionsSring += ";";
                    }
                    optionsSring += op;
                }
                sprintf(buffer, optionsSring.c_str());
            }
            else if (response._Equal("exit")) {
                sprintf(buffer, "end");
            }
            else {
                std::string result = options.at(rand() % options.size());
                int winner = chooseWinner(std::string(buffer), result);
                _stage++;
                if(winner != 0)
                    _result[winner == -1 ? 1 : 0] += 1;
                sprintf(buffer, generateResponse(_stage, winner, _result, response, result).c_str());
            }
            connSocket->sendTo(buffer, std::string(buffer).size() + 1);

            //std::this_thread::sleep_for(std::chrono::milliseconds(10000));
        }
    }

    SocketUtils::shutdown();
}
