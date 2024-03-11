これはconvert_gpt関数単独の開発プログラムです。

main.cppと同じ階層にconfig.hをおいてください。 
```h
#ifndef SUPERIME_API_CONFIG_H
#define SUPERIME_API_CONFIG_H

#include <string>

const std::string API_KEY = "YOUR_API_KEY";

#endif //SUPERIME_API_CONFIG_H
```

コンパイルはこうやってください
```
g++ src/main.cpp -lssl -lcrypto -pthread -D CPPHTTPLIB_OPENSSL_SUPPORT
```