#include <iostream>
#include <iomanip>
#include <stdexcept>

#include "DIKUgrafik/ofile.h"
#include "DIKUgrafik/ofileopr.h"


int main()
{
/*******************************************************************\
*                                                                   *
*            W r i t e   t o   a n   O u t p u t F i l e            *
*                                                                   *
\*******************************************************************/

    try {
	OutputFile outputfile("odata");
	
	for (int i = 1; i <= 10; ++i) {
	    outputfile << " " << i;
	}
	
	std::cout << "Data written to file: \"" << outputfile.Filename() << "\"" << std::endl;
	std::cout << "The numbers: 1 2 3 4 5 6 7 8 9 10" << std::endl;

	return 0;
    }
    catch (std::exception const& Exception) {
	std::cout << Exception.what() << std::endl;
    }
    return -1;
}
