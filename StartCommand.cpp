#include "Command.h"
#include "HandleClient.h"

class StartCommand: public Command {
    virtual void execute(vector<string> args, int clientSocket1,int clientSocket2, map<string,int> &games) {
        games.insert(pair<string,int>(args[0],clientSocket1));

    }
};
