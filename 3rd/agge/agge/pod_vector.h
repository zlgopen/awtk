#pragma once

#include "tools.h"
#include "types.h"

namespace agge
{
	template <typename T>
	class pod_vector
	{
	public:
		typedef T *iterator;
		typedef const T *const_iterator;
		typedef T value_type;

	public:
		explicit pod_vector(count_t initial_size = 0);
		pod_vector(const pod_vector &other);
		~pod_vector();

		iterator push_back(const T &element);
		void pop_back();
		void clear();
		void clear_cache();
		void resize(count_t size);
		void set_end(iterator end);
		void assign(count_t size, const T &value);
		void swap(pod_vector &other);

		const T *data() const;
		bool empty() const;
		count_t size() const;
		count_t capacity() const;

		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;

		T &operator [](count_t index);
		const T &operator [](count_t index) const;

	private:
		union pod_constraint
		{
			T _unused1;
			int _unused2;
		};

	private:
		const pod_vector &operator =(const pod_vector &rhs);

		void grow(count_t by = 0);

	private:
		T *_begin, *_end, *_limit;
	};



	template <typename T>
	inline pod_vector<T>::pod_vector(count_t initial_size)
		: _begin(0), _end(0), _limit(0)
	{	resize(initial_size);	}

	template <typename T>
	inline pod_vector<T>::pod_vector(const pod_vector &other)
		: _begin(new T[other.capacity()]), _end(_begin + other.size()), _limit(_begin + other.capacity())
	{
		const_iterator i = other.begin();
		iterator j = begin();

		while (i != other.end())
			*j++ = *i++;
	}

	template <typename T>
	inline pod_vector<T>::~pod_vector()
	{	clear_cache();	}

	template <typename T>
	inline void pod_vector<T>::clear_cache() 
	{
		if (_begin != 0) {
			delete []_begin;
		}
		_end = 0;
		_begin = 0;
		_limit = 0;
	}

	template <typename T>
	inline typename pod_vector<T>::iterator pod_vector<T>::push_back(const T &element)
	{
		if (_end == _limit)
			grow();
		*_end = element;
		return _end++;
	}

	template <typename T>
	inline void pod_vector<T>::pop_back()
	{	--_end;	}

	template <typename T>
	inline void pod_vector<T>::clear()
	{	_end = _begin;	}

	template <typename T>
	inline void pod_vector<T>::resize(count_t size_)
	{
		if (size_ > capacity())
			grow(size_ - capacity());
		_end = _begin + size_;
	}

	template <typename T>
	inline void pod_vector<T>::set_end(iterator end)
	{	_end = end;	}

	template <typename T>
	inline void pod_vector<T>::assign(count_t size_, const T &value)
	{
		resize(size_);

		for (T *i = _begin; size_; --size_, ++i)
			*i = value;
	}

	template <typename T>
	inline void pod_vector<T>::swap(pod_vector &other)
	{
		iterator t;
		t = _begin, _begin = other._begin, other._begin = t;
		t = _end, _end = other._end, other._end = t;
		t = _limit, _limit = other._limit, other._limit = t;
	}

	template <typename T>
	inline const T *pod_vector<T>::data() const
	{	return _begin;	}

	template <typename T>
	inline bool pod_vector<T>::empty() const
	{	return _begin == _end;	}

	template <typename T>
	inline count_t pod_vector<T>::size() const
	{	return static_cast<count_t>(_end - _begin);	}

	template <typename T>
	inline count_t pod_vector<T>::capacity() const
	{	return static_cast<count_t>(_limit - _begin);	}

	template <typename T>
	inline typename pod_vector<T>::iterator pod_vector<T>::begin()
	{	return _begin;	}

	template <typename T>
	inline typename pod_vector<T>::iterator pod_vector<T>::end()
	{	return _end;	}

	template <typename T>
	inline typename pod_vector<T>::const_iterator pod_vector<T>::begin() const
	{	return _begin;	}

	template <typename T>
	inline typename pod_vector<T>::const_iterator pod_vector<T>::end() const
	{	return _end;	}

	template <typename T>
	inline T &pod_vector<T>::operator [](count_t index)
	{	return _begin[index];	}

	template <typename T>
	inline const T &pod_vector<T>::operator [](count_t index) const
	{	return _begin[index];	}

	template <typename T>
	inline void pod_vector<T>::grow(count_t by)
	{
		count_t size = this->size(), new_capacity = capacity();

		new_capacity += agge_max(2 * by > new_capacity ? by : new_capacity / 2, 1u);

		T *buffer = new T[new_capacity], *p = buffer;
		
		for (iterator i = _begin; i != _end; )
			*p++ = *i++;
		if (_begin != 0) {
			delete []_begin;
		}
		_begin = buffer;
		_end = _begin + size;
		_limit = _begin + new_capacity;
	}
}
