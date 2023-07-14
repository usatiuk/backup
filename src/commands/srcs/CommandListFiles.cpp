//
// Created by Stepan Usatiuk on 23.05.2023.
//

#include "../includes/CommandListFiles.h"

#include "../../repo/includes/Serialize.h"
#include "../../repo/includes/objects/Archive.h"
#include "../../repo/includes/objects/Chunk.h"
#include "../../repo/includes/objects/File.h"
#include "../../utils/includes/BytesFormatter.h"

CommandListFiles::CommandListFiles() : Command() {
}

void CommandListFiles::run(Context ctx) {
    auto archive = Serialize::deserialize<Archive>(ctx.repo->getObject(ctx.repo->getConfig().getInt("aid")));
    for (auto const &fid: archive.files) {
        auto file = Serialize::deserialize<File>(ctx.repo->getObject(fid));
        std::cout << "Name: " << file.name << " type: " << File::TypeToStr.at(file.fileType) << " size: " << BytesFormatter::formatStr(file.bytes) << std::endl;
    }
}
