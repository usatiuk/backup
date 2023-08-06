//
// Created by Stepan Usatiuk on 30.04.2023.
//

#include "ChunkerFactory.h"

#include "BuzhashChunker.h"
#include "ConstChunker.h"
#include "Exception.h"

std::unique_ptr<Chunker> ChunkerFactory::getChunker(const Config &config, std::streambuf *buf) {
    if (config.getStr("chunker") == "const") {
        return std::make_unique<ConstChunker>(buf, config.getInt("chunker-max") * 1024);
    } else if (config.getStr("chunker") == "buzhash") {
        return std::make_unique<BuzhashChunker>(buf, config.getInt("chunker-min") * 1024,
                                                config.getInt("chunker-max") * 1024, config.getInt("chunker-mask"));
    } else {
        throw Exception("Unknown chunker type!");
    }
}
