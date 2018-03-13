//Ryan Mecham A01839282

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>//fork()
#include <sys/types.h>//wait()
#include <sys/wait.h>//wait()
#include <stdio.h>//strtok()
#include <string.h>//strtok()

std::vector<std::string> getInput();
int callProgram(std::vector<std::string> newArgv);

int main(){

	bool blowThePopsicleStand = false;

	while(!blowThePopsicleStand){
		auto args = getInput();
		if(args[0] == "exit"){
			return 0;
		}
	}

	return 0;
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
