#ifndef UTILS_MEMORY_MEMORY_H_
#define UTILS_MEMORY_MEMORY_H_

#include <memory>
#include <utility>

namespace utils {
namespace memory {

// START Herb Sutter code sample adaptation
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
// END Herb Sutter code sample adaptation

}  // namespace memory
}  // namespace utils


#endif  // UTILS_MEMORY_MEMORY_H_
