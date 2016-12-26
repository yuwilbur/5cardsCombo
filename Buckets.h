#pragma once

#include <map>
#include <vector>
#include <algorithm>

template<int ITEMS>
class Buckets {
public:
  Buckets() = default;

  void AddSpace(const int item) {
    if (bucketsMap_.find(item) == bucketsMap_.end()) {
      bucketsMap_[item] = 1;
    }
    else {
      bucketsMap_[item]++;
    }
  }

  void Initialize() {
    for (auto& bucketMap : bucketsMap_) {
      buckets_.push_back({ bucketMap.first, 0, bucketMap.second });
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
      buckets_[i + 1].filled++;
      buckets_[i].filled--;
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

      size_t itemIndex = 0;
      for (size_t j = 0; j <= i + 1; ++j) {
        for (size_t k = 0; k < buckets_[j].filled; ++k) {
          items_[itemIndex++] = buckets_[j].item;
        }
      }
      break;
    }
    Print();
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

  std::array<int, ITEMS> GetItems() { return items_; }

private:
  struct Bucket {
    int item;
    size_t filled;
    size_t capacity;
  };

  std::array<int, ITEMS> endItems_;
  std::array<int, ITEMS> items_;
  std::map<int, size_t> bucketsMap_;
  std::vector<Bucket> buckets_;
};
