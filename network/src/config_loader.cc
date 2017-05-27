/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "config_loader.h"
namespace ConfigLoader {
    //服务器的信息, 包括 ip, 端口, 名称, id
    using namespace rapidjson;
    using namespace std;
    
    void parse_server_config(const Value& doc, ServerConfig& conf){
        assert(doc.HasMember("id"));
        assert(doc["id"].IsInt());
        conf.id_ = doc["id"].GetInt();

        assert(doc.HasMember("name"));
        assert(doc["name"].IsString());
        conf.name_ = doc["name"].GetString();

        assert(doc.HasMember("ip"));
        assert(doc["ip"].IsString());
        conf.ip_ = doc["ip"].GetString();

        assert(doc.HasMember("port"));
        assert(doc["port"].IsInt());
        conf.port_ = doc["port"].GetInt();
        
        LOG(INFO) << "id[" << conf.id_ << "] name[" << conf.name_  << "] ip[" << conf.ip_  << "] port[" << conf.port_  << "]";

    }
    void parse_net_init(const Value& doc, ServerInfo* info){
        parse_server_config(doc, info->config_);

        assert(doc.HasMember("connect"));
        assert(doc["connect"].IsArray());
        info->connected_.clear();
        const Value& value = doc["connect"];
        LOG(INFO) << "connected servers:";
        for(SizeType c = 0; c < value.Size(); ++c){
            ServerConfig con;
            parse_server_config(value[c], con);
            info->connected_.push_back(con);
        }
    }
    void parse_rec_init(const Value& doc, int32_t &open, int32_t &time, int32_t &max){
        assert(doc.HasMember("open_reconnect"));
        assert(doc["open_reconnect"].IsInt());
        open = doc["open_reconnect"].GetInt();
        
        assert(doc.HasMember("time_out"));
        assert(doc["time_out"].IsInt());
        time = doc["time_out"].GetInt();
        
        assert(doc.HasMember("max_count"));
        assert(doc["max_count"].IsInt());
        max = doc["max_count"].GetInt();
    }
    
    ServerInfo* load_server_config() {
        {
            const char* json = "{\"project\":\"rapidjson\", \"stars\":10}";
            Document d;
            d.Parse(json);
            Value& s = d["stars"];
            s.SetInt(s.GetInt() + 1);
            LOG(INFO) << "stars:" << s.GetInt();
        }
        ServerInfo *info = new ServerInfo;
        FILE* fp = fopen("server_config.json", "r");
        char readBuffer[1024 * 1];
        FileReadStream is(fp, readBuffer, sizeof(readBuffer));
        Document dconf;
        dconf.ParseStream(is);
        
        assert(dconf.HasMember("net_init"));
        const auto& net = dconf["net_init"];
        parse_net_init(net, info);
        
        assert(dconf.HasMember("reconnect_init"));
        const auto& rec = dconf["reconnect_init"];
        
        int32_t open_re = 0;
        int32_t time_out = 0;
        int32_t max_count = 0;
        parse_rec_init(rec, open_re, time_out, max_count);  
        LOG(INFO) << "open_reconnect[" << open_re << "] time_out[" << time_out <<  "] max_count[" << max_count << "]";
        info->open_reconnect_ = open_re;
        info->time_out_ = time_out;
        info->max_count_ = max_count;
        fclose(fp);
        return info;
    }
    

}
