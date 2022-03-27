extern "C"
{
#include "sdk_errors.h"
#include "app_timer.h"
}
#include "HZM_Timer.h"

HZM_Timer::HZM_Timer(/* args */)
{
}

HZM_Timer::~HZM_Timer()
{
}

/**@brief Function for the Timer initialization.
 *
 * @details Initializes the timer module. This creates and starts application timers.
 */
void HZM_Timer::timers_init(void)
{
	// Initialize timer module.
	ret_code_t err_code = app_timer_init();
	APP_ERROR_CHECK(err_code);

	// Create timers.

	/* YOUR_JOB: Create any timers to be used by the application.
		     Below is an example of how to create a timer.
		     For every new timer needed, increase the value of the macro APP_TIMER_MAX_TIMERS by
		     one.
	   ret_code_t err_code;
	   err_code = app_timer_create(&m_app_timer_id, APP_TIMER_MODE_REPEATED, timer_timeout_handler);
	   APP_ERROR_CHECK(err_code); */
}

// Function for starting timers.
void HZM_Timer::application_timers_start(void)
{
	/* YOUR_JOB: Start your timers. below is an example of how to start a timer.
	   ret_code_t err_code;
	   err_code = app_timer_start(m_app_timer_id, TIMER_INTERVAL, NULL);
	   APP_ERROR_CHECK(err_code); */
}