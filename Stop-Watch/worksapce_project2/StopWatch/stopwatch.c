#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char displays[6] = {0};
unsigned char flag = 0;


ISR(INT0_vect){//reset
	int i;
	for(i=0; i<6; i++){
		displays[i]=0;
	}
}

void INT0_Init(void){
	DDRD &= ~(1<<2);
	PORTD |= (1<<2);
	MCUCR |= (1<<1);
	MCUCR &= ~(1<<0);
	GICR |= (1<<6);
}

ISR(INT1_vect){//pause
	TCCR1B &= ~(1<<1) &~(1<<0);
}

void INT1_Init(void){
	DDRD &= ~(1<<3);
	MCUCR |= (1<<3)|(1<<2);
	GICR |= (1<<7);
}

ISR(INT2_vect){//resume
	TCCR1B |= (1<<1) |(1<<0);
}

void INT2_Init(void){
	DDRB &= ~(1<<2);
	PORTB |= (1<<2);
	MCUCSR &= ~(1<<6);
	GICR |= (1<<5);
}


ISR(TIMER1_COMPA_vect){
	flag =1;
}

void Timer1_Init_Compare_Mode(void){
	TCNT1= 0;
	OCR1A=15625;
	TIMSK |= (1<<4);
	TCCR1A=(1<<3);
	TCCR1B = (1<<3)|(1<<1)|(1<<0);
}

void SEG_Switch(int i){
	for(i=0; i<6; i++){
		if(PORTA &(1<<i))
			PORTC = (PORTC &(0xF0))| (displays[i] & (0x0F));
	}
}

void TIME_Display(void){
	int i;
	PORTA |=(1<<0);
	SEG_Switch(0);
	for(i=0; i<5; i++){
		_delay_us(20);
		PORTA &=~(1<<i);
		PORTA |=(1<<(i+1));
		SEG_Switch(i);
	}
	_delay_us(20);
	PORTA &=~(1<<5);
}


void TIME_Count(void){
	if(displays[0]<9)//SEC1
		displays[0]++;
	else{
		displays[0]=0;
		displays[1]++;
	}
	if(displays[1]==6){//SEC2
		displays[1]=0;
		displays[2]++;
	}
	if(displays[2]==9){//MIN1
		displays[2]=0;
		displays[3]++;
	}
	if(displays[3]==6){//MIN2
		displays[3]=0;
		displays[4]++;
	}
	if(displays[4]==9){//HOUR1
		displays[4]=0;
		displays[5]++;
	}
	if(displays[5]==9)//HOUR2
		displays[5]=0;
}


int main(void){
	DDRC |= 0x0F;
	PORTC &= 0xF0;
	DDRA |=0x3F;
	PORTA &=0xC0;
	SREG |= (1<<7);
	Timer1_Init_Compare_Mode();
	INT0_Init();
	INT1_Init();
	INT2_Init();
    while(1){
    	TIME_Display();
    	if(flag ==1 ){
    		TIME_Count();
    		flag =0;
    	}
    }
}
