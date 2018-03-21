#pragma once
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

std::string getMycwd();

void dup2Call();

void chmodCall(bool permission);

void getCwdCall();

void chdirCall();

void accessCall();

void syncCall();
