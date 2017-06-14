/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "tcp_socket.h"
#include "socket_buffer.h"

using namespace easynet;

TcpSocket::TcpSocket(EventLoopPtr loop) : event_loop_(loop) {


}

TcpSocket::~TcpSocket() {

}

bool TcpSocket::do_connect(std::string ip, int32_t port) {
    if (false == NetPackage::Connect(event_data_.fd_, ip, port)) {
        LOG(ERROR) << "connect ip:" << ip << " port:" << port << " fail! error:" << strerror(errno);
        return false;
    }
    this->attach(next_id(), event_data_.fd_, ip, port);
    return true;


}

void TcpSocket::on_message(int32_t events) {
    if (on_receive_ == nullptr) {
        LOG(FATAL) << "receive handler is nullptr!";
    }
    if (event_data_.status_ != LS_ESTABLISHED) {
        LOG(ERROR) << "status not right! status:" << event_data_.status_;
        return;
    }
    auto f = on_receive_;
    NetErrorCode ec;
    for (;;) {
        int32_t ret = NetPackage::Read(event_data_.fd_, event_loop_->get_buffer(), MAX_SOCK_BUFF);
        if (0 == ret) {
            //客户端关闭
            close_conn();
            //客户端回调
            ec.err_ = NetErrorCode::NE_CLOSED;
            f(identify_, nullptr, ret, ec);
            return;
        }
        if (ret < 0) {
            if (EAGAIN == errno || EWOULDBLOCK == errno) {
                ec.err_ = NetErrorCode::NE_AGAIN;
                f(identify_, nullptr, ret, ec);
                return;
            } else {
                LOG(INFO) << "read fail! fd[" << event_data_.fd_ << "] errno[" << errno << "] msg[" << strerror(errno) << "]";
                close_conn();
                ec.err_ = NetErrorCode::NE_ERROR;
                f(identify_, nullptr, ret, ec);
                return;
            }
        }
        NET_WORK_STATUS.add_receive_packet( ret );
        ec.err_ = NetErrorCode::NE_DATA;
        f(identify_, event_loop_->get_buffer(), ret, ec);
        return;
    }

}

// 同步发送消息
bool TcpSocket::send_msg(void* msg, int32_t sz) {
    unsigned char* pPos = (unsigned char*)msg;
    int32_t sendcnt = 0;
    int32_t leftcnt = sz;
    while (leftcnt > 0) {
        sendcnt = NetPackage::Write(event_data_.fd_, pPos, leftcnt);
        if (sendcnt < 0) {
            if (EAGAIN == errno || EWOULDBLOCK == errno) {
                continue;
            } else {
                return false;
            }
        }
        leftcnt -= sendcnt;
        pPos += sendcnt;
    }
    NET_WORK_STATUS.add_send_packet(sz);
    return true;


}

bool TcpSocket::do_receive(OnReceiveHandler h) {
    if (false == event_loop_->ev_control_.add_in_event(event_loop_->get_efd(), event_data_)) {
        LOG(ERROR) << "add receive event fail!";
        close_conn();
        return false;
    }
    on_receive_ = std::move(h);

    return true;


}

void TcpSocket::attach(uint64_t id, int32_t fd, std::string ip, int32_t port) {
    ip_ = std::move(ip);
    port_ = port;
    identify_ = id;
    event_data_.fd_ = fd;
    event_data_.tcpsocket_ = shared_from_this();
    event_data_.link_type_ = LT_CLIENT;
    event_data_.status_ = LS_ESTABLISHED;
    event_data_.epoll_event_.data.ptr = &event_data_;
}

void TcpSocket::close_conn() {
    if ( false == event_loop_->ev_control_.remove_in_event(event_loop_->get_efd(), event_data_)){
        LOG(ERROR) << "remove event fail!";        
    }
    NetPackage::Close(event_data_.fd_);
    event_data_.status_ = LS_CLOSED;
    NET_WORK_STATUS.del_online_count();
}
