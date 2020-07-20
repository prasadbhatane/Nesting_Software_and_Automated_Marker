#include "sra.c"
#include <math.h>
#define PI 3.14159265






int steps;
int x;
int pdel = 20;

void req_init(void)
{
	DDRC= 0b00001111;
	lcd_init(underline);
	lcd_clear();

}


void forward(int steps)
{
  
  
	lcd_write_string("forward");
	delay_millisec(1000);
	lcd_clear();
	for(x=0;x<steps;x++)
	{
		PORTC= 0b0001110;
		delay_millisec(pdel);
			
		PORTC= 0b0000100;
		delay_millisec(pdel);
	}

}


void spot_left(int spotsteps)
{
  
	lcd_write_string("spot left");
	delay_millisec(1000);
	lcd_clear();
	for(x=0;x<spotsteps;x++)
	{
		PORTC= 0b0001010;
		delay_millisec(pdel);
			
		PORTC= 0b0000000;
		delay_millisec(pdel);
	}


}

void spot_right(int spotsteps)
{
  
	lcd_write_string("spot right");
	delay_millisec(1000);
	lcd_clear();
	for(x=0;x<spotsteps;x++)
	{
		PORTC= 0b0001111;
		delay_millisec(pdel);
			
		PORTC= 0b0000101;
		delay_millisec(pdel);
	}
}





void goto_coordinate(int x1,int y1,int x2,int y2)
{
    lcd_write_string("goto crdnt");
	delay_millisec(2000);
	lcd_clear();
	
	
	int defy = y2-y1;
	int defx = x2-x1;
	
	
	lcd_write_int_xy(0,0,defx,6);
	lcd_write_int_xy(0,1,defy,6);
	
	delay_millisec(5000);
	lcd_clear();
	
	float angle = atan (defy/defx) * 180 / PI;
	float distance = sqrt (pow (defx, 2.0)+pow (defy, 2.0));
	int steps = distance/0.11;
	int spotsteps = angle*1.58;
	
	
	
	
	lcd_write_int_xy(0,0,angle,6);
	lcd_write_int_xy(0,1,spotsteps,6);
	
	delay_millisec(5000);
	lcd_clear();
	
	
	
	if(defx<0)
	{
	   
	   
		lcd_write_string("spot left");
		lcd_write_int_xy(0,1,angle,3);
		spot_left(spotsteps);
		
		
		
		lcd_clear();
		forward(steps);
		lcd_clear();
	   
	   
	}
	
	else if(defx>0)
	{
	  
	    
		lcd_write_string("spot right");
		lcd_write_int_xy(0,1,angle,3);
		spot_right(spotsteps);
		
		
		lcd_clear();
		forward(steps);
		lcd_clear();
		
		
	}
	
	else 
	{
		forward(steps);
		lcd_clear();
		
	}

}



void sqr(int x,int y,int l)
{


	lcd_write_string("square");
	delay_millisec(4000);
	lcd_clear();



	goto_coordinate(0,0,x,y);
	int angle = 90;
	int steps = l/0.11;
	int spotsteps = angle*1.59;
	
	forward(steps);
	spot_right(spotsteps);
	forward(steps);
	spot_right(spotsteps);
	forward(steps);
	spot_right(spotsteps);
	forward(steps);
    
	
	
}

void rect(int x,int y,int l,int b)
{

	lcd_write_string("rectangle");
	delay_millisec(4000);
	lcd_clear();




	goto_coordinate(0,0,x,y);
	int angle = 90;
	int stepsl = l/0.11;
	int stepsb = b/0.11;
	int spotsteps = angle*1.56;
	
	
	forward(stepsb);
	spot_right(spotsteps);
	forward(stepsl);
	spot_right(spotsteps);
	forward(stepsb);
	spot_right(spotsteps);
	forward(stepsl);
}

void star(int x,int y,int l)
{
	
	
	lcd_write_string("star");
	delay_millisec(4000);
	lcd_clear();
	
	
	
	

	goto_coordinate(0,0,x,y);
	
	int angle1= 108;
	int angle2= 72;
	int angle3= 144;
	
	
	
	int stepsl = l/0.11;
	int spotsteps1 = angle1*1.56;
	int spotsteps2 = angle2*1.56;
	int spotsteps3 = angle3*1.56;
	
	
	
	
	

	
	spot_left(spotsteps1);
	int p= 0;
	for(p=0;p<5;p++)
	{
		forward(stepsl);
		spot_left(spotsteps2);
		forward(stepsl);
		spot_right(spotsteps3);
	}
	
	
	
	
}

void circle(int x,int y,int r)
{




	lcd_write_string("circle");
	delay_millisec(4000);
	lcd_clear();
	
	
	
	//goto_coordinate(0,0,x,y);
	goto_coordinate(x,y,x+r,y);
	
	int spotsteps =3;
	float lc = 90*r/140;
	int steps= lc/0.11;
	
	spot_right(140);
	

	int p=0;
	for(p=0;p<560;p++)
	{
	  forward(steps);
	  spot_right(spotsteps);
	}
	
	
	
	
}






void main()
{
  req_init();
  lcd_write_string("starting");
  delay_millisec(5000);
  lcd_clear();
  
  //sqr(0,0,20);
  //rect(0,0,20,30);
 star(0,0,10);
  //circle(0,0,10);
 //goto_coordinate(0,0,20,20);

  
}