#ifndef __MESSAGE_MANAGER_H__
#define __MESSAGE_MANAGER_H__ 1

#include "ia/defines.h"

#include <cstdint>
#include <unordered_map>

class MessageManager {
  public:
    ~MessageManager() {}
    MessageManager(MessageManager const&) = delete;

    static MessageManager& instance() {
        static MessageManager instance;
        return  instance;
    }

    void changeStack() {
        _messages[_stack].clear();
        _stack = abs(_stack -1);
    }
    void addMessage(Message msg) {        
        _messages[abs(_stack - 1)].push_back(msg);
    }

    std::vector<const Message*> getMessage(AgentType type, uint32_t UID) const{
        std::vector<const Message*> msg;
        for (auto it = _messages[_stack].begin(); it < _messages[_stack].end(); it++) {
            if ((*it).receiver == type && (UID == 0 || (*it).uid == 0 || (*it).uid == UID)) {
                msg.push_back(&(*it));
            }
        }
        return msg;
    }

  private:
      MessageManager() {}
    
    uint8_t _stack = 0;
    std::vector<Message> _messages[2];
};

#endif
