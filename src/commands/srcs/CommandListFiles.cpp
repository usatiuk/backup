//
// Created by Stepan Usatiuk on 23.05.2023.
//

#include "CommandListFiles.h"

#include "BytesFormatter.h"
#include "Serialize.h"
#include "objects/Archive.h"
#include "objects/Chunk.h"
#include "objects/File.h"

CommandListFiles::CommandListFiles() : Command() {
}

void CommandListFiles::run(Context ctx) {
    auto archive = Serialize::deserialize<Archive>(ctx.repo->getObject(ctx.repo->getConfig().getInt("aid")));
    for (auto const &fid: archive.files) {
        auto file = Serialize::deserialize<File>(ctx.repo->getObject(fid));
        std::cout << "Name: " << file.name << " type: " << File::TypeToStr.at(file.fileType) << " size: " << BytesFormatter::formatStr(file.bytes) << std::endl;
    }
}
