class HZM_Power
{
private:
	/* data */
public:
	HZM_Power();
	~HZM_Power();
	static void power_management_init(void);
	static void idle_state_handle(void);
};
