//
// Created by Stepan Usatiuk on 23.05.2023.
//

#include "../includes/CommandList.h"

CommandList::CommandList() : Command() {
}

void CommandList::run(Context ctx) {
    auto list = ctx.repo->getObjects(Object::ObjectType::Archive);
    std::sort(list.begin(), list.end(), [](const auto &l, const auto &r) { return l.second < r.second; });
    for (auto const &aid: list) {
        std::cout << "Name: " << aid.first << " Id: " << aid.second << std::endl;
    }
}
