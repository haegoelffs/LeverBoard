//log
logUnsignedInt("actual_current",actual_current,30);
logVar("duty_cycle",duty_cycle,10);

//main
signed char new_current = give_newcurrent();
rise_sink_pwm_dutyc(new_current,actual_current,duty_cycle);

if(temp > 200) temp = 1;

 if (temp == 0)
 {
 _delay_ms(10000);
 switchPwmOnOff(1);
 ++temp;
 }


 while(1)
 {
 changePhaseState(0);
 _delay_us(10000);
 changePhaseState(1);
 _delay_us(10000);
 changePhaseState(2);
 _delay_us(10000);
 changePhaseState(3);
 _delay_us(10000);
 changePhaseState(4);
 _delay_us(10000);
 changePhaseState(5);
 _delay_us(10000);
 }


//Interface

signed char give_newcurrent(void);		//Returns new wisched current of the phases, considering the angle of the upper Board (Not in use at the moment)

 signed char give_newcurrent(void)			//gets hallvoltage returns wished power (MAX POWER 42.5 A)
	{
 char front = readInterfaceSensorsVoltage(0);	//Hallsensor front
 char tail = readInterfaceSensorsVoltage(1);	   //Hallsensor tail
 char delta_sensors;
 if(front > tail)
 {
 delta_sensors = front - tail;
 }
 else delta_sensors = tail - front;
 if((delta_sensors < 51))
 {
 return (signed char)0;
 }
 else if (delta_sensors > 255) return (signed char 42);
 else return (signed char)((42 * delta_sensors)/255);
 };

//Drive

void rise_sink_pwm_dutyc(char new_current,char actual_current, char duty_cycle); //Not used at the moment
/*
 void rise_sink_pwm_dutyc(char new_current,char actual_current, char duty_cycle)
 {
	
	if((actual_current < new_current) && (duty_cycle < 100))
	{
 ++duty_cycle;
 setPWMDutyCycle(duty_cycle);
	}
	else if ((actual_current > new_current) && (duty_cycle > 0))
	{
 --duty_cycle;
 setPWMDutyCycle(duty_cycle);
	}
	else duty_cycle= 0;
	setPWMDutyCycle(duty_cycle);
 }*/



