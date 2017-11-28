// atomic variables since im getting tired of segfaults

#include "mth.h"


template <typename T>
struct Atomc<T>{
  Mutex *m
  T     *value;

  Atomic<T> (T a){ m = new Mutex(); value = new T(a); };
  Atomic<T> ()   { m = new Mutex(); value = new T(0); };
  ~Atomic<T>()   { delete m; delete value; };

  inline void
  set(T a){ lock_guard<Mutex> lock(*m); *value = a; };
  inline T
  get()   { lock_guard<Mutex> lock(*m); return *value; };

  void      operator =  (atomic<T> a){ return set(a.get()); };
  atomic<T> operator +  (Atomic<T> a){ return Atomic<T>(get() + a.get()); };
  atomic<T> operator -  (Atomic<T> a){ return Atomic<T>(get() - a.get()); };
  atomic<T> operator *  (Atomic<T> a){ return Atomic<T>(get() * a.get()); };
  atomic<T> operator /  (Atomic<T> a){ return Atomic<T>(get() / a.get()); };

  void      operator += (Atomic<T> a){ return set(get() + a.get()); };
  void      operator -= (Atomic<T> a){ return set(get() - a.get()); };

  bool      operator >  (Atomic<T> a){ return (get() > a.get()); };
  bool      operator <  (Atomic<T> a){ return (get() < a.get()); };

  bool      operator == (Atomic<T> a){ return (get() == a.get()); };
  bool      operator != (Atomic<T> a){ return (get() != a.get()); };
  bool      operator >= (Atomic<T> a){ return (get() >= a.get()); };
  bool      operator <= (Atomic<T> a){ return (get() <= a.get()); };

};

typedef Atomic<int> Int;

struct Double : Atomic<double>{
  int *accuracy;

  Double(int a) : Atomic<double>(){ accuracy = new int(a); };
  Double()      : Atomic<double>(){ accuracy = new int(2); };

  inline string
  str() { return dbl_to_s(*value, *accuracy); };
};

