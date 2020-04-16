#include "Logger.h"

bool output_to_std_output = true;

void log(TrackedObject* sender_, std::string msg_)
{
	buildMessage("LOG", sender_->getUID(), sender_->getName(), msg_);
}

void warn(TrackedObject* sender_, std::string msg_)
{
	buildMessage("WARNING", sender_->getUID(), sender_->getName(), msg_);
}

void buildMessage(std::string header_, int ID_, std::string name_, std::string msg_)
{
	std::ostringstream complete_msg;
	complete_msg << header_ << " from\t" << name_ << " with ID:\t" << ID_ << ".\t" << msg_;
	outputMessage(complete_msg.str());
}

void outputMessage(std::string msg)
{
	if (output_to_std_output)
		std::cout << msg << std::endl;
	//else
		// Write into file
}