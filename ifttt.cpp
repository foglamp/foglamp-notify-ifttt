/*
 * Fledge "IFTTT" notification delivery plugin.
 *
 * Copyright (c) 2019 Dianomic Systems
 *
 * Released under the Apache 2.0 Licence
 *
 * Author: Mark Riddoch           
 */
#include "ifttt.h"
#include <logger.h>
#include <simple_https.h>
#include <rapidjson/document.h>

using namespace std;
using namespace rapidjson;

/**
 * Construct a IFTTT notification plugin
 *
 * @param category	The configuration of the plugin
 */
IFTTT::IFTTT(ConfigCategory *category)
{
	m_key = category->getValue("key");
	m_trigger = category->getValue("trigger");
}

/**
 * The destructure for the IFTTT plugin
 */
IFTTT::~IFTTT()
{
}

/**
 * Send a notification via the IFTTT webhook
 *
 * @param notificationName 	The name of this notification
 * @param triggerReason		Why the notification is being sent
 * @param message		The message to send
 */
void IFTTT::notify(const string& notificationName, const string& triggerReason, const string& message)
{
SimpleHttps	https("maker.ifttt.com");

	lock_guard<mutex> guard(m_mutex);
	std::vector<std::pair<std::string, std::string>> headers;
	pair<string, string> header = make_pair("Content-type", "application/json");
	headers.push_back(header);

	string url = "https://maker.ifttt.com/trigger/" + m_trigger + "/with/key/" + m_key;

	Logger::getLogger()->debug("POST IFTTT notification to %s", url.c_str());
	try {
		int errorCode;
		if ((errorCode = https.sendRequest("POST", url, headers, "")) != 200 && errorCode != 202)
		{
			Logger::getLogger()->error("Failed to send notification to IFTTT %s, errorCode %d", url.c_str(), errorCode);
		}
	} catch (exception& e) {
		Logger::getLogger()->error("IFTTT notification failed: %s", e.what());
	} catch (...) {
		Logger::getLogger()->error("IFTTT notification failed.");
	}
}

/**
 * Reconfigure the IFTTT delivery plugin
 *
 * @param newConfig	The new configuration
 */
void IFTTT::reconfigure(const string& newConfig)
{
	ConfigCategory category("new", newConfig);
	lock_guard<mutex> guard(m_mutex);
	m_key = category.getValue("key");
	m_trigger = category.getValue("trigger");
}
