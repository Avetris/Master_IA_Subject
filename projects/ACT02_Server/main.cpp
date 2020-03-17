#include "net/socketutils.h"
#include "net/socketaddress.h"
#include <string>
#include <thread>
#include <vector>
#include <memory>
#include <mutex>

const std::vector<std::string> options = {
    "Rock", "Paper", "Scissors", "Lizard", "Spock"
};
const int NUMBER_OF_GAMES = 20;
class Game {
public:
    Game(TCPSocketPtr connSocket) {
        _connSocket = std::move(connSocket);
    }

    void run() {
        constexpr size_t bufferLength = 255;
        char buffer[bufferLength];
        for (;;) {
            _error[0] = _connSocket->receiveFrom(buffer, bufferLength);
            std::cout << "Received = " << buffer << std::endl;

            if (std::string(buffer) == "exit") {
                std::cout << "Exiting = " << buffer << std::endl;
                break;
            }

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
                srand(time(NULL));
                std::string result = options.at(rand() % options.size());
                int winner = chooseWinner(std::string(buffer), result);
                _stage++;
                if (winner != RESULT::DRAW)
                    _result[winner == RESULT::LOSE ? 1 : 0] += 1;
                sprintf(buffer, generateResponse(winner, response, result).c_str());
            }
            _error[1] = _connSocket->sendTo(buffer, std::string(buffer).size() + 1);

            if (response._Equal("exit")) {
                disconnect(false);
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            if (_error[0] < 0 && _error[1] < 0) {
                std::cout << "Client Disconnected" << std::endl;
                disconnect(false);
                break;
            }
        }
    }

    private:
        const enum RESULT {
            WIN = 1,
            LOSE = -1,
            DRAW = 0
        };
        TCPSocketPtr _connSocket;
        int _stage = 0;
        int _result[2] = { 0 };
        int _error[2] = { 0 };
        int chooseWinner(std::string player, std::string decission) {
            int result = RESULT::DRAW;
            if (player._Equal(decission)) {
                result = RESULT::DRAW;
            }
            else if (player._Equal(options[0])) {
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

        std::string generateResponse(int result, std::string player, std::string decission) {
            std::string returnResult = std::to_string(_stage) + ";";
            returnResult += std::to_string(result) + "/";
            returnResult += player + "/";
            returnResult += decission + ";";
            returnResult += std::to_string(_result[0]) + "/";
            returnResult += std::to_string(_result[1]);

            if (_stage > NUMBER_OF_GAMES) {
                returnResult += ";end";
            }
            return returnResult;
        }

        void disconnect(bool sayToClient) {
            if (sayToClient) {
                constexpr size_t bufferLength = 255;
                char buffer[bufferLength];

                sprintf(buffer, "end");

                _connSocket->sendTo(buffer, std::string(buffer).size() + 1);
            }
            delete this;
        }
};

int main() {
    SocketUtils::init();

    {
        TCPSocketPtr socket = SocketUtils::createTCPSocket(SocketUtils::INET);

        SocketAddressPtr address = SocketUtils::createIPv4FromString("127.0.0.1:9090");

        socket->bindTo(*address.get());
        socket->listenTo();

        for (;;) {
            SocketAddress addressIn;
            TCPSocketPtr connSocket = socket->acceptCon(addressIn);
            Game* g = new Game(std::move(connSocket));
            std::thread thread = std::thread(&Game::run, g);
            thread.detach();
        }
    }

    SocketUtils::shutdown();
}
