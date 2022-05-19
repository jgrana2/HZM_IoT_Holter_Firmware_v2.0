#define LED_PIN 7

class HZM_LED
{
private:
	
public:
	HZM_LED();
	~HZM_LED();
	static void init();
	static void turn_on();
	static void turn_off();
};
