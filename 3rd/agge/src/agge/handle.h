#pragma once

namespace agge {
class handle {
 public:
  enum { storage_size = 32 };

 public:
  template <typename NativeHandleT>
  handle(NativeHandleT native_handle);

  template <typename NativeHandleT>
  operator NativeHandleT() const;

  template <typename NativeHandleT>
  const NativeHandleT* address_of() const;

  template <typename NativeHandleT>
  NativeHandleT* address_of();

 private:
  unsigned char _handle[storage_size];
};

template <typename NativeHandleT>
inline handle::handle(NativeHandleT native_handle) {
  *address_of<NativeHandleT>() = native_handle;
}

template <typename NativeHandleT>
inline handle::operator NativeHandleT() const {
  return *address_of<NativeHandleT>();
}

template <typename NativeHandleT>
inline const NativeHandleT* handle::address_of() const {
  typedef char validate[sizeof(NativeHandleT) <= storage_size ? 1 : -1];
  return static_cast<const NativeHandleT*>(static_cast<const void*>(_handle));
}

template <typename NativeHandleT>
inline NativeHandleT* handle::address_of() {
  typedef char validate[sizeof(NativeHandleT) <= storage_size ? 1 : -1];
  return static_cast<NativeHandleT*>(static_cast<void*>(_handle));
}
}  // namespace agge
