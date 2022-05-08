#ifndef KANON_HTTP_SERVER2_H
#define KANON_HTTP_SERVER2_H

#include <sys/types.h>
#include <unordered_map>

#include <kanon/net/user_server.h>
#include <kanon/thread/rw_lock.h>
#include <kanon/util/optional.h>

namespace http {

class HttpSession;

class HttpServer : public kanon::TcpServer {
  friend class HttpSession;
public:
  HttpServer(EventLoop* loop, InetAddr const& addr);

  ~HttpServer() noexcept;
private:
  void EmplaceOffset(uint64_t session, off_t off);
  void EraseOffset(uint64_t session);
  kanon::optional<off_t> SearchOffset(uint64_t session);

  // Cache factory method
  // @see Modern Effective C++ Item 21
  std::shared_ptr<int> GetFd(std::string const& path);
  std::shared_ptr<char*> GetAddr(std::string const& path, size_t len);

  kanon::MutexLock mutex_;
  std::unordered_map<std::string, std::weak_ptr<int>> fd_map_;

  kanon::MutexLock mutex_addr_;
  std::unordered_map<std::string, std::weak_ptr<char*>> addr_map_;

  kanon::RWLock lock_offset_;
  std::unordered_map<uint64_t, off_t> offset_map_;
};

} // namespace http

#endif // KANON_HTTP_SERVER2_H
