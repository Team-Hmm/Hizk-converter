#include "httplib.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

typedef nlohmann::basic_json<map, vector, basic_string<char>, bool, int64_t, uint64_t, double, allocator, nlohmann::adl_serializer, vector<uint8_t>, void> config_t;

#define URL "generativelanguage.googleapis.com"
//#define URL "httpbin.org"

void CreateCorpora(){
    httplib::Client cli(URL,80);
    httplib::Headers headers= {
        {"Content-Type", "application/json"},
        {"Authorization","Bearer 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"}
    };
    auto res = cli.Post("/v1beta/corpora?key=AIzaSyA-DNHGjHkpmyOy1N8Gr6IEoCurnyV02p0",headers);
    if (res && res->status == 200) {
        std::cout << "status:" + res->status << std::endl;
        std::cout << res->body << std::endl;
    }else{
        std::cout << res.error() << std::endl;
        if(res)std::cout << "status:"<<res->status << std::endl;
    }
}

config_t configRead(std::string path){
    ifstream ifs(path);
    std::string jsonstr;
    if(ifs.fail()){
        cout << "File Open Error" << endl;
        exit(-1);
    }
    while(getline(ifs, jsonstr)){
        cout<<"[Read result] "<<endl;
    }
    return nlohmann::json::parse(jsonstr);
}

int main(void)
{
    config_t config = configRead("../config.json");

    CreateCorpora();
    httplib::Client cli(URL,80);
    
    char path[256] = "";
    sprintf(path,"/v1beta/models/gemini-pro:generateContent?key=%s",config["API_KEY"]);
    httplib::Headers headers= {
        {"Content-Type", "application/json"},
        {"Authorization","Bearer 921347703943-9pg1ichsroraukhjbodflagfr1bjbq1p.apps.googleusercontent.com"}
    };
    string body = R"({
        contents": [
            {
                "parts":[
                    {"text": "Write a story about a magic backpack."}
                ]
            }
        ]
    })";
    auto res = cli.Post(path,headers,body,"application/json");
    if (res && res->status == 200) {
        std::cout << "status:" + res->status << std::endl;
        std::cout << res->body << std::endl;
    }else{
        std::cout << res.error() << std::endl;
        if(res)std::cout << "status:"<<res->status << std::endl;
    }
}