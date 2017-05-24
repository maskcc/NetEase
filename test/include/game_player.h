/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   game_player.h
 * Author: admin
 *
 * Created on 2017年5月24日, 下午7:53
 */

#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H
#include <stdint.h>
#include <memory>

class GamePlayer{
public:
    void on_reconnect();
public:
    int age;
    int code;
    
private:
    //用户id
    uint64_t user_id_;
    //连接id
    uint64_t id_;
};
using GamePlayerPtr = std::shared_ptr<GamePlayer>;

#endif /* GAME_PLAYER_H */

