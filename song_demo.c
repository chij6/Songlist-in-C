#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h> 
#define ROW 900
#define Songlist_Max 1000
#define SWAP(x, y, t) {(t) = (x) ; (x) = (y) ; (y) = (t) ;}

typedef struct
{ 
	char Song[100];
	char Singer[100];
	int Time;
	char fav;	
	char srcData[256] ; 
} alldata;

void Search_song(alldata *s, int sum);//依照歌名搜尋 
void Search_singersortbysong(alldata *s, int sum); //依照歌手搜尋後按照歌名排序
void Search_singersortbytime(alldata *s, int sum); //依照歌手搜尋後按照歌曲時間排序
void Search_timesortbysong(alldata *s, int sum); //依照時間區間搜尋按照歌名排序
void Search_timesortbysinger(alldata *s, int sum); //依照時間區間搜尋按照歌手排序

void quicksortsong(alldata *s, int left, int right); //歌名快速排序(以歌名第一個字母排序)
void quicksortsinger(alldata *s, int left, int right); //歌手快速排序
void quicksorttime(alldata *s, int left, int right); //歌曲長度排序
void printlist (alldata *s, int sum); //印出歌單  

void Modify(alldata *AllData, int RowCount );
void Randomplay(alldata *s, int sum);
void AddFavorite(alldata *s, int sum);
void SearchFavorite();

int fooOperate(alldata *AllData, int RowCount );
int fooAdd(alldata* AllData , int  RowCount ); // 新增
int fooDelete(alldata* AllData, int  RowCount); // 移除  


char *getData( char *outName , char *inStr )
{
	char *retStr ;
	if( inStr[0] == '\"' )
	{
		retStr = strtok( inStr, "\"" );
		strcpy( outName , retStr );
		retStr += strlen( outName ) + 1 ;
	} else
	{
		retStr = strtok( inStr, "," );
		strcpy( outName , retStr );
		retStr += strlen( outName ) ;
	}

	return retStr + 1 ;

}

void getData3( char *A , char *B , char *C , const char *inStr )// AllData[RowCount].Song ,  AllData[RowCount].Singer , C , Row
{
	char ch[1024] ;
	char * str ;  
	strcpy( ch , inStr );  // ex: Pass Out,Tinie Tempah,237 
	str = ch ;
	str = getData( A , str );//Song,Row
	str = getData( B , str );//singer,row
	str = getData( C , str );//c,row
}

int main()
{

	alldata AllData[ROW]; // define ROW 900 //歌曲結構  
	FILE *fp;	//檔案指標  
	fp = fopen("songlist.csv", "r"); //只讀 不改  
	if (fp == NULL) 
	{
		fprintf(stderr, "fopen() failed.\n");
		exit(EXIT_FAILURE);
	}

	char Row[80];
	char C[200] ;
	char *RowData;
	int RowCount=0,index=0;   

	while (fgets(Row, 80, fp) != NULL)
	{
		// 存到原始資料位置(不區分)的最後一個  
		strcpy(AllData[RowCount].srcData ,Row );
		// 去換行符號   
		strtok( AllData[RowCount].srcData , "\n" );
		// 拆資料 
		getData3( AllData[RowCount].Song ,  AllData[RowCount].Singer , C , Row );
		AllData[RowCount].Time = atoi( C );//轉成數字  
		AllData[RowCount].fav='n'; 
		RowCount++;
		printf("\n");
	
	}  
	printlist(AllData,  RowCount);
    printf("Welcome to our music player system !!!\n ");
    int Function;
    do
	{
		printf("請在下方選擇欲使用的功\能代碼 :  \n 1.功\能操作(新增、刪除、修改)\n 2.查詢\n 3.加入最愛清單\n 4.查詢最愛清單\n 5.隨機播放\n 6.離開 : \n");
		scanf("%d",&Function);
		switch(Function)
		{
			case 1:
				RowCount=fooOperate(AllData, RowCount );
				break;
			case 2:
				Search_singersortbysong(AllData,RowCount);
				break;
			case 3:
				AddFavorite(AllData, RowCount);
				printlist(AllData,  RowCount);
				break;
			case 4:
				SearchFavorite(); 
				break;	
			case 5:
				Randomplay(AllData,RowCount);
				break;
			case 6:
				printf("成功\離開\n"); 
				break;
			default:
				printf("請重新輸入,並無此功\能\n");
		}	
	}while(Function != 6);
	return 0;	
}

int fooOperate(alldata *AllData, int RowCount )
{
	int Function ;
	while( 1 )
	{
		printf("請輸入代碼 (1.新增2.刪除3.修改4.離開): \n");
		scanf("%d",&Function);
		switch(Function)
		{
		case 1:
			printf("輸入要新增的歌曲資訊(歌名、演唱者、歌曲長度):\n");
			return fooAdd(AllData, RowCount ); //呼叫  
			break;
		case 2:
			printf("輸入要刪除的歌名或演唱者(該演唱者所有歌曲皆刪除):\n"); 
			return  fooDelete(AllData, RowCount); //呼叫  
			break;
		case 3:
			Modify(AllData , RowCount );  
			return RowCount ;  
			break;
		case 4 :
			return RowCount ;
		default :
			printf("輸入錯誤\n" );
		}
		
	}

	return RowCount ;
 
}

int fooAdd(alldata* AllData, int  RowCount)
{
	alldata saveData ; //這個是呼叫的新結構 以存取輸入的資訊  
	alldata* list = AllData ;//原本資料給 list  
	FILE *fp ; //檔案指標   
	char songinfo_name[100];	  //要修改資訊的歌名 
	char songinfo_namecpy[100];
	char songinfo_new[100];		  //修改內容 
								  
	char song_time[1024] ;
	char song_modify[100];
	int mm , ss ;
	int i ;

	scanf("\n");
	gets(songinfo_new); //開始傳入使用者的資料  
	song_time[0] = 0 ;  //先把時間預設為0，因為題目要求要輸入幾分幾秒，最後再做換算，換成秒數  
	sscanf( songinfo_new , "%s %s %s" , saveData.Song , saveData.Singer , song_time ); //意思是將songinfo_new以%s %s %s的形式存入三個變數中 
	if( song_time[0] == 0 )
	{
		printf("輸入錯誤\n" );
		return RowCount ; 
	}
	mm = ss = 0 ;
	sscanf( song_time , "%d:%d" , &mm , &ss );  //將song_time以%d:%d的形式存入兩個變數中
	ss += mm * 60 ; //計算總秒數  
	saveData.Time = ss ; //存起來，更新 
	if(ss <= 0 ) //如果時間<0，代表不合格  
	{
		printf("輸入錯誤\n" );
		return RowCount ; //回傳原本的資料量，不加1  
	}

	// 自動產生 n
	saveData.fav = 'n' ;

	// 字串格式化  將後面三個值依照這個格式存入前面的變數中  讓格式會符合csv檔的要求  因為輸入的時候我們是輸入字元型態 這時要改成csv想要的格式  
	sprintf( saveData.srcData , "%s,%s,%d" , saveData.Song , saveData.Singer , saveData.Time );



	// 加入  把saveData所指由NULL結束的字符串複製到AllData + RowCount所指的數組中  
	memcpy( AllData + RowCount , &saveData , sizeof( saveData ));




	//把新增的歌曲加到陣列最後一個  // 把saveData所指記憶體區域複製 sizeof( saveData )個字節到 AllData + RowCount所指記憶體區域 
	RowCount ++ ; //確保資料量加1列，表示歌曲量多一首  

	// 存檔  
	fp = fopen("songlist.csv", "w"); //存檔，寫入更改  
	list = AllData ; // 定指標頭  
	//存檔順便確認 重頭來過   
	for( i = 0 ; i < RowCount ; ++i , ++list ) // 跑 RowCount 次，每次結束指標會 + 1
	{
		// 存檔  
		sprintf( song_modify , "%s\n" , list->srcData ); //把後面的字串加一個換行符號丟給songmodify  
		//sprintf 字串格式化  
		fwrite( song_modify , strlen( song_modify ) , 1 , fp );  
	}

	printf("新增成功\n");
	fclose(fp);
	//
	return RowCount  ;
}
  
int fooDelete(alldata* AllData, int  RowCount)
{
	//alldata saveData ; //這個是呼叫的新結構 以存取輸入的資訊 
	alldata* list = AllData ; //原本資料給 list   
	FILE *fp ;
	char songinfo_name[100];	  //要修改資訊的歌名 
	char songinfo_namecpy[100];
	char songinfo_new[100];		  //修改內容 

	char song_time[1024] ;
	char song_modify[100];
	int mm , ss ;
	int i ;
	int k ;
	int count ;

	scanf("\n"); // 題目要求是"輸入要刪除的歌名或演唱者(該演唱者所有歌曲皆刪除):\n"
	gets(songinfo_name);

	// 找  
	count = 0 ;
	list = AllData + RowCount - 1 ; //從後面開始找  
	for( i = RowCount - 1 ; i >= 0 ; i-- , list --  ){//strcmp 字符串比較  strcmp(str1,str2)，若str1=str2，則返回零；若str1<str2，則返回負數；若str1>str2，則返回正數
		if(( strcmp( list->Song , songinfo_name ) == 0 )|| ( strcmp( list->Singer , songinfo_name ) == 0 )) //若str1=str2 // || => 或是 //若str1=str2 
		{
			count ++ ; //代表有找到相同  ，最後可以看到底刪了多少個 
			// 往前搬  
			RowCount -- ;  //有相等的話 ，音樂數量就-1 
			for( k = i ; k < RowCount ; ++k )
			{
				memcpy( AllData + k , AllData + k + 1 , sizeof( alldata )); // 把AllData + k + 1所指記憶體區域複製 sizeof( alldata )個字節到 AllData + k所指記憶體區域 
			}  //也就是把找到相同處刪掉 由下一個覆蓋掉  
		}
	}   
	
	//
	if( count == 0 )
	{
		printf( "找不到相同的歌曲，此系統不執行刪除功\能\n" );
		return RowCount ; //沒有，音樂數量就不動  
	}
	// 存檔  
	fp = fopen("songlist.csv", "w");
	list = AllData ; 
	for( i = 0 ; i < RowCount ; ++i , ++list ) 
	{
		// 存檔 就是重新更新  
		sprintf( song_modify , "%s\n" , list->srcData );
		fwrite( song_modify , strlen( song_modify ) , 1 , fp );
	}

	printf("移除完成，共移除了%d個\n" , count );
	fclose(fp);

	return RowCount ;
}
 
void Modify(alldata* AllData , int  RowCount )
{
	alldata saveData ;
	alldata* list = AllData ;
	FILE *fp ;
	char songinfo_name[100];	    
	char songinfo_new[100];		   

	char song_time[1024] ;
	char song_modify[100];
	int mm , ss ;
	int i ;

	printf("輸入要修改的歌名:\n");
	scanf("\n");
	gets(songinfo_name); //讀取使用者的歌名  
  
	for( i = 0 ; i < RowCount ; ++i , list++ )  
		if( strcmp( list->Song , songinfo_name ) == 0 )  
			break ;   
	if( ROW == i )  
	{
		printf("找不到這歌名\n" );
		return ; 
	}

	printf("修改內容:\n");
	scanf("\n");
	gets(songinfo_new);   
	song_time[0] = 0 ;
	sscanf( songinfo_new , "%s %s %s" , saveData.Song , saveData.Singer , song_time ); //將songinfo_new以%s %s %s的形式存入三個變數中 
	if( song_time[0] == 0 )
	{
		printf("輸入錯誤\n" );
		return ; 
	}
	mm = ss = 0 ;   
	sscanf( song_time , "%d:%d" , &mm , &ss ); //強制從字元轉換成數字型  
	ss += mm * 60 ;  
	saveData.Time = ss ;  
	if(ss <= 0 )
	{
		printf("輸入錯誤\n" );
		return ;
	}

	// 開始資料格式化
	sprintf( saveData.srcData , "%s,%s,%d" , saveData.Song , saveData.Singer , saveData.Time );
	memcpy( list , &saveData , sizeof(saveData));

	fp = fopen("songlist.csv", "w");
	list = AllData ;
	for( i = 0 ; i < RowCount ; ++i , ++list )
	{
		// 存檔  就是重新更新一遍 
		sprintf( song_modify , "%s\n" , list->srcData );
		fwrite( song_modify , strlen( song_modify ) , 1 , fp );
	}
	
	printf("修改成功\\n");
	fclose(fp);
}

void Search_song(alldata *s, int sum)
{
	int i;
	int select = 0;
	char songname[60];
	alldata selectlist[sum];
	printf("請輸入歌名 :\n");
	scanf("\n");
	gets(songname);
	for(i = 0 ; i < sum ; i++)
	{
		if(strcmp(songname,s[i].Song) == 0)
		{
			strcpy(selectlist[select].Song, s[i].Song);
			strcpy(selectlist[select].Singer, s[i].Singer);
			selectlist[select].Time = s[i].Time;
			selectlist[select].fav=s[i].fav;
			select++;
		}
	}

	if(select == 0)
	{
		return;
	}
	else
	{
		printlist(selectlist, select);
	}
}

void Search_singersortbysong(alldata *s, int sum)
{
	int i;
	int select = 0;
	char singername[60];
	alldata selectlist[sum];
	printf("請輸入演唱者 :\n");
	scanf("\n");
	gets(singername);
	for(i = 0 ; i < sum ; i++)
	{
		if(strcmp(singername,s[i].Singer) == 0)
		{
			strcpy(selectlist[select].Song, s[i].Song);
			strcpy(selectlist[select].Singer, s[i].Singer); 
			selectlist[select].Time = s[i].Time;
			selectlist[select].fav=s[i].fav;
			select++;
		}
	}
	if(select == 0)
	{
		return;
	}
	else
	{
		quicksortsong(selectlist, 0, select-1);
		printlist(selectlist, select);
	}
}

void Search_singersortbytime(alldata *s, int sum)
{
	int i;
	int select = 0;
	char singername[60];
	alldata selectlist[sum];
	printf("請輸入演唱者 :\n");
	scanf("\n");
	gets(singername);
	for(i = 0 ; i < sum ; i++)
	{
		if(strcmp(singername,s[i].Singer) == 0)
		{
			strcpy(selectlist[select].Song, s[i].Song);
			strcpy(selectlist[select].Singer, s[i].Singer);
			selectlist[select].Time = s[i].Time;
			selectlist[select].fav=s[i].fav;
			select++;
		}
	}

	if(select == 0)
	{
		return;
	}
	else
	{
		quicksorttime(selectlist, 0, select-1);
		printlist(selectlist, select);
	}
}

void Search_timesortbysong(alldata *s, int sum)
{
	int i, t1, t2;
	int select = 0;
	char songtime[60];
	alldata selectlist[sum];
	printf("請輸入時間區段 :\n");
	scanf("%d %d", &t1, &t2);
	for(i = 0 ; i < sum ; i++)
	{
		if(s[i].Time > t1 && s[i].Time < t2)
		{
			strcpy(selectlist[select].Song, s[i].Song);
			strcpy(selectlist[select].Singer, s[i].Singer);
			selectlist[select].Time = s[i].Time;
			selectlist[select].fav=s[i].fav;
			select++;
		}
	}

	if(select == 0)
	{
		return;
	}
	else
	{
		quicksortsong(selectlist, 0, select-1);
		printlist(selectlist, select);
	}
}

void Search_timesortbysinger(alldata *s, int sum)
{
	int i, t1, t2;
	int select = 0;
	char songtime[60];
	alldata selectlist[sum];
	printf("請輸入時間區段 :\n");
	scanf("%d %d", &t1, &t2);
	for(i = 0 ; i < sum ; i++)
	{
		if(s[i].Time > t1 && s[i].Time < t2)
		{
			strcpy(selectlist[select].Song, s[i].Song);
			strcpy(selectlist[select].Singer, s[i].Singer);
			selectlist[select].Time = s[i].Time;
			selectlist[select].fav=s[i].fav;
			select++;
		}
	}

	if(select == 0)
	{
		return;
	}
	else
	{
		quicksortsinger(selectlist, 0, select-1);
		printlist(selectlist, select);
	}
}

void Randomplay(alldata *s, int sum)
{
	int min,sec;
	srand(time(0));
	int i,j,randarr[sum];
	for(i = 0 ; i < sum ; i++)
	{
		do{
			randarr[i] = rand() % sum;
			for(j = 0 ; j < i ; j++)
			{
				if(randarr[i] == randarr[j])
				{
					break;
				}
			}
		} while(j != i);
		min = s[randarr[i]].Time / 60;
		sec = s[randarr[i]].Time % 60;
		if(sec >= 10)
			printf("Song[%3d]: Name:%-30s,Singer:%-30s,Time: %d : %d,fav: %c\n", randarr[i]+1, s[randarr[i]].Song, s[randarr[i]].Singer, min ,sec , s[randarr[i]].fav);
		else
			printf("Song[%3d]: Name:%-30s,Singer:%-30s,Time: %d : 0%d,fav: %c\n", randarr[i]+1, s[randarr[i]].Song, s[randarr[i]].Singer, min ,sec , s[randarr[i]].fav);
	}
}

void quicksortsong(alldata *s, int left, int right)
{
	int i, j;
	char pivot;
	alldata t;
	if (left < right)
	{
		i = left;
		j = right + 1;
		pivot = s[left].Song[0];
		do
		{
			do { i++; } while (s[i].Song[0] < pivot);
			do { j--; } while (s[j].Song[0] > pivot);
			if (i < j)
			{
				SWAP(s[i], s[j], t);
			}
		} while (i < j);

		SWAP(s[left], s[j], t);
		quicksortsong(s, left, j - 1);
		quicksortsong(s, j + 1, right);
	}
}

void quicksortsinger(alldata *s, int left, int right)
{
	int i, j;
	char pivot[70];
	alldata t;
	if (left < right)
	{
		i = left;
		j = right + 1;
		strcpy(pivot, s[left].Singer);
		do
		{
			do { i++; } while (strcmp(s[i].Singer , pivot) == -1);
			do { j--; } while (strcmp(s[j].Singer , pivot) == 1);
			if (i < j)
			{
				SWAP(s[i], s[j], t);
			}
		} while (i < j);

		SWAP(s[left], s[j], t);
		quicksortsinger(s, left, j - 1);
		quicksortsinger(s, j + 1, right);
	}
}

void quicksorttime(alldata *s, int left, int right)
{
	int i, j, pivot;
	alldata t;
	if (left < right)
	{
		i = left;
		j = right + 1;
		pivot = s[left].Time;
		do
		{
			do { i++; } while (s[i].Time < pivot);
			do { j--; } while (s[j].Time > pivot);
			if (i < j)
			{
				SWAP(s[i], s[j], t);
			}
		} while (i < j);

		SWAP(s[left], s[j], t);
		quicksorttime(s, left, j - 1);
		quicksorttime(s, j + 1, right);
	}
}

void printlist (alldata *s, int sum)
{
	int i, min, sec;
	for( i = 0 ; i < sum ; i++)
	{
		min = s[i].Time / 60;
		sec = s[i].Time % 60;
		
		if(sec >= 10)
			printf("Song[%3d]: Name:%-30s,Singer:%-30s,Time: %d : %d, fav: %c\n", i+1, s[i].Song, s[i].Singer, min,sec,s[i].fav);
		else
			printf("Song[%3d]: Name:%-30s,Singer:%-30s,Time: %d : 0%d, fav: %c\n", i+1, s[i].Song, s[i].Singer, min, sec,s[i].fav);
	}
}

void AddFavorite(alldata *s, int sum)
{
	char song[100];
	char favo[100];
	char time[5];
	int i;
	
	alldata favolist;
	printf("請輸入要加入最愛清單的歌曲:");
	scanf("\n");
	gets(song);
	for(i = 0 ; i < sum ; i++)
	{
		if(strcmp(song, s[i].Song) == 0)
		{
			strcpy(favolist.Song, s[i].Song);
			strcpy(favolist.Singer, s[i].Singer);
			favolist.Time = s[i].Time;
			s[i].fav='Y';
		}
	}
	printf("請輸入最愛清單名:");
	scanf("%s", favo); 
	strcat(favo, ".csv");
	
	FILE *fp;	
	fp = fopen(favo, "a+");
    if(fp != NULL)
	{
		fputs(favolist.Song,fp);
		fprintf(fp,",");
		fputs(favolist.Singer,fp);
		fprintf(fp,",");
		fputs(itoa(favolist.Time, time, 10),fp);
		fprintf(fp,"\n");	
	}       
	else 
	{
		printf("File Error !\n");
	}
	fclose(fp);
	printf("%s已加入最愛清單！\n\n", song, favo);
	
}

void SearchFavorite()
{
	char favo[100];
	alldata favodata[ROW];
	printf("請輸入最愛清單名:");
	scanf("%s", favo);
	strcat(favo, ".csv");
	
	FILE *fp;
	fp = fopen(favo, "r");
    if (fp == NULL) 
	{
        fprintf(stderr, "fopen() failed.\n");
        exit(EXIT_FAILURE);
    }

    char Row[80];
    char *RowData;
    int RowCount=0,index=0;
    
    while (fgets(Row, 80, fp) != NULL)
	{
		RowData = strtok(Row, ",");
		index=0;
		while(RowData!=NULL)
		{
			if(index==0)
			{
				strcpy(favodata[RowCount].Song,RowData);
            	index++;
			}
			else if(index==1)
			{
                strcpy(favodata[RowCount].Singer,RowData);
                index++;
            }
            else if(index==2)
			{
				favodata[RowCount].Time = atoi(RowData);
			}
			RowData = strtok(NULL, ",");
		}
		RowCount++;
    }
	printlist(favodata, RowCount);
	printf("\n");
}
