//
// Created by Stepan Usatiuk on 06.05.2023.
//

#ifndef SEMBACKUP_DIFF_H
#define SEMBACKUP_DIFF_H

#include <sstream>
#include <string>

#include "ComparableFile.h"

/// Utility class to compute difference between two ComparableFile%s
class Diff {
public:
    /// Compute the difference between two ComparableFile%s
    /// If the file is binary, calls diffPercent, which outputs the difference between files in bytes
    /// Otherwise prints linewise difference
    /// \param c1 Constant reference to the first ComparableFile
    /// \param c2 Constant reference to the second ComparableFile
    /// \returns  Difference message
    static std::string diff(const ComparableFile &c1, const ComparableFile &c2);

    /// Calculates the difference between \p c1 amd \p c2 in bytes
    /// \param c1 Constant reference to the first ComparableFile
    /// \param c2 Constant reference to the second ComparableFile
    /// \returns  Difference message
    static std::string diffPercent(const ComparableFile &c1, const ComparableFile &c2);

    /// Checks if a file is binary
    /// A file is considered binary if its first 2048 bytes contain a null byte
    /// \param c1 Constant reference to the checked ComparableFile
    /// \return True if the file is considered binary, false otherwise
    static bool isBinary(const ComparableFile &c1);
};


#endif//SEMBACKUP_DIFF_H
