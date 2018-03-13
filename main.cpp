//Ryan Mecham A01839282

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>//fork()
#include <sys/types.h>//wait()
#include <sys/wait.h>//wait()

std::vector<std::string> getInput();
int callProgram(std::vector<std::string> newArgv);
void printHistory(std::vector<std::vector<std::string>> history);//print a history of commands
void runCommand(int commandNum);//runs a certain command from the history

int main(){

	bool blowThePopsicleStand = false;
	std::vector<std::vector<std::string>> history;

	while(!blowThePopsicleStand){
		auto args = getInput();
		history.push_back(args);
		if(args[0] == "exit"){
			return 0;
		}else if(args[0] == "ptime"){
			std::cout << "it's ptime!\n";
		}else if(args[0] == "history"){
			printHistory(history);
		}else if(args[0] == "^"){
			if (args.size() > 1){
				std::stringstream checkInt(args[1]);
				int commandNum;
				if((checkInt >> commandNum).fail()){//verify argument is int
					std::cout << "Invalid Argument: Second Arg to '^' must be a number.\n";	
				}else{
					runCommand(commandNum);
				}
			}else{
				std::cout << "Insufficient Arguments: No defined behavior for '^' by itself.\n";
			}
		}

	}

	return 0;
}

void runCommand(int commandNum){
	std::cout << "running command number " << commandNum << "...\n";
}

void printHistory(std::vector<std::vector<std::string>> history){
	std::cout << "-- Command History -- \n\n";
	for(int i = 0; i < history.size(); i++){
		std::vector<std::string> command = history[i];
		std::cout << i + 1 << " : ";
		for(int j = 0; j < command.size(); j++){
			std::cout << command[j];
		}
		std::cout << std::endl;
	}
}

//get input from user via prompt
std::vector<std::string> getInput(){
	std::cout << "[cmd]: ";

	std::string input;
	std::getline(std::cin,input);

	std::vector<std::string> newArgv;//list of args from prompt
	std::stringstream tokenizer(input);//stream to hold them
	std::string token;//temp variable for individual tokens
	//get tokens
	while(getline(tokenizer, token, ' ')){
		newArgv.push_back(token);
	}

	return newArgv;
	// debugging, print newArgv
	//	for(int i = 0; i < newArgv.size(); i++){
	//		std::cout << newArgv[i] << "\n";
	//	}
}

//this function handles calling of an external program
int callProgram(std::vector<std::string> newArgv){
	auto result = fork();
	int wstatus;
	if(result == 0){//child
		exit(0);
	}else{//parent
		wait(&wstatus);//reap dead children
	}
}
