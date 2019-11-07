/*
 -----------------------------------------------------------------------------------------------------------------------
 The MIT License (MIT)
 Copyright (c) 2019 Ralph-Gordon Paul. All rights reserved.
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
 rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
 persons to whom the Software is furnished to do so, subject to the following conditions:
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
 Software.
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 -----------------------------------------------------------------------------------------------------------------------
*/

#include <chrono>
#include <iostream>
#include <optional>
#include <thread>

#include "Sequencer.hpp"

int main(const int argc, const char **argv)
{
    using namespace rgp;
    using namespace std::chrono_literals;

    std::cout << "Sample Program" << std::endl;

    auto sequencer = Sequencer<std::optional<int>>::createSharedPointer();

    sequencer->enqueueStep([](auto result, auto completion) {
        std::cout << "Step 1" << std::endl;
        completion(5);
    });

    sequencer->enqueueStep([](auto result, auto completion) {
        std::cout << "Step 2" << std::endl;
        if (result.has_value())
        {
            std::cout << "  found value: " << std::to_string(result.value()) << std::endl;
        }

        completion(std::nullopt);
    });

    sequencer->enqueueStep([](auto result, auto completion) {
        std::cout << "Step 3" << std::endl;
        std::thread threadObj([completion] {
            for (int i = 0; i < 3; i++)
            {
                std::cout << "Thread executing ..." << std::endl;
                std::this_thread::sleep_for(1s);
            }

            completion(std::nullopt);
        });
        threadObj.detach();
    });

    sequencer->enqueueStep([](auto result, auto completion) { 
        std::cout << "Step 4" << std::endl;
        completion(std::nullopt);
    });

    sequencer->run(std::nullopt);

    std::cout << "Sleep 10 seconds ..." << std::endl;
    std::this_thread::sleep_for(10s);
    std::cout << "... Done" << std::endl;

    return EXIT_SUCCESS;
}
