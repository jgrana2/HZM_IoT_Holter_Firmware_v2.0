#define BUTTON_PIN 6

class HZM_Button
{
private:
	
public:
	HZM_Button();
	~HZM_Button();
	static void init(bool *p_erase_bonds);
	static bool read();
};
