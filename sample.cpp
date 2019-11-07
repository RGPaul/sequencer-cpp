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
