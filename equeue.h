/*
 * @file  equeue.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Queue tamplate class.
 */

#ifndef EQUEUE_H
#define EQUEUE_H

#include "FreeRTOS.h"
#include "ethread.h"
#include "queue.h"
#include <cassert>

template <typename T, size_t Size> class EQueue {
  E_DISABLE_COPY_MOVE(EQueue)

public:
  EQueue() {
	_handle = xQueueCreate(Size, sizeof(T));
		configASSERT(_handle != nullptr);
	}
  ~EQueue() { vQueueDelete(_handle); }
  void reset() const { xQueueReset(_handle); }
  size_t available() const { return uxQueueSpacesAvailable(_handle); }
  size_t size() const { return Size; }
  bool push(const T &item) const {
    return xQueueSend(_handle, &item, portMAX_DELAY);
  }
  bool push(const T &item, const size_t ms) const {
    return xQueueSend(_handle, &item, ECore::msToTick(ms));
  }
  bool pushToBack(const T &item) const {
    return xQueueSendToBack(_handle, &item, portMAX_DELAY);
  }
  bool pushToBack(const T &item, const size_t ms) const {
    return xQueueSendToBack(_handle, &item, ECore::msToTick(ms));
  }
  bool pushToFront(const T &item) const {
    return xQueueSendToFront(_handle, &item, portMAX_DELAY);
  }
  bool pushToFront(const T &item, const size_t ms) const {
    return xQueueSendToFront(_handle, &item, ECore::msToTick(ms));
  }
  T pop() const{
    T item;
    xQueueReceive(_handle, &item, portMAX_DELAY);
    return item;
  }
  bool pop(T &item, const size_t ms) {
    return xQueueReceive(_handle, &item, ECore::msToTick(ms));
  }
  T peek() const{
    T item;
    xQueuePeek(_handle, &item, portMAX_DELAY);
    return item;
  }
  bool peek(T &item, const size_t ms) const{
    return xQueuePeek(_handle, &item, ECore::msToTick(ms));
  }

  bool pushToBackFromIrq(const T &item) const {
    BaseType_t taskWoken = pdFALSE;
    bool status = xQueueSendToBackFromISR(_handle, &item, &taskWoken);
    if (taskWoken)
      portYIELD_FROM_ISR(taskWoken);
    return status;
  }

  bool pushToFrontFromIrq(const T &item) const {
    BaseType_t taskWoken = pdFALSE;
    bool status = xQueueSendToFrontFromISR(_handle, &item, &taskWoken);
    if (taskWoken)
      portYIELD_FROM_ISR(taskWoken);
    return status;
  }
  T popFromIrq() const{
    T item;
    BaseType_t taskWoken = pdFALSE;
    xQueueReceiveFromISR(_handle, &item, &taskWoken);
    if (taskWoken)
      portYIELD_FROM_ISR(taskWoken);
    return item;
  }
  bool popFromIrq(T &item) const{
    BaseType_t taskWoken = pdFALSE;
    bool status = xQueueReceiveFromISR(_handle, &item, &taskWoken);
    if (taskWoken)
      portYIELD_FROM_ISR(taskWoken);
    return status;
  }
  T peekFromIrq() const{
    T item;
    BaseType_t taskWoken = pdFALSE;
    xQueuePeekFromISR(_handle, &item, &taskWoken);
    if (taskWoken)
      portYIELD_FROM_ISR(taskWoken);
    return item;
  }
  bool peekFromIrq(T &item) const{
    BaseType_t taskWoken = pdFALSE;
    bool status = xQueuePeekFromISR(_handle, &item, &taskWoken);
    if (taskWoken)
      portYIELD_FROM_ISR(taskWoken);
    return status;
  }

private:
  QueueHandle_t _handle;
};

#endif // EQUEUE_H
