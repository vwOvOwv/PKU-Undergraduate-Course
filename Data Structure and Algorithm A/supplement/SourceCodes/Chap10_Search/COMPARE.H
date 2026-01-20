
class intintCompare {
public:
  static bool lt(int x, int y) { return x < y; }
  static bool eq(int x, int y) { return x == y; }
  static bool gt(int x, int y) { return x > y; }
};

class IntIntCompare {
public:
  static bool lt(Int x, Int y) { return x.key() < y.key(); }
  static bool eq(Int x, Int y) { return x.key() == y.key(); }
  static bool gt(Int x, Int y) { return x.key() > y.key(); }
};

class intIntCompare {
public:
  static bool lt(int x, Int y) { return x < y.key(); }
  static bool eq(int x, Int y) { return x == y.key(); }
  static bool gt(int x, Int y) { return x > y.key(); }
};

class intIntsCompare {
public:
  static bool lt(int x, Int* y) { return x < y->key(); }
  static bool eq(int x, Int* y) { return x == y->key(); }
  static bool gt(int x, Int* y) { return x > y->key(); }
};

class IntsIntsCompare {
public:
  static bool lt(Int* x, Int* y) { return x->key() < y->key(); }
  static bool eq(Int* x, Int* y) { return x->key() == y->key(); }
  static bool gt(Int* x, Int* y) { return x->key() > y->key(); }
};

class CCCompare {
public:
  static bool lt(char* x, char* y) { return strcmp(x, y) < 0; }
  static bool eq(char* x, char* y) { return strcmp(x, y) == 0; }
  static bool gt(char* x, char* y) { return strcmp(x, y) > 0; }
};
