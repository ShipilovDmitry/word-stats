#include "multithreading.hpp"
#include "mmap.hpp"
#include <future>
#include <vector>

namespace word_stats {
StatsTable processChunk(char const *buffer, size_t start, size_t end) {
  StatsTable stats;
  std::string currentWord;

  for (size_t i = start; i < end; ++i) {
    auto const c = buffer[i];
    if (isalpha(c)) {
      currentWord.push_back(std::tolower(c));
    } else if (!currentWord.empty()) {
      ++stats[currentWord];
      currentWord.clear();
    }
  }

  if (!currentWord.empty()) {
    ++stats[currentWord];
  }

  return stats;
}

void mergeStatsTables(StatsTable &destination, StatsTable const &source) {
  for (auto const &[word, count] : source) {
    destination[word] += count;
  }
}

StatsTable fillStatsFromFileAsync(std::string const &filename) {
  StatsTable stats;

  // Use memory mapped file for fast data access by pointer
  auto const data = MMapData(filename);

  size_t const numThreads = std::thread::hardware_concurrency();
  std::vector<std::future<StatsTable>> futures;

  size_t offset = 0;
  size_t end = 0;

  auto const *ptr = data.ptr();
  for (size_t i = 0; end < data.size() && i < numThreads; ++i) {
    size_t start = (i * data.size()) / numThreads + offset;
    end = ((i + 1) * data.size()) / numThreads + offset;

    // Because of offset we can get out of bounds
    if (end > data.size()) {
      end = data.size();
    }

    // Process case when we split a word between chunks
    // One part in one chunk and second in another
    while (end < data.size()) {
      if (std::isalpha(data.ptr()[end])) {
        offset += 1;
        ++end;
        continue;
      }
      break;
    }

    futures.push_back(std::async(std::launch::async, processChunk,
                                 std::cref(ptr), start, end));
  }

  // Merge results from each thread
  // To single `stats` table
  // We don't need mutex because future.get() is blocking
  for (auto &future : futures) {
    mergeStatsTables(stats, future.get());
  }

  return stats;
}

} // namespace word_stats
