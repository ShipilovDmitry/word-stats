#pragma once

#include "mmap.hpp"
#include <fcntl.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace word_stats {

class MMapData {
public:
  explicit MMapData(std::string const &filename) {
    m_fd = open(filename.c_str(), O_RDONLY);
    if (m_fd == -1) {
      std::cerr << "Failed to open file: " << filename << std::endl;
      throw std::logic_error{"Failed to open file"};
    }

    struct stat fileStat;
    if (fstat(m_fd, &fileStat) == -1) {
      std::cerr << "Failed to get file stats: " << filename << std::endl;
      close(m_fd);
      throw std::logic_error{"Failed to get file stats"};
    }

    m_size = fileStat.st_size;
    if (m_size == 0) {
      std::cerr << "File is empty: " << filename << std::endl;
      close(m_fd);
      throw std::logic_error{"File is empty"};
    }

    m_ptr = static_cast<char *>(
        mmap(nullptr, m_size, PROT_READ, MAP_PRIVATE, m_fd, 0));
    if (m_ptr == MAP_FAILED) {
      std::cerr << "Failed to map file: " << filename << std::endl;
      close(m_fd);
      throw std::logic_error{"Failed to map file"};
    }
  }

  char const *ptr() const { return m_ptr; }
  size_t size() const { return m_size; }

  ~MMapData() {
    if (munmap(reinterpret_cast<void *>(m_ptr), m_size) == -1) {
      std::cerr << "Failed to unmap memory" << std::endl;
    }
    close(m_fd);
  }

private:
  char *m_ptr;
  size_t m_size;
  int m_fd;
};

} // namespace word_stats
