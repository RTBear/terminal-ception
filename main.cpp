//Ryan Mecham A01839282

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>//fork()
#include <sys/types.h>//wait()
#include <sys/wait.h>//wait()
#include <cstring>

std::vector<std::string> getInput();
void callProgram(std::vector<std::string> newArgv);
void printHistory(std::vector<std::vector<std::string>> &history);//print a history of commands
void runCommand(int commandNum, std::vector<std::vector<std::string>> &history, bool &blowThePopsicleStand);//runs a certain command from the history
void printVector(std::vector<std::string> vec);
void runArgs(std::vector<std::string> args, std::vector<std::vector<std::string>> &history, bool &blowThePopsicleStand);



int main(){
	bool blowThePopsicleStand = false;
	std::vector<std::vector<std::string>> history;

	while(!blowThePopsicleStand){
		auto args = getInput();
		runArgs(args, history, blowThePopsicleStand);
	}

	return 0;
}

void runArgs(std::vector<std::string> args, std::vector<std::vector<std::string>> &history, bool &blowThePopsicleStand){
	if(args.size() > 0){
		history.push_back(args);
		if(args[0] == "exit"){
			blowThePopsicleStand = true;
		}else if(args[0] == "ptime"){
			std::cout << "it's ptime!\n";
		}else if(args[0] == "history"){
			printHistory(history);
		}else if(args[0] == "^"){
			history.pop_back();
			if (args.size() > 1){
				std::stringstream checkInt(args[1]);
				int commandNum;
				if((checkInt >> commandNum).fail()){//verify argument is int
					std::cout << "Invalid Argument: Second Arg to '^' must be a number.\n";	
				}else{
					runCommand(commandNum, history, blowThePopsicleStand);
				}
			}else{
				std::cout << "Insufficient Arguments: No defined behavior for '^' by itself.\n";
			}
		}else{
			callProgram(args);
		}
	}
}

void printVector(std::vector<std::string> vec){
	for(int i = 0; i < vec.size(); i++){
		std::cout << vec[i];
		if(i != vec.size() - 1) {
			std::cout << " ";
		}
	}
}

void runCommand(int commandNum, std::vector<std::vector<std::string>> &history, bool &blowThePopsicleStand){
	if(commandNum > 0){
		if(history.size() > 0){
			printVector(history[commandNum - 1]);
			std::cout << "\n";
			runArgs(history[commandNum - 1], history, blowThePopsicleStand);
		}else{
			std::cout << "Error: No History Yet\n";
		}
	}else{
		std::cout << "Invalid Argument: Number must be greater than zero.\n";
	}
}

void printHistory(std::vector<std::vector<std::string>> &history){
	std::cout << "-- Command History -- \n\n";
	for(int i = 0; i < history.size(); i++){
		std::vector<std::string> command = history[i];
		std::cout << i + 1 << " : ";
		printVector(command);
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
}

//const char* getPath(std::string program){
//	int succ = execvp("/usr/bin/which"
//}

//this function handles calling of an external program
void callProgram(std::vector<std::string> newArgv){
	int wstatus;
	
	const char *path = (char*)newArgv[0].c_str();

	//convert vector to carray
	char* charArgv[newArgv.size() + 1];// make room to add nullptr to end of array
	for(int i = 0; i < newArgv.size(); i++){
		charArgv[i] = (char*)newArgv[i].c_str();
	}
	//ex...
	//newArgv:  [1, 2, 3, 4]
	//charargv: [1, 2, 3, 4, nullptr]

	charArgv[newArgv.size()] = nullptr;

	auto result = fork();
	if(result == 0){//child
//		std::cout << "path: " << path << "\n";
//		std::cout << "argv: ";
//		for(int i = 0; i < newArgv.size(); i++){
//			std::cout << charArgv[i] << " ";
//		}
		
		int status = execvp(path, charArgv);
		std::cout << "\nAn error occured executing program: " << path << std::endl;
		exit(0);
	}else{//parent
		wait(&wstatus);//reap dead children
	}
}
