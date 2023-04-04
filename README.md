# 音樂播放系統 - Songlist in C
## 1.系統綱要與介紹

此為學校資料結構結業作業，旨在設計出一套簡略的歌曲撥放系統。
預設歌單為透過csv檔建立，可參考附件 : *songlist.csv*
* 以下為系統功能 :
	* 歌曲資訊
	* 功能操作
	* 查詢
	* 隨機播放
	* 加入喜愛播放清單
	* 查詢最愛歌曲清單


## 2.條件說明

1. 歌曲資訊需有（歌名、歌手、歌曲長度(單位：分秒)、我的最愛）   EX：Dynamite BTS 3:44 Y
2. 須從 CSV 檔中讀入歌曲資料，以利後續修改。
3. 結束程式後須寫入新的個人設定檔


## 3.功能說明
### 新增
* 輸入所要新增之歌曲資訊，並在重新查詢歌單時需顯示新增後結果。EX：請選擇功能  Add


1. 輸入要新增的歌曲資訊(演唱者、歌名、歌曲長度) : 
2. Butter BTS 2:10
3. 新增成功

### 刪除
* 刪除指定歌曲資訊，並在重新查詢歌單時為刪除後結果。EX：請選擇功能 Delete 
    1. 輸入要刪除的歌名或演唱者(該演唱者所有歌曲皆刪除)
    2. 刪除成功

### 修改
* 輸入要修改之歌曲資訊，並在重新查詢歌單時須為修改後結果。EX：請選擇功能 Modify 

    1. 輸入要修改的歌名：
    2. XXXX
    3. 修改內容
    4. Butter BTS 2:10
    5. 修改成功
    
    
### 查詢
* 從歌曲清單中，進行多種查詢(由歌手、歌名與時間區段做條件)，並排序後顯示結果。


Ex：請選擇功能 : Search 
    1. 請輸入演唱者 : 
    2. BTS 
    3. BTS 的歌曲有 :
    
| **no** | **Song** | **Singer** | **Time** |
|-------|:-----:|------:|------:|
| 1   |   Butter   |  BTS | 4:10 |
| 2   |  Dynamite  |  BTS | 3:57 |
| 3   |     ON     |  BTS | 3:59 |
### 加入喜愛播放清單
* 查詢特定歌曲後加入我的最愛清單。如：創立 A 清單、B 清單(此處有預設的 **0628.csv歌單**)

### 查詢最愛歌曲清單
* 查詢我的最愛清單：查詢特定最愛清單(可讀入 csv)
EX：請輸入功能： Search Favorite
    1. 請輸入清單名：
    2. Eric
    3. Eric 的最愛清單有：
    
| **no** | **Song** | **Singer** | **Time** |
|-------|:-----:|------:|------:|
| 1   |   Butter   |  BTS | 4:10 |
| 2   |  Dynamite  |  BTS | 3:57 |
| 3   |     ON     |  BTS | 3:59 |
