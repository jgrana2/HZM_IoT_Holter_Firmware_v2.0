#include <stdint.h>

class HZM_Log
{
private:
	/* data */
public:
	HZM_Log(/* args */);
	~HZM_Log();
	static void log_init(void);
	static void print(char *text);
	static void hexdump(uint8_t *p_data, uint32_t len);
};
