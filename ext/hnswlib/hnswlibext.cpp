#include "hnswlibext.hpp"

VALUE rb_mHnswlib;

extern "C"
void Init_hnswlibext(void) {
  rb_mHnswlib = rb_define_module("Hnswlib");
}
