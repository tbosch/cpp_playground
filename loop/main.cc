#include <iostream>

class CallableInner;

class Callable {
  public:
   Callable() = default;
   ~Callable() { reset(nullptr); }
   Callable ( Callable && other ) {
     reset(other.inner_);
     other.inner_ = nullptr;
   }
   Callable &  operator= (Callable && other) {
     reset(other.inner_);
     other.inner_ = nullptr;
     return *this;
   }
   Callable operator()();

   void reset(CallableInner* inner);
  private:  
   CallableInner* inner_ = nullptr;
};


class CallableInner {
  public:
   virtual Callable operator()() = 0;
   virtual ~CallableInner() = default;
};


void Callable::reset(CallableInner* inner) {
 if (inner_ != nullptr) {
   delete inner_;
 }
 inner_ = inner;
}

Callable Callable::operator()() {
  if (inner_ != nullptr) {
    return (*inner_)();
  }
  return Callable();
}

template<typename Fn>
struct CallableInnerImpl: public CallableInner {
 public:
    CallableInnerImpl(Fn fn): fn_(fn) {}

    Callable operator()() override {
      return fn_();
    }
  private:
    Fn fn_;
};

template<typename Fn, typename... Args>
Callable call(Fn fn, Args... args) {
   auto lambda = [fn, args...](){
    return fn(args...);
   };
   Callable c;
   c.reset(new CallableInnerImpl<decltype(lambda)>(lambda));
   return c;
}

Callable inc_servo(int offset);
Callable dec_servo(int offset);

Callable sweep() {
  std::cout << "sweep start" << std::endl;
  int offset = 90;
  return call(inc_servo, offset);
}

Callable inc_servo(int offset) {
  std::cout << "sweep inc " << offset << std::endl;
  offset += 10;
  if (offset >= 135) {
    return call(dec_servo, offset);
  }
  return call(inc_servo, offset);
}

Callable dec_servo(int offset) {
  std::cout << "sweep dec " << offset << std::endl;
  offset -= 10;
  if (offset < 45) {
    return call(inc_servo, offset);
  }
  return call(dec_servo, offset);
}

Callable sweep_next;

void setup() {
  sweep_next = sweep();
}

void loop() {
  sweep_next = sweep_next();
}

int main() {
    std::cout << "Hello world!" << std::endl;

    setup();
    for (int i=0; i<100; i++) {
        loop();
    }

    return 0;
}
