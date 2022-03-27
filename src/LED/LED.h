#define LED_PIN 7

class LED
{
private:
	/* data */
public:
	LED(/* args */);
	~LED();
	static void init();
	static void turn_on();
	static void turn_off();
};
