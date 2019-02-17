//
// Created by mwo on 13/04/16.
//

#include "easylogging++.h"
#include "om_log.h"

#include "RPCCalls.h"

namespace xmreg
{

RPCCalls::RPCCalls(string _daemon_url, chrono::seconds _timeout)
    : daemon_url {_daemon_url}, rpc_timeout {_timeout}
{
    epee::net_utils::parse_url(daemon_url, url);

    port = std::to_string(url.port);

    m_http_client.set_server(
            daemon_url,
            boost::optional<epee::net_utils::http::login>{});
}

bool
RPCCalls::connect_to_arqma_daemon()
{
    if(m_http_client.is_connected())
    {
        return true;
    }

    return m_http_client.connect(rpc_timeout);
}

bool
RPCCalls::commit_tx(
        const string& tx_blob,
        string& error_msg,
        bool do_not_relay)
{
    COMMAND_RPC_SEND_RAW_TX::request  req;
    COMMAND_RPC_SEND_RAW_TX::response res;

    req.tx_as_hex = tx_blob;
    req.do_not_relay = do_not_relay;

    bool r {false};

    {
        std::lock_guard<std::mutex> guard(m_daemon_rpc_mutex);

        r = epee::net_utils::invoke_http_json(
            "/sendrawtransaction", req, res,
            m_http_client, rpc_timeout);
    }

    if (!r)
    {
        error_msg = res.reason;

        if (error_msg.empty())
        {
            error_msg = "Reason not given by daemon. A guess is 'Failed to check ringct signatures!'.";
        }

        cerr << "Error sending tx: " << error_msg << endl;

        return false;
    }

    if (res.status == CORE_RPC_STATUS_BUSY)
    {
        error_msg = "Deamon is BUSY. Cant sent now " + res.reason;

        cerr << "Error sending tx: " << error_msg << endl;

        return false;
    }

    if (res.status != CORE_RPC_STATUS_OK)
    {
        error_msg = "Tx rejected: " + res.reason;

        cerr << "Error sending tx: " << error_msg << endl;

        return false;
    }

    if (do_not_relay)
    {
        cout << "Tx accepted by deamon but not relayed (useful for testing of constructing txs)" << endl;
    }

    return true;
}

bool
RPCCalls::commit_tx(
        tools::wallet2::pending_tx& ptx,
        string& error_msg)
{

    string tx_blob = epee::string_tools::buff_to_hex_nodelimer(
            tx_to_blob(ptx.tx)
    );

    return commit_tx(tx_blob, error_msg);
}

bool
RPCCalls::get_current_height(uint64_t& current_height)
{
  COMMAND_RPC_GET_HEIGHT::request  req;
  COMMAND_RPC_GET_HEIGHT::response res;

  bool r {false};

  {
    std::lock_guard<std::mutex> guard(m_daemon_rpc_mutex);

    r = epee::net_utils::invoke_http_json(
      "/getheight",
      req, res, m_http_client, rpc_timeout);
  }

  if (!r)
  {
    cerr << "Error connecting to Arqma Daemon at: "
         << daemon_url << endl;
    return false;
  }

  current_height = res.height;

  return true;
}

}
