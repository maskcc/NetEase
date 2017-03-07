/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "socket.h"

using namespace easynet;
//Socket
    Socket::Socket() {
        addr_ = "";
        fd_ = NetPackage::kINVALID_FD;
        port_ = -1;
    }
