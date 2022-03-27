#define BUTTON_PIN 6

class HZM_Button
{
private:
	/* data */
public:
	HZM_Button(/* args */);
	~HZM_Button();
	static void init(bool *p_erase_bonds);
	static bool read();
};
