/**
 * hnswlib.rb is a Ruby binding for the Hnswlib.
 *
 * Copyright (c) 2021 Atsushi Tatsuma
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HNSWLIBEXT_HPP
#define HNSWLIBEXT_HPP 1

#include <ruby.h>

#include <hnswlib.h>

VALUE rb_cHnswlibL2Space;
VALUE rb_cHnswlibInnerProductSpace;

class RbHnswlibL2Space {
  public:
    static VALUE hnsw_l2space_alloc(VALUE self) {
      hnswlib::L2Space* ptr = (hnswlib::L2Space*)ruby_xmalloc(sizeof(hnswlib::L2Space));
      return TypedData_Wrap_Struct(self, &hnsw_l2space_type, ptr);
    };

    static void hnsw_l2space_free(void* ptr) {
      ((hnswlib::L2Space*)ptr)->~L2Space();
      ruby_xfree(ptr);
    };

    static size_t hnsw_l2space_size(const void* ptr) {
      return sizeof(*((hnswlib::L2Space*)ptr));
    };

    static hnswlib::L2Space* get_hnsw_l2space(VALUE self) {
      hnswlib::L2Space* ptr;
      TypedData_Get_Struct(self, hnswlib::L2Space, &hnsw_l2space_type, ptr);
      return ptr;
    };

    static VALUE define_class(VALUE rb_mHnswlib) {
      rb_cHnswlibL2Space = rb_define_class_under(rb_mHnswlib, "L2Space", rb_cObject);
      rb_define_alloc_func(rb_cHnswlibL2Space, hnsw_l2space_alloc);
      rb_define_method(rb_cHnswlibL2Space, "initialize", RUBY_METHOD_FUNC(_hnsw_l2space_init), 1);
      rb_define_attr(rb_cHnswlibL2Space, "dim", 1, 0);
      return rb_cHnswlibL2Space;
    };

  private:
    static const rb_data_type_t hnsw_l2space_type;

    static VALUE _hnsw_l2space_init(VALUE self, VALUE dim) {
      rb_iv_set(self, "@dim", dim);
      hnswlib::L2Space* ptr = get_hnsw_l2space(self);
      new (ptr) hnswlib::L2Space(NUM2INT(rb_iv_get(self, "@dim")));
      return Qnil;
    };
};

const rb_data_type_t RbHnswlibL2Space::hnsw_l2space_type = {
  "RbHnswlibL2Space",
  {
    NULL,
    RbHnswlibL2Space::hnsw_l2space_free,
    RbHnswlibL2Space::hnsw_l2space_size
  },
  NULL,
  NULL,
  RUBY_TYPED_FREE_IMMEDIATELY
};

class RbHnswlibInnerProductSpace {
  public:
    static VALUE hnsw_ipspace_alloc(VALUE self) {
      hnswlib::InnerProductSpace* ptr = (hnswlib::InnerProductSpace*)ruby_xmalloc(sizeof(hnswlib::InnerProductSpace));
      return TypedData_Wrap_Struct(self, &hnsw_ipspace_type, ptr);
    };

    static void hnsw_ipspace_free(void* ptr) {
      ((hnswlib::InnerProductSpace*)ptr)->~InnerProductSpace();
      ruby_xfree(ptr);
    };

    static size_t hnsw_ipspace_size(const void* ptr) {
      return sizeof(*((hnswlib::InnerProductSpace*)ptr));
    };

    static hnswlib::InnerProductSpace* get_hnsw_ipspace(VALUE self) {
      hnswlib::InnerProductSpace* ptr;
      TypedData_Get_Struct(self, hnswlib::InnerProductSpace, &hnsw_ipspace_type, ptr);
      return ptr;
    };

    static VALUE define_class(VALUE rb_mHnswlib) {
      rb_cHnswlibInnerProductSpace = rb_define_class_under(rb_mHnswlib, "InnerProductSpace", rb_cObject);
      rb_define_alloc_func(rb_cHnswlibInnerProductSpace, hnsw_ipspace_alloc);
      rb_define_method(rb_cHnswlibInnerProductSpace, "initialize", RUBY_METHOD_FUNC(_hnsw_ipspace_init), 1);
      rb_define_attr(rb_cHnswlibInnerProductSpace, "dim", 1, 0);
      return rb_cHnswlibInnerProductSpace;
    };

  private:
    static const rb_data_type_t hnsw_ipspace_type;

    static VALUE _hnsw_ipspace_init(VALUE self, VALUE dim) {
      rb_iv_set(self, "@dim", dim);
      hnswlib::InnerProductSpace* ptr = get_hnsw_ipspace(self);
      new (ptr) hnswlib::InnerProductSpace(NUM2INT(rb_iv_get(self, "@dim")));
      return Qnil;
    };
};

const rb_data_type_t RbHnswlibInnerProductSpace::hnsw_ipspace_type = {
  "RbHnswlibInnerProductSpace",
  {
    NULL,
    RbHnswlibInnerProductSpace::hnsw_ipspace_free,
    RbHnswlibInnerProductSpace::hnsw_ipspace_size
  },
  NULL,
  NULL,
  RUBY_TYPED_FREE_IMMEDIATELY
};

#endif /* HNSWLIBEXT_HPP */
