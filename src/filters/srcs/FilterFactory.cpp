//
// Created by Stepan Usatiuk on 22.04.2023.
//

#include "FilterFactory.h"

#include "CheckFilter.h"
#include "Exception.h"
#include "FilterAES.h"
#include "FilterShift.h"
#include "FilterShiftSecret.h"
#include "FilterZlib.h"

std::unique_ptr<Filter> FilterFactory::makeFilter(const std::string &type, const Config &config) {
    if (type == "none") throw Exception("Trying to make a \"none\" filter!");

    if (type == "aes") {
        return std::make_unique<FilterAES>(config.getStr("password"), config.getStr("salt"));
    } else if (type == "zlib") {
        return std::make_unique<FilterZlib>(config.getInt("compression-level"));
    } else if (type == "crc") {
        return std::make_unique<CheckFilter>();
    } else if (type == "shiftC") {
        return std::make_unique<FilterShift>(config.getInt("compression-level"));
    } else if (type == "shiftE")
        return std::make_unique<FilterShiftSecret>(config.getStr("password"), config.getStr("salt"));

    throw Exception("Unknown filter value");
}
