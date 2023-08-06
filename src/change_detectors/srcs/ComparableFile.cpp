//
// Created by Stepan Usatiuk on 05.05.2023.
//


#include "ComparableFile.h"

#include <fstream>
#include <sstream>

#include "Exception.h"
#include "objects/FileBuffer.h"

ComparableFile::ComparableFile(const File &file, const Repository *repo)
    : path(file.name), type(file.fileType), bytes(file.bytes), mtime(file.mtime),
      contents([file, repo]() { return std::make_unique<FileBuffer>(repo, file.id); }) {}

ComparableFile::ComparableFile(const std::filesystem::path &p, const std::filesystem::path &base)
    : path(p.lexically_relative(base).u8string()), type(File::getFileType(p)), bytes(File::getFileSize(p)),
      mtime(File::getFileMtime(p)),
      contents([p, path = this->path, type = this->type]() -> std::unique_ptr<std::streambuf> {
          if (type == File::Type::Normal) {
              auto fb = std::make_unique<std::filebuf>();
              fb->open(p, std::ios::in | std::ios::binary);
              if (!fb->is_open()) throw Exception("Can't open " + p.u8string() + " for reading!");
              return fb;
          }

          auto contentsVector = File::getFileContents(p);
          std::string contents = {contentsVector.begin(), contentsVector.end()};

          return std::make_unique<std::stringbuf>(contents, std::ios::in | std::ios::binary);
      }) {}
