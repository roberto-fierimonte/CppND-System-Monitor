#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
  public:
    float Utilization();

  private:
    long PrevIdle{0};
    long PrevNonIdle{0};
};

#endif