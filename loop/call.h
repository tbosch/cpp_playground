#ifndef Callable_h
#define Callable_h

// Size in bytes in every Callable.
#define CALLABLE_BUF_SIZE 64

class Callable
{
public:
    Callable operator()();

    void reset(void* data, int size);
private:
    // Note: We are using an int array to make sure
    // it's aligned.
    int buf_[CALLABLE_BUF_SIZE / 4];
    bool filled_;
};

template <typename Fn, typename... Args>
Callable call(Fn fn, Args... args);
#endif
