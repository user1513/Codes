			 void conut0()
{
	bufchar[2] = TL0;
	bufchar[1] = TH0;
	TH0 = 0;
	TL0 = 0;
	bufchar[0] = TF0;
	TF0 = 0;
}	
uchar caul_0(uint x)
{  
	uchar i;
	for(i=0;i<15;i++)
	if((x<=buff[i])&&(x>=buff[i+1])) return i;
	return  0;
}
void conut( )
{		
	uchar ii; 
	uint x;
	conut0();
	x=_irol_(bufchar[0],8)|bufchar[1];
	ii=caul_0( x) ;
	if (ii==1) Y=ii*5+10+5-(bufchar[1]-buff[2])/(256+buff[1]-buff[2])*5;
	else if  (ii==11) Y=ii*5+10+5-(_irol_(bufchar[1],8)|bufchar[2]-_irol_(buff[ii+1],8)|0x00)/768*5;		//768=3*256
	else if (ii==12) Y=ii*5+10+5-(_irol_(bufchar[1],8)|bufchar[2]-_irol_(buff[ii+1],8)|0x00)/512*5;		//512=2*256 
	else if (ii==13) Y=ii*5+10+5-(_irol_(bufchar[1],8)|bufchar[2]-_irol_(buff[ii+1],8)|0x00)/256*5;
	else Y=ii*5+10+5-(_irol_(bufchar[1],8)|bufchar[2]-_irol_(buff[ii+1],8)|0x00)*5/((buff[ii]-buff[ii+1])*256)	  ;
 

 

}