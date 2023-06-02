//
// Created by Stepan Usatiuk on 16.04.2023.
//

#include "FullTest.h"
#include "../../src/Logger.h"
#include "../../src/commands/CommandRestore.h"
#include "../../src/commands/CommandRun.h"
#include "../../src/repo/FileRepository.h"
#include "../utils/Cleaner.h"

#include <filesystem>
#include <functional>
#include <string>

void randomChange(std::string path) {
    std::fstream s(path);
    auto pos = rand() % 50;
    s.seekp(pos, std::ios::beg);
    char out = rand() % 254;
    s.write(&out, 1);
}

std::unique_ptr<TestGroup> FullTest::operator()() {
    auto tg = std::make_unique<TestGroup>("Full tests");

    tg->addTest(std::make_unique<Test>("Simple backup/restore test", std::function<bool()>([]() {
                                           Cleaner c({"testfrom", "testto", "testtores"});
                                           int aid = -1;
                                           {
                                               std::filesystem::create_directories("testfrom");
                                               for (int i = 0; i < 257; i++) {
                                                   std::ofstream o(std::filesystem::path("testfrom") / ("f" + std::to_string(i)), std::ios::binary | std::ios::out | std::ios::trunc);
                                                   for (int j = 0; j < i; j++) o.put(j % 256);
                                               }
                                               std::filesystem::create_directories("testfrom/testdir");

                                               Config conf;
                                               conf.add("repo", "testto").add("from", "testfrom");

                                               auto repo = std::make_unique<FileRepository>(conf);
                                               repo->init();

                                               std::stringstream runnerout;
                                               Logger logger(1, runnerout);

                                               CommandRun run;
                                               run.run(Context{&logger, repo.get()});

                                               aid = repo->getObjects(Object::ObjectType::Archive).begin()->second;
                                           }
                                           {
                                               Config conf;
                                               conf.add("repo", "testto").add("aid", std::to_string(aid)).add("to", "testtores");

                                               auto repo = std::make_unique<FileRepository>(conf);
                                               repo->open();
                                               std::stringstream runnerout;
                                               Logger logger(1, runnerout);

                                               CommandRestore cmd;
                                               cmd.run(Context{&logger, repo.get()});
                                           }
                                           {
                                               try {
                                                   HelpfulAssert<bool>()(std::filesystem::is_directory("testtores/testdir"), true);
                                               } catch (...) {
                                                   std::cerr << "Empty directory doesn't exist!" << std::endl;
                                                   throw;
                                               }

                                               for (int i = 0; i < 257; i++) {
                                                   std::ifstream o(std::filesystem::path("testtores") / ("f" + std::to_string(i)), std::ios::binary | std::ios::in);
                                                   try {
                                                       HelpfulAssert<bool>()(o.is_open(), true);
                                                       for (int j = 0; j < i; j++) {
                                                           char c;
                                                           HelpfulAssert<bool>()(o.get(c).operator bool(), true);
                                                           HelpfulAssert<char>()(j % 256, c);
                                                       }
                                                   } catch (...) {
                                                       std::cerr << "Error comparing file " << i << std::endl;
                                                       throw;
                                                   }
                                               }
                                           }


                                           return true;
                                       })));
    tg->addTest(std::make_unique<Test>("Simple backup/restore with ignore test", std::function<bool()>([]() {
                                           Cleaner c({"testfrom", "testto", "testtores"});
                                           int aid = -1;
                                           {
                                               std::filesystem::create_directories("testfrom");
                                               for (int i = 0; i < 257; i++) {
                                                   std::ofstream o(std::filesystem::path("testfrom") / ("f" + std::to_string(i)), std::ios::binary | std::ios::out | std::ios::trunc);
                                                   for (int j = 0; j < i; j++) o.put(j % 256);
                                               }
                                               std::filesystem::create_directories("testfrom/testdir");
                                               std::filesystem::create_directories("testfrom/testdir2/testdir3");
                                               std::filesystem::create_directories("testfrom/testdir2/testdir4");
                                               {
                                                   std::ofstream file("testfrom/testdir2/.ignore");
                                                   file << "hello.txt";
                                               }
                                               {
                                                   std::ofstream file("testfrom/testdir2/testdir3/.ignore");
                                                   file << ".*\\.txt";
                                               }
                                               {
                                                   std::ofstream file("testfrom/testdir2/hello.txt");
                                                   file << "hello";
                                               }
                                               {
                                                   std::ofstream file("testfrom/testdir2/testdir3/hello.txt");
                                                   file << "hello2";
                                               }
                                               {
                                                   std::ofstream file("testfrom/testdir2/testdir3/asdf.txt");
                                                   file << "asdf1";
                                               }
                                               {
                                                   std::ofstream file("testfrom/testdir2/testdir4/asdf.txt");
                                                   file << "asdf2";
                                               }

                                               Config conf;
                                               conf.add("repo", "testto").add("from", "testfrom");

                                               auto repo = std::make_unique<FileRepository>(conf);
                                               repo->init();
                                               std::stringstream runnerout;
                                               Logger logger(1, runnerout);

                                               CommandRun cmd;
                                               cmd.run(Context{&logger, repo.get()});
                                               aid = repo->getObjects(Object::ObjectType::Archive).begin()->second;
                                           }
                                           {
                                               Config conf;
                                               conf.add("repo", "testto").add("aid", std::to_string(aid)).add("to", "testtores");

                                               auto repo = std::make_unique<FileRepository>(conf);
                                               repo->open();
                                               std::stringstream runnerout;
                                               Logger logger(1, runnerout);

                                               CommandRestore cmd;
                                               cmd.run(Context{&logger, repo.get()});
                                           }
                                           {
                                               HelpfulAssert<bool>()(std::filesystem::is_directory("testtores/testdir"), true);

                                               for (int i = 0; i < 257; i++) {
                                                   std::ifstream o(std::filesystem::path("testtores") / ("f" + std::to_string(i)), std::ios::binary | std::ios::in);
                                                   HelpfulAssert<bool>()(o.is_open(), true);
                                                   for (int j = 0; j < i; j++) {
                                                       char c;
                                                       HelpfulAssert<bool>()(o.get(c).operator bool(), true);
                                                       HelpfulAssert<char>()(j % 256, c);
                                                   }
                                               }

                                               {
                                                   std::ifstream file("testtores/testdir2/.ignore");
                                                   std::string s;
                                                   file >> s;
                                                   HelpfulAssert<std::string>()(s, "hello.txt");
                                               }
                                               {
                                                   std::ifstream file("testtores/testdir2/testdir3/.ignore");
                                                   std::string s;
                                                   file >> s;
                                                   HelpfulAssert<std::string>()(s, ".*\\.txt");
                                               }
                                               {
                                                   std::ifstream file("testtores/testdir2/hello.txt");
                                                   HelpfulAssert<bool>()(!file, true);
                                               }
                                               {
                                                   std::ifstream file("testtores/testdir2/testdir3/hello.txt");
                                                   HelpfulAssert<bool>()(!file, true);
                                               }
                                               {
                                                   std::ifstream file("testtores/testdir2/testdir3/asdf.txt");
                                                   HelpfulAssert<bool>()(!file, true);
                                               }
                                               {
                                                   std::ifstream file("testtores/testdir2/testdir4/asdf.txt");
                                                   std::string s;
                                                   file >> s;
                                                   HelpfulAssert<std::string>()(s, "asdf2");
                                               }
                                           }


                                           return true;
                                       })));

    tg->addTest(std::make_unique<Test>("Backup/restore test with compression", std::function<bool()>([]() {
                                           Cleaner c({"testfrom", "testto", "testtores"});
                                           int aid = -1;
                                           {
                                               std::filesystem::create_directories("testfrom");
                                               for (int i = 0; i < 257; i++) {
                                                   std::ofstream o(std::filesystem::path("testfrom") / ("f" + std::to_string(i)), std::ios::binary | std::ios::out | std::ios::trunc);
                                                   for (int j = 0; j < i; j++) o.put(j % 256);
                                               }
                                               std::filesystem::create_directories("testfrom/testdir");

                                               Config conf;
                                               conf.add("repo", "testto").add("compression", "zlib").add("from", "testfrom");

                                               auto repo = std::make_unique<FileRepository>(conf);
                                               repo->init();
                                               std::stringstream runnerout;
                                               Logger logger(1, runnerout);

                                               CommandRun cmd;
                                               cmd.run(Context{&logger, repo.get()});
                                               aid = repo->getObjects(Object::ObjectType::Archive).begin()->second;
                                           }
                                           {
                                               Config conf;
                                               conf.add("repo", "testto").add("aid", std::to_string(aid)).add("to", "testtores");

                                               auto repo = std::make_unique<FileRepository>(conf);
                                               repo->open();
                                               std::stringstream runnerout;
                                               Logger logger(1, runnerout);

                                               CommandRestore cmd;
                                               cmd.run(Context{&logger, repo.get()});
                                           }
                                           {
                                               HelpfulAssert<bool>()(std::filesystem::is_directory("testtores/testdir"), true);

                                               for (int i = 0; i < 257; i++) {
                                                   std::ifstream o(std::filesystem::path("testtores") / ("f" + std::to_string(i)), std::ios::binary | std::ios::in);
                                                   HelpfulAssert<bool>()(o.is_open(), true);
                                                   for (int j = 0; j < i; j++) {
                                                       char c;
                                                       HelpfulAssert<bool>()(o.get(c).operator bool(), true);
                                                       HelpfulAssert<char>()(j % 256, c);
                                                   }
                                               }
                                           }


                                           return true;
                                       })));
    tg->addTest(std::make_unique<Test>("Backup/restore test with compression/encryption", std::function<bool()>([]() {
                                           Cleaner c({"testfrom", "testto", "testtores"});
                                           int aid = -1;
                                           {
                                               std::filesystem::create_directories("testfrom");
                                               for (int i = 0; i < 257; i++) {
                                                   std::ofstream o(std::filesystem::path("testfrom") / ("f" + std::to_string(i)), std::ios::binary | std::ios::out | std::ios::trunc);
                                                   for (int j = 0; j < i; j++) o.put(j % 256);
                                               }
                                               std::filesystem::create_directories("testfrom/testdir");

                                               Config conf;
                                               conf.add("repo", "testto").add("compression", "zlib").add("from", "testfrom").add("encryption", "aes").add("password", "testp").add("salt", "tests");

                                               auto repo = std::make_unique<FileRepository>(conf);
                                               repo->init();

                                               std::stringstream runnerout;
                                               Logger logger(1, runnerout);

                                               CommandRun cmd;
                                               cmd.run(Context{&logger, repo.get()});
                                               aid = repo->getObjects(Object::ObjectType::Archive).begin()->second;
                                           }
                                           {
                                               Config conf;
                                               conf.add("repo", "testto");

                                               auto repo = std::make_unique<FileRepository>(conf);

                                               bool ok = true;
                                               try {
                                                   HelpfulAssert<bool>()(repo->open(), false);
                                                   ok = false;
                                               } catch (...) {}
                                               HelpfulAssert<bool>()(ok, true);
                                           }
                                           {
                                               Config conf;
                                               conf.add("repo", "testto").add("password", "testp").add("aid", std::to_string(aid)).add("to", "testtores");

                                               auto repo = std::make_unique<FileRepository>(conf);
                                               repo->open();

                                               std::stringstream runnerout;
                                               Logger logger(1, runnerout);

                                               CommandRestore cmd;
                                               cmd.run(Context{&logger, repo.get()});
                                           }
                                           {
                                               HelpfulAssert<bool>()(std::filesystem::is_directory("testtores/testdir"), true);

                                               for (int i = 0; i < 257; i++) {
                                                   std::ifstream o(std::filesystem::path("testtores") / ("f" + std::to_string(i)), std::ios::binary | std::ios::in);
                                                   HelpfulAssert<bool>()(o.is_open(), true);
                                                   for (int j = 0; j < i; j++) {
                                                       char c;
                                                       HelpfulAssert<bool>()(o.get(c).operator bool(), true);
                                                       HelpfulAssert<char>()(j % 256, c);
                                                   }
                                               }
                                           }


                                           return true;
                                       })));
    tg->addTest(std::make_unique<Test>("Repo data fuzz test", std::function<bool()>([]() {
                                           srand(time(nullptr));
                                           std::vector<Config> confs;
                                           Config conf;
                                           conf.add("repo", "testto").add("compression", "none").add("from", "testfrom").add("encryption", "none").add("password", "testp").add("salt", "tests").add("progress", "none");
                                           confs.emplace_back(conf);
                                           conf = Config();
                                           conf.add("repo", "testto").add("compression", "zlib").add("from", "testfrom").add("encryption", "none").add("password", "testp").add("salt", "tests").add("progress", "none");
                                           confs.emplace_back(conf);
                                           conf = Config();
                                           conf.add("repo", "testto").add("compression", "none").add("from", "testfrom").add("encryption", "zlib").add("password", "testp").add("salt", "tests").add("progress", "none");
                                           confs.emplace_back(conf);
                                           conf = Config();
                                           conf.add("repo", "testto").add("compression", "zlib").add("from", "testfrom").add("encryption", "aes").add("password", "testp").add("salt", "tests").add("progress", "none");
                                           confs.emplace_back(conf);

                                           for (auto const &conf: confs) {
                                               for (uint8_t filetobreak = 1; filetobreak <= 15; filetobreak++) {
                                                   std::cout << static_cast<int>(filetobreak) << " / 15 tests done" << std::endl;
                                                   for (uint8_t cutoff = 1; cutoff < 20; cutoff++) {
                                                       Cleaner c({"testfrom", "testto", "testtores"});

                                                       int aid = -1;
                                                       {
                                                           std::filesystem::create_directories("testfrom");
                                                           for (int i = 0; i < 2; i++) {
                                                               std::ofstream o(std::filesystem::path("testfrom") / ("f" + std::to_string(i)), std::ios::binary | std::ios::out | std::ios::trunc);
                                                               for (int j = 0; j < i; j++) o.put(j % 2);
                                                           }


                                                           auto repo = std::make_unique<FileRepository>(conf);
                                                           repo->init();

                                                           std::stringstream runnerout;
                                                           Logger logger(1, runnerout);

                                                           CommandRun cmd;
                                                           cmd.run(Context{&logger, repo.get()});
                                                           aid = repo->getObjects(Object::ObjectType::Archive).begin()->second;
                                                       }

                                                       {
                                                           if (filetobreak & 0b00000001) {
                                                               for (int i = 0; i < cutoff; i++)
                                                                   randomChange("testto/1");
                                                               if (cutoff > 5)
                                                                   std::filesystem::resize_file("testto/1", std::filesystem::file_size("testto/1") - cutoff);
                                                           }
                                                           if (filetobreak & 0b00000010) {
                                                               for (int i = 0; i < cutoff; i++)
                                                                   randomChange("testto/index");
                                                               if (cutoff > 5)
                                                                   std::filesystem::resize_file("testto/index", std::filesystem::file_size("testto/index") - cutoff);
                                                           }
                                                           if (filetobreak & 0b00000100) {
                                                               for (int i = 0; i < cutoff; i++)
                                                                   randomChange("testto/offsets");
                                                               if (cutoff > 5)
                                                                   std::filesystem::resize_file("testto/offsets", std::filesystem::file_size("testto/offsets") - cutoff);
                                                           }
                                                           if (filetobreak & 0b00001000) {
                                                               for (int i = 0; i < cutoff; i++)
                                                                   randomChange("testto/info");
                                                               if (cutoff > 5)
                                                                   std::filesystem::resize_file("testto/info", std::filesystem::file_size("testto/info") - cutoff);
                                                           }
                                                       }


                                                       {
                                                           bool ok = true;
                                                           try {
                                                               Config confr = conf;
                                                               confr.add("aid", std::to_string(aid)).add("to", "testtores");

                                                               auto repo = std::make_unique<FileRepository>(confr);
                                                               repo->open();

                                                               std::stringstream runnerout;
                                                               Logger logger(1, runnerout);

                                                               CommandRestore cmd;
                                                               cmd.run(Context{&logger, repo.get()});
                                                               auto outstr = runnerout.str();
                                                               if (outstr.find("Error") == std::string::npos)
                                                                   ok = false;
                                                           } catch (...) {}
                                                           HelpfulAssert<bool>()(ok, true);
                                                       }
                                                   }
                                               }
                                           }


                                           return true;
                                       })));
    return tg;
}