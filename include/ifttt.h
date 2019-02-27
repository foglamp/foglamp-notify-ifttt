#ifndef _IFTTT_H
#define _IFTTT_H
#include <config_category.h>
#include <string>
#include <logger.h>
#include <mutex>

/**
 * A simple IFTTT notification class that sends a message
 * to the IFTTT button_pressed aptlet to simulate the pressign
 * of a button. IFTTT will then perform whatever action is
 * associated with that button press.
 */
class IFTTT {
	public:
		IFTTT(ConfigCategory *config);
		~IFTTT();
		void	notify(const std::string& notificationName, const std::string& triggerReason, const std::string& message);
		void	reconfigure(const std::string& newConfig);
	private:
		std::string	m_key;
		std::string	m_trigger;
		std::mutex	m_mutex;
};
#endif
