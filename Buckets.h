#pragma once

#include <map>
#include <vector>
#include <algorithm>

template<int ITEMS>
class Buckets {
public:
  Buckets() = default;

  void AddSpace(const int item) {
    if (histogram_.find(item) == histogram_.end()) {
      histogram_[item] = 1;
    }
    else {
      histogram_[item]++;
    }
  }

  void Initialize() {
    for (auto& histogram : histogram_) {
      buckets_.push_back({ histogram.first, 0, histogram.second });
    }

    for (auto& item : items_) {
      for (auto& bucket : buckets_) {
        if (bucket.filled != bucket.capacity) {
          bucket.filled++;
          item = bucket.item;
          break;
        }
      }
    }

    size_t endItemIndex = 0;
    for (int i = buckets_.size() - 1; i >= 0; --i) {
      for (size_t j = 0; j < buckets_[i].capacity; ++j) {
        endItems_[endItemIndex++] = buckets_[i].item;
        if (endItemIndex == endItems_.size())
          break;
      }
      if (endItemIndex == endItems_.size())
        break;
    }
    std::sort(endItems_.begin(), endItems_.end());
  }

  bool Increment() {
    if (items_ == endItems_)
      return false;
    for (size_t i = 0; i < buckets_.size() - 1; ++i) {
      if (buckets_[i].filled == 0)
        continue;
      buckets_[i].filled--;
      buckets_[i + 1].filled++;
      
      // Check for carry
      for (i = i + 1; i < buckets_.size() - 1; ++i) {
        if (buckets_[i].filled <= buckets_[i].capacity)
          break;
        buckets_[i].filled--;
        buckets_[i + 1].filled++;
      }
      i--;

      // Re-organize the number so it increments properly
      if (i != 0) {
        size_t filled = 0;
        for (size_t j = 0; j <= i; ++j) {
          filled += buckets_[j].filled;
          buckets_[j].filled = 0;
        }
        for (size_t j = 0; j < filled; ++j) {
          for (auto& bucket : buckets_) {
            if (bucket.filled != bucket.capacity) {
              bucket.filled++;
              break;
            }
          }
        }
      }

      size_t itemIndex = 0;
      for (size_t j = 0; j <= i + 1; ++j) {
        for (size_t k = 0; k < buckets_[j].filled; ++k) {
          items_[itemIndex++] = buckets_[j].item;
        }
      }
      break;
    }
    //Print();
    return true;
  }

  void Print() {
    std::cout << "ITEMS [ ";
    for (auto& item : items_)
      std::cout << item << " ";
    std::cout << "] ";
    std::cout << "BUCKET [ ";
    for (auto& bucket : buckets_) {
      std::cout << bucket.item << "(" << bucket.filled << "/" << bucket.capacity << ") ";
    }
    std::cout << "] ";
    std::cout << std::endl;
  }

  const std::array<int, ITEMS>& GetItems() const { return items_; }
  const std::map<int, size_t>& GetHistogram() const { return histogram_; }

private:
  struct Bucket {
    int item;
    size_t filled;
    size_t capacity;
  };

  std::array<int, ITEMS> endItems_;
  std::array<int, ITEMS> items_;
  std::map<int, size_t> histogram_;
  std::vector<Bucket> buckets_;
};
