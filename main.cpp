#include <string>
#include "include/httplib.h"
#include <iostream>
#include <cassert>
#include "rocksdb/db.h"
#include "enconding.h"

using namespace std;
using namespace httplib;
using namespace rocksdb;

DB* db;

void server(int port_number, string url);

int main () {
    Options options;
    options.create_if_missing = true;
    Status status = rocksdb::DB::Open(options, "./database/proxydb", &db);
    
    if (!status.ok()) {
        cout << "Error creating rocksdb instance";
        return -1;
    }

    int port_number;
    string url;
    cout << "Enter port number: " << "\n";
    cin >> port_number;
    cout << "Enter port url: " << "\n";
    cin >> url;

    server(port_number, url);
    delete db;
    return 0;
}

void server(int port_number, string url) {
    Encoding encoder(url);
    string value = encoder.encode();
    string composite_key = value + to_string(port_number);
    Server svr;

    svr.Get("/", [&](const Request& req, Response& res) {
        Status s = db->Get(ReadOptions(), composite_key, &value);
        if (!s.ok()) {
            res.set_content("X-Cache: MISS", "text/plain");
            db->Put(WriteOptions(), composite_key, value);
        } else {
            res.set_content("X-Cache: HIT", "text/plain");
        }
    });

    svr.set_error_handler([](const Request& req, Response& res) {
        res.set_content("Error" + to_string(res.status) + " occured!", "text/plain");
    });
    cout << "Server started listing on port " << port_number << "\n"; 
    svr.listen("0.0.0.0", port_number);
}