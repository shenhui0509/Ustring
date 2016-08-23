#ifndef USTR_LOGGING_H_
#define USTR_LOGGING_H_

#include <cassert>
#include <cstdio>
#include <cstdint>
#include <ostream>
#include <sstream>

#define DEFAULT_LOG_LEVEL 0
#ifndef LOG_LEVEL
  #define LOG_LEVEL DEFAULT_LOG_LEVEL
#endif

int32_t min_log_lovel = LOG_LEVEL;

/* Debug-only checking */
#define DCHECK(condition) assert(condition)
#define DCHECK_EQ(val1, val2) assert((val1) == (val2))
#define DCHECK_NE(val1, val2) assert((val1) != (val2))
#define DCHECK_LE(val1, val2) assert((val1) <= (val2))
#define DCHECK_LT(val1, val2) assert((val1) < (val2))
#define DCHECK_GE(val1, val2) assert((val1) >= (val2))
#define DCHECK_GT(val1, val2) assert((val1) > (val2))

/* Always on checking */
#define CHECK(x)  if(x){} else LogMessageFatal(__FILE__, __LINE__).stream() << "Check failed : " #x
#define CHECK_LT(x, y)	CHECK((x) < (y))
#define CHECK_GT(x, y)	CHECK((x) > (y))
#define CHECK_LE(x, y)	CHECK((x) <= (y))
#define CHECK_GE(x, y)	CHECK((x) >= (y))
#define CHECK_EQ(x, y)	CHECK((x) == (y))
#define CHECK_NE(x, y)	CHECK((x) != (y))

#define LOG_INFO LogMessage(__FILE__, __LINE__, 0)
#define LOG_WARNING LogMessage(__FILE__, __LINE__, 1)
#define LOG_ERROR   LogMessage(__FILE__, __LINE__, 2)
#define LOG_FATAL   LogMessage(__FILE__, __LINE__)
#define LOG_QFATAL  LOG_FATAL

#ifdef NDEBUG
  #define DEBUG_MODE 0
  #define LOG_DFATAL LOG_ERROR
#else
  #define DEBUG_MODE 1
  #define LOG_DFATAL LOG_FATAL
#endif

#define LOG(severity) LOG_ ## severity.stream()

class LogMessage
{
public:
  LogMessage(const char* file, int line, int severity)
    : severity_(severity), flushed_(false)
  {
    stream() << file << " : " << line << ": ";
  }
  void Flush()
  {
    stream() << "\n";
    if(severity_ >= min_log_level){
      string s = str_.str();
      size_t n = s.size();
      if(fwrite(s.data(), 1, n, stderr) < n){}
    }
    flushed_ = true;
  }
  ~LogMessage()
  {
    if(!flushed)
      Flush();
  }
  std::ostream& stream() {return str_;}
private:
  const int severity_;
  bool flushed_;
  std::ostringstream str_;
  LogMessage(const LogMessage&) = delete;
  LogMessage& operator=(const LogMessage&) = delete;
};

class LogMessageFatal : public LogMessage
{
public:
  LogMessageFatal(const char *file, int line):
    LogMessage(file, line, 3) {}
  ~LogMessageFatal() {
    Flush();
    abort();
  }
private:
  LogMessageFatal(const LogMessageFatal&) = delete;
  LogMessageFatal& operator=(const LogMessageFatal&) = delete;
};

#endif
