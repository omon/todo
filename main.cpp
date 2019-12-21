#include <iostream>
#include <map>
#include <functional>
#include <cstdbool>
#include <memory>

class Command{
private:
    std::string description;
    std::function<bool(std::vector<std::string>)> func;
public:
    Command(const std::string &description_, const std::function<bool(std::vector<std::string>)> &func_){
        description = description_;
        func = func_;
    }
    Command() = delete;
    bool execute(const std::vector<std::string> &args){
        return func(args);
    }
    const std::string &get_description(){
        return description;
    }
};

class Parser{
private:
    std::map<std::string, std::unique_ptr<Command>> cmds;
public:
    void add(const std::string &cmd, const std::string &description, const std::function<bool(std::vector<std::string>)> &func){
        cmds[cmd] = std::make_unique<Command>(description, func);
    }
    void exec(const std::string &command, const std::vector<std::string> &args){
       if(cmds.count(command) == 0){
           std::cout << "[" << command << "] command is not registered. \nsee help command.\n";
           return ;
       }
        cmds[command]->execute(args);
    }
    void show_usage(){
        std::cout << "[Usage]:\n";
        for(const auto &[key, value] : cmds){
            std::cout << key << " : " << value->get_description() << "\n";
        }
    }

};

int main(int argc, char* argv[]) {
    Parser parser;

    // == register commands. ==========
    parser.add("help", "show usage.", [&](std::vector<std::string> arg){
        parser.show_usage();
        return true;
    });

    parser.add("add", "add todo.", [](std::vector<std::string> arg){
        std::cout << "excute add command.\n";
       for(const auto &v: arg){
           std::cout << v << "\n";
       }
       return true;
    });

    if(argc == 1){
        std::cout << "input command .... show usage.\n";
        parser.show_usage();
        return EXIT_SUCCESS;
    }


    std::vector<std::string> args;
    for (int j = 2; j < argc; ++j) {
       args.push_back(std::string(argv[j]));
    }

    // execute commands
    parser.exec(argv[1], args);
    return EXIT_SUCCESS;
}