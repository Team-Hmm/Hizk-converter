#include <iostream>
#include <string>
#include "json.hpp"
#include "httplib.h"
#include "config.h"

int main() {
    std::string pronpt;
    std::cout << "Enter your prompt: ";
    std::cin >> pronpt;

    httplib::SSLClient cli("api.openai.com", 443);
    // ヘッダーを作成
    httplib::Headers headers = {
        {"Content-Type", "application/json"},
        {"Authorization", "Bearer " + API_KEY}
    };


    nlohmann::json body;
    body["model"] = "gpt-3.5-turbo";
    body["messages"] = {{
        {"role", "user"},
        {"content", pronpt}
    }};
    body["temperature"] = 0.7;

    auto res = cli.Post("/v1/chat/completions", headers, body.dump(), "application/json");

    if (res && res->status == 200) {
        nlohmann::json resJson = nlohmann::json::parse(res->body);
        std::cout << "response: " << resJson["choices"][0]["message"]["content"] << std::endl;
    }
    else {
        std::cout << "Request failed!" << std::endl;
        nlohmann::json resJson = nlohmann::json::parse(res->body);
        if (res) {
            std::cout << "Status code: " << res->status << std::endl;
            std::cout << "Body: " << res->body << std::endl;
        }
    }

    return 0;
}
