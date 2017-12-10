#include "DIKUgrafik/traceinfo.h"


int TotalErrors = 0;


void TestTrace()
{
    std::cout << "*** Begin Trace --- Test of the Trace Macros ***" << std::endl << std::endl;

    try {
        Trace("TestTrace", "outer block");

        TraceMessage("about to enter inner block ..." << std::endl);
        {
            Trace("TestTrace", "inner block");

            
            TraceMessage("int N  = 5;" << std::endl);
            int N = 5;
            CondTraceMessage(N > 0,
                             "Variable N = " << std::setw(2) << N << " >  0" << std::endl,
                             "Variable N = " << std::setw(2) << N << " <= 0" << std::endl);

            TraceMessage("N -= 10;" << std::endl);
            N -= 10;
            CondTraceMessage(N > 0,
                             "Variable N = " << std::setw(2) << N << " >  0" << std::endl,
			     "Variable N = " << std::setw(2) << N << " <= 0" << std::endl);
        }
        TraceMessage("returned from the inner block ..." << std::endl);
    }
    catch(std::exception& Exception) {
        std::cout << "Exception: " << Exception.what() << std::endl;
    }

    std::cout << std::endl << "*** End Trace --- Test of the Trace Macros ***" << std::endl << std::endl;
}

int main(int argc, char** argv)
{
    ::TestTrace();

    return 0;
}
