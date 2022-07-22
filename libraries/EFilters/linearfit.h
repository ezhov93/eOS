#ifndef LINEARFIT_H
#define LINEARFIT_H

class LinearFit {
 public:
  void setData(const float[], const float[], const int size);
  float a() const { return _a; }
  float b() const { return _b; }
  float delta() const { return _delta; }

 private:
  float _a, _b, _delta;
};

#endif  // LINEARFIT_H
