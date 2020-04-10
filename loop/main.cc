#include <iostream>
#include "call.h"
#include "call.cc"

Callable inc_servo(int offset);
Callable dec_servo(int offset);

Callable sweep()
{
    std::cout << "sweep start" << std::endl;
    int offset = 90;
    return call(inc_servo, offset);
}

Callable inc_servo(int offset)
{
    std::cout << "sweep inc " << offset << std::endl;
    offset += 10;
    if (offset >= 135)
    {
        return call(dec_servo, offset);
    }
    return call(inc_servo, offset);
}

Callable dec_servo(int offset)
{
    std::cout << "sweep dec " << offset << std::endl;
    offset -= 10;
    if (offset < 45)
    {
        return call(inc_servo, offset);
    }
    return call(dec_servo, offset);
}

Callable sweep_next;

void setup()
{
    sweep_next = sweep();
}

void loop()
{
    sweep_next = sweep_next();
}

int main()
{
    std::cout << "Hello world!" << std::endl;

    setup();
    for (int i=0; i<100; i++) {
        loop();
    }

    return 0;
}
