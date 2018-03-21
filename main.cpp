#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>//fork()
#include <sys/types.h>//wait()
#include <sys/wait.h>//wait()
#include <sys/time.h>//gettimeofday()
#include <cstring>
#include <ctime>//ptime()
#include <chrono>//ptime()
#include <csignal>//ignoreSig()

#include "filesystem.hpp"

const int WRITE = 1;
const int READ = 0;

std::vector<std::string> getInput();
void callProgram(std::vector<std::string> newArgv);
void printHistory(std::vector<std::vector<std::string>> &history);//print a history of commands
void runCommand(int commandNum, std::vector<std::vector<std::string>> &history, bool &blowThePopsicleStand);//runs a certain command from the history
void printVector(std::vector<std::string> vec);
void runArgs(std::vector<std::string> args, std::vector<std::vector<std::string>> &history, bool &blowThePopsicleStand);
std::string formatTime(timeval now);
std::string makeNum2Digit(int num);
int countPipes(std::vector<std::string> args);
std::vector<std::vector<std::string>> parsePipes(std::vector<std::string> args);

timeval beginRunTime;

//ignore ctrl-c
void ignoreSig(int signal){
	//do nothing :)
}

int main(){
	signal(SIGINT, ignoreSig);

	bool blowThePopsicleStand = false;
	std::vector<std::vector<std::string>> history;

	gettimeofday(&beginRunTime, NULL);

	while(!blowThePopsicleStand){
		auto args = getInput();
		int pipeCount = countPipes(args);
		if(pipeCount == 1){//can be changed to pipeCount > 0 later
			//create pipe
			//int p[2];
			//pipe(p);
			auto pipedArgs = parsePipes(args);

			std::cout << "piped args\n";

			printHistory(pipedArgs);//debugging

			//runArgs(args, history, blowThePopsicleStand);

		}
	//	runArgs(args, history, blowThePopsicleStand);
	}

	return 0;
}

std::vector<std::vector<std::string>> parsePipes(std::vector<std::string> args){
	std::vector<std::vector<std::string>> pipedArgs;
	std::vector<std::string> parsedVec;

	for(int i = 0; i < args.size(); i++){
		if(args[i] != "|"){
			parsedVec.push_back(args[i]);
		}else{
			pipedArgs.push_back(parsedVec);
			parsedVec.clear();
		}
	}
	if(parsedVec.size() > 0){
		pipedArgs.push_back(parsedVec);
	}
	return pipedArgs;
}

std::string makeNum2Digit(int num){
	std::string numStr;
	if(num < 10){
		numStr = "0" + std::to_string(num);
	}else{
		numStr = std::to_string(num);
	}
	return numStr;
}

std::string formatTime(timeval now){
	double runTime = now.tv_sec - beginRunTime.tv_sec;

	int hours = runTime / 3600;
	int minutes = (int)(runTime / 60) % 60;
	int seconds = (int)runTime % 60;

	std::string s_hrs = makeNum2Digit(hours);
	std::string s_min = makeNum2Digit(minutes);
	std::string s_sec = makeNum2Digit(seconds);

	std::string time = s_hrs + ":" + s_min + ":" + s_sec;

	return time;
}

int countPipes(std::vector<std::string> args){
	int pipeCount = 0;
	for(int i = 0; i < args.size(); i++){
		if(args[i] == "|"){
			pipeCount++;
		}
	}
	return pipeCount;
}

void runArgs(std::vector<std::string> args, std::vector<std::vector<std::string>> &history, bool &blowThePopsicleStand){
	static double elapsed_seconds = 0;
	if(args.size() > 0){
		std::string cmd = args[0];
		history.push_back(args);
		if(cmd == "exit"){
			blowThePopsicleStand = true;
		}else if(cmd == "ptime"){
			std::cout << "Time spent executing child process: " << std::setprecision(4) << std::fixed << elapsed_seconds << " seconds\n";
		}else if(cmd == "history"){
			printHistory(history);
		}else if(cmd == "^"){
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
		}else if(cmd == "living_time"){
			timeval now;
			gettimeofday(&now, NULL);
			std::cout << "Shell up time: " << formatTime(now) << "\n";
		}else if(cmd == "cd"){
			if(args.size() > 1){
				chdir(args[1].c_str());	
			}else{
				std::cout << "Insufficient Arguments: No defined behavior for 'cd' by itself.\n";
			}
		}else{
			auto start = std::chrono::system_clock::now();
			callProgram(args);
			auto end = std::chrono::system_clock::now();
			std::chrono::duration<double> diff = end - start;
			elapsed_seconds = diff.count();
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
	std::cout << "[" << getMycwd() << "/shell]: ";

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
