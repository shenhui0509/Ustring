#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <type_traits>

static std::random_device r;
static std::default_random_engine ngx(r());

using std::vector;
using std::string;

template<class T, bool IsInt = true>
struct DispatcherImpl{
  typedef std::uniform_int_distribution<T> type;
};

template<class T>
struct DispatcherImpl<T, false>{
  typedef std::uniform_real_distribution<T> type;
};

template<class T>
struct DistributionDispatcher{
  typedef
    typename DispatcherImpl<T, std::is_integral<T>::value>::type type;
};

template<typename T>
vector<T> generate_n_numerics(size_t n, T lo, T hi)
{
  static_assert(std::is_arithmetic<T>::value, "T must be arithmetic type");
  vector<T> result(n);
  typename DistributionDispatcher<T>::type udist(lo, hi);
  std::generate_n(begin(result), n, [&udist](){return udist(ngx);});
  return result;
}

string generate_random_string(size_t n)
{
  string result(n,0);
  std::uniform_int_distribution<char> dist('a','z');
  std::generate_n(begin(result), n, [&dist](){return dist(ngx);});
  return result;
}
