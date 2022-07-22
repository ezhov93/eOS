/*
 * @file   ECommand.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Command pattern interface.
 */

#ifndef ECOMMAND_H
#define ECOMMAND_H

class ECommand {
 public:
  virtual ~ECommand() {}
  virtual void exec() = 0;
  virtual void undo() = 0;
};
#endif  // ECOMMAND_H
