//
// Created by Stepan Usatiuk on 06.05.2023.
//

#include "Diff.h"

#include "BuzhashChunker.h"
#include "BytesFormatter.h"
#include "Exception.h"
#include "Signals.h"

bool Diff::isBinary(const ComparableFile &c) {
    auto b = c.contents();
    for (unsigned int i = 0; i < std::min(c.bytes, 2048ULL); i++) {
        auto e = b->sbumpc();
        if (std::streambuf::traits_type::to_char_type(e) == '\0') return true;
        if (e == std::streambuf::traits_type::eof()) return false;
    }
    return false;
}

std::string Diff::diff(const ComparableFile &c1, const ComparableFile &c2) {
    if (isBinary(c1) || isBinary(c2)) {
        if (!(isBinary(c1) && isBinary(c2))) return "One of the files is binary, the other is not";
        return diffPercent(c1, c2);
    }

    std::stringstream out;
    auto b1 = c1.contents();
    auto b2 = c2.contents();
    std::multimap<std::string, unsigned long> f1lines;
    std::multimap<std::string, unsigned long> f2diff;
    std::string line;
    std::istream is1(b1.get());
    std::istream is2(b2.get());

    int i = 0;
    while (std::getline(is1, line)) {
        /// Exit when asked to
        if (Signals::shouldQuit) throw Exception("Quitting");
        f1lines.emplace(line, ++i);
    }

    i = 0;
    while (std::getline(is2, line)) {
        /// Exit when asked to
        if (Signals::shouldQuit) throw Exception("Quitting");
        if (f1lines.count(line) > 0) f1lines.erase(f1lines.find(line));
        else
            f2diff.emplace(line, ++i);
    }

    out << "\nLines only in first file: " << std::endl;
    for (const auto &s: f1lines) { out << s.second << "<" << s.first << std::endl; }
    out << "Lines only in second file: " << std::endl;
    for (const auto &s: f2diff) { out << s.second << ">" << s.first << std::endl; }
    out << "^^^\n";
    return out.str();
}

std::string Diff::diffPercent(const ComparableFile &c1, const ComparableFile &c2) {
    auto b1 = c1.contents();
    auto b2 = c2.contents();
    BuzhashChunker ch1(b1.get(), 512 * 1024, 1024 * 1024, 19, 31);
    BuzhashChunker ch2(b2.get(), 512 * 1024, 1024 * 1024, 19, 31);
    std::multiset<std::string> ch1hashes;
    std::multiset<std::string> ch2diff;
    std::unordered_map<std::string, unsigned long long> hashsize;
    for (auto chunkp: ch1) {
        /// Exit when asked to
        if (Signals::shouldQuit) throw Exception("Quitting");
        if (chunkp.second.empty()) continue;
        std::string SHA(chunkp.first.begin(), chunkp.first.end());
        ch1hashes.emplace(SHA);
        hashsize[SHA] = chunkp.second.size();
    }

    for (auto chunkp: ch2) {
        /// Exit when asked to
        if (Signals::shouldQuit) throw Exception("Quitting");
        if (chunkp.second.empty()) continue;
        std::string SHA(chunkp.first.begin(), chunkp.first.end());
        hashsize[SHA] = chunkp.second.size();
        if (ch1hashes.count(SHA) > 0) ch1hashes.erase(SHA);
        else if (ch1hashes.count(SHA) == 0)
            ch2diff.emplace(SHA);
    }

    unsigned long long diff = 0;

    for (const auto &c: ch1hashes) { diff += hashsize[c]; }
    for (const auto &c: ch2diff) { diff += hashsize[c]; }

    return "at most " + BytesFormatter::formatStr(diff);
}
