#include "call.h"
#include <string.h> // memcpy

class LambdaWrapper
{
public:
    virtual Callable operator()() = 0;
    virtual ~LambdaWrapper() = default;
};

void Callable::reset(void* data, int size) {
    if (size > CALLABLE_BUF_SIZE) {
        // TODO: error!
        filled_ = false;
        return;
    }
    memcpy(buf_, data, size);
    filled_ = true;
}

Callable Callable::operator()()
{
    if (!filled_) {
        return Callable();
    }
    LambdaWrapper *wrapper = reinterpret_cast<LambdaWrapper *>(buf_);
    return (*wrapper)();
}

template <typename Fn>
struct LambdaWrapperImpl : public LambdaWrapper
{
public:
    LambdaWrapperImpl(Fn fn) : fn_(fn) {}

    Callable operator()() override
    {
        return fn_();
    }

private:
    Fn fn_;
};

template <typename Fn, typename... Args>
Callable call(Fn fn, Args... args)
{
    auto lambda = [fn, args...]() {
        return fn(args...);
    };
    LambdaWrapperImpl<decltype(lambda)> wrapper(lambda);
    Callable c;
    c.reset((void*)&wrapper, sizeof(wrapper));
    return c;
}
