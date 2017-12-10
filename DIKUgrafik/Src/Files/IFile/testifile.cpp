#include <iostream>
#include <iomanip>
#include <stdexcept>

#include "DIKUgrafik/traceinfo.h"
#include "DIKUgrafik/ifile.h"


int main()
{
    Trace("", "main()");

    try {

/*******************************************************************\
*                                                                   *
*                             No Such File                          *
*                                                                   *
\*******************************************************************/

	try {
	    Trace("", "try: No Such File");

	    TraceMessage("Open a non-existing file: odata" << std::endl;);
	    InputFile inputfile("odata");
	    inputfile.CheckStatus();
	}
	catch (std::exception const& Exception) {
	    std::cout << Exception.what() << std::endl;
	}
	    
/*******************************************************************\
*                                                                   *
*                           Empty InputFile                         *
*                                                                   *
\*******************************************************************/

	try {
	    Trace("", "try: Empty infile");

	    TraceMessage("Open an empty file: idata" << std::endl;);

	    InputFile inputfile("idata");
	    inputfile.CheckStatus();

	    while (!inputfile.eof()) {
		int tmpvalue = int();
		inputfile >> tmpvalue;
		if (!inputfile.fail()) {
		    TraceMessage("value read = " << tmpvalue << std::endl;);
		}
	    }
	    inputfile.Close();
	}
	catch (std::exception const& Exception) {
	    std::cout << std::endl << "main: " << Exception.what() << std::endl;
	}

/*******************************************************************\
*                                                                   *
*                    Unexpected Eof on InputFile                    *
*                                                                   *
\*******************************************************************/

	try {
	    Trace("", "Unexpected Eof on InputFile");

	    TraceMessage("Read 10 numbers from a file which only contains 7" << std::endl;);

	    InputFile inputfile("idatashort");
	    inputfile.CheckStatus();

	    for (int i = 1; i <= 10; ++i) {
		int tmpvalue = int();
		inputfile >> tmpvalue;
		if (!inputfile.fail()) {
		    TraceMessage("value read = " << tmpvalue << std::endl;);
		}
	    }
	    inputfile.Close();
	}
	catch (std::exception const& Exception) {
	    std::cout << std::endl << "main: " << Exception.what() << std::endl;
	}

/*******************************************************************\
*                                                                   *
*          B a d   d a t a   i n   a n   I n p u t F i l e          *
*                                                                   *
\*******************************************************************/

	try {
	    Trace("", "Bad data in InputFile");
	    
	    InputFile inputfile("idatabad");
	    inputfile.CheckStatus();

#if 0
	    bool readdata = true;
	    while (readdata) {
		int tmpvalue = int();
		inputfile >> tmpvalue;
		if (inputfile.fail()) {
		    TraceMessage("fail on idatabad" << std::endl;);
		    inputfile.CheckStatus();
		    if (!inputfile.eof()) {
			TraceMessage("clear the error" << std::endl;);
			inputfile.clear();
			inputfile.CheckStatus();
		    }
		    readdata = false;
		}
		else {
		    TraceMessage("value read = " << tmpvalue << std::endl;);
		}
	    }
#else
	    while (!inputfile.bad_input()) {
		int tmpvalue = int();
		inputfile >> tmpvalue;
		if (inputfile.good()) {
		    TraceMessage("value read = " << tmpvalue << std::endl;);
		}
	    }
#endif
	    inputfile.Close();
	}
	catch (std::exception const& Exception) {
	    std::cout << std::endl << "::main: Exception: " << Exception.what() << std::endl;
	}
    }
    catch (std::exception const& Exception) {
	std::cout << std::endl << "Excepton: ::main loop: " << Exception.what() << std::endl;
    }

    return 0;
}
