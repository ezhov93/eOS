#ifndef ETHREAD_H
#define ETHREAD_H

#include "ecore.h"

class EThread {
  E_DISABLE_COPY_MOVE(EThread);

public:
  using ThreadHandle = TaskHandle_t;

  enum StackSize {
    StackMinimal = configMINIMAL_STACK_SIZE, // size_t to bytes
    StackMedium = StackMinimal * 2,
    StackBig = StackMinimal * 4,
    StackMax = StackMinimal * 8
  };
  enum Priority {
    PriorityIdle = tskIDLE_PRIORITY,
    PriorityLow,
    PriorityMedium,
    PriorityHigh,
    PriorityHighest
  };

  friend class ERtos;

  explicit EThread(const char *name, size_t size = PriorityMedium)
      : _name(name), _stackSize(size) {}
  virtual ~EThread() { vTaskDelete(_handle); }
  const char *name() const { return _name; }
  void setPriority(Priority priority) {
    if (_handle != NULL)
      vTaskPrioritySet(_handle, priority);
    _priority = priority;
  }
  inline Priority priority() const { return _priority; }
  inline size_t stackSize() const { return _stackSize; }
  inline void resume() const { vTaskResume(_handle); }
  inline void resumeFromIrq() const { xTaskResumeFromISR(_handle); };
  inline void pause() const { vTaskSuspend(_handle); }
  inline bool moveToCore() {
    return xTaskCreate(taskFunction, _name, _stackSize, this, _priority,
                       &_handle);
  }
  inline TaskHandle_t handle() const { return _handle; }

  inline static void sleep(int ms) { vTaskDelay(ECore::msToTick(ms)); }
  inline static void yield() { taskYIELD(); }
  struct Crittical {
    Crittical() { enter(); }
    ~Crittical() { leave(); }
    inline static void enter() { taskENTER_CRITICAL(); }
    inline static void leave() { taskEXIT_CRITICAL(); }
  };

  void notify() { xTaskNotifyGive(_handle); }
  void notify(size_t event) { xTaskNotify(_handle, event, eSetBits); }
  void notifyFromIrq() {
    BaseType_t taskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(_handle, &taskWoken);
    if (taskWoken)
      portYIELD_FROM_ISR(taskWoken);
  }
  void notifyFromIrq(size_t event) {
    BaseType_t taskWoken = pdFALSE;
    xTaskNotifyFromISR(_handle, event, eSetBits, &taskWoken);
    if (taskWoken)
      portYIELD_FROM_ISR(taskWoken);
  }

protected:
  virtual void run() = 0;
  void sleepUntil(size_t ms) const {
    vTaskDelayUntil(const_cast<TickType_t *>(&_lastWakeTime),
                    ECore::msToTick(ms));
  }
  static bool waitEvent(const size_t event, size_t ms = ECore::MaxDelay) {
    size_t dst;
    bool ok = xTaskNotifyWait(pdFALSE, /* Не очищать биты на входе. */
                              event, /* На выходе очищаются биты event. */
                              &dst, ECore::msToTick(ms));
    return ok && event == dst;
  }
  static bool wait(const size_t ms = ECore::MaxDelay) {
    return ulTaskNotifyTake(pdFALSE, /* Не очищать биты на входе. */
                            ECore::msToTick(ms));
  }

private:
  const char *_name;
  const size_t _stackSize;
  Priority _priority = PriorityLow;
  TaskHandle_t _handle = nullptr;
  TickType_t _lastWakeTime = 0;
  void exec() {
    _lastWakeTime = xTaskGetTickCount();
    run();
		delete this;
  }
  static void taskFunction(void *thread) {
    const auto obj = static_cast<EThread *>(thread);
    obj->exec();
  }
};

#endif // ETHREAD_H
