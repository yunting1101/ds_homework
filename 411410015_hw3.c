#include<stdio.h>
#include<stdlib.h>
int node = 0, link = 0, timeSlots = 0, req = 0;//timeSlot是total高
int totalreq = 0;//被接受的路徑數量
int graph[1000][1000]={0};//建圖
int memory_table[1000][1000]={0};//記所有記憶體的表
int print_table[1000][1000]={0};//輸出用的表

void print(){
    printf("%d\n", totalreq);//輸出接受的數量
    for(int i = 0 ; i < totalreq ; i++){
        int now = print_table[i][1];
        printf("%d ",print_table[i][0]);//輸出接受的req名稱
        for(int j = 2 ; j < now+2 ; j++){//輸出最短路徑
            printf("%d ",print_table[i][j]);
        }
        printf("\n");
        
        if(now == 2){
            printf("%d %d %d\n",print_table[i][2], print_table[i][3], 2);
        }
        else if(now > 2){//依照每次增加一個連結並swap一次的complete binary tree 輸出
            printf("%d %d %d\n",print_table[i][2], print_table[i][3], 2);
            printf("%d %d %d\n",print_table[i][3], print_table[i][4], 2);
            for(int j = 2 ; j < now-1 ; j++){
                printf("%d %d %d\n",print_table[i][j+2], print_table[i][j+3], j+1);
                printf("%d %d %d %d %d %d %d\n", print_table[i][2], print_table[i][j+2], print_table[i][2], print_table[i][j+1], print_table[i][j+1], print_table[i][j+2], j+1);
            }
            printf("%d %d %d %d %d %d %d\n", print_table[i][2], print_table[i][now+1], print_table[i][2], print_table[i][now], print_table[i][now], print_table[i][now+1], now);
        }
    }
}

void memory_used(int time, int *shortpath, int level, int reqID){//確認記憶體的使用情況
    int check_memory = 0;//確認是否可以接受
    int now_time = time;
    if(level == 2){//若最短路徑是2個node的情況
        if(memory_table[time][shortpath[0]]-1 < 0) check_memory = 1;
        if(memory_table[time][shortpath[1]]-1 < 0) check_memory = 1;
        if(memory_table[time+1][shortpath[0]]-1 < 0) check_memory = 1;
        if(memory_table[time+1][shortpath[1]]-1 < 0) check_memory = 1;
    }
    else{
        if(memory_table[time][shortpath[0]]-1 < 0) check_memory = 1;//確認扣掉time=0的記憶體
        if(memory_table[time][shortpath[1]]-2 < 0) check_memory = 1;
        if(memory_table[time][shortpath[2]]-1 < 0) check_memory = 1;
        now_time++;
        if(level > 3){//若是大於三個node則中間會以 1 2 2 1 的方式扣掉記憶體
            for(int i = 1 ; i < level ; i++){//確認是否可以扣除
                if(memory_table[now_time][shortpath[0]]-1 < 0){
                    check_memory = 1;
                    break;
                }
                if(memory_table[now_time][shortpath[i]]-2 < 0){
                    check_memory = 1;
                    break;
                }
                if(memory_table[now_time][shortpath[i+1]]-2 < 0){
                    check_memory = 1;
                    break;
                }
                if(memory_table[now_time][shortpath[i+2]]-1 < 0){
                    check_memory = 1;
                    break;
                }
                now_time++;
                if(now_time == level-2) break;
            }
        }
        if(memory_table[now_time][shortpath[0]]-1 < 0) check_memory = 1;//確認是否扣除最後兩行記憶體
        if(memory_table[now_time][shortpath[level-2]]-2 < 0) check_memory = 1;
        if(memory_table[now_time][shortpath[level-1]]-1 < 0) check_memory = 1;
        if(memory_table[now_time+1][shortpath[0]]-1 < 0) check_memory = 1;
        if(memory_table[now_time+1][shortpath[level-1]]-1 < 0) check_memory = 1;
    }

    if(check_memory == 0){//若確認可以減去，在表上減去相對應的記憶體
        int now_time = time;
        if(level == 2){
        memory_table[time][shortpath[0]]-=1;
        memory_table[time][shortpath[1]]-=1;
        memory_table[time+1][shortpath[0]]-=1;
        memory_table[time+1][shortpath[1]]-=1;
        }
        else{
            
            memory_table[time][shortpath[0]]-=1;
            memory_table[time][shortpath[1]]-=2;
            memory_table[time][shortpath[2]]-=1;
            now_time++;
            if(level > 3){
                for(int i = 1 ; i < level ; i++){
                    memory_table[now_time][shortpath[0]]-=1;
                    memory_table[now_time][shortpath[i]]-=2;
                    memory_table[now_time][shortpath[i+1]]-=2;
                    memory_table[now_time][shortpath[i+2]]-=1;
                    now_time++;
                    if(now_time == level-2) break;
                }
            }
            memory_table[now_time][shortpath[0]]-=1;
            memory_table[now_time][shortpath[level-2]]-=2;
            memory_table[now_time][shortpath[level-1]]-=1;
            memory_table[now_time+1][shortpath[0]]-=1;
            memory_table[now_time+1][shortpath[level-1]]-=1;
        }
        for(int i = 0 ; i < level+2 ; i++){//將接收的req存入輸出的表
            if(i == 0){
                print_table[totalreq][i] = reqID;
            }
            else if(i == 1){
                print_table[totalreq][i] = level;
            }
            else{
                print_table[totalreq][i] = shortpath[i-2];                
            }

        }
        totalreq++;//接收的req總數增加
    }
}

void path(int reqID, int dst, int level, int* pre){//存路徑在shortpath的array中
    int *shortpath = (int*)malloc(sizeof(int)*level);
    shortpath[level-1] = dst;
    int now = dst;
    for(int i = level-2 ; i >= 0  ; i--){//每次去尋找前一個點
        shortpath[i] = pre[now];
        now = pre[now];
    }
    if(level <= timeSlots){//若層數建出來的樹高小於等於最大高度才能使用記憶體
        int time = 0;//樹底在記憶體表格的位置
        memory_used(time, shortpath, level, reqID);
    }
}

void short_path_bfs(int reqID, int src, int dst){
    int *level = (int*)calloc(node,sizeof(int));//建立三個array紀錄 現在的層數 前一個node 是否走訪過
    int *pre = (int*)calloc(node,sizeof(int));
    int *check = (int*)calloc(node,sizeof(int));
    int now_level = 1;
    int *queue = (int*)calloc(node*node,sizeof(int));//建queue
    int rear = -1, front = -1;
    queue[++rear] = src;//輸入起點進queue
    level[src] = now_level;
    check[src] = 1;
    pre[src] = 1;
    int q_length = rear - front;
    while(q_length > 0){//若queue中有值
        now_level++;
        for(int i = 0 ; i < q_length ; i++){//跑queue長度的所有點
            for(int j = 0 ; j < node ; j++){//跑圖上所有點確認是否可走
                if(queue[front+1] == dst){//若抵達終點 
                    path(reqID, dst, now_level-1, pre);
                    i = q_length + 1;
                    break;
                }
                if(graph[queue[front+1]][j] == 1){//若找到可走的下一個點
                    if(check[j] == 0){//確認沒被走訪過
                        check[j] = 1;
                        pre[j] = queue[front + 1];
                        level[j] = now_level;
                        queue[++rear] = j;
                    }
                }
            }
            ++front;
        }
        q_length = rear - front;
    }
}

int main(){
    scanf("%d %d %d %d", &node, &link, &timeSlots, &req);
    for(int i = 0 ; i < node ; i++){
        int nodeID = 0, qMemory = 0;
        scanf("%d %d", &nodeID, &qMemory);
        for(int j = 0 ; j < timeSlots ; j++){//建記憶體的表
            memory_table[j][nodeID] = qMemory;
        } 
    }
    for(int i = 0 ; i < link ; i++){
        int linkID = 0, End1 = 0, End2 = 0;
        scanf("%d %d %d", &linkID, &End1, &End2);
        graph[End1][End2] = 1;//建圖
        graph[End2][End1] = 1;
    }
    for(int i = 0 ; i < req ; i++){//輸入req
        int reqID = 0, reqSrc = 0, reqDst = 0;
        scanf("%d %d %d", &reqID, &reqSrc, &reqDst);
        short_path_bfs(reqID, reqSrc, reqDst);
    }
    print();//輸出
    return 0;
}
