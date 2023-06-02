//
// Created by Stepan Usatiuk on 13.04.2023.
//

#include "HelpfulAssertTest.h"

std::unique_ptr<TestGroup> HelpfulAssertTest::operator()() {
    auto tg = std::make_unique<TestGroup>("HelpfulAssert tests");

    tg->addTest(std::make_unique<Test>("Equal test", std::function<bool()>([]() {
                                           HelpfulAssert<int>()(1, 1);

                                           HelpfulAssert<std::string>()("hello", "hello");

                                           try {
                                               HelpfulAssert<std::string>()("hello", "hello2");
                                           } catch (const Exception &e) {
                                               if (std::string_view(e.what()).find("Expected lhs to compare to hello2\nBut lhs is hello\n") == std::string_view::npos)
                                                   throw Exception("HelpfulAssert text is wrong");
                                           } catch (...) {
                                               throw Exception("HelpfulAssert throws wrong exception");
                                           }

                                           try {
                                               HelpfulAssert<int>()(1, 2);
                                           } catch (const Exception &e) {
                                               if (std::string_view(e.what()).find("Expected lhs to compare to 2\nBut lhs is 1\n") == std::string_view::npos)
                                                   throw Exception("HelpfulAssert text is wrong");
                                           } catch (...) {
                                               throw Exception("HelpfulAssert throws wrong exception");
                                           }

                                           return true;
                                       })));

    tg->addTest(std::make_unique<Test>("Greater than test", std::function<bool()>([]() {
                                           HelpfulAssert<int, std::greater<>>()(2, 1);

                                           try {
                                               HelpfulAssert<int, std::greater<>>()(1, 2);
                                           } catch (const Exception &e) {
                                               if (std::string_view(e.what()).find("Expected lhs to compare to 2\nBut lhs is 1\n") == std::string_view::npos)
                                                   throw Exception("HelpfulAssert text is wrong");
                                           } catch (...) {
                                               throw Exception("HelpfulAssert throws wrong exception");
                                           }

                                           return true;
                                       })));
    tg->addTest(std::make_unique<Test>("Less than test", std::function<bool()>([]() {
                                           HelpfulAssert<int, std::less<>>()(1, 2);

                                           try {
                                               HelpfulAssert<int, std::less<>>()(2, 1);
                                           } catch (const Exception &e) {
                                               if (std::string_view(e.what()).find("Expected lhs to compare to 1\nBut lhs is 2\n") == std::string_view::npos)
                                                   throw Exception("HelpfulAssert text is wrong");
                                           } catch (...) {
                                               throw Exception("HelpfulAssert throws wrong exception");
                                           }

                                           return true;
                                       })));
    return tg;
}
