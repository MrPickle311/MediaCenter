#include "EventLoop.hpp"
#include <thread>

void DelayedEventLoop::emitDelayedStartSignal() {
  std::this_thread::sleep_for(this->delay_);
  emit runned();
}

DelayedEventLoop::DelayedEventLoop(int ms) : delay_{ms} {}

void DelayedEventLoop::startTestEventLoop() {
  std::thread th{&DelayedEventLoop::emitDelayedStartSignal, this};
  loop_.exec();
  th.join();
}

void DelayedEventLoop::killTestEventLoop() { loop_.quit(); }