//
// Created by mwo on 13/04/16.
//


#ifndef CROWXMR_RPCCALLS_H
#define CROWXMR_RPCCALLS_H

#include "arqma_headers.h"

#include <mutex>
#include <chrono>

namespace xmreg
{

using namespace cryptonote;
using namespace crypto;
using namespace std;

using namespace std::chrono_literals;

class RPCCalls
{
    string daemon_url;

    uint64_t timeout_time;

    chrono::seconds rpc_timeout;

    epee::net_utils::http::url_content url;

    epee::net_utils::http::http_simple_client m_http_client;

    std::mutex m_daemon_rpc_mutex;

    string port;

public:

    RPCCalls(string _daemon_url = "http://127.0.0.1:19994",
             chrono::seconds _timeout = 30min + 30s);

    virtual bool
    connect_to_arqma_daemon();

    virtual bool
    commit_tx(const string& tx_blob,
              string& error_msg,
              bool do_not_relay = false);

    virtual bool
    commit_tx(tools::wallet2::pending_tx& ptx,
              string& error_msg);

    virtual bool
    get_current_height(uint64_t& current_height);

    virtual ~RPCCalls() = default;
};


}

#endif //CROWXMR_RPCCALLS_H
