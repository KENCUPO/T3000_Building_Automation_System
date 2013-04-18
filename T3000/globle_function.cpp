
#pragma once

#include "stdafx.h"
#include "globle_function.h"
#include "Windows.h"
#include "T3000.h"





#define DELAY_TIME	 10	//MS
#define Modbus_Serial	0
#define	Modbus_TCP	1

#include "MainFrm.h"
//#include "global_variable.h"
//#include "global_variable_extern.h"

//how to linker the modbus.dll;;
int read_one(unsigned char device_var,unsigned short address,int retry_times)
{//programmer can control retry times,by parameter retry_times
	BOOL bTemp = g_bEnableRefreshTreeView;
	g_bEnableRefreshTreeView = FALSE;
	int j = read_one_org(device_var,address,retry_times);
	g_bEnableRefreshTreeView |= bTemp;
	return j;
}

// 这个是原始的
int read_one_org(unsigned char device_var,unsigned short address,int retry_times)
{//programmer can control retry times,by parameter retry_times	
// 	CString str;
// 	str.Format(_T("ID:%d [Tx=%d : Rx=%d]"), device_var, g_llTxCount++, g_llRxCount);
// 	SetPaneString(0,str);
	int j=0;
	for(int i=0;i<retry_times;i++)
	{
		register_critical_section.Lock();
		j=Read_One(device_var,address);
		register_critical_section.Unlock();
		if (g_CommunicationType==Modbus_Serial)
		{
		}

		if(j!=-2 && j!=-3)
		{
			CString str;
		
			if (j == -1) // no connection
			{
				str.Format(_T("Addr:%d [Tx=%d Rx=%d Err=%d]"), device_var, ++g_llTxCount, g_llRxCount, g_llTxCount-g_llRxCount);
			}
			else
			{
				str.Format(_T("Addr:%d [Tx=%d Rx=%d Err=%d]"), device_var, ++g_llTxCount, ++g_llRxCount, g_llTxCount-g_llRxCount);
			}
			SetPaneString(0,str);
			if(address==101 && j>32767)
			{//the temperature is below zero;;;;;-23.3
				j=0-65535+j;
			}
			return j;//return reght ,success
		}	 
	}
	CString str;
	str.Format(_T("Addr:%d [Tx=%d Rx=%d : Err=%d]"), device_var, ++g_llTxCount, g_llRxCount,g_llTxCount-g_llRxCount );
	SetPaneString(0,str);
	return j;
}

void SetPaneString(int nIndext,CString str)
{
	/*
//	static CMFCStatusBar * pStatusBar=NULL;
	CMFCStatusBar * pStatusBar=NULL;
	if(AfxGetMainWnd()->GetActiveWindow()==NULL)//if this function is called by a thread ,return 
		return;
	if(pStatusBar==NULL)
		pStatusBar = (CMFCStatusBar *) AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	pStatusBar->SetPaneText(nIndext,str.GetString());
	*/
	CMFCStatusBar * pStatusBar=NULL;
	if(AfxGetMainWnd()->GetActiveWindow()==NULL)//if this function is called by a thread ,return 
		return;
	pStatusBar = (CMFCStatusBar *) AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	pStatusBar->SetPaneText(nIndext,str.GetString());
	//pStatusBar->SetPaneTextColor (RGB(128,0,0));

// 	CRect rc;
// 	pStatusBar->GetItemRect(0, &rc);
// 	CDC* pDC = pStatusBar->GetWindowDC();
// 	rc.DeflateRect(2,0,2,0);
// 	pDC->Rectangle(rc);


// 	if (nIndext == 0)
// 	{
// 		HBITMAP hBmp = LoadBitmap(AfxGetResourceHandle(),  MAKEINTRESOURCE(IDB_UPBMP));
// 		pStatusBar->SetPaneIcon(nIndext, hBmp, RGB(192,192,192));
// 
// 	}
	/*
	status_info infoStatusbar;
	infoStatusbar.nIndex=nIndext;
	infoStatusbar.strInfoText=str;
	::PostMessage(AfxGetMainWnd()->GetActiveWindow()->m_hWnd,WM_USER_FRESHSTATUSBAR,0,(WPARAM)&infoStatusbar);
*/


}


int write_one(unsigned char device_var,unsigned short address,short value,int retry_times)
{//retry 

	BOOL bTemp = g_bEnableRefreshTreeView;
	g_bEnableRefreshTreeView = FALSE;
	int j = write_one_org(device_var,address,value,retry_times);
	g_bEnableRefreshTreeView |= bTemp;
	return j;
}

int write_one_org(unsigned char device_var,unsigned short address,short value,int retry_times)
{//retry 
// 	CString str;
// 	str.Format(_T("ID :%d Writing %d"),device_var,address);
// 	SetPaneString(0,str);
	short temp_value=value;
	if(address==101 && temp_value<0)
	{//for the temperature is below zero;;;;;;;;-23.3
		temp_value=65535+temp_value;
	}
	int j=0;
	for(int i=0;i<retry_times;i++)
	{
		//register_critical_section.Lock();
		j=Write_One(device_var,address,temp_value);
		multi_register_value[address]=value;//mark***********************
		//register_critical_section.Unlock();
		if (g_CommunicationType==Modbus_Serial)
		{
			//Sleep(DELAY_TIME);//do this for better quickly
		}

		if(j!=-2 && j!=-3)
		{
			CString str;
			if (j == -1) // no connetiong
			{
				str.Format(_T("Addr:%d [Tx=%d Rx=%d : Err=%d]"), device_var, ++g_llTxCount, g_llRxCount, g_llTxCount-g_llRxCount);
			}
			else
			{
				str.Format(_T("Addr:%d [Tx=%d Rx=%d : Err=%d]"), device_var, ++g_llTxCount, ++g_llRxCount, g_llTxCount-g_llRxCount);
			}
			SetPaneString(0,str);
			return j;//return right success
		}
	}
	CString str;
	str.Format(_T("Addr:%d [Tx=%d Rx=%d : Err=%d]"), device_var, ++g_llTxCount, g_llRxCount, g_llTxCount-g_llRxCount);
	SetPaneString(0,str);
	return j;
}

int Write_Multi(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length,int retry_times)
{
	BOOL bTemp = g_bEnableRefreshTreeView;
	g_bEnableRefreshTreeView = FALSE;
	int j = Write_Multi_org(device_var, to_write, start_address, length, retry_times);
	g_bEnableRefreshTreeView |= bTemp;
	return j;
}

int Write_Multi_org(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length,int retry_times)
{
// 	CString str;
// 	str.Format(_T("ID :%d Multi writing start :%d Amount: %d"),device_var,start_address,length);
// 	SetPaneString(2,str);
	int j=0;
	for(int i=0;i<retry_times;i++)
	{
		register_critical_section.Lock();
		j=write_multi(device_var,to_write,start_address,length);
		register_critical_section.Unlock();
		if (g_CommunicationType==Modbus_Serial)
		{
			Sleep(DELAY_TIME);//do this for better quickly
		}
		if(j!=-2)
		{
			//SetPaneString(2,_T("Multi-Write successful!"));
			CString str;
			str.Format(_T("Addr:%d [Tx=%d Rx=%d : Err=%d]"), device_var, ++g_llTxCount, ++g_llRxCount,g_llTxCount-g_llRxCount);
			SetPaneString(0,str);
			return j;
		}
	}
	//SetPaneString(2,_T("Multi-write failure!"));
	CString str;
	str.Format(_T("Addr:%d [Tx=%d Rx=%d : Err=%d]"), device_var, ++g_llTxCount, g_llRxCount,g_llTxCount-g_llRxCount);
	SetPaneString(0,str);
	return j;
}

int Read_Multi(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length,int retry_times)
{
	BOOL bTemp = g_bEnableRefreshTreeView;
	g_bEnableRefreshTreeView = FALSE;
	int j = Read_Multi_org(device_var,put_data_into_here,start_address,length,retry_times);
	g_bEnableRefreshTreeView |= bTemp;
	return j;
}


int Read_Multi_org(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length,int retry_times)
{
// 	CString str;
// 	str.Format(_T("ID:%d [Tx=%d : Rx=%d]"), device_var, g_llTxCount++, g_llRxCount++);
// 	SetPaneString(0,str);

	int j=0;
	for(int i=0;i<retry_times;i++)
	{

		j=read_multi(device_var,put_data_into_here,start_address,length);
		if (g_CommunicationType==Modbus_Serial)
		{
			Sleep(DELAY_TIME*3);//do this for better quickly
		}
		if(j!=-2)
		{
			CString str;
			str.Format(_T("Addr:%d [Tx=%d Rx=%d : Err=%d]"), device_var, ++g_llTxCount, ++g_llRxCount, g_llTxCount-g_llRxCount);
			SetPaneString(0,str);
			return j;
		}
	}
	CString str;
	str.Format(_T("Addr:%d [Tx=%d Rx=%d : Err=%d]"), device_var, ++g_llTxCount, g_llRxCount, g_llTxCount-g_llRxCount);
	SetPaneString(0,str);
	return j;
}



int turn_hex_str_to_ten_num(char *source)
{
	int j=0,k=0,l=0;
	for(int i=0;i<2;i++)//***********************************2
		if(j==0)
		{
			switch(source[i])
			{
			case '0':k=0;break;
			case '1':k=1;break;
			case '2':k=2;break;
			case '3':k=3;break;
			case '4':k=4;break;
			case '5':k=5;break;
			case '6':k=6;break;
			case '7':k=7;break;
			case '8':k=8;break;
			case '9':k=9;break;

			case 'a':k=10;break;
			case 'b':k=11;break;
			case 'c':k=12;break;
			case 'd':k=13;break;
			case 'e':k=14;break;
			case 'f':k=15;break;
			case 'A':k=10;break;
			case 'B':k=11;break;
			case 'C':k=12;break;
			case 'D':k=13;break;
			case 'E':k=14;break;
			case 'F':k=15;break;
			
			default:return -1;
			}
			for( ;j<2-i-1;j++)
				k*=16;
		}
		else
		{
			l+=k;
			j=0;
			i--;
		}
		l+=k;
	return l;
}

unsigned short turn_4_hex_char_to_unsigned_short(char *source)
{//can only turn four char(hex string) to int
	//return -1,the wrong input ,the char number>4;or the input char is not hex string;

	int a=strlen(source);
	if(a>4)
	{
//		AfxMessageBox("wrong input!\nthat the length of the input string !=2!");
//		for(int i=0;i<a;i++)
//			*source='\0';
		return -1;//input wrong 1
	}
	unsigned short k=0,l=0;
	for(int j=0;j<a;j++)
	{
		switch(source[j])
		{
		case '0':k=0;break;
		case '1':k=1;break;
		case '2':k=2;break;
		case '3':k=3;break;
		case '4':k=4;break;
		case '5':k=5;break;
		case '6':k=6;break;
		case '7':k=7;break;
		case '8':k=8;break;
		case '9':k=9;break;

		case 'a':k=10;break;
		case 'b':k=11;break;
		case 'c':k=12;break;
		case 'd':k=13;break;
		case 'e':k=14;break;
		case 'f':k=15;break;
		case 'A':k=10;break;
		case 'B':k=11;break;
		case 'C':k=12;break;
		case 'D':k=13;break;
		case 'E':k=14;break;
		case 'F':k=15;break;

		default: return -1;//2
		}
		l*=16;
		l+=k;
	}
	return l;
}
int turn_hex_char_to_int(char c)
{
		int k=0;
		switch(c)
		{
		case '0':k=0;break;
		case '1':k=1;break;
		case '2':k=2;break;
		case '3':k=3;break;
		case '4':k=4;break;
		case '5':k=5;break;
		case '6':k=6;break;
		case '7':k=7;break;
		case '8':k=8;break;
		case '9':k=9;break;

		case 'a':k=10;break;
		case 'b':k=11;break;
		case 'c':k=12;break;
		case 'd':k=13;break;
		case 'e':k=14;break;
		case 'f':k=15;break;
		case 'A':k=10;break;
		case 'B':k=11;break;
		case 'C':k=12;break;
		case 'D':k=13;break;
		case 'E':k=14;break;
		case 'F':k=15;break;

		default: return -1;//2
		}
		return k;
}

bool turn_hex_file_line_to_unsigned_char(char *str)
{
	char *p_temp=str;
	int itemp=strlen(p_temp);
	for(int i=0;i<itemp;i++)
	{
		*(p_temp+i)=turn_hex_char_to_int(*(p_temp+i));	
		if(*(p_temp+i)==-1)
			return false;
	}
	return true;
}


void turn_int_to_unsigned_char(char *source,int length_source,unsigned char *aim)
{
	char *p_c_temp=source;
	unsigned char *p_uc_temp=aim;
	unsigned char uctemp;
	for(int i=0;i <length_source;i++)
		if(i%2==0)
		{
			char ctemp=*(p_c_temp+i);
			uctemp = ctemp*16;
		}
		else
		{
			char ctemp=*(p_c_temp+i);
			uctemp+=ctemp;
			*(p_uc_temp+i/2)=uctemp;
			uctemp=0;
		}
}

void turn_unsigned_short_to_hex_char(char *order,unsigned short source)
{//return value ,1 is success ,0 is failure
	//the number of the order ,should >=5,is good
	//the source must >0 and <65535
	int itemp=0;
	unsigned short ustemp=0;
	do{
		ustemp=source%16;
		order[itemp]=turn_unsigned_to_char(ustemp);
		source/=16;
		itemp++;
	}while(source!=0);
	order[itemp]='\0';	
	if(strlen(order)%2)
	{		
		order[itemp]='0';
		order[itemp+1]='\0';
	}
	order=strrev(order);
}
char turn_unsigned_to_char(unsigned short source)
{//return value ,is the char 
	//0 is failure
	if(source>15)
		return 0;
	if(source>=0 && source<=9)
		return source+48;
	else
		if(source>=10 && source<=15)
			return source+55;
}

void delete_folder(CString DirName)
{
	CFileFind tempFind; 
	CString temp_str;
	temp_str=DirName+_T("\\*.*");
	BOOL IsFinded=(BOOL)tempFind.FindFile(temp_str.GetString()); 
	while(IsFinded) 
	{ 
		IsFinded=(BOOL)tempFind.FindNextFile(); 
		if(!tempFind.IsDots()) 
		{ 
			CString file_name=tempFind.GetFileName();			
			file_name=DirName+"\\"+file_name;				
			if(tempFind.IsDirectory()) 
				delete_folder(file_name); 
			else 
				DeleteFile(file_name); 
		} 
	} 
	tempFind.Close(); 
	RemoveDirectory(DirName);//delete blank directory
}

CString get_units_from_now_range(int the_tstat_id,int analog1_or_analog2)
{//parameter analog1_or_analog2 is the register id
	//analog1 is 180;analog2 is 181
	//if analog1_or_analog2 is negative ,only get temperature units ,
	CString geted_units;
	int the_rang;
	if(analog1_or_analog2<0)
	{
		the_rang=read_one(the_tstat_id,121);
		UINT uint_temp=GetOEMCP();//get system is for chinese or english
		if(the_rang==0)
		{
			if(uint_temp!=936 && uint_temp!=950)
				geted_units.Format(_T("%dC"),176);
			else
				geted_units=_T("°C");
		}
		else
		{
			if(uint_temp!=936 && uint_temp!=950)
				geted_units.Format(_T("%dF"),176);
			else
				geted_units=_T("°F");
		}
		return geted_units;
	}
	if(analog1_or_analog2==180)
		the_rang=read_one(the_tstat_id,188);
	else if(analog1_or_analog2==181)
		the_rang=read_one(the_tstat_id,189);
	if(the_rang==0)
		//Raw
		geted_units="";
	else if(the_rang==1)
	{//
		the_rang=read_one(the_tstat_id,121);
		UINT uint_temp=GetOEMCP();//get system is for chinese or english
		if(the_rang==0)
		{
			if(uint_temp!=936 && uint_temp!=950)
				geted_units.Format(_T("%dC"),176);
			else
				geted_units=_T("°C");
		}
		else
		{
			if(uint_temp!=936 && uint_temp!=950)
				geted_units.Format(_T("%dF"),176);
			else
				geted_units=_T("°F");
		}
	}
	else if(the_rang==2)
	{//
		geted_units=_T("%");
	}
	else if(the_rang==3)
	{//ON/OFF
		geted_units=_T("");
	}
	else if(the_rang==4)
	{//Customer Sersor
		if(analog1_or_analog2==180)
		{
			int m_271=read_one(the_tstat_id,271);
			int m_272=read_one(the_tstat_id,272);
			if(m_271>>8=='0')
			{
				if((m_271 & 0xFF) =='0')
				{
					if(m_272>>8=='0')
						geted_units.Format(_T("%c"),m_272 & 0xFF);
					else
						geted_units.Format(_T("%c%c"),m_272>>8,m_272 & 0xFF);
				}
				else
					geted_units.Format(_T("%c%c%c"),m_271 & 0xFF,m_272>>8,m_272 & 0xFF);
			}
			else
				geted_units.Format(_T("%c%c%c%c"),m_271>>8,m_271 & 0xFF,m_272>>8,m_272 & 0xFF);
		}
		else if(analog1_or_analog2==181)
		{
			int m_273=read_one(the_tstat_id,273);
			int m_274=read_one(the_tstat_id,274);
			if(m_273>>8=='0')
			{
				if((m_273 & 0xFF)=='0')
				{
					if(m_274>>8=='0')
						geted_units.Format(_T("%c"),m_274 & 0xFF);
					else
						geted_units.Format(_T("%c%c"),m_274>>8,m_274 & 0xFF);
				}
				else
					geted_units.Format(_T("%c%c%c"),m_273 & 0xFF,m_274>>8,m_274 & 0xFF);
			}		
			else
				geted_units.Format(_T("%c%c%c%c"),m_273>>8,m_273 & 0xFF,m_274>>8,m_274 & 0xFF);

		}
	}	
	return geted_units;
}

void SetPaneCommunicationPrompt(CString strComInfo)
{
	/*
	static CStatusBar * pStatusBar=NULL;
	if(AfxGetMainWnd()->GetActiveWindow()==NULL)//if this function is called by a thread ,return 
		return;
	if(pStatusBar==NULL)
		pStatusBar = (CStatusBar *) AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	pStatusBar->SetPaneText(2,strComInfo.GetString(),true);
	*/

}



float get_tstat_version(unsigned short tstat_id)
{//get tstat version and judge the tstat is online or no
	//tstat is online ,return >0
	//tstat is not online ,return -2
	
	float tstat_version2=(float)read_one((unsigned char)tstat_id,4);//tstat version			
	if(tstat_version2==-2 || tstat_version2==-3)
		return tstat_version2;
	if(tstat_version2 >=240 && tstat_version2 <250)
		tstat_version2 /=10;
	else 
	{
		tstat_version2 = (float)(read_one((unsigned char)tstat_id,5,1)*256+read_one((unsigned char)tstat_id,4,1));	
		tstat_version2 /=10;
	}//tstat_version
	return tstat_version2;
}
float get_curtstat_version()
{
	float tstat_version2=multi_register_value[MODBUS_VERSION_NUMBER_LO];//tstat version			
	if(tstat_version2<=0)
		return tstat_version2;
	if(tstat_version2 >=240 && tstat_version2 <250)
		tstat_version2 /=10;
	else 
	{
		tstat_version2 = (float)(multi_register_value[MODBUS_VERSION_NUMBER_HI]*256+multi_register_value[MODBUS_VERSION_NUMBER_LO]);	
		tstat_version2 /=10;
	}//tstat_version
	return tstat_version2;

}

int make_sure_isp_mode(int the_tstat_id)
{
	unsigned short isp_unsigned_short[20];
	int i=Read_Multi(the_tstat_id,isp_unsigned_short,100,20);
	if(i==-2 || i==-1)
		return i;//no response
	if(i<0)
		return i;//no in isp mode
	else
	{
		for(int j=0;j<20;j++)
		{
			if(isp_unsigned_short[j]!=1)
				return 0;// no in isp mode
		}
	}
	return 1;//in isp mode 
}

bool get_serialnumber(long & serial,int the_id_of_product)
{

	unsigned short SerialNum[4]={0};
	int nRet=0;
	nRet=Read_Multi(the_id_of_product,&SerialNum[0],0,4);
	serial=0;
	if(nRet>0)
	{
		serial=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
		return TRUE;
	}
		return FALSE;
}


UINT get_serialnumber()
{
	return multi_register_value[MODBUS_SERIALNUMBER_LOWORD]+multi_register_value[MODBUS_SERIALNUMBER_LOWORD+1]*256+multi_register_value[MODBUS_SERIALNUMBER_HIWORD]*256*256+multi_register_value[MODBUS_SERIALNUMBER_HIWORD+1]*256*256*256;
}





bool multi_read_tstat(int id)
{

	bool return_value=true;
	int i;
	for(i=0;i<7;i++)
	{
		//register_critical_section.Lock();
		//int nStart = GetTickCount();
		if(-2==Read_Multi(id,&multi_register_value[i*64],i*64,64,1))
			return_value=false;

		//TRACE(_T("Read_Multi once = %d \n"), GetTickCount()-nStart);
		Sleep(50);
		//register_critical_section.Unlock();
	}
	return return_value;
}


bool can_be_writed_hex_file(int product_model,int hex_file_product_model)
{
	//product model
	// T3-8IO-------------20
	// T3-32I-------------22
	// T3-8i/60-----------23
	// Flexdriver---------25
	//Tstat5A-------------2
	//Tstat5B-------------1
	//Tstat5B2------------3
	//Tstat5C-------------4
	//Tstat5D-------------12
	//Solar---------------30
	//hex_file_product_model parameter is the hex_file_register 0x100 (256)
//	if (product_model==18||product_model==17)
	{
		return true;
	}
	if(hex_file_product_model==255)//////////////old version hex file,before 2005.11.15
		return true;
	if(product_model<=TSTAT_PRODUCT_MODEL && hex_file_product_model<=TSTAT_PRODUCT_MODEL)
		return true;
	if(product_model==LED_PRODUCT_MODEL && hex_file_product_model==LED_PRODUCT_MODEL)
		return true;
	if(product_model==PM_NC && hex_file_product_model==PM_NC)
		return true;
	if(product_model==PM_T3IOA && hex_file_product_model==PM_T3IOA)
		return true;
	if(product_model==PM_T38AIOD && hex_file_product_model==PM_T38AIOD)
		return true;
	if(product_model==T3_32I_PRODUCT_MODEL && hex_file_product_model==T3_32I_PRODUCT_MODEL)
		return true;
	if(product_model==T3_8I_16O_PRODUCT_MODEL && hex_file_product_model==T3_8I_16O_PRODUCT_MODEL)
		return true;
	if(product_model==PM_SOLAR && hex_file_product_model==PM_SOLAR)
		return true;
	if(product_model==PM_ZIGBEE && hex_file_product_model==PM_ZIGBEE)
		return true;	
	return false;
}
CString get_product_name_by_product_model(int product_model)
{
	CString return_str;
	if(product_model>0 && product_model<=TSTAT_PRODUCT_MODEL)
		product_model=TSTAT_PRODUCT_MODEL;
	switch(product_model)
	{
	case 19:return_str=_T("Tstat");break;
	case 20:return_str=_T("T3-8IO");break;
	case 22:return_str=_T("T3-32I");break;
	case 23:return_str=_T("T3-8i/60");break;
	case 25:return_str=_T("Flexdriver");break;
	case 30:return_str=_T("Solar");break;
	case PM_ZIGBEE:return_str=_T("ZigBee");break;
	default:return_str=_T("Unknown");break;
	}
	return return_str;
}

// Function : 获得单位名称，此单位用于Input Grid，Output Grid，Output Set Grid，主界面的Grid等等。
// Param: int nRange: 指示当前的Range的选择值。函数应该根据Range的选择以及TStat的型号，
//					获得单位名称，如摄氏度，华氏度，百分比，自定义的单位等。
//           int nPIDNO: 区分PID1 还是PID2，1＝PID1，2＝PID2
// return ： 单位名称 
CString GetTempUnit(int nRange, int nPIDNO)
{
	CString strTemp=_T("");
	
	if(nRange<0) // 使用默认的温度单位
	{
		UINT uint_temp=GetOEMCP();//get system is for chinese or english
		if(uint_temp!=936 && uint_temp!=950)
		{
			if(multi_register_value[121]==0)
			{
				strTemp.Format(_T("%cC"),176);
			}
			else
			{
				strTemp.Format(_T("%cF"),176);
			}
		}
		else
		{
			//Chinese.
			if(multi_register_value[121]==0)
			{
				strTemp=_T("°C");
			}
			else
			{
				strTemp=_T("°F");
			}
		}
		return strTemp;
	}

	if(nRange==0)		// Raw value, no unit
		strTemp=_T("");
	else if(nRange==1)
	{//
		UINT uint_temp=GetOEMCP();//get system is for chinese or english
		if(uint_temp!=936 && uint_temp!=950)
		{
			if(multi_register_value[121]==0)
			{
				strTemp.Format(_T("%cC"),176);
			}
			else
			{
				strTemp.Format(_T("%cF"),176);
			}
		}
		else
		{
			//chinese.
			if(multi_register_value[121]==0)
			{
				strTemp=_T("°C");
			}
			else
			{
				strTemp=_T("°F");
			}
		}
		return strTemp;
	}
	else if(nRange==2)
	{//
		strTemp=_T("%");
	}
	else if(nRange==3)
	{//ON/OFF
		strTemp=_T("");
	}
	else if(nRange==4)
	{//Customer Sersor
		if(nPIDNO==1)
		{
			int m_271=multi_register_value[271];
			int m_272=multi_register_value[272];
			if(m_271>>8=='0')
			{
				if((m_271 & 0xFF) =='0')
				{
					if(m_272>>8=='0')
						strTemp.Format(_T("%c"),m_272 & 0xFF);
					else
						strTemp.Format(_T("%c%c"),m_272>>8,m_272 & 0xFF);
				}
				else
					strTemp.Format(_T("%c%c%c"),m_271 & 0xFF,m_272>>8,m_272 & 0xFF);
			}
			else
				strTemp.Format(_T("%c%c%c%c"),m_271>>8,m_271 & 0xFF,m_272>>8,m_272 & 0xFF);
		}
		else if(nPIDNO==2)
		{
			int m_273=multi_register_value[273];
			int m_274=multi_register_value[274];
			if(m_273>>8=='0')
			{
				if((m_273 & 0xFF)=='0')
				{
					if(m_274>>8=='0')
						strTemp.Format(_T("%c"),m_274 & 0xFF);
					else
						strTemp.Format(_T("%c%c"),m_274>>8,m_274 & 0xFF);
				}
				else
					strTemp.Format(_T("%c%c%c"),m_273 & 0xFF,m_274>>8,m_274 & 0xFF);
			}		
			else
				strTemp.Format(_T("%c%c%c%c"),m_273>>8,m_273 & 0xFF,m_274>>8,m_274 & 0xFF);

		}
	}	
	return strTemp;
}

CString get_product_class_name_by_model_ID(int nModelID)
{
	CString strClassName;
	switch(nModelID)
	{
	case 2:strClassName=g_strTstat5a;break;
	case 1:strClassName=g_strTstat5b;break;
	case 3:strClassName=g_strTstat5b;break;
	case 4:strClassName=g_strTstat5c;break;
	case 6:strClassName=g_strTstat6;break;
	case 7:strClassName=g_strTstat7;break;
	case 12:strClassName=g_strTstat5d;break;
	case PM_NC:strClassName=g_strnetWork;break;
	case NET_WORK_OR485_PRODUCT_MODEL:strClassName=g_strOR485;break;
	case 17: strClassName=g_strTstat5f;break;
	case 18:strClassName=g_strTstat5g;break;
	case 16:strClassName=g_strTstat5e;break;
	case 19:strClassName=g_strTstat5h;break;
	case PM_LightingController: strClassName = g_strLightingCtrl;
	
	case 13:
	case 14:break;
	default:strClassName=g_strTstat5a;break;
	}

	return strClassName;
}


BOOL GetSerialComPortNumber1(vector<CString>& szComm)
{
	LPCTSTR strRegEntry = _T("HARDWARE\\DEVICEMAP\\SERIALCOMM\\");

	HKEY   hKey;   
	LONG   lReturnCode=0;   
	lReturnCode=::RegOpenKeyEx(HKEY_LOCAL_MACHINE, strRegEntry, 0, KEY_READ, &hKey);   

	if(lReturnCode==ERROR_SUCCESS)   
	{  
		DWORD dwIndex = 0;
		WCHAR lpValueName[MAX_PATH];
		ZeroMemory(lpValueName, MAX_PATH);
		DWORD lpcchValueName = MAX_PATH; 
		DWORD lpReserved = 0; 
		DWORD lpType = REG_SZ; 
		BYTE		lpData[MAX_PATH]; 
		ZeroMemory(lpData, MAX_PATH);
		DWORD lpcbData = MAX_PATH;
		dwIndex = 0;
		while(RegEnumValue(	hKey, dwIndex, lpValueName, &lpcchValueName, 0, &lpType, lpData, &lpcbData ) != ERROR_NO_MORE_ITEMS)
		{   
			//TRACE("Registry's   Read!");   
			dwIndex++;

			lpcchValueName = MAX_PATH; 
			//lpValueName[0] = '\0'; 
			
			CString strValueName= CString(lpValueName);
		
			WCHAR ch[MAX_PATH];
			ZeroMemory(ch, MAX_PATH);
			memcpy(ch, lpData, lpcbData);
			CString str = CString(ch);
			szComm.push_back(str);

			ZeroMemory(lpData, MAX_PATH);
			lpcbData = MAX_PATH;

		}   
		::RegCloseKey(hKey);   		   

		return TRUE;
	}

	return FALSE;   
}
