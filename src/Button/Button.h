#define BUTTON_PIN 6

class Button
{
private:
	/* data */
public:
	Button(/* args */);
	~Button();
	static void init(bool *p_erase_bonds);
	static bool read();
};
