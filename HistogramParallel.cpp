#include <stdlib.h>
#include <stdio.h> 
#include <time.h> 
#include <float.h> 
#include <conio.h> 
#include <string.h> 
#include <iostream>
#include <math.h>
#include <omp.h>

using namespace std;

int main(int argc, char *argv[])
{
	// int dizi[1000]={1000,15,18,2,5,7,10,10,12,12,7,1000,19,14,8,6,18,13,19,11};
	int sayac = 0, i, j, sutunSayisi = 0, temp, min, max, degerAraligi, aralikOlcumu;
	//int *dizi, *baslangic, *bitis;
	int dizi[1000],baslangic[21],bitis[21];
	int ToplamThreadSayisi, ThreadID, chunk = 10; 

	clock_t HesaplamayaBasla, HesaplamayiBitir;  
	double SureFarki;	

	srand(time(NULL));  //her defasinda rand'den farki deger aliyor.	

	printf("\nHesaplama Islemine Basladi : "); 
	HesaplamayaBasla = clock(); 
	printf("%f saniye surdu. \n", (double) HesaplamayaBasla ); 
	

	//dizi = (int *) malloc(1000*sizeof(int));
	//baslangic = (int *) malloc (21*sizeof(int));
	//bitis = (int *) malloc (21*sizeof(int));

	 
		

	#pragma omp parallel shared(dizi,baslangic,bitis,ToplamThreadSayisi,chunk) private(ThreadID,i,j,temp,sutunSayisi,sayac,min,max,degerAraligi,aralikOlcumu) 
	{ 
		ThreadID = omp_get_thread_num(); 
     
		if (ThreadID == 0) 
		{ 
			ToplamThreadSayisi = omp_get_num_threads(); 
			printf("Kullandiginiz Toplam Thread Sayisi : %d \n",ToplamThreadSayisi); 
		}  

	#pragma omp parallel for schedule (static, chunk) 
	 	for(i=0;i<1000;i++)
		{	
			dizi[i] = rand()%1000; 	 //1-1000 arasi sayi uretiyorum 1000 tane.
		}
	 
		min = dizi[0];
		max = dizi[0];
	
	#pragma omp parallel for schedule (static, chunk) 
	 	for(i=1;i<1000;i++)   
		{
			if(dizi[i]<min) min = dizi[i];
			if(dizi[i]>max)	max = dizi[i];
		}
		
	#pragma omp parallel for schedule (static, chunk) 
	 	for(i=1; i<1000; i++)   
			for(j=i; j<1000;j++)
			{
				if(dizi[i]>dizi[j])
				{
					temp = dizi[i];
					dizi[i]=dizi[j];
					dizi[j]=temp;
				}
			}
	 	
	#pragma omp single
	{	
		    cout << "Rastgele Uretilen 1000 Deger ; " << endl;

			for(i = 1; i<1000; i++) 
				cout << dizi[i] << ",";

			printf("\n");

			cout << "\nDizinin En Buyuk Elemani : " << max << endl; 
			cout << "Dizinin En Kucuk Elemani : " << min << endl;
		
			degerAraligi = max - min;
			cout << "Dizinin Deger Araligi (max-min) : " << degerAraligi << endl; 

			cout << "Histogram Icin Sutun Sayisi Giriniz : "; 	
			cin >> sutunSayisi;
			 
			aralikOlcumu = ((float) degerAraligi) / ((float) sutunSayisi) + 0.5f;     
			cout << "Aralik Olcum Sonucu : " << aralikOlcumu << endl;
	}

	baslangic[0] = min;
	
	#pragma omp parallel for schedule (static, chunk) 
		for(i = 0; i<sutunSayisi; i++)
		{
			baslangic[i+1] = (baslangic[i] + aralikOlcumu);
			bitis[i] = (baslangic[i+1] - 1);
			bitis[i+1] = (bitis[i] + aralikOlcumu);
		}
		 
	#pragma omp parallel for schedule (static, chunk)  
		for(j = 0; j<sutunSayisi; j++)
		{	
			sayac = 0;	
			for(i = 1; i<1000; i++)
	 		{
				if ((dizi[i] >= baslangic[j]) && (dizi[i] <= bitis[j]))	
					sayac++; 
			}
				printf("\n");
				printf("%d - %d Araligi ... \t %d Adet \t |", baslangic[j], bitis[j], sayac);	
				for(int k=0; k<sayac; k++) 
						printf("*");	
		}
	}
	 
	printf("\n");
	printf ("\nHesaplama Islemi Sona Erdi : "); 
	HesaplamayiBitir = clock();  
	printf("%f saniye surdu. \n",(double) HesaplamayiBitir ); 
	SureFarki = ((double) (HesaplamayiBitir - HesaplamayaBasla)) / CLOCKS_PER_SEC ; 
	printf("Hesaplanan Sure Farki : %f  saniyedir.", SureFarki);

	/*free(dizi);
	free(baslangic);
	free(bitis);*/

	getche();
	return 0;

} 