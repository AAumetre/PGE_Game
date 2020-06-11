#include "Logger.h"

size_t log_count = 0;
size_t warn_count = 0;

bool output_to_std_output = true;

void log(TrackedObject* sender_, std::string msg_)
{
	std::ostringstream header;
	header << "LOG " << log_count;
	buildMessage(header.str(), sender_->getUID(), sender_->getName(), msg_);
	log_count++;
}

void warn(TrackedObject* sender_, std::string msg_)
{
	std::ostringstream header;
	header << "WARNING " << warn_count;
	buildMessage(header.str(), sender_->getUID(), sender_->getName(), msg_);
	warn_count++;
}

void buildMessage(std::string header_, int ID_, std::string name_, std::string msg_)
{
	std::ostringstream complete_msg;
	complete_msg << header_ << " from " << name_ << " with ID: " << ID_ << ".\t" << msg_;
	outputMessage(complete_msg.str());
}

void outputMessage(std::string msg)
{
	if (output_to_std_output)
		std::cout << msg << std::endl;
	//else
		// Write into file
}