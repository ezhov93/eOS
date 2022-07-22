/*
 * @file   ecli.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  FreeRTOS-CLI wrapper.
 * Copyright 2021 Ezhov Mihail ezhov93@gmail.com.
 */

#ifndef ECLI_H
#define ECLI_H

class Command {};

// class EButtonNotify {
//  public:
//   void attach(EButtonNotifiable &);
//   void dettach(EButtonNotifiable &);

//  protected:
//   void pressed();
//   void released();

//   private:
//   EButtonNotifiable *_notifiable;
// }

class ECli {
  class Command {
   public:
    const char* name() { return _name; }
    const char* description() { return _name; }
    const char* exeptedNumberOfParametrs() { return _exeptedNumberOfParametrs; }

   private:
    const char* const _name;
    const char* const _description;
    int _exeptedNumberOfParametrs;
    virtual void exec() = 0;
  };

void addCommand(Command &);
char* out()

};

#endif  // ECLI_H