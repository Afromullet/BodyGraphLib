#pragma once

#include <iostream>
#include <fstream>
#include <string>


struct UnitTestFileHandler
{

public:
	UnitTestFileHandler(std::string _filename)
	{
		filename = _filename;
		if (ofstream.is_open())
		{
			ofstream.close();
		}
		ofstream = std::ofstream(filename);
	}

	void write_unit_test_fail_information(std::string functionName, std::string messsage)
	{
		ofstream << functionName << " " << messsage << std::endl;

	}

	void closefile()
	{
		if (ofstream.is_open())
		{
			ofstream.close();
		}
	}

private:
	std::string filename;
	std::ofstream ofstream;

};