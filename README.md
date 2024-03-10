これはAPIのテストです

main.cppと同じ階層にconfig.hをおいてください。 
```json
{"API_KEY": "YOUR_API_KEY"}
```

コンパイルはこうやってください
```
g++ src/main.cpp -lssl -lcrypto -pthread -D CPPHTTPLIB_OPENSSL_SUPPORT
```