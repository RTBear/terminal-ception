//Ryan Mecham A01839282

#include <iostream>

int main(int argc, char** argv){

	std::cout << "[cmd]: ";

	return 0;
}

//this function handles calling of an external program
int callProgram(int newArgc, char** newArgv){
	auto result = fork();
	int wstatus;
	if(result == 0){//child
		exit(0);
	}else{//parent
		wait(&wstatus);//reap dead children
	}
}
