#ifndef __RASTERMAN_OFILE_OPR_H
#define __RASTERMAN_OFILE_OPR_H

/**
 * \file
 */

#include "DIKUgrafik/traceinfo.h"
#include "DIKUgrafik/ofileplain.h"

/**
 * Utility template operator which makes it easy to open, close, and write to a file.
 * \param file - the file to be written to.
 * \param value - the value to be written.
 * \return a reference to the OutputFile written to.
 */
template <typename Type>
OutputFile& operator<<(OutputFile& file, Type& value);

#include "DIKUgrafik/ofileopr.impl"

#endif
