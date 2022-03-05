/**
 * hnswlib.rb is a Ruby binding for the Hnswlib.
 *
 * Copyright (c) 2021-2022 Atsushi Tatsuma
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
VALUE rb_cHnswlibHierarchicalNSW;
VALUE rb_cHnswlibBruteforceSearch;

class RbHnswlibL2Space {
  public:
    static VALUE hnsw_l2space_alloc(VALUE self) {
      hnswlib::L2Space* ptr = (hnswlib::L2Space*)ruby_xmalloc(sizeof(hnswlib::L2Space));
      new (ptr) hnswlib::L2Space(); // dummy call to constructor for GC.
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
      rb_define_method(rb_cHnswlibL2Space, "distance", RUBY_METHOD_FUNC(_hnsw_l2space_distance), 2);
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

    static VALUE _hnsw_l2space_distance(VALUE self, VALUE arr_a, VALUE arr_b) {
      const int dim = NUM2INT(rb_iv_get(self, "@dim"));
      if (dim != RARRAY_LEN(arr_a) || dim != RARRAY_LEN(arr_b)) {
        rb_raise(rb_eArgError, "Array size does not match to space dimensionality.");
        return Qnil;
      }
      float* vec_a = (float*)ruby_xmalloc(dim * sizeof(float));
      for (int i = 0; i < dim; i++) {
        vec_a[i] = (float)NUM2DBL(rb_ary_entry(arr_a, i));
      }
      float* vec_b = (float*)ruby_xmalloc(dim * sizeof(float));
      for (int i = 0; i < dim; i++) {
        vec_b[i] = (float)NUM2DBL(rb_ary_entry(arr_b, i));
      }
      hnswlib::DISTFUNC<float> dist_func = get_hnsw_l2space(self)->get_dist_func();
      const float dist = dist_func(vec_a, vec_b, get_hnsw_l2space(self)->get_dist_func_param());
      ruby_xfree(vec_a);
      ruby_xfree(vec_b);
      return DBL2NUM((double)dist);
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
      new (ptr) hnswlib::InnerProductSpace(); // dummy call to constructor for GC.
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
      rb_define_method(rb_cHnswlibInnerProductSpace, "distance", RUBY_METHOD_FUNC(_hnsw_ipspace_distance), 2);
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

    static VALUE _hnsw_ipspace_distance(VALUE self, VALUE arr_a, VALUE arr_b) {
      const int dim = NUM2INT(rb_iv_get(self, "@dim"));
      if (dim != RARRAY_LEN(arr_a) || dim != RARRAY_LEN(arr_b)) {
        rb_raise(rb_eArgError, "Array size does not match to space dimensionality.");
        return Qnil;
      }
      float* vec_a = (float*)ruby_xmalloc(dim * sizeof(float));
      for (int i = 0; i < dim; i++) {
        vec_a[i] = (float)NUM2DBL(rb_ary_entry(arr_a, i));
      }
      float* vec_b = (float*)ruby_xmalloc(dim * sizeof(float));
      for (int i = 0; i < dim; i++) {
        vec_b[i] = (float)NUM2DBL(rb_ary_entry(arr_b, i));
      }
      hnswlib::DISTFUNC<float> dist_func = get_hnsw_ipspace(self)->get_dist_func();
      const float dist = dist_func(vec_a, vec_b, get_hnsw_ipspace(self)->get_dist_func_param());
      ruby_xfree(vec_a);
      ruby_xfree(vec_b);
      return DBL2NUM((double)dist);
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

class RbHnswlibHierarchicalNSW {
  public:
    static VALUE hnsw_hierarchicalnsw_alloc(VALUE self) {
      hnswlib::HierarchicalNSW<float>* ptr = (hnswlib::HierarchicalNSW<float>*)ruby_xmalloc(sizeof(hnswlib::HierarchicalNSW<float>));
      new (ptr) hnswlib::HierarchicalNSW<float>(); // dummy call to constructor for GC.
      return TypedData_Wrap_Struct(self, &hnsw_hierarchicalnsw_type, ptr);
    };

    static void hnsw_hierarchicalnsw_free(void* ptr) {
      ((hnswlib::HierarchicalNSW<float>*)ptr)->~HierarchicalNSW();
      ruby_xfree(ptr);
    };

    static size_t hnsw_hierarchicalnsw_size(const void* ptr) {
      return sizeof(*((hnswlib::HierarchicalNSW<float>*)ptr));
    };

    static hnswlib::HierarchicalNSW<float>* get_hnsw_hierarchicalnsw(VALUE self) {
      hnswlib::HierarchicalNSW<float>* ptr;
      TypedData_Get_Struct(self, hnswlib::HierarchicalNSW<float>, &hnsw_hierarchicalnsw_type, ptr);
      return ptr;
    };

    static VALUE define_class(VALUE rb_mHnswlib) {
      rb_cHnswlibHierarchicalNSW = rb_define_class_under(rb_mHnswlib, "HierarchicalNSW", rb_cObject);
      rb_define_alloc_func(rb_cHnswlibHierarchicalNSW, hnsw_hierarchicalnsw_alloc);
      rb_define_method(rb_cHnswlibHierarchicalNSW, "initialize", RUBY_METHOD_FUNC(_hnsw_hierarchicalnsw_init), -1);
      rb_define_method(rb_cHnswlibHierarchicalNSW, "add_point", RUBY_METHOD_FUNC(_hnsw_hierarchicalnsw_add_point), 2);
      rb_define_method(rb_cHnswlibHierarchicalNSW, "search_knn", RUBY_METHOD_FUNC(_hnsw_hierarchicalnsw_search_knn), 2);
      rb_define_method(rb_cHnswlibHierarchicalNSW, "save_index", RUBY_METHOD_FUNC(_hnsw_hierarchicalnsw_save_index), 1);
      rb_define_method(rb_cHnswlibHierarchicalNSW, "load_index", RUBY_METHOD_FUNC(_hnsw_hierarchicalnsw_load_index), 1);
      rb_define_method(rb_cHnswlibHierarchicalNSW, "get_point", RUBY_METHOD_FUNC(_hnsw_hierarchicalnsw_get_point), 1);
      rb_define_method(rb_cHnswlibHierarchicalNSW, "get_ids", RUBY_METHOD_FUNC(_hnsw_hierarchicalnsw_get_ids), 0);
      rb_define_method(rb_cHnswlibHierarchicalNSW, "mark_deleted", RUBY_METHOD_FUNC(_hnsw_hierarchicalnsw_mark_deleted), 1);
      rb_define_method(rb_cHnswlibHierarchicalNSW, "resize_index", RUBY_METHOD_FUNC(_hnsw_hierarchicalnsw_resize_index), 1);
      rb_define_method(rb_cHnswlibHierarchicalNSW, "set_ef", RUBY_METHOD_FUNC(_hnsw_hierarchicalnsw_set_ef), 1);
      rb_define_method(rb_cHnswlibHierarchicalNSW, "max_elements", RUBY_METHOD_FUNC(_hnsw_hierarchicalnsw_max_elements), 0);
      rb_define_method(rb_cHnswlibHierarchicalNSW, "current_count", RUBY_METHOD_FUNC(_hnsw_hierarchicalnsw_current_count), 0);
      rb_define_attr(rb_cHnswlibHierarchicalNSW, "space", 1, 0);
      return rb_cHnswlibHierarchicalNSW;
    };

  private:
    static const rb_data_type_t hnsw_hierarchicalnsw_type;

    static VALUE _hnsw_hierarchicalnsw_init(int argc, VALUE* argv, VALUE self) {
      VALUE kw_args = Qnil;
      ID kw_table[5] = {
        rb_intern("space"),
        rb_intern("max_elements"),
        rb_intern("m"),
        rb_intern("ef_construction"),
        rb_intern("random_seed")
      };
      VALUE kw_values[5] = {
        Qundef, Qundef, Qundef, Qundef, Qundef
      };
      rb_scan_args(argc, argv, ":", &kw_args);
      rb_get_kwargs(kw_args, kw_table, 2, 3, kw_values);
      if (kw_values[2] == Qundef) kw_values[2] = INT2NUM(16);
      if (kw_values[3] == Qundef) kw_values[3] = INT2NUM(200);
      if (kw_values[4] == Qundef) kw_values[4] = INT2NUM(100);

      if (!(rb_obj_is_instance_of(kw_values[0], rb_cHnswlibL2Space) || rb_obj_is_instance_of(kw_values[0], rb_cHnswlibInnerProductSpace))) {
        rb_raise(rb_eTypeError, "expected space, Hnswlib::L2Space or Hnswlib::InnerProductSpace");
        return Qnil;
      }
      if (!RB_INTEGER_TYPE_P(kw_values[1])) {
        rb_raise(rb_eTypeError, "expected max_elements, Integer");
        return Qnil;
      }
      if (!RB_INTEGER_TYPE_P(kw_values[2])) {
        rb_raise(rb_eTypeError, "expected m, Integer");
        return Qnil;
      }
      if (!RB_INTEGER_TYPE_P(kw_values[3])) {
        rb_raise(rb_eTypeError, "expected ef_construction, Integer");
        return Qnil;
      }
      if (!RB_INTEGER_TYPE_P(kw_values[4])) {
        rb_raise(rb_eTypeError, "expected random_seed, Integer");
        return Qnil;
      }

      rb_iv_set(self, "@space", kw_values[0]);
      hnswlib::SpaceInterface<float>* space;
      if (rb_obj_is_instance_of(kw_values[0], rb_cHnswlibL2Space)) {
        space = RbHnswlibL2Space::get_hnsw_l2space(kw_values[0]);
      } else {
        space = RbHnswlibInnerProductSpace::get_hnsw_ipspace(kw_values[0]);
      }
      const size_t max_elements = (size_t)NUM2INT(kw_values[1]);
      const size_t m = (size_t)NUM2INT(kw_values[2]);
      const size_t ef_construction = (size_t)NUM2INT(kw_values[3]);
      const size_t random_seed = (size_t)NUM2INT(kw_values[4]);

      hnswlib::HierarchicalNSW<float>* ptr = get_hnsw_hierarchicalnsw(self);
      try {
        new (ptr) hnswlib::HierarchicalNSW<float>(space, max_elements, m, ef_construction, random_seed);
      } catch(const std::runtime_error& e) {
        rb_raise(rb_eRuntimeError, "%s", e.what());
        return Qnil;
      }

      return Qnil;
    };

    static VALUE _hnsw_hierarchicalnsw_add_point(VALUE self, VALUE arr, VALUE idx) {
      const int dim = NUM2INT(rb_iv_get(rb_iv_get(self, "@space"), "@dim"));

      if (!RB_TYPE_P(arr, T_ARRAY)) {
        rb_raise(rb_eArgError, "Expect point vector to be Ruby Array.");
        return Qfalse;
      }

      if (!RB_INTEGER_TYPE_P(idx)) {
        rb_raise(rb_eArgError, "Expect index to be Ruby Integer.");
        return Qfalse;
      }

      if (dim != RARRAY_LEN(arr)) {
        rb_raise(rb_eArgError, "Array size does not match to index dimensionality.");
        return Qfalse;
      }

      float* vec = (float*)ruby_xmalloc(dim * sizeof(float));
      for (int i = 0; i < dim; i++) {
        vec[i] = (float)NUM2DBL(rb_ary_entry(arr, i));
      }

      get_hnsw_hierarchicalnsw(self)->addPoint((void *)vec, (size_t)NUM2INT(idx));

      ruby_xfree(vec);
      return Qtrue;
    };

    static VALUE _hnsw_hierarchicalnsw_search_knn(VALUE self, VALUE arr, VALUE k) {
      const int dim = NUM2INT(rb_iv_get(rb_iv_get(self, "@space"), "@dim"));

      if (!RB_TYPE_P(arr, T_ARRAY)) {
        rb_raise(rb_eArgError, "Expect query vector to be Ruby Array.");
        return Qnil;
      }

      if (!RB_INTEGER_TYPE_P(k)) {
        rb_raise(rb_eArgError, "Expect the number of nearest neighbors to be Ruby Integer.");
        return Qnil;
      }

      if (dim != RARRAY_LEN(arr)) {
        rb_raise(rb_eArgError, "Array size does not match to index dimensionality.");
        return Qnil;
      }

      float* vec = (float*)ruby_xmalloc(dim * sizeof(float));
      for (int i = 0; i < dim; i++) {
        vec[i] = (float)NUM2DBL(rb_ary_entry(arr, i));
      }

      std::priority_queue<std::pair<float, size_t>> result;
      try {
        result = get_hnsw_hierarchicalnsw(self)->searchKnn((void *)vec, (size_t)NUM2INT(k));
      } catch(const std::runtime_error& e) {
        ruby_xfree(vec);
        rb_raise(rb_eRuntimeError, "%s", e.what());
        return Qnil;
      }

      ruby_xfree(vec);

      if (result.size() != (size_t)NUM2INT(k)) {
        rb_warning("Cannot return as many search results as the requested number of neighbors. Probably ef or M is too small.");
      }

      VALUE distances_arr = rb_ary_new();
      VALUE neighbors_arr = rb_ary_new();

      while (!result.empty()) {
        const std::pair<float, size_t>& result_tuple = result.top();
        rb_ary_unshift(distances_arr, DBL2NUM((double)result_tuple.first));
        rb_ary_unshift(neighbors_arr, INT2NUM((int)result_tuple.second));
        result.pop();
      }

      VALUE ret = rb_ary_new2(2);
      rb_ary_store(ret, 0, neighbors_arr);
      rb_ary_store(ret, 1, distances_arr);
      return ret;
    };

    static VALUE _hnsw_hierarchicalnsw_save_index(VALUE self, VALUE _filename) {
      std::string filename(StringValuePtr(_filename));
      get_hnsw_hierarchicalnsw(self)->saveIndex(filename);
      RB_GC_GUARD(_filename);
      return Qnil;
    };

    static VALUE _hnsw_hierarchicalnsw_load_index(VALUE self, VALUE _filename) {
      std::string filename(StringValuePtr(_filename));
      VALUE ivspace = rb_iv_get(self, "@space");
      hnswlib::SpaceInterface<float>* space;
      if (rb_obj_is_instance_of(ivspace, rb_cHnswlibL2Space)) {
        space = RbHnswlibL2Space::get_hnsw_l2space(ivspace);
      } else {
        space = RbHnswlibInnerProductSpace::get_hnsw_ipspace(ivspace);
      }
      hnswlib::HierarchicalNSW<float>* index = get_hnsw_hierarchicalnsw(self);
      if (index->data_level0_memory_) free(index->data_level0_memory_);
      if (index->linkLists_) {
        for (hnswlib::tableint i = 0; i < index->cur_element_count; i++) {
          if (index->element_levels_[i] > 0 && index->linkLists_[i]) free(index->linkLists_[i]);
        }
        free(index->linkLists_);
      }
      if (index->visited_list_pool_) delete index->visited_list_pool_;
      try {
        index->loadIndex(filename, space);
      } catch(const std::runtime_error& e) {
        rb_raise(rb_eRuntimeError, "%s", e.what());
        return Qnil;
      }
      RB_GC_GUARD(_filename);
      return Qnil;
    };

    static VALUE _hnsw_hierarchicalnsw_get_point(VALUE self, VALUE idx) {
      VALUE ret = Qnil;
      try {
        std::vector<float> vec = get_hnsw_hierarchicalnsw(self)->template getDataByLabel<float>((size_t)NUM2INT(idx));
        ret = rb_ary_new2(vec.size());
        for (size_t i = 0; i < vec.size(); i++) {
          rb_ary_store(ret, i, DBL2NUM((double)vec[i]));
        }
      } catch(const std::runtime_error& e) {
        rb_raise(rb_eRuntimeError, "%s", e.what());
        return Qnil;
      }
      return ret;
    };

    static VALUE _hnsw_hierarchicalnsw_get_ids(VALUE self) {
      VALUE ret = rb_ary_new();
      for (auto kv : get_hnsw_hierarchicalnsw(self)->label_lookup_) {
        rb_ary_push(ret, INT2NUM((int)kv.first));
      }
      return ret;
    };

    static VALUE _hnsw_hierarchicalnsw_mark_deleted(VALUE self, VALUE idx) {
      try {
        get_hnsw_hierarchicalnsw(self)->markDelete((size_t)NUM2INT(idx));
      } catch(const std::runtime_error& e) {
        rb_raise(rb_eRuntimeError, "%s", e.what());
        return Qnil;
      }
      return Qnil;
    };

    static VALUE _hnsw_hierarchicalnsw_resize_index(VALUE self, VALUE new_max_elements) {
      if ((size_t)NUM2INT(new_max_elements) < get_hnsw_hierarchicalnsw(self)->cur_element_count) {
        rb_raise(rb_eArgError, "Cannot resize, max element is less than the current number of elements.");
        return Qnil;
      }
      try {
        get_hnsw_hierarchicalnsw(self)->resizeIndex((size_t)NUM2INT(new_max_elements));
      } catch(const std::runtime_error& e) {
        rb_raise(rb_eRuntimeError, "%s", e.what());
        return Qnil;
      }
      return Qnil;
    };

    static VALUE _hnsw_hierarchicalnsw_set_ef(VALUE self, VALUE ef) {
      get_hnsw_hierarchicalnsw(self)->ef_ = (size_t)NUM2INT(ef);
      return Qnil;
    };

    static VALUE _hnsw_hierarchicalnsw_max_elements(VALUE self) {
      return INT2NUM((int)(get_hnsw_hierarchicalnsw(self)->max_elements_));
    };

    static VALUE _hnsw_hierarchicalnsw_current_count(VALUE self) {
      return INT2NUM((int)(get_hnsw_hierarchicalnsw(self)->cur_element_count));
    };
};

const rb_data_type_t RbHnswlibHierarchicalNSW::hnsw_hierarchicalnsw_type = {
  "RbHnswlibHierarchicalNSW",
  {
    NULL,
    RbHnswlibHierarchicalNSW::hnsw_hierarchicalnsw_free,
    RbHnswlibHierarchicalNSW::hnsw_hierarchicalnsw_size
  },
  NULL,
  NULL,
  RUBY_TYPED_FREE_IMMEDIATELY
};

class RbHnswlibBruteforceSearch {
  public:
    static VALUE hnsw_bruteforcesearch_alloc(VALUE self) {
      hnswlib::BruteforceSearch<float>* ptr = (hnswlib::BruteforceSearch<float>*)ruby_xmalloc(sizeof(hnswlib::BruteforceSearch<float>));
      new (ptr) hnswlib::BruteforceSearch<float>(); // dummy call to constructor for GC.
      return TypedData_Wrap_Struct(self, &hnsw_bruteforcesearch_type, ptr);
    };

    static void hnsw_bruteforcesearch_free(void* ptr) {
      ((hnswlib::BruteforceSearch<float>*)ptr)->~BruteforceSearch();
      ruby_xfree(ptr);
    };

    static size_t hnsw_bruteforcesearch_size(const void* ptr) {
      return sizeof(*((hnswlib::BruteforceSearch<float>*)ptr));
    };

    static hnswlib::BruteforceSearch<float>* get_hnsw_bruteforcesearch(VALUE self) {
      hnswlib::BruteforceSearch<float>* ptr;
      TypedData_Get_Struct(self, hnswlib::BruteforceSearch<float>, &hnsw_bruteforcesearch_type, ptr);
      return ptr;
    };

    static VALUE define_class(VALUE rb_mHnswlib) {
      rb_cHnswlibBruteforceSearch = rb_define_class_under(rb_mHnswlib, "BruteforceSearch", rb_cObject);
      rb_define_alloc_func(rb_cHnswlibBruteforceSearch, hnsw_bruteforcesearch_alloc);
      rb_define_method(rb_cHnswlibBruteforceSearch, "initialize", RUBY_METHOD_FUNC(_hnsw_bruteforcesearch_init), -1);
      rb_define_method(rb_cHnswlibBruteforceSearch, "add_point", RUBY_METHOD_FUNC(_hnsw_bruteforcesearch_add_point), 2);
      rb_define_method(rb_cHnswlibBruteforceSearch, "search_knn", RUBY_METHOD_FUNC(_hnsw_bruteforcesearch_search_knn), 2);
      rb_define_method(rb_cHnswlibBruteforceSearch, "save_index", RUBY_METHOD_FUNC(_hnsw_bruteforcesearch_save_index), 1);
      rb_define_method(rb_cHnswlibBruteforceSearch, "load_index", RUBY_METHOD_FUNC(_hnsw_bruteforcesearch_load_index), 1);
      rb_define_method(rb_cHnswlibBruteforceSearch, "remove_point", RUBY_METHOD_FUNC(_hnsw_bruteforcesearch_remove_point), 1);
      rb_define_method(rb_cHnswlibBruteforceSearch, "max_elements", RUBY_METHOD_FUNC(_hnsw_bruteforcesearch_max_elements), 0);
      rb_define_method(rb_cHnswlibBruteforceSearch, "current_count", RUBY_METHOD_FUNC(_hnsw_bruteforcesearch_current_count), 0);
      rb_define_attr(rb_cHnswlibBruteforceSearch, "space", 1, 0);
      return rb_cHnswlibBruteforceSearch;
    };

  private:
    static const rb_data_type_t hnsw_bruteforcesearch_type;

    static VALUE _hnsw_bruteforcesearch_init(int argc, VALUE* argv, VALUE self) {
      VALUE kw_args = Qnil;
      ID kw_table[2] = { rb_intern("space"), rb_intern("max_elements") };
      VALUE kw_values[2] = { Qundef, Qundef };
      rb_scan_args(argc, argv, ":", &kw_args);
      rb_get_kwargs(kw_args, kw_table, 2, 0, kw_values);

      if (!(rb_obj_is_instance_of(kw_values[0], rb_cHnswlibL2Space) || rb_obj_is_instance_of(kw_values[0], rb_cHnswlibInnerProductSpace))) {
        rb_raise(rb_eTypeError, "expected space, Hnswlib::L2Space or Hnswlib::InnerProductSpace");
        return Qnil;
      }
      if (!RB_INTEGER_TYPE_P(kw_values[1])) {
        rb_raise(rb_eTypeError, "expected max_elements, Integer");
        return Qnil;
      }

      rb_iv_set(self, "@space", kw_values[0]);
      hnswlib::SpaceInterface<float>* space;
      if (rb_obj_is_instance_of(kw_values[0], rb_cHnswlibL2Space)) {
        space = RbHnswlibL2Space::get_hnsw_l2space(kw_values[0]);
      } else {
        space = RbHnswlibInnerProductSpace::get_hnsw_ipspace(kw_values[0]);
      }
      const size_t max_elements = (size_t)NUM2INT(kw_values[1]);

      hnswlib::BruteforceSearch<float>* ptr = get_hnsw_bruteforcesearch(self);
      try {
        new (ptr) hnswlib::BruteforceSearch<float>(space, max_elements);
      } catch(const std::runtime_error& e) {
        rb_raise(rb_eRuntimeError, "%s", e.what());
        return Qnil;
      }

      return Qnil;
    };

    static VALUE _hnsw_bruteforcesearch_add_point(VALUE self, VALUE arr, VALUE idx) {
      const int dim = NUM2INT(rb_iv_get(rb_iv_get(self, "@space"), "@dim"));

      if (!RB_TYPE_P(arr, T_ARRAY)) {
        rb_raise(rb_eArgError, "Expect point vector to be Ruby Array.");
        return Qfalse;
      }

      if (!RB_INTEGER_TYPE_P(idx)) {
        rb_raise(rb_eArgError, "Expect index to be Ruby Integer.");
        return Qfalse;
      }

      if (dim != RARRAY_LEN(arr)) {
        rb_raise(rb_eArgError, "Array size does not match to index dimensionality.");
        return Qfalse;
      }

      float* vec = (float*)ruby_xmalloc(dim * sizeof(float));
      for (int i = 0; i < dim; i++) {
        vec[i] = (float)NUM2DBL(rb_ary_entry(arr, i));
      }

      try {
        get_hnsw_bruteforcesearch(self)->addPoint((void *)vec, (size_t)NUM2INT(idx));
      } catch(const std::runtime_error& e) {
        ruby_xfree(vec);
        rb_raise(rb_eRuntimeError, "%s", e.what());
        return Qfalse;
      }

      ruby_xfree(vec);
      return Qtrue;
    };

    static VALUE _hnsw_bruteforcesearch_search_knn(VALUE self, VALUE arr, VALUE k) {
      const int dim = NUM2INT(rb_iv_get(rb_iv_get(self, "@space"), "@dim"));

      if (!RB_TYPE_P(arr, T_ARRAY)) {
        rb_raise(rb_eArgError, "Expect query vector to be Ruby Array.");
        return Qnil;
      }

      if (!RB_INTEGER_TYPE_P(k)) {
        rb_raise(rb_eArgError, "Expect the number of nearest neighbors to be Ruby Integer.");
        return Qnil;
      }

      if (dim != RARRAY_LEN(arr)) {
        rb_raise(rb_eArgError, "Array size does not match to index dimensionality.");
        return Qnil;
      }

      float* vec = (float*)ruby_xmalloc(dim * sizeof(float));
      for (int i = 0; i < dim; i++) {
        vec[i] = (float)NUM2DBL(rb_ary_entry(arr, i));
      }

      std::priority_queue<std::pair<float, size_t>> result =
        get_hnsw_bruteforcesearch(self)->searchKnn((void *)vec, (size_t)NUM2INT(k));

      ruby_xfree(vec);

      if (result.size() != (size_t)NUM2INT(k)) {
        rb_raise(rb_eRuntimeError, "Cannot return the results in a contigious 2D array. Probably ef or M is too small.");
        return Qnil;
      }

      VALUE distances_arr = rb_ary_new2(result.size());
      VALUE neighbors_arr = rb_ary_new2(result.size());

      for (int i = NUM2INT(k) - 1; i >= 0; i--) {
        const std::pair<float, size_t>& result_tuple = result.top();
        rb_ary_store(distances_arr, i, DBL2NUM((double)result_tuple.first));
        rb_ary_store(neighbors_arr, i, INT2NUM((int)result_tuple.second));
        result.pop();
      }

      VALUE ret = rb_ary_new2(2);
      rb_ary_store(ret, 0, neighbors_arr);
      rb_ary_store(ret, 1, distances_arr);
      return ret;
    };

    static VALUE _hnsw_bruteforcesearch_save_index(VALUE self, VALUE _filename) {
      std::string filename(StringValuePtr(_filename));
      get_hnsw_bruteforcesearch(self)->saveIndex(filename);
      RB_GC_GUARD(_filename);
      return Qnil;
    };

    static VALUE _hnsw_bruteforcesearch_load_index(VALUE self, VALUE _filename) {
      std::string filename(StringValuePtr(_filename));
      VALUE ivspace = rb_iv_get(self, "@space");
      hnswlib::SpaceInterface<float>* space;
      if (rb_obj_is_instance_of(ivspace, rb_cHnswlibL2Space)) {
        space = RbHnswlibL2Space::get_hnsw_l2space(ivspace);
      } else {
        space = RbHnswlibInnerProductSpace::get_hnsw_ipspace(ivspace);
      }
      hnswlib::BruteforceSearch<float>* index = get_hnsw_bruteforcesearch(self);
      if (index->data_) free(index->data_);
      try {
        index->loadIndex(filename, space);
      } catch(const std::runtime_error& e) {
        rb_raise(rb_eRuntimeError, "%s", e.what());
        return Qnil;
      }
      RB_GC_GUARD(_filename);
      return Qnil;
    };

    static VALUE _hnsw_bruteforcesearch_remove_point(VALUE self, VALUE idx) {
      get_hnsw_bruteforcesearch(self)->removePoint((size_t)NUM2INT(idx));
      return Qnil;
    };

    static VALUE _hnsw_bruteforcesearch_max_elements(VALUE self) {
      return INT2NUM((int)(get_hnsw_bruteforcesearch(self)->maxelements_));
    };

    static VALUE _hnsw_bruteforcesearch_current_count(VALUE self) {
      return INT2NUM((int)(get_hnsw_bruteforcesearch(self)->cur_element_count));
    };
};

const rb_data_type_t RbHnswlibBruteforceSearch::hnsw_bruteforcesearch_type = {
  "RbHnswlibBruteforceSearch",
  {
    NULL,
    RbHnswlibBruteforceSearch::hnsw_bruteforcesearch_free,
    RbHnswlibBruteforceSearch::hnsw_bruteforcesearch_size
  },
  NULL,
  NULL,
  RUBY_TYPED_FREE_IMMEDIATELY
};

#endif /* HNSWLIBEXT_HPP */
