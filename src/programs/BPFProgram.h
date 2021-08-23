#ifndef __BPFPROGRAM_H__
#define __BPFPROGRAM_H__

#include <stdint.h>
#include <utils/Util.h>

class BPFProgram
{
public:
  BPFProgram(/* args */);
  virtual ~BPFProgram();
  uint32_t getId() const;

protected:
  uint32_t mId;

private:
  static uint32_t sIdCounter;
};

#endif // __BPFPROGRAM_H__
