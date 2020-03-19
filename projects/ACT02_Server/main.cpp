#include "net/socketutils.h"
#include "net/commands.h"
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

            std::pair<COMUNICATION_COMMAND, std::string> response = Command::getCommand(std::string(buffer));

            if (response.first == COMUNICATION_COMMAND::START) {
                sendOptions(buffer);
            }
            else {
                srand(time(NULL));
                std::string result = options.at(rand() % options.size());
                int winner = chooseWinner(std::string(buffer), result);
                _stage++;
                if (winner != RESULT::DRAW)
                    _result[winner == RESULT::LOSE ? 1 : 0] += 1;
                generateResponse(buffer, winner, response.second, result);
                if (_stage >= NUMBER_OF_GAMES) {
                    break;
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            if (_error[0] < 0 && _error[1] < 0) {
                std::cout << "Client Disconnected" << std::endl;
                break;
            }
        }
        int res = 0;
        if (_result[0] > _result[1]) {
            res = 1;
        }
        else if (_result[0] < _result[1]) {
            res = -1;
        }
        createResponse(buffer, COMUNICATION_COMMAND::FINISH, std::to_string(res));
        _connSocket.release();
        delete this;
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
        void sendOptions(char* buffer) {
            for (std::string op : options) {
                createResponse(buffer, COMUNICATION_COMMAND::OPTIONS, op);
            }
            generateResponse(buffer, 0, "", "", true);
        }

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
        
        void createResponse(char* buffer, COMUNICATION_COMMAND command, std::string message) {
            sprintf(buffer, Command::setCommand(command, message).c_str());
            _error[1] = _connSocket->sendTo(buffer, std::string(buffer).size() + 1);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        void generateResponse(char* buffer, int winner, std::string player, std::string decission, bool firstStage = false) {
            createResponse(buffer, COMUNICATION_COMMAND::STAGE, std::to_string(_stage));
            if (!firstStage) {
                createResponse(buffer, COMUNICATION_COMMAND::WINNER, std::to_string(winner));
                createResponse(buffer, COMUNICATION_COMMAND::SELECTION, player + " - " + decission);
            }
            createResponse(buffer, COMUNICATION_COMMAND::RESULT, std::to_string(_result[0]) + "-" + std::to_string(_result[1]));

            if (_stage < NUMBER_OF_GAMES) {
                createResponse(buffer, COMUNICATION_COMMAND::NEXT_STAGE, "");
            }
        }
};

int main() {
    SocketUtils::init();

    {
        srand(time(NULL));
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
