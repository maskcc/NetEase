/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "event_data.h"
namespace easynet{
EventData::EventData(){
    link_type_ = LINK_TYPE::LT_NONE;
    status_ = LINK_STATUS::LS_NONE;
    fd_ = NetPackage::kINVALID_FD;
    name_ = "";
    acceptor_ = nullptr;
    tcpsocket_ = nullptr;
    
}

EventData::~EventData(){
    
    
}

}