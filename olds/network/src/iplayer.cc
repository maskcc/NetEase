/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "iplayer.h"
using namespace easynet;
//IPlayer
    IPlayer::IPlayer() {        
        identify_ = next_id();
    }
    IPlayer::~IPlayer() {
    }
    void IPlayer::OnNetMessage() {
    }
    bool IPlayer::Send() {
    }
    uint64_t IPlayer::getID() {
        return identify_;
    }
    void IPlayer::KickOut() {
        NetPackage::Close(peer_.fd_);
    }
    void IPlayer::SetHandler(On_Accept_Handler h){
        
    }
    void IPlayer::SetHandler(On_Socket_Handler h){
        
    }
