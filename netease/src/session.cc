/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "session.h"

Session::Session(){
}
Session::~Session(){
}

void Session::set(IPlayerPtr pl){
    player_ = pl;
    
}