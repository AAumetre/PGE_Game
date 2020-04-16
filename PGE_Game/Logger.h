#pragma once
#include "AllIncludes.h"
#include "TrackedObject.h"

void log(TrackedObject* sender_, std::string msg_);
void warn(TrackedObject* sender_, std::string msg_);

void buildMessage(std::string header_, int ID_, std::string name_, std::string msg_);
void outputMessage(std::string msg);