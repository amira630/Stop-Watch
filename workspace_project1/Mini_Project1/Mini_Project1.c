/**************************************************************************************************
 Name        : Mini_Project1.c
 Author      : Amira Atef Ismaeil El Komy
 Description : Vehicle Control System Project
 Date        : 15/08/2023
 **************************************************************************************************/

#include <stdio.h>
#include<string.h>

#define WITH_ENGINE_TEMP_CONTROLLER 1

/*Creating bool data type*/
typedef enum{
	FALSE=0, TRUE=1
}bool;

/*engine structure*/
struct eng{
	char Engine_State[4], AC[4], Engine_Temperature_Controller[4];
	int vehicle_speed, room_temp, engine_temp;
};


/*Functions Prototypes*/
void HomePage(void);//Main Menu options; only prints
void Sensors_set_menu(void);// Menu options for when engine is: ON; only prints
void current_state(struct eng * ENG);// latest values for engine takes a pointer to struct eng and doesn't return anything
void traffic_light_data(struct eng * ENG);//changes speed depending on traffic lights color
void room_temp_data(struct eng * ENG);//changes room temp
void engine_temp_data(struct eng * ENG);//changes engine temp
void isSpeed30(struct eng * ENG);// adjusts values accordingly depending on whether speed==30 or not

/*main function*/
int main(void){
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	char op1, op2;
	struct eng ENGINE={"ON", "OFF", "OFF", 50, 35, 90};
	bool END=FALSE;
	HomePage();
	scanf("%c", &op1);
	while(!END){
		switch(op1){
		case 'a':
		case 'A':
			Sensors_set_menu();
			scanf(" %c", &op2);
			while(op2 != 'a' && op2 != 'A'){
				switch(op2){
					case'b':
					case'B':
						traffic_light_data(&ENGINE);
						break;
					case'c':
					case'C':
						room_temp_data(&ENGINE);
						break;
					case'd':
					case'D':
						#if WITH_ENGINE_TEMP_CONTROLLER
							engine_temp_data(&ENGINE);
						#endif
						break;
				}
				scanf(" %c", &op2);
			}
			HomePage();
			scanf(" %c", &op1);
			break;
		case 'b':
		case 'B':
			printf("\nTurn off the vehicle engine\n\n");
			HomePage();
			scanf(" %c", &op1);
			break;
		case 'c':
		case 'C':
			END =TRUE;
			break;
		default:
			scanf(" %c", &op1);
			break;
		}
	}
	printf("Quit the system\n");
	return 0;
}
/*Functions Definitions*/
void HomePage(void){
	printf("a. Turn on the vehicle engine\n");
	printf("b. Turn off the vehicle engine\n");
	printf("c. Quit the system\n\n");
}
void Sensors_set_menu(void){
	printf("a. Turn off the engine\n");
	printf("b. Set the traffic light color.\n");
	printf("c. Set the room temperature (Temperature Sensor)\n");
	#if WITH_ENGINE_TEMP_CONTROLLER
		printf("d. Set the engine temperature (Engine Temperature Sensor)\n\n");
	#endif
}
void traffic_light_data(struct eng * ENG){
	char color;
	printf("Enter the required color:\n");
	do{
		scanf("%c",&color);
	}while(!(color=='g' || color=='G' || color=='o' || color=='O'|| color=='r' || color=='R'));
	switch(color){
		case 'g':
		case 'G': ENG->vehicle_speed= 100;
				  break;
		case 'o':
		case 'O': ENG->vehicle_speed= 30;
				  break;
		case 'r':
		case 'R': ENG->vehicle_speed= 0;
				  break;
	}
	isSpeed30(ENG);
	current_state(ENG);
}
void room_temp_data(struct eng * ENG){
	int temp;
	printf("Enter the required room temperature:\n");
	scanf("%d",&temp);
	if(temp <10 || temp>30){
		strcpy(ENG->AC,"ON");
		ENG->room_temp= 20;
	}
	else
		strcpy(ENG->AC,"OFF");
	isSpeed30(ENG);
	current_state(ENG);
}
void engine_temp_data(struct eng * ENG){
	int temp;
	printf("Enter the required engine temperature:\n");
	scanf("%d",&temp);
	if(temp <100 || temp>150){
		strcpy(ENG->Engine_Temperature_Controller,"ON");
		ENG->engine_temp= 125;
	}
	else
		strcpy(ENG->Engine_Temperature_Controller,"OFF");
	isSpeed30(ENG);
	current_state(ENG);
}
void current_state(struct eng * ENG){
	printf("Engine is: %s\n",ENG->Engine_State);
	printf("AC is: %s\n", ENG->AC);
	printf("Vehicle Speed: %d Km/hr\n",ENG->vehicle_speed);
	printf("Room Temperature: %d C\n", ENG->room_temp);
	#if WITH_ENGINE_TEMP_CONTROLLER
		printf("Engine Temp Controller is: %s\n",ENG->Engine_Temperature_Controller);
		printf("Engine Temperature: %d C\n\n", ENG->engine_temp);
	#endif
	Sensors_set_menu();
}
void isSpeed30(struct eng * ENG){
	if(ENG->vehicle_speed==30){
		strcpy(ENG->AC,"ON");
		ENG->room_temp =ENG->room_temp*(5/4)+1;
		#if WITH_ENGINE_TEMP_CONTROLLER
			strcpy(ENG->Engine_Temperature_Controller,"ON");
			ENG->engine_temp =ENG->engine_temp*(5/4)+1;
		#endif
	}
}
