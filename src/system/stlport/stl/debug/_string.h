/*
 * Copyright (c) 1997-1999
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1999
 * Boris Fomitchev
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 */

#ifndef _STLP_DBG_STRING_H
#define _STLP_DBG_STRING_H

#ifndef _STLP_DBG_ITERATOR_H
#  include <stl/debug/_iterator.h>
#endif

namespace _STLP_STD {

#define _STLP_NON_DBG_STRING_NAME _STLP_NON_DBG_NAME(str)
#define _STLP_NON_DBG_STRING _STLP_PRIV::_STLP_NON_DBG_STRING_NAME <_CharT, _Traits, _Alloc>

template <class _CharT, class _Traits, class _Alloc>
class basic_string : private _STLP_PRIV::__construct_checker<_STLP_NON_DBG_STRING >
{
protected:
  typedef _STLP_NON_DBG_STRING _Base;
  typedef basic_string<_CharT, _Traits, _Alloc> _Self;
  typedef _STLP_PRIV::__construct_checker<_STLP_NON_DBG_STRING > _ConstructCheck;
  typedef typename _IsPOD<_CharT>::_Type _Char_Is_POD;

public:
  __IMPORT_CONTAINER_TYPEDEFS(_Base)
  typedef typename _Base::traits_type traits_type;
  typedef _STLP_PRIV::_DBG_iter<_Base, _STLP_PRIV::_DbgTraits<_Nonconst_traits<value_type> > > iterator;
  typedef _STLP_PRIV::_DBG_iter<_Base, _STLP_PRIV::_DbgTraits<_Const_traits<value_type> > > const_iterator;
  _STLP_DECLARE_RANDOM_ACCESS_REVERSE_ITERATORS;

public:                         // Constructor, destructor, assignment.
  typedef typename _Base::_Reserve_t _Reserve_t;

private:
  _Base _M_non_dbg_impl;
  _STLP_PRIV::__owned_list _M_iter_list;

  void _Invalidate_all()
  { _M_iter_list._Invalidate_all(); }
  void _Compare_Capacity (size_type __old_capacity) {
    if (this->capacity() > __old_capacity) {
      _Invalidate_all();
    }
  }
  void _Invalidate_iterator(const iterator& __it)
  { _STLP_PRIV::__invalidate_iterator(&_M_iter_list, __it); }
  void _Invalidate_iterators(const iterator& __f, const iterator& __l)
  { _STLP_PRIV::__invalidate_range(&_M_iter_list, __f, __l); }

public:
  static const size_t npos = ~(size_t)0;

  allocator_type get_allocator() const { return _M_non_dbg_impl.get_allocator(); }

  explicit basic_string(const allocator_type& __a = allocator_type())
    : _M_non_dbg_impl(__a), _M_iter_list(&_M_non_dbg_impl) {}

  basic_string(_Reserve_t __r, size_t __n,
               const allocator_type& __a = allocator_type())
    : _M_non_dbg_impl(__r, __n, __a), _M_iter_list(&_M_non_dbg_impl) {}

  basic_string(const _Self& __s)
    : _ConstructCheck(__s),
      _M_non_dbg_impl(__s._M_non_dbg_impl), _M_iter_list(&_M_non_dbg_impl) {}

  basic_string(const _Self& __s, size_type __pos, size_type __n = npos,
               const allocator_type& __a = allocator_type())
    : _M_non_dbg_impl(__s._M_non_dbg_impl, __pos, __n, __a),
      _M_iter_list(&_M_non_dbg_impl) {}

  basic_string(const _CharT* __s, size_type __n,
               const allocator_type& __a = allocator_type())
    : _ConstructCheck(__s), _M_non_dbg_impl(__s, __n, __a),
      _M_iter_list(&_M_non_dbg_impl) {}

  basic_string(const _CharT* __s,
               const allocator_type& __a = allocator_type())
    : _ConstructCheck(__s),
      _M_non_dbg_impl(__s, __a), _M_iter_list(&_M_non_dbg_impl) {}

  basic_string(size_type __n, _CharT __c,
               const allocator_type& __a = allocator_type())
    : _M_non_dbg_impl(__n, __c, __a), _M_iter_list(&_M_non_dbg_impl) {}

  basic_string(__move_source<_Self> src)
    : _M_non_dbg_impl(__move_source<_Base >(src.get()._M_non_dbg_impl)),
      _M_iter_list(&_M_non_dbg_impl) {
#if defined (_STLP_NO_EXTENSIONS) || (_STLP_DEBUG_LEVEL == _STLP_STANDARD_DBG_LEVEL)
    src.get()._M_iter_list._Invalidate_all();
#else
    src.get()._M_iter_list._Set_owner(_M_iter_list);
#endif
  }

  template <class _InputIterator>
  basic_string(_InputIterator __f, _InputIterator __l,
               const allocator_type & __a = allocator_type())
    : _ConstructCheck(__f, __l),
      _M_non_dbg_impl(_STLP_PRIV::_Non_Dbg_iter(__f), _STLP_PRIV::_Non_Dbg_iter(__l), __a),
      _M_iter_list(&_M_non_dbg_impl) {}

private:
  // constructor from non-debug version for substr
  basic_string (const _Base& __x)
    : _M_non_dbg_impl(__x), _M_iter_list(&_M_non_dbg_impl) {}

public:
  _Self& operator=(const _Self& __s) {
    if (this != &__s) {
      assign(__s);
    }
    return *this;
  }

  _Self& operator=(const _CharT* __s) {
    _STLP_VERBOSE_ASSERT((__s != 0), _StlMsg_INVALID_ARGUMENT)
    return assign(__s);
  }

  _Self& operator=(_CharT __c) {
    return assign(1, __c);
  }

  // Iterators.
  iterator begin() { return iterator(&_M_iter_list, _M_non_dbg_impl.begin()); }
  const_iterator begin() const { return const_iterator(&_M_iter_list, _M_non_dbg_impl.begin()); }
  iterator end() { return iterator(&_M_iter_list, _M_non_dbg_impl.end()); }
  const_iterator end() const { return const_iterator(&_M_iter_list, _M_non_dbg_impl.end()); }

  reverse_iterator rbegin()             { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
  reverse_iterator rend()               { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const   { return const_reverse_iterator(begin()); }

  // Size, capacity, etc.
  size_type size() const { return _M_non_dbg_impl.size(); }
  size_type length() const { return _M_non_dbg_impl.length(); }
  size_t max_size() const { return _M_non_dbg_impl.max_size(); }

  void resize(size_type __n, _CharT __c) {
    if (__n > capacity())
      _Invalidate_all();
    else if (__n < size())
      _Invalidate_iterators(begin() + __n, end());
    _M_non_dbg_impl.resize(__n, __c);
  }
  void resize(size_type __n) { resize(__n, _CharT()); }
  size_type capacity() const { return _M_non_dbg_impl.capacity(); }

  void reserve(size_type __s = 0) {
    if (__s > capacity()) _Invalidate_all();
    _M_non_dbg_impl.reserve(__s);
  }

  void clear() {
    _Invalidate_all();
    _M_non_dbg_impl.clear();
  }

  bool empty() const { return _M_non_dbg_impl.empty(); }

  const_reference operator[](size_type __n) const {
    _STLP_VERBOSE_ASSERT(__n <= this->size(), _StlMsg_OUT_OF_BOUNDS);
    return _M_non_dbg_impl[__n];
  }

  reference operator[](size_type __n) {
    _STLP_VERBOSE_ASSERT(__n < this->size(), _StlMsg_OUT_OF_BOUNDS)
    return _M_non_dbg_impl[__n];
  }

  const_reference at(size_type __n) const { return _M_non_dbg_impl.at(__n); }
  reference at(size_type __n) { return _M_non_dbg_impl.at(__n); }

  // Append, operator+=, push_back.
  _Self& operator+=(const _Self& __s) { return append(__s); }
  _Self& operator+=(const _CharT* __s) {
    _STLP_VERBOSE_ASSERT((__s != 0), _StlMsg_INVALID_ARGUMENT)
    return append(__s);
  }
  _Self& operator+=(_CharT __c) { return append(1, __c); }

  template <class _InputIter>
  _Self& append(_InputIter __first, _InputIter __last) {
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_range(__first, __last))
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.append(_STLP_PRIV::_Non_Dbg_iter(__first), _STLP_PRIV::_Non_Dbg_iter(__last));
    _Compare_Capacity(__old_capacity);
    return *this;
  }

#if !defined (_STLP_NO_EXTENSIONS)
  _Self& append(const _CharT* __f, const _CharT* __l) {
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_ptr_range(__f, __l))
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.append(__f, __l);
    _Compare_Capacity(__old_capacity);
    return *this;
  }

  _Self& append(const_iterator __f, const_iterator __l) {
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_range(__f, __l))
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.append(__f._M_iterator, __l._M_iterator);
    _Compare_Capacity(__old_capacity);
    return *this;
  }
#endif

  _Self& append(const _Self& __s) {
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.append(__s._M_non_dbg_impl);
    _Compare_Capacity(__old_capacity);
    return *this;
  }

  _Self& append(const _Self& __s, size_type __pos, size_type __n) {
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.append(__s._M_non_dbg_impl, __pos, __n);
    _Compare_Capacity(__old_capacity);
    return *this;
  }

  _Self& append(const _CharT* __s, size_type __n) {
    _STLP_VERBOSE_ASSERT((__s != 0), _StlMsg_INVALID_ARGUMENT)
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.append(__s, __n);
    _Compare_Capacity(__old_capacity);
    return *this;
  }

  _Self& append(const _CharT* __s) {
    _STLP_VERBOSE_ASSERT((__s != 0), _StlMsg_INVALID_ARGUMENT)
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.append(__s);
    _Compare_Capacity(__old_capacity);
    return *this;
  }

  _Self& append(size_type __n, _CharT __c) {
    size_type __old_capacity = this->capacity();
    _M_non_dbg_impl.append(__n, __c);
    _Compare_Capacity(__old_capacity);
    return *this;
  }

  void push_back(_CharT __c) {
    size_type __old_capacity = this->capacity();
    _M_non_dbg_impl.push_back(__c);
    _Compare_Capacity(__old_capacity);
  }

  void pop_back() {
    _Invalidate_iterator(this->end());
    _M_non_dbg_impl.pop_back();
  }

  // Assign
private:
  void _M_check_assign(size_type __n) {
    if (__n > capacity()) {
      _Invalidate_all();
    }
    else if (__n < size()) {
      _Invalidate_iterators(begin() + __n, end());
    }
  }

public:
  _Self& assign(const _Self& __s) {
    _M_check_assign(__s.size());
    _M_non_dbg_impl.assign(__s._M_non_dbg_impl);
    return *this;
  }

  _Self& assign(const _Self& __s, size_type __pos, size_type __n) {
    if (__pos < __s.size()) {
      _M_check_assign((min) (__n, __s.size() - __pos));
    }
    _M_non_dbg_impl.assign(__s._M_non_dbg_impl, __pos, __n);
    return *this;
  }

  _Self& assign(const _CharT* __s, size_type __n) {
    _STLP_VERBOSE_ASSERT((__s != 0), _StlMsg_INVALID_ARGUMENT)
    _M_check_assign((min) (_Traits::length(__s), __n));
    _M_non_dbg_impl.assign(__s, __s + __n);
    return *this;
  }

  _Self& assign(const _CharT* __s) {
    _STLP_VERBOSE_ASSERT((__s != 0), _StlMsg_INVALID_ARGUMENT)
    _M_check_assign(_Traits::length(__s));
    _M_non_dbg_impl.assign(__s);
    return *this;
  }

  _Self& assign(size_type __n, _CharT __c) {
    _M_check_assign(__n);
    _M_non_dbg_impl.assign(__n, __c);
    return *this;
  }

private:
  template <class _Integer>
  void _M_assign_dispatch(_Integer __n, _Integer __x, const __true_type& /*_Integral*/) {
    _M_check_assign(__n);
    _M_non_dbg_impl.assign((size_type)__n, (_CharT)__x);
  }

  template <class _InputIter>
  void _M_assign_dispatch(_InputIter __f, _InputIter __l, const __false_type& /*_Integral*/)  {
    _M_check_assign(distance(__f, __l));
    _M_non_dbg_impl.assign(_STLP_PRIV::_Non_Dbg_iter(__f), _STLP_PRIV::_Non_Dbg_iter(__l));
  }
public:
  template <class _InputIter>
  inline _Self& assign(_InputIter __first, _InputIter __last) {
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_range(__first, __last))
    typedef typename _IsIntegral<_InputIter>::_Ret _Integral;
    _M_assign_dispatch(__first, __last, _Integral());
    return *this;
  }

#if !defined (_STLP_NO_EXTENSIONS)
  _Self& assign(const _CharT* __f, const _CharT* __l) {
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_ptr_range(__f, __l))
    _M_check_assign(distance(__f, __l));
    _M_non_dbg_impl.assign(__f, __l);
    return *this;
  }
  _Self& assign(const_iterator __f, const_iterator __l) {
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_range(__f, __l))
    _M_check_assign(distance(__f, __l));
    _M_non_dbg_impl.assign(__f._M_iterator, __l._M_iterator);
    return *this;
  }
#endif

  // Insert
  _Self& insert(size_type __pos, const _Self& __s) {
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.insert(__pos, __s._M_non_dbg_impl);
    _Compare_Capacity(__old_capacity);
    return *this;
  }

  _Self& insert(size_type __pos, const _Self& __s,
                size_type __beg, size_type __n) {
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.insert(__pos, __s._M_non_dbg_impl, __beg, __n);
    _Compare_Capacity(__old_capacity);
    return *this;
  }

  _Self& insert(size_type __pos, const _CharT* __s, size_type __n) {
    _STLP_VERBOSE_ASSERT((__s != 0), _StlMsg_INVALID_ARGUMENT)
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.insert(__pos, __s, __n);
    _Compare_Capacity(__old_capacity);
    return *this;
  }

  _Self& insert(size_type __pos, const _CharT* __s) {
    _STLP_VERBOSE_ASSERT((__s != 0), _StlMsg_INVALID_ARGUMENT)
    return insert(__pos, __s, _Traits::length(__s));
  }

  _Self& insert(size_type __pos, size_type __n, _CharT __c) {
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.insert(__pos, __n, __c);
    _Compare_Capacity(__old_capacity);
    return *this;
  }

  iterator insert(iterator __p, _CharT __c) {
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_if_owner(&_M_iter_list,__p))
    size_type __old_capacity = capacity();
    typename _Base::iterator __ret = _M_non_dbg_impl.insert(__p._M_iterator, __c);
    _Compare_Capacity(__old_capacity);
    return iterator(&_M_iter_list, __ret);
  }

  void insert(iterator __p, size_t __n, _CharT __c) {
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_if_owner(&_M_iter_list,__p))
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.insert(__p._M_iterator, __n, __c);
    _Compare_Capacity(__old_capacity);
  }

private:
  template <class _RandomIter>
  void _M_insert_aux (iterator __p, _RandomIter __first, _RandomIter __last,
                      const __true_type& /*_IsIterator*/)
  { _M_non_dbg_impl.insert(__p._M_iterator, __first._M_iterator, __last._M_iterator); }

  template<class _InputIter>
  void _M_insert_aux (iterator __p, _InputIter __first, _InputIter __last,
                      const __false_type& /*_IsIterator*/) {
    _M_non_dbg_impl.insert(__p._M_iterator,
                           _STLP_PRIV::_Non_Dbg_iter(__first), _STLP_PRIV::_Non_Dbg_iter(__last));
  }

public:
  template <class _InputIter>
  void insert(iterator __p, _InputIter __first, _InputIter __last) {
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_if_owner(&_M_iter_list,__p))
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_range(__first,__last))

    /* In debug mode we are encapsulating non debug string iterators in debug one.
     * Doing so the non debug implementation will not check for self insertion
     * (x.insert(x.begin(), x.begin(), x.end()). To avoid this problem we try to
     * guess when _InputIter is iterator or const_iterator and in this case call
     * the non debug insert method with non debug string iterator.
     */
    typedef typename _AreSameUnCVTypes<_InputIter, iterator>::_Ret _IsNonConstIterator;
    typedef typename _AreSameUnCVTypes<_InputIter, const_iterator>::_Ret _IsConstIterator;
    typedef typename _Lor2<_IsNonConstIterator, _IsConstIterator>::_Ret _IsIterator;

    size_type __old_capacity = this->capacity();
    _M_insert_aux(__p, __first, __last, _IsIterator());
    _Compare_Capacity(__old_capacity);
  }

#if !defined (_STLP_NO_EXTENSIONS)
  void insert(iterator __p, const_iterator __f, const_iterator __l) {
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_if_owner(&_M_iter_list,__p))
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_range(__f,__l))
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.insert(__p._M_iterator, __f._M_iterator, __l._M_iterator);
    _Compare_Capacity(__old_capacity);
  }
  void insert(iterator __p, const _CharT* __f, const _CharT* __l) {
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_if_owner(&_M_iter_list,__p))
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_ptr_range(__f,__l))
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.insert(__p._M_iterator, __f, __l);
    _Compare_Capacity(__old_capacity);
  }
#endif

  // Erase.
  _Self& erase(size_type __pos = 0, size_type __n = npos) {
    if (__pos < size()) {
      _Invalidate_iterators(begin() + __pos, end());
    }
    _M_non_dbg_impl.erase(__pos, __n);
    return *this;
  }
  iterator erase(iterator __pos) {
    _STLP_DEBUG_CHECK(_STLP_PRIV::_Dereferenceable(__pos))
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_if_owner(&_M_iter_list,__pos))
    _Invalidate_iterators(__pos, end());
    return iterator(&_M_iter_list, _M_non_dbg_impl.erase(__pos._M_iterator));
  }
  iterator erase(iterator __f, iterator __l) {
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_range(__f, __l, begin(), end()))
    _Invalidate_iterators(__f, end());
    return iterator(&_M_iter_list, _M_non_dbg_impl.erase(__f._M_iterator, __l._M_iterator));
  }

  // Substring.
  _Self substr(size_type __pos = 0, size_type __n = npos) const
  { return _M_non_dbg_impl.substr(__pos, __n); }

  // Replace.  (Conceptually equivalent to erase followed by insert.)
  _Self& replace(size_type __pos, size_type __n, const _Self& __s) {
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.replace(__pos, __n, __s._M_non_dbg_impl);
    _Compare_Capacity(__old_capacity);
    return *this;
  }

  _Self& replace(size_type __pos1, size_type __n1, const _Self& __s,
                 size_type __pos2, size_type __n2) {
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.replace(__pos1, __n1, __s._M_non_dbg_impl, __pos2, __n2);
    _Compare_Capacity(__old_capacity);
    return *this;
  }

  _Self& replace(size_type __pos, size_type __n1, const _CharT* __s, size_type __n2) {
    _STLP_VERBOSE_ASSERT((__s != 0), _StlMsg_INVALID_ARGUMENT)
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.replace(__pos, __n1, __s, __n2);
    _Compare_Capacity(__old_capacity);
    return *this;
  }

  _Self& replace(size_type __pos, size_type __n1, const _CharT* __s) {
    _STLP_VERBOSE_ASSERT((__s != 0), _StlMsg_INVALID_ARGUMENT)
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.replace(__pos, __n1, __s);
    _Compare_Capacity(__old_capacity);
    return *this;
  }

  _Self& replace(size_type __pos, size_type __n1, size_type __n2, _CharT __c) {
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.replace(__pos, __n1, __n2, __c);
    _Compare_Capacity(__old_capacity);
    return *this;
  }

  _Self& replace(iterator __f, iterator __l, const _Self& __s) {
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_range(__f, __l, begin(), end()))
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.replace(__f._M_iterator, __l._M_iterator, __s._M_non_dbg_impl);
    _Compare_Capacity(__old_capacity);
    return *this;
  }

  _Self& replace(iterator __f, iterator __l, const _CharT* __s, size_type __n) {
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_range(__f, __l, begin(), end()))
    _STLP_VERBOSE_ASSERT((__s != 0), _StlMsg_INVALID_ARGUMENT)
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.replace(__f._M_iterator, __l._M_iterator, __s, __n);
    _Compare_Capacity(__old_capacity);
    return *this;
  }

  _Self& replace(iterator __f, iterator __l, const _CharT* __s) {
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_range(__f, __l, begin(), end()))
    _STLP_VERBOSE_ASSERT((__s != 0), _StlMsg_INVALID_ARGUMENT)
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.replace(__f._M_iterator, __l._M_iterator, __s);
    _Compare_Capacity(__old_capacity);
    return *this;
  }

  _Self& replace(iterator __f, iterator __l, size_type __n, _CharT __c) {
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_range(__f, __l, begin(), end()))
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.replace(__f._M_iterator, __l._M_iterator, __n, __c);
    _Compare_Capacity(__old_capacity);
    return *this;
  }

private:
  template <class _RandomIter>
  void _M_replace_aux(iterator __first, iterator __last,
                      _RandomIter __f, _RandomIter __l, __true_type const& /*_IsIterator*/)
  { _M_non_dbg_impl.replace(__first._M_iterator, __last._M_iterator, __f._M_iterator, __l._M_iterator); }

  template <class _InputIter>
  void _M_replace_aux(iterator __first, iterator __last,
                      _InputIter __f, _InputIter __l, __false_type const& /*_IsIterator*/) {
    _M_non_dbg_impl.replace(__first._M_iterator, __last._M_iterator,
                            _STLP_PRIV::_Non_Dbg_iter(__f), _STLP_PRIV::_Non_Dbg_iter(__l));
  }

public:
  template <class _InputIter>
  _Self& replace(iterator __first, iterator __last,
                 _InputIter __f, _InputIter __l) {
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_range(__first, __last, begin(), end()))
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_range(__f, __l))

    /* See insert comment for reson of iterator detection.
     */
    typedef typename _AreSameUnCVTypes<_InputIter, iterator>::_Ret _IsNonConstIterator;
    typedef typename _AreSameUnCVTypes<_InputIter, const_iterator>::_Ret _IsConstIterator;
    typedef typename _Lor2<_IsNonConstIterator, _IsConstIterator>::_Ret _IsIterator;

    size_type __old_capacity = capacity();
    _M_replace_aux(__first, __last, __f, __l, _IsIterator());
    _Compare_Capacity(__old_capacity);
    return *this;
  }

#if !defined (_STLP_NO_EXTENSIONS)
  _Self& replace(iterator __first, iterator __last,
                 const_iterator __f, const_iterator __l) {
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_range(__first, __last, begin(), end()))
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_range(__f, __l))
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.replace(__first._M_iterator, __last._M_iterator,
                            __f._M_iterator, __l._M_iterator);
    _Compare_Capacity(__old_capacity);
    return *this;
  }

  _Self& replace(iterator __first, iterator __last,
                 const _CharT* __f, const _CharT* __l) {
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_range(__first, __last, begin(), end()))
    _STLP_DEBUG_CHECK(_STLP_PRIV::__check_ptr_range(__f, __l))
    size_type __old_capacity = capacity();
    _M_non_dbg_impl.replace(__first._M_iterator, __last._M_iterator, __f, __l);
    _Compare_Capacity(__old_capacity);
    return *this;
  }
#endif

  // Other modifier member functions.
  void swap(_Self& __s) {
    _M_iter_list._Swap_owners(__s._M_iter_list);
    _M_non_dbg_impl.swap(__s._M_non_dbg_impl);
  }

  int compare(const _Self& __s) const
  { return _M_non_dbg_impl.compare(__s._M_non_dbg_impl); }
  int compare(size_type __pos, size_type __n, const _Self& __s) const
  { return _M_non_dbg_impl.compare(__pos, __n, __s._M_non_dbg_impl); }
  int compare(size_type __pos1, size_type __n1, const _Self& __s,
              size_type __pos2, size_type __n2) const
  { return _M_non_dbg_impl.compare(__pos1, __n1, __s._M_non_dbg_impl, __pos2, __n2); }
  int compare(const _CharT* __s) const {
    return _M_non_dbg_impl.compare(__s);
  }
  int compare(size_type __pos, size_type __n, const _CharT* __s) const {
    return _M_non_dbg_impl.compare(__pos, __n, __s);
  }
  int compare(size_type __pos1, size_type __n1, const _CharT* __s,
              size_type __n2) const {
    return _M_non_dbg_impl.compare(__pos1, __n1, __s, __n2);
  }

  // Helper functions for compare.
  static int _M_compare(const _CharT* __f1, const _CharT* __l1,
                                   const _CharT* __f2, const _CharT* __l2)
  { return _Base::_M_compare(__f1, __l1, __f2, __l2); }
  static int _M_compare(const_iterator __f1, const_iterator __l1,
                                   const _CharT* __f2, const _CharT* __l2)
  { return _Base::_M_compare(__f1._M_iterator, __l1._M_iterator, __f2, __l2); }
  static int _M_compare(const _CharT* __f1, const _CharT* __l1,
                                   const_iterator __f2, const_iterator __l2)
  { return _Base::_M_compare(__f1, __l1, __f2._M_iterator, __l2._M_iterator); }
  static int _M_compare(const_iterator __f1, const_iterator __l1,
                                   const_iterator __f2, const_iterator __l2)
  { return _Base::_M_compare(__f1._M_iterator, __l1._M_iterator, __f2._M_iterator, __l2._M_iterator); }

  const _CharT* c_str() const { return _M_non_dbg_impl.c_str(); }
  const _CharT* data()  const { return _M_non_dbg_impl.data(); }

  size_type copy(_CharT* __s, size_type __n, size_type __pos = 0) const
  { return _M_non_dbg_impl.copy(__s, __n, __pos); }

  // find.
  size_type find(const _Self& __s, size_type __pos = 0) const
  { return _M_non_dbg_impl.find(__s._M_non_dbg_impl, __pos); }
  size_type find(const _CharT* __s, size_type __pos = 0) const {
    _STLP_VERBOSE_ASSERT((__s != 0), _StlMsg_INVALID_ARGUMENT)
    return _M_non_dbg_impl.find(__s, __pos);
  }
  size_type find(const _CharT* __s, size_type __pos, size_type __n) const {
    _STLP_VERBOSE_ASSERT((__s != 0), _StlMsg_INVALID_ARGUMENT)
    return _M_non_dbg_impl.find(__s, __pos, __n);
  }
  // WIE: Versant schema compiler 5.2.2 ICE workaround
  size_type find(_CharT __c) const { return find(__c, 0); }
  size_type find(_CharT __c, size_type __pos /* = 0 */) const
  { return _M_non_dbg_impl.find(__c, __pos); }

  // rfind.
  size_type rfind(const _Self& __s, size_type __pos = npos) const
  { return _M_non_dbg_impl.rfind(__s._M_non_dbg_impl, __pos); }
  size_type rfind(const _CharT* __s, size_type __pos = npos) const {
    _STLP_VERBOSE_ASSERT((__s != 0), _StlMsg_INVALID_ARGUMENT)
    return _M_non_dbg_impl.rfind(__s, __pos);
  }
  size_type rfind(const _CharT* __s, size_type __pos, size_type __n) const {
    _STLP_VERBOSE_ASSERT((__s != 0), _StlMsg_INVALID_ARGUMENT)
    return _M_non_dbg_impl.rfind(__s, __pos, __n);
  }
  size_type rfind(_CharT __c, size_type __pos = npos) const
  { return _M_non_dbg_impl.rfind(__c, __pos); }

  // find_first_of
  size_type find_first_of(const _Self& __s, size_type __pos = 0) const
  { return _M_non_dbg_impl.find_first_of(__s._M_non_dbg_impl, __pos); }
  size_type find_first_of(const _CharT* __s, size_type __pos = 0) const {
    _STLP_VERBOSE_ASSERT((__s != 0), _StlMsg_INVALID_ARGUMENT)
    return _M_non_dbg_impl.find_first_of(__s, __pos);
  }
  size_type find_first_of(const _CharT* __s, size_type __pos, size_type __n) const {
    _STLP_VERBOSE_ASSERT((__s != 0), _StlMsg_INVALID_ARGUMENT)
    return _M_non_dbg_impl.find_first_of(__s, __pos, __n);
  }
  size_type find_first_of(_CharT __c, size_type __pos = 0) const
  { return _M_non_dbg_impl.find_first_of(__c, __pos); }

  // find_last_of
  size_type find_last_of(const _Self& __s, size_type __pos = npos) const
  { return _M_non_dbg_impl.find_last_of(__s._M_non_dbg_impl, __pos); }
  size_type find_last_of(const _CharT* __s, size_type __pos = npos) const {
    _STLP_VERBOSE_ASSERT((__s != 0), _StlMsg_INVALID_ARGUMENT)
    return _M_non_dbg_impl.find_last_of(__s, __pos);
  }
  size_type find_last_of(const _CharT* __s, size_type __pos, size_type __n) const {
    _STLP_VERBOSE_ASSERT((__s != 0), _StlMsg_INVALID_ARGUMENT)
    return _M_non_dbg_impl.find_last_of(__s, __pos, __n);
  }
  size_type find_last_of(_CharT __c, size_type __pos = npos) const
  { return _M_non_dbg_impl.rfind(__c, __pos); }

  // find_first_not_of
  size_type find_first_not_of(const _Self& __s, size_type __pos = 0) const
  { return _M_non_dbg_impl.find_first_not_of(__s._M_non_dbg_impl, __pos); }
  size_type find_first_not_of(const _CharT* __s, size_type __pos = 0) const {
    _STLP_VERBOSE_ASSERT((__s != 0), _StlMsg_INVALID_ARGUMENT)
    return _M_non_dbg_impl.find_first_not_of(__s, __pos);
  }
  size_type find_first_not_of(const _CharT* __s, size_type __pos, size_type __n) const {
    _STLP_VERBOSE_ASSERT((__s != 0), _StlMsg_INVALID_ARGUMENT)
    return _M_non_dbg_impl.find_first_not_of(__s, __pos, __n);
  }
  size_type find_first_not_of(_CharT __c, size_type __pos = 0) const
  { return _M_non_dbg_impl.find_first_not_of(__c, __pos); }

  // find_last_not_of
  size_type find_last_not_of(const _Self& __s, size_type __pos = npos) const
  { return _M_non_dbg_impl.find_last_not_of(__s._M_non_dbg_impl, __pos); }
  size_type find_last_not_of(const _CharT* __s, size_type __pos = npos) const {
    _STLP_VERBOSE_ASSERT((__s != 0), _StlMsg_INVALID_ARGUMENT)
    return _M_non_dbg_impl.find_last_not_of(__s, __pos);
  }
  size_type find_last_not_of(const _CharT* __s, size_type __pos, size_type __n) const {
    _STLP_VERBOSE_ASSERT((__s != 0), _StlMsg_INVALID_ARGUMENT)
    return _M_non_dbg_impl.find_last_not_of(__s, __pos, __n);
  }
  size_type find_last_not_of(_CharT __c, size_type __pos = npos) const
  { return _M_non_dbg_impl.find_last_not_of(__c, __pos); }

#if defined (_STLP_USE_TEMPLATE_EXPRESSION)
#  include <stl/debug/_string_sum_methods.h>
#endif /* _STLP_USE_TEMPLATE_EXPRESSION */
};

#undef _STLP_NON_DBG_STRING
#undef _STLP_NON_DBG_STRING_NAME

template <class _CharT, class _Traits, class _Alloc>
const size_t basic_string<_CharT, _Traits, _Alloc>::npos;

}

#if defined (basic_string)
#undef basic_string
#endif

#endif /* _STLP_DBG_STRING */

// Local Variables:
// mode:C++
// End:
