
#ifndef COMMAND_H
#define COMMAND_H 1

#include <string>

const int COMUNICATION_LENGTH = 9;

const enum COMUNICATION_COMMAND
{
    NONE,
    START,
    NEXT_STAGE,
    FINISH,
    OPTIONS,
    SELECTION,
    STAGE,
    WINNER,
    RESULT,
};
const char* COMUNICATION_COMMAND_STRING[] = {
    "NONE",
    "START",
    "NEXT_STAGE",
    "FINISH",
    "OPTIONS",
    "SELECTION",
    "STAGE",
    "WINNER",
    "RESULT"
};

const char COMMAND_SPLITER = ':';

class Command {
  public:
    static std::pair<COMUNICATION_COMMAND, std::string> getCommand(std::string message) {
        COMUNICATION_COMMAND command  = COMUNICATION_COMMAND::NONE;

        std::string word;
        for (char c : message) {
            if (c == COMMAND_SPLITER) {
                for (int i = 0; i < COMUNICATION_LENGTH; i++) {
                    if (word._Equal(COMUNICATION_COMMAND_STRING[i])) {
                        command = static_cast<COMUNICATION_COMMAND>(i);
                        break;
                    }
                }
                word = "";
            }
            else {
                word += c;
            }
        }
      
        return std::pair< COMUNICATION_COMMAND, std::string>(command, word);
    }
    static std::string setCommand(COMUNICATION_COMMAND command, std::string message) {
        std::string returnMessage = "";
        if (command >= 0 && command < COMUNICATION_LENGTH) {
            returnMessage = COMUNICATION_COMMAND_STRING[command];
            returnMessage += COMMAND_SPLITER;
            returnMessage += message;
        }
        return returnMessage;
    }
};

#endif
