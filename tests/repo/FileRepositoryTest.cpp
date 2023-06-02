//
// Created by Stepan Usatiuk on 14.04.2023.
//

#include "FileRepositoryTest.h"

std::unique_ptr<TestGroup> FileRepositoryTest::operator()() {
    auto tg = std::make_unique<TestGroup>("FileRepository tests");

    tg->addTest(std::make_unique<Test>("De/serialize test", std::function<bool()>([]() {
                                           Cleaner c({"testrepo"});

                                           {
                                               Config conf;
                                               conf.add("repo", "testrepo");
                                               FileRepository repo(conf);
                                               repo.init();
                                               std::vector<char>
                                                       data1{'a', 'b', 'c', 'e'};

                                               std::string o1k(16, '\0');
                                               std::string o2k(16, '\0');
                                               o2k[0] = 1;
                                               o2k[1] = 2;

                                               Chunk o1(666, o1k, data1);
                                               std::vector<char> data2{'q', 'w', 'e', 'r', static_cast<char>(255)};
                                               Chunk o2(777, o2k, data2);

                                               repo.putObject(o1);
                                               repo.putObject(o2);


                                               HelpfulAssert<Object::idType>()(repo.getObjectId(Object::ObjectType::Chunk, o1k), 666);
                                               HelpfulAssert<Object::idType>()(repo.getObjectId(Object::ObjectType::Chunk, o2k), 777);
                                           }
                                           {
                                               Config conf;
                                               conf.add("repo", "testrepo");
                                               FileRepository repo(conf);
                                               repo.open();

                                               std::string o1k(16, '\0');
                                               std::string o2k(16, '\0');
                                               o2k[0] = 1;
                                               o2k[1] = 2;
                                               HelpfulAssert<Object::idType>()(repo.getObjectId(Object::ObjectType::Chunk, o1k), 666);
                                               HelpfulAssert<Object::idType>()(repo.getObjectId(Object::ObjectType::Chunk, o2k), 777);

                                               auto o1o = repo.getObject(666);
                                               auto o2o = repo.getObject(777);

                                               auto o1ob = o1o.cbegin();
                                               auto o2ob = o2o.cbegin();

                                               Chunk o1(o1ob, o1o.cend()), o2(o2ob, o2o.cend());

                                               std::vector<char> data1{'a', 'b', 'c', 'e'};
                                               Chunk o1e(666, o1k, data1);
                                               std::vector<char> data2{'q', 'w', 'e', 'r', static_cast<char>(255)};
                                               Chunk o2e(777, o2k, data2);

                                               HelpfulAssert<Object::idType>()(o1.id, o1e.id);
                                               HelpfulAssert<Object::idType>()(o2.id, o2e.id);
                                               HelpfulAssert<int>()((int) o1.type, (int) o1e.type);
                                               HelpfulAssert<int>()((int) o2.type, (int) o2e.type);
                                               auto o1d = o1.data;
                                               auto o1ed = o1e.data;
                                               auto o2d = o2.data;
                                               auto o2ed = o2e.data;
                                               HelpfulAssert<size_t>()(o1.data.size(), o1e.data.size());
                                               HelpfulAssert<size_t>()(o2.data.size(), o2e.data.size());
                                               for (int i = 0; i < o1.data.size(); i++) {
                                                   HelpfulAssert<char>()(o1.data[i], o1e.data[i]);
                                               }
                                               for (int i = 0; i < o2.data.size(); i++) {
                                                   HelpfulAssert<char>()(o2.data[i], o2e.data[i]);
                                               }
                                           }
                                           return true;
                                       })));


    tg->addTest(std::make_unique<Test>("De/serialize with sync/read filter", std::function<bool()>([]() {
                                           Cleaner c({"testrepo"});

                                           {
                                               Config conf;
                                               conf.add("repo", "testrepo")
                                                       .add("compression", "shiftC")
                                                       .add("compression-level", "1")
                                                       .add("encryption", "shiftE")
                                                       .add("password", "\a")
                                                       .add("salt", "a");

                                               FileRepository repo(conf);
                                               repo.init();
                                               std::string o1k(16, '\0');
                                               std::string o2k(16, '\0');
                                               o2k[0] = 1;
                                               o2k[1] = 2;
                                               std::vector<char>
                                                       data1{'a', 'b', 'c', 'e'};
                                               Chunk o1(666, o1k, data1);
                                               std::vector<char> data2{'q', 'w', 'e', 'r', 'b'};
                                               Chunk o2(777, o2k, data2);

                                               repo.putObject(o1);
                                               repo.putObject(o2);


                                               HelpfulAssert<Object::idType>()(repo.getObjectId(Object::ObjectType::Chunk, o1k), 666);
                                               HelpfulAssert<Object::idType>()(repo.getObjectId(Object::ObjectType::Chunk, o2k), 777);
                                           }
                                           {
                                               Config conf;
                                               conf.add("repo", "testrepo")
                                                       .add("compression", "shiftC")
                                                       .add("compression-level", "1")
                                                       .add("encryption", "shiftE")
                                                       .add("password", "\b")
                                                       .add("salt", "a");

                                               FileRepository repo(conf);

                                               bool err = false;

                                               try {
                                                   repo.open();
                                                   err = true;
                                               } catch (...) {}


                                               try {
                                                   auto o1o = repo.getObject(666);
                                                   auto o1ob = o1o.cbegin();

                                                   Chunk o1(o1ob, o1o.cend());
                                                   err = true;
                                               } catch (...) {}

                                               try {
                                                   auto o2o = repo.getObject(777);
                                                   auto o2ob = o2o.cbegin();

                                                   Chunk o2(o2ob, o2o.cend());
                                                   err = true;
                                               } catch (...) {}

                                               if (err)
                                                   throw Exception("Object constructed with garbage data!");
                                           }
                                           {
                                               Config conf;
                                               conf.add("repo", "testrepo")
                                                       .add("compression", "shiftC")
                                                       .add("compression-level", "1")
                                                       .add("encryption", "shiftE")
                                                       .add("password", "\a")
                                                       .add("salt", "a");

                                               FileRepository repo(conf);
                                               repo.open();


                                               std::string o1k(16, '\0');
                                               std::string o2k(16, '\0');
                                               o2k[0] = 1;
                                               o2k[1] = 2;
                                               HelpfulAssert<Object::idType>()(repo.getObjectId(Object::ObjectType::Chunk, o1k), 666);
                                               HelpfulAssert<Object::idType>()(repo.getObjectId(Object::ObjectType::Chunk, o2k), 777);

                                               auto o1o = repo.getObject(666);
                                               auto o2o = repo.getObject(777);

                                               auto o1ob = o1o.cbegin();
                                               auto o2ob = o2o.cbegin();

                                               Chunk o1(o1ob, o1o.cend()), o2(o2ob, o2o.cend());

                                               std::vector<char> data1{'a', 'b', 'c', 'e'};
                                               Chunk o1e(666, o1k, data1);
                                               std::vector<char> data2{'q', 'w', 'e', 'r', 'b'};
                                               Chunk o2e(777, o2k, data2);

                                               HelpfulAssert<Object::idType>()(o1.id, o1e.id);
                                               HelpfulAssert<Object::idType>()(o2.id, o2e.id);
                                               HelpfulAssert<int>()((int) o1.type, (int) o1e.type);
                                               HelpfulAssert<int>()((int) o2.type, (int) o2e.type);
                                               auto o1d = o1.data;
                                               auto o1ed = o1e.data;
                                               auto o2d = o2.data;
                                               auto o2ed = o2e.data;
                                               HelpfulAssert<size_t>()(o1.data.size(), o1e.data.size());
                                               HelpfulAssert<size_t>()(o2.data.size(), o2e.data.size());
                                               for (int i = 0; i < o1.data.size(); i++) {
                                                   HelpfulAssert<char>()(o1.data[i], o1e.data[i]);
                                               }
                                               for (int i = 0; i < o2.data.size(); i++) {
                                                   HelpfulAssert<char>()(o2.data[i], o2e.data[i]);
                                               }
                                           }
                                           return true;
                                       })));

    tg->addTest(std::make_unique<Test>("IDs test (disabled)", std::function<bool()>([]() {
                                           return true;
                                           Cleaner c({"testrepo"});
                                           {
                                               Config conf;
                                               conf.add("repo", "testrepo");
                                               FileRepository repo(conf);
                                               repo.init();

                                               std::string o1k(16, '\0');
                                               std::string o2k(16, '\0');
                                               o2k[0] = 1;

                                               std::vector<char>
                                                       data1{'a', 'b', 'c', 'e'};
                                               Chunk o1(repo.getId(), o1k, data1);
                                               std::vector<char> data2{'q', 'w', 'e', 'r', 'b'};
                                               Chunk o2(repo.getId(), o2k, data2);

                                               HelpfulAssert<int>()(o1.id, 1);
                                               HelpfulAssert<int>()(o2.id, 2);

                                               repo.putObject(o1);
                                               repo.putObject(o2);


                                               HelpfulAssert<Object::idType>()(repo.getObjectId(Object::ObjectType::Chunk, o1k), 1);
                                               HelpfulAssert<Object::idType>()(repo.getObjectId(Object::ObjectType::Chunk, o2k), 2);
                                           }
                                           {
                                               Config conf;
                                               conf.add("repo", "testrepo");
                                               FileRepository repo(conf);
                                               repo.open();
                                               auto o1o = repo.getObject(1);
                                               auto o2o = repo.getObject(2);

                                               auto o1ob = o1o.cbegin();
                                               auto o2ob = o2o.cbegin();

                                               Chunk o1(o1ob, o1o.cend()), o2(o2ob, o2o.cend());
                                               std::string o1k(16, '\0');
                                               std::string o2k(16, '\0');
                                               o2k[0] = 1;
                                               std::vector<char> data1{'a', 'b', 'c', 'e'};
                                               Chunk o1e(1, o1k, data1);
                                               std::vector<char> data2{'q', 'w', 'e', 'r', 'b'};
                                               Chunk o2e(2, o2k, data2);

                                               HelpfulAssert<Object::idType>()(o1.id, o1e.id);
                                               HelpfulAssert<Object::idType>()(o2.id, o2e.id);
                                               HelpfulAssert<int>()((int) o1.type, (int) o1e.type);
                                               HelpfulAssert<int>()((int) o2.type, (int) o2e.type);
                                               auto o1d = o1.data;
                                               auto o1ed = o1e.data;
                                               auto o2d = o2.data;
                                               auto o2ed = o2e.data;
                                               HelpfulAssert<size_t>()(o1.data.size(), o1e.data.size());
                                               HelpfulAssert<size_t>()(o2.data.size(), o2e.data.size());
                                               for (int i = 0; i < o1.data.size(); i++) {
                                                   HelpfulAssert<char>()(o1.data[i], o1e.data[i]);
                                               }
                                               for (int i = 0; i < o2.data.size(); i++) {
                                                   HelpfulAssert<char>()(o2.data[i], o2e.data[i]);
                                               }


                                               HelpfulAssert<Object::idType>()(repo.getObjectId(Object::ObjectType::Chunk, o1k), 1);
                                               HelpfulAssert<Object::idType>()(repo.getObjectId(Object::ObjectType::Chunk, o2k), 2);


                                               repo.deleteObject(o1);
                                           }
                                           {
                                               Config conf;
                                               conf.add("repo", "testrepo");
                                               FileRepository repo(conf);
                                               repo.open();

                                               std::string o2k(16, '\0');
                                               o2k[0] = 1;
                                               HelpfulAssert<Object::idType>()(repo.getObjectId(Object::ObjectType::Chunk, o2k), 2);

                                               auto id = repo.getId();
                                               HelpfulAssert<int>()(id, 1);
                                               std::vector<char>
                                                       data1{'a', 'b', 'c', 'e'};
                                               Chunk o1(id, o2k, data1);
                                               HelpfulAssert<int>()(repo.getId(), 3);
                                           }
                                           return true;
                                       })));


    return tg;
}
