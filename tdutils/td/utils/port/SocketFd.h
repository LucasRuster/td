//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2018
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "td/utils/port/config.h"

#include "td/utils/port/detail/NativeFd.h"
#include "td/utils/port/detail/PollableFd.h"
#include "td/utils/port/IPAddress.h"
#include "td/utils/Slice.h"
#include "td/utils/Status.h"

namespace td {

namespace detail {
class SocketFdImpl;
class SocketFdImplDeleter {
 public:
  void operator()(SocketFdImpl *impl);
};
}  // namespace detail

class SocketFd {
 public:
  SocketFd();
  SocketFd(const SocketFd &) = delete;
  SocketFd &operator=(const SocketFd &) = delete;
  SocketFd(SocketFd &&);
  SocketFd &operator=(SocketFd &&);
  ~SocketFd();

  static Result<SocketFd> open(const IPAddress &address) TD_WARN_UNUSED_RESULT;

  PollableFdInfo &get_poll_info();
  const PollableFdInfo &get_poll_info() const;

  Status get_pending_error() TD_WARN_UNUSED_RESULT;

  Result<size_t> write(Slice slice) TD_WARN_UNUSED_RESULT;
  Result<size_t> read(MutableSlice slice) TD_WARN_UNUSED_RESULT;

  const NativeFd &get_native_fd() const;
  static Result<SocketFd> from_native_fd(NativeFd fd);

  void close();
  bool empty() const;

 private:
  std::unique_ptr<detail::SocketFdImpl, detail::SocketFdImplDeleter> impl_;
  explicit SocketFd(std::unique_ptr<detail::SocketFdImpl> impl);
};

#if TD_PORT_POSIX
namespace detail {
Status get_socket_pending_error(const NativeFd &fd);
}  // namespace detail
#endif

}  // namespace td
