#include "filesystem.hpp"
//the following is from Stack Overflow R Samuel Klatchko https://stackoverflow.com/questions/2868680/what-is-a-cross-platform-way-to-get-the-current-directory
std::string getMycwd(){
	char buffer[180];
	char *answer = getcwd(buffer, sizeof(buffer));
	std::string cwd;
	if(answer){
		cwd = answer;
	}
	return cwd;
}

void dup2Call(){
	int newFd = 181;
	int fd = open("../test.txt", O_CREAT | O_RDWR | O_PATH);
	int actualFd = dup2(fd, newFd);
	if(actualFd != -1){//success
		close(fd);
		close(actualFd);
	}else{
		close(fd);
	}
}

void chmodCall(bool permission){
	std::string path = "/proc/self/exe";
	if(permission){
		//remove read permission
		chmod(path.c_str(), S_IWUSR | S_IXUSR);
	}else{//add read permission
		chmod(path.c_str(), S_IWUSR | S_IXUSR | S_IRUSR);
	}
}

void chdirCall(){
	chdir(getMycwd().c_str());
}

void accessCall(){
	std::string path = "/proc/self/exe";
	access(path.c_str(), F_OK);
	access(path.c_str(), R_OK);
	access(path.c_str(), W_OK);
	access(path.c_str(), X_OK);
}

void syncCall(){
	sync();
}
