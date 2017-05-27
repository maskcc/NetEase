/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ddz_server.h
 * Author: admin
 *
 * Created on 2017年5月22日, 下午4:59
 */

#ifndef DDZ_SERVER_H
#define DDZ_SERVER_H


#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include "log.h"
#include "game_frame.h"
#include "login_req.h"

class DDZServer : public GameFrame{
public:
    bool timer() override;
    void reg_event() override;
    void start();
    
private:
    
};



#endif /* DDZ_SERVER_H */

