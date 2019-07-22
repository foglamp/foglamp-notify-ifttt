/*
 * FogLAMP "ifttt" notification plugin.
 *
 * Copyright (c) 2019 Dianomic Systems
 *
 * Released under the Apache 2.0 Licence
 *
 * Author: Mark Riddoch
 */

#include <string>
#include <plugin_api.h>
#include <config_category.h>
#include <logger.h>
#include <ifttt.h>
#include <version.h>

#define PLUGIN_NAME "ifttt"

#define DEFAULT_CONFIG "{\"plugin\" : { \"description\" : \"IFTTT notification plugin\", " \
                       		"\"type\" : \"string\", " \
				"\"default\" : \"" PLUGIN_NAME "\", \"readonly\" : \"true\" }, " \
			 "\"enable\": {\"description\": \"A switch that can be used to enable or disable execution of " \
					 "the IFTTT notification plugin.\", " \
				"\"type\": \"boolean\", " \
				"\"displayName\" : \"Enabled\", " \
				"\"default\": \"false\" }, " \
			"\"trigger\" : {\"description\" : \"IFTTT trigger event\", " \
				"\"type\" : \"string\", " \
				"\"default\" : \"button_press\", " \
				"\"order\" : \"1\", \"displayName\" : \"IFTTT Trigger\"}," \
			"\"key\" : {\"description\" : \"IFTTT maker key\", " \
				"\"type\" : \"string\", " \
				"\"default\" : \"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\", " \
				"\"order\" : \"2\", \"displayName\" : \"IFTTT Key\"} }" \

using namespace std;
using namespace rapidjson;

/**
 * The Notification plugin interface
 */
extern "C" {

/**
 * The plugin information structure
 */
static PLUGIN_INFORMATION info = {
        PLUGIN_NAME,              // Name
        VERSION,                  // Version
        0,                        // Flags
        PLUGIN_TYPE_NOTIFICATION_DELIVERY,       // Type
        "1.0.0",                  // Interface version
        DEFAULT_CONFIG	          // Default plugin configuration
};

/**
 * Return the information about this plugin
 */
PLUGIN_INFORMATION *plugin_info()
{
	return &info;
}


/**
 * Initialise the plugin, called to get the plugin handle and setup the
 * plugin configuration
 *
 * @param config	The configuration category for the plugin
 * @return		An opaque handle that is used in all subsequent calls to the plugin
 */
PLUGIN_HANDLE plugin_init(ConfigCategory* config)
{
	IFTTT *ifttt = new IFTTT(config);
	
	return (PLUGIN_HANDLE)ifttt;
}

/**
 * Deliver received notification data
 *
 * @param handle		The plugin handle returned from plugin_init
 * @param deliveryName		The delivery category name
 * @param notificationName	The notification name
 * @param triggerReason		The trigger reason for notification
 * @param message		The message from notification
 */
bool plugin_deliver(PLUGIN_HANDLE handle,
                    const std::string& deliveryName,
                    const std::string& notificationName,
                    const std::string& triggerReason,
                    const std::string& message)
{
	Logger::getLogger()->info("IFTTT notification plugin_deliver(): deliveryName=%s, notificationName=%s, triggerReason=%s, message=%s",
							deliveryName.c_str(), notificationName.c_str(), triggerReason.c_str(), message.c_str());
	IFTTT *ifttt = (IFTTT *)handle;
	ifttt->notify(notificationName, triggerReason, message);
	return true;
}

/**
 * Reconfigure the plugin
 */
void plugin_reconfigure(PLUGIN_HANDLE *handle, string& newConfig)
{
	Logger::getLogger()->info("IFTTT notification plugin: plugin_reconfigure()");
	IFTTT *ifttt = (IFTTT *)handle;
	
	ifttt->reconfigure(newConfig);
	return;
}

/**
 * Call the shutdown method in the plugin
 */
void plugin_shutdown(PLUGIN_HANDLE *handle)
{
	IFTTT *ifttt = (IFTTT *)handle;
	delete ifttt;
}

// End of extern "C"
};

