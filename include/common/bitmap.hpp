#ifndef COMMON_BITMAP_HPP
#define COMMON_BITMAP_HPP 1

#include <cstddef>
#include <cstdint>

class Bitmap {
 public:
  Bitmap() = default;
  Bitmap(uint8_t* buffer, size_t size) : m_buffer(buffer), m_size(size) {}
  ~Bitmap() = default;

  void initialize(uint8_t* data, size_t size) {
    this->m_buffer = data;
    this->m_size = size;
  }

  uint8_t* data() const { return this->m_buffer; }

  void set(size_t idx) {
    if (idx > this->m_size) {
      return;
    }

    const size_t byte = idx / 8;
    const size_t bit = idx % 8;

    this->m_buffer[byte] |= (1 << bit);
  }

  bool get(size_t idx) {
    if (idx > this->m_size) {
      return false;
    }

    const size_t byte = idx / 8;
    const size_t bit = idx % 8;

    return this->m_buffer[byte] & (1 << bit);
  }

  void clear(size_t idx) {
    if (idx > this->m_size) {
      return;
    }

    const size_t byte = idx / 8;
    const size_t bit = idx % 8;

    this->m_buffer[byte] &= ~(1 << bit);
  }

 private:
  uint8_t* m_buffer = nullptr;
  size_t m_size = 0;
};

#endif  // COMMON_BITMAP_HPP