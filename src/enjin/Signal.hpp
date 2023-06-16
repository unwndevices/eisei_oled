#ifndef SIGNAL_H
#define SIGNAL_H
#include <vector>
#include <functional>

#define CONNECT_SLOT_1(signalName, memberFunc, objPtr) \
    signalName.Connect(std::bind(&std::remove_reference<decltype(*objPtr)>::type::memberFunc, objPtr, std::placeholders::_1))
#define CONNECT_SLOT_2(signalName, memberFunc, objPtr) \
    signalName.Connect(std::bind(&std::remove_reference<decltype(*objPtr)>::type::memberFunc, objPtr, std::placeholders::_1, std::placeholders::_2))

template <typename... Args>
class Signal
{
public:
    using Slot = std::function<void(Args...)>;

    void Connect(Slot slot)
    {
        m_slots.push_back(slot);
    }

    void Emit(Args... args)
    {
        for (const auto &slot : m_slots)
        {
            slot(args...);
        }
    }

private:
    std::vector<Slot> m_slots;
};

#endif // SIGNAL_H
