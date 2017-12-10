#include "DIKUgrafik/ofileplain.h"


/*
 * \class OutputFile
 * OutputFile is a utility class which makes it easy to open, close, and write to a file.
 */

/*
 * Parameterized constructor, creates an instance of an OutputFile and opens an external file given by a filename
 * for writing.
 * \param Filename - a string containing the name of the file to be opened.
 */
OutputFile::OutputFile(std::string const& Filename) : filename(Filename), open(false)
{
    Trace("OutputFile", "OutputFile(std::string&");

    this->Open(this->filename);
}

/*
 * Destructor, closes the external file and destroys the instance of OutputFile.
 */
OutputFile::~OutputFile()
{
    Trace("OutputFile", "~OutputFile()");

    this->Close();
}

/*
 * Opens an external file given by the parameter. If there already exists an open file, it is closed before the 
 * new one is opened.
 * \param Filename - a string containing the name of the file to be opened.
 */
void OutputFile::Open(std::string const& Filename)
{
    Trace("OutputFile", "Open(std::string&)");

    if (this->open == true) {
	if (this->filename != Filename) {
	    this->Close();
	    this->filename = "";
	}
    }

    if (this->open == false) {
	this->filename = Filename;
	this->std::ofstream::open(Filename.c_str());
	if (!*this) throw std::runtime_error(std::string("Cannot open file: ") + this->filename);
	this->open = true;
    }
}

/*
 * Closes the external file if it is open.
 */
void OutputFile::Close()
{
    Trace("OutputFile", "Close()");

    if (this->open == false) {
	this->std::ofstream::close();
	this->open = false;
    }
}

/*
 * Returns the name of the external file.
 * \return the name of the currently open external file.
 */
std::string OutputFile::Filename() const
{
    Trace("OutputFile", "Filename()");

    return this->filename;
}

#if 0
/*
 * Checks the status of the currently open OutputFile. If some error has occurred on the external file
 * a runtime_error exception is thrown.
 */
void OutputFile::CheckStatus()
{
    Trace("OutputFile", "CheckStatus()");

    if (!(*this)) {
	std::ostringstream errormessage;
	errormessage << "Error on writing to file: \"" << this->Filename() << "\": ";
#if 0
	if (this->eof()) {
	    errormessage << "End of File" << std::ends;
	}
	else {
	    errormessage << "Bad Input" << std::ends;
	}
	TraceMessage("OutputFile::CheckStatus: " << errormessage.str() << std::endl;);
	throw std::runtime_error(errormessage.str());
#endif
    }
}
#endif
