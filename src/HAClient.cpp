#include "HAClient.h"

const char *HAClient::LOG_TAG = "HAClient";
bool HAClient::running = false;
bool HAClient::authenticated = false;
long HAClient::seq = 1;
uint8_t HAClient::attempt = 0;

void HAClient::init()
{
    if (BLERC::ha_api_enable && BLERC::ha_ip != "" && BLERC::ha_port > 0 && BLERC::ha_token != "")
    {
        Logger::instance().printf("[%s] initialized...\n", LOG_TAG);
    }
}

void HAClient::run()
{
    if (BLERC::ha_api_enable && BLERC::ha_ip != "" && BLERC::ha_port > 0 && BLERC::ha_token != "")
    {
        onEvent(eventHandler);
        this->begin(BLERC::ha_ip, BLERC::ha_port, url, protocol);
        running = true;
        Logger::instance().printf("[%s] running...\n", LOG_TAG);
    }
}

void HAClient::eventHandler(WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        seq = 1;
        Logger::instance().printf("[%s] Disconnected!\n", LOG_TAG);
        break;
    case WStype_CONNECTED:
        Logger::instance().printf("[%s] Connected to url: ws://%s:%d%s\n", LOG_TAG, BLERC::ha_ip.c_str(), BLERC::ha_port, payload);
        break;
    case WStype_TEXT:
        ESP_LOGD(LOG_TAG, "get text: %s\n", payload);
        instance().handlePayload(payload);
        break;
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
        break;
    case WStype_PING:
        // Logger::instance().println("ping");
        break;
    case WStype_PONG:
        // Logger::instance().println("pong");
        break;
    }
}

void HAClient::handlePayload(uint8_t *payload)
{
    ESP_LOGD(LOG_TAG, "Payload: %s", payload);
    JSONVar jsonBody = JSON.parse((const char *)payload);
    if (JSON.typeof(jsonBody) == "undefined")
    {
        ESP_LOGE(LOG_TAG, "jsonBody missing");
        return;
    }
    if (jsonBody.hasOwnProperty("type"))
    {
        const char *type = (const char *)jsonBody["type"];
        if (strcmp(type, "auth_required") == 0)
        {
            sendToken();
        }
        if (strcmp(type, "auth_ok") == 0)
        {
            authenticated = true;
            Logger::instance().printf("[%s] authenticated\n", LOG_TAG);
            enableHeartbeat(10000, 1000, 30); // every 10 seconds with a timout of 1 second, diconnect after 5 Minutes
            subscribe();
        }
        if (strcmp(type, "auth_invalid") == 0)
        {
            authenticated = false;
            attempt++;
            Logger::instance().printf("[%s] authentication failed", LOG_TAG);
            disconnect();
            if (attempt >= maxAttempts)
            {
                running = false;
                WSEvent::instance().broadcastTXT(
                    "{\"error\":401,\"message\":\"Home Assistant websocket authentication failed. Giving up\"}");
            }
            else
            {
                WSEvent::instance().broadcastTXT(
                    "{\"error\":401,\"message\":\"Home Assistant websocket authentication failed.\"}");
            }
        }
        if (strcmp(type, "result") == 0)
        {
            if (jsonBody.hasOwnProperty("success"))
            {
                Logger::instance().printf("[%s] %s\n", LOG_TAG, (const char *)(bool(jsonBody["success"]) ? "OK" : "Error"));
            }
        }
        if (strcmp(type, "event") == 0)
        {
            if (jsonBody.hasOwnProperty("event") && jsonBody["event"].hasOwnProperty("data"))
            {
                if (jsonBody["event"].hasOwnProperty("data") && jsonBody["event"]["data"].hasOwnProperty("room"))
                {
                    if (strcmp(jsonBody["event"]["data"]["room"], BLERC::room.c_str()) == 0)
                    {
                        JSONVar params = jsonBody["event"]["data"];
                        instance().handleHAEvent(params);
                    }
                }
            }
        }
    }
}

void HAClient::sendToken()
{
    JSONVar auth;
    auth["type"] = String("auth");
    auth["access_token"] = String(BLERC::ha_token);
    sendTXT(JSON.stringify(auth).c_str());
}

void HAClient::subscribe()
{
    JSONVar subscription;
    subscriptionId = seq;
    subscription["id"] = long(seq);
    subscription["type"] = String("subscribe_events");
    subscription["event_type"] = "ha_to_ble_rc";
    instance().sendTXT(JSON.stringify(subscription).c_str());
    incrementSeq();
}

void HAClient::unsubscribe()
{
    JSONVar unsubscription;
    unsubscription["id"] = long(seq);
    unsubscription["type"] = String("unsubscribe_events");
    unsubscription["subscription"] = long(subscriptionId);
    instance().sendTXT(JSON.stringify(unsubscription).c_str());
    incrementSeq();
}

void HAClient::callService(const char *method, const char *keyType, const char *keyCode, int protocol, long irCode, bool longpress)
{
    if (!running || !authenticated)
    {
        return;
    }
    JSONVar serviceCall;
    serviceCall["id"] = long(seq);
    serviceCall["type"] = String("call_service");
    serviceCall["domain"] = String("script");
    serviceCall["service"] = String("ble_rc_to_ha");
    serviceCall["service_data"] = JSONVar();
    serviceCall["service_data"]["method"] = String(method);
    serviceCall["service_data"]["ir_protocol"] = (int)protocol;
    serviceCall["service_data"]["ir_code"] = (long)irCode;
    serviceCall["service_data"]["type"] = String(keyType);
    serviceCall["service_data"]["code"] = String(keyCode);
    serviceCall["service_data"]["room"] = BLERC::room;
    serviceCall["service_data"]["longpress"] = longpress;
    instance().sendTXT(JSON.stringify(serviceCall).c_str());
    incrementSeq();
}

void HAClient::handleHAEvent(JSONVar &params)
{
    if (Bluetooth::BLEconnected && params.hasOwnProperty("method"))
    {
        if (strcmp(params["method"], "keypress") == 0)
        {
            bluetooth.keypress(params);
        }
        if (strcmp(params["method"], "keydown") == 0)
        {
            bluetooth.keydown(params);
        }
        if (strcmp(params["method"], "keyup") == 0)
        {
            bluetooth.keyup(params);
        }
    }
    if (params.hasOwnProperty("method") && strcmp(params["method"], "reboot") == 0)
    {
        ESP.restart();
    }
}

void HAClient::incrementSeq()
{
    if (seq + 1 >= LONG_MAX)
    {
        instance().disconnect();
    }
    else
    {
        seq++;
    }
}