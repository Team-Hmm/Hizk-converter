#include <iostream>
#include <string>
#include "json.hpp"
#include "httplib.h"
#include "config.h"

std::string convert_gpt(std::string &&str){
    std::string pronpt = "あなたは辞書ですが、通常の辞書と異なり意味から見出し語を探す辞書です。意味から元の見出し語を推測して、元の見出し語のみを出力してください。「"  + str + "」の見出し語はなんですか?";

    httplib::SSLClient cli("api.openai.com", 443);
    // ヘッダーを作成
    httplib::Headers headers = {
        {"Content-Type", "application/json"},
        {"Authorization", "Bearer " + API_KEY}
    };


    nlohmann::json body;
    body["model"] = "gpt-4-0125-preview";
    body["messages"] = {{
        {"role", "user"},
        {"content", pronpt}
    }};
    body["temperature"] = 0.7;

    auto res = cli.Post("/v1/chat/completions", headers, body.dump(), "application/json");

    if (res && res->status == 200) {
        nlohmann::json resJson = nlohmann::json::parse(res->body);
        std::string result = resJson["choices"][0]["message"]["content"];
        return result;
    }
    else {
        return "エラーが発生しました";
    }
}

int main() {
    std::string str;
    std::cout << "変換前: ";
    std::cin >> str;
    std::string &&pronpt = std::forward<std::string&&>(str);
    std::string &&result = convert_gpt(std::forward<std::string>(pronpt));

    std::cout << "変換後: " << result << std::endl;
}
