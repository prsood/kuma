/* Copyright (c) 2014-2017, Fengping Bao <jamol@live.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef __IocpUdpSocket_H__
#define __IocpUdpSocket_H__

#include "kmdefs.h"
#include "kmapi.h"
#include "evdefs.h"
#include "UdpSocketBase.h"
#include "util/kmbuffer.h"

KUMA_NS_BEGIN

class IocpUdpSocket : public UdpSocketBase
{
public:
    IocpUdpSocket(const EventLoopPtr &loop);
    ~IocpUdpSocket();
    
    KMError bind(const char* bind_host, uint16_t bind_port, uint32_t udp_flags) override;
    int receive(void* data, size_t length, char* ip, size_t ip_len, uint16_t& port) override;
    KMError close() override;

    bool isPending() const override { return hasPendingOperation(); }
    
protected:
    void ioReady(KMEvent events, void* ol, size_t io_size) override;
    void onReceive(size_t io_size);

    int postRecvOperation();
    bool hasPendingOperation() const;

    void cleanup() override;
    void cancel();

protected:
    bool            recv_pending_ = false;
    KMBuffer        recv_buf_;
    WSABUF          wsa_buf_r_;
    OVERLAPPED      recv_ol_;
    sockaddr_storage recv_addr_;
    int             recv_addr_len_ = 0;
    bool            closing_ = false;
};

KUMA_NS_END

#endif
