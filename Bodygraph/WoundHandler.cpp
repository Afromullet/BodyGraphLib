#include <fstream>
#include <iostream>
#include<boost/tokenizer.hpp>
#include<boost/lexical_cast.hpp>
#include "WoundHandler.hpp"


using namespace boost;
//Due to the way I parse the CSV, this will make filling the WoundProperties easier. I'm using boost tokenizer to handle the CSV file, where the tokenized
//Values are traversed with an iterator. We want to construct WoundProperties as we iterate. Since the order is guaranteed, we can use this vector
//to create the WoundProperties vector
std::vector<WoundTypes::EnWoundProperties> wound_property_vector = boost::assign::list_of(WoundTypes::EnWoundProperties::enBleed)(WoundTypes::EnWoundProperties::enFracture)(WoundTypes::EnWoundProperties::enRupture);

WoundTypes::WoundPossibleMap WoundTypes::process_wound_characteristics_file(std::string fname)
{
	std::string line;
	std::ifstream myfile(fname);
	WoundPossibleMap woundsPossibleMap;

	if (myfile.is_open())
	{
		//Reading the first line, containing header information. 
		getline(myfile, line);
		
		if (line != WOUND_CHARACTERISTICS_HEADER)
		{
			//Todo handle error better
			std::cout << "Error processing. Invalid header." << fname << std::endl;
			return woundsPossibleMap;
		}
		
		while (getline(myfile, line))
		{
			
			tokenizer<escaped_list_separator<char> > tk(line, escaped_list_separator<char>('\\', ',', '\"'));
			int index = 0; //The order of the wound characteristics file won't change. So we can use the index to know which enumerated type to create the WoundProperties with by using it as an index into wound_property_vector

			WoundTypes::WoundProperties woundProps;
			std::string bodypartToken = "";
			for (tokenizer<escaped_list_separator<char> >::iterator i(tk.begin()); i != tk.end(); ++i)
			{
				if (index == 0)
				{
					bodypartToken = *i;
				}
				else if (*i == "1" || *i == "0" && index > 0)
				{											
					//Subtracting 1 from the index because the first time it's incremented is when we read the bodypart Token. 
					woundProps.push_back(std::make_pair(wound_property_vector.at(index-1), boost::lexical_cast<bool>(*i)));
				
				}
				else
				{
					std::cout << "Invalid boolean values";
					//todo handle error better;
				}
				 index++;				
			}

			//This reaches the end of the line for one bodypart token, so we can add to the map and reset all data. 
			woundsPossibleMap.insert(std::pair(bodypartToken, woundProps));
			bodypartToken = "";
			woundProps.clear();
			index = 0;
			
			
		}
		myfile.close();
		


	}

	return woundsPossibleMap;

}

/*
Use the last element of the std::list<AppliedForceEffect> to calculate the wound severity.
The last element is the force remaining after it has been absorbed by armor etc.

Todo add a function to get the last element instead of specifically calling it

void WoundTypes::CalculateWoundSeverity(AppliedForceBodypartPair &appliedForcePair)
{

}
*/