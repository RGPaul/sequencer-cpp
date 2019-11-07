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

#include <functional>
#include <memory>
#include <vector>

namespace rgp
{
template <class T>
class Sequencer
{
    using SequencerCompletion = std::function<void(T)>;
    using SequencerStep = std::function<void(T, SequencerCompletion)>;

  public:
    explicit Sequencer() {}

    void run(T result) { runNextStepWithResult(result); }

    void enqueueStep(const SequencerStep& step) { steps.emplace_back(step); }

    //! creates a new sequencer as shared pointer
    static std::shared_ptr<Sequencer> createSharedPointer() { return std::make_shared<Sequencer<T>>(); }

  private:
    Sequencer(const Sequencer& other) = delete;

    std::vector<SequencerStep> steps;

    SequencerStep dequeueNextStep()
    {
        SequencerStep step = steps.front();
        steps.erase(steps.begin());
        return step;
    }

    void runNextStepWithResult(T result)
    {
        if (steps.empty())
        {
            return;
        }

        SequencerStep step = dequeueNextStep();

        step(result, [this](T nextResult) { runNextStepWithResult(nextResult); });
    }
};
}  // namespace rgp
