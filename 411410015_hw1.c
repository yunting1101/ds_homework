#include<stdio.h>
#include<stdlib.h>

int dst1_x = 0, dst1_y = 0, dst2_x = 0, dst2_y = 0;//終點
int src1_x = 0, src1_y = 0, src2_x = 0, src2_y = 0;//起點
int level = 2;//層數
int maze_edge = 0;//地圖邊長
int dx[4] = { 0, 1, 0, -1 };//上右下左
int dy[4] = { 1, 0, -1, 0 };
int tempX = 0, tempY = 0;//暫存第一顆球的位置
int temp2X = 0, temp2Y = 0;//在bfs2暫存移動球的位置
int temp_moveX = 0, temp_moveY = 0;//暫存跟著移動的球的位置

typedef struct maze_queue{//每次走訪的佇列節點
    int x_axis;//第一顆球的座標
    int y_axis;
    int moveX_axis;//跟隨球的座標
    int moveY_axis;
    struct maze_queue *next;
    struct maze_queue *prev;
}M_queue;

int is_empty(M_queue *rear, M_queue *front){//佇列為空
    if(rear->next == front){
        return 1 ;
    }
    return 0 ;
}

int queue_size(M_queue *rear,M_queue *front){//佇列大小
    int count = 0;
    M_queue *search = rear->next;
    while(search != front){
        count++;
        search = search->next;
    }
    return count;
}

void enqueue_M(M_queue *rear,int X,int Y,int moveX,int moveY){//加入可走的格子進佇列
    M_queue *new=(M_queue *)malloc(sizeof(M_queue));
    new->x_axis = X;//存入該點座標
    new->y_axis = Y;
    new->moveX_axis = moveX;//存入跟隨點座標
    new->moveY_axis = moveY;
    new->next = rear->next;
    rear->next = new;
    new->prev = rear;
    new->next->prev=new;
}

void dequeue_M(M_queue *front){//刪除走過的格子的佇列
    M_queue *remove = front->prev;
    front->prev = remove->prev;
    front->prev->next = front;
    temp_moveX = remove->moveX_axis;
    temp_moveY = remove->moveY_axis;
    free(remove);
}

void path(int **maze,int x,int y,int level){//紀錄路徑，傳入紀錄level的matrix
    int *toltal_step = (int*)malloc(sizeof(int)*(maze_edge-2)*(maze_edge-2)*2);//建立一個陣列紀錄方向
    int length = 0;
    while(maze[y][x] != 0){

        if(maze[y][x-1] == level-1){//若四個方向中有位置等於最層數減1，就存入方向數值至陣列，並更新點至該位置
            toltal_step[length] = 1;
            x = x-1;
        }   
        else if(maze[y][x+1] == level-1){
            toltal_step[length] = 3;
            x = x+1;
        }    
        else if(maze[y-1][x] == level-1){
            toltal_step[length] = 0;
            y = y-1;
        }    
        else if(maze[y+1][x] == level-1){
            toltal_step[length] = 2;
            y = y+1;
        }    
        else{
            break;
        }
        length++;
        level--;
    }
    for(int i = length-1 ; i >= 0 ; i--){//反向輸出存入的方向數值
        printf("%d",toltal_step[i]);
    }
}

void bfs_src2(M_queue *rear,M_queue *front,int **maze,int src2X,int src2Y){//第二顆球的bfs
    while(is_empty(rear,front)==0){//清空佇列
        dequeue_M(front);
    }
    level = 2;//層數重新設為2
    int **mazeVisit = (int**)calloc(maze_edge,sizeof(int*));//建一個新matrix，記層數及走訪
    for(int i = 0 ; i < maze_edge ; i++){
        mazeVisit[i] = (int*)calloc(maze_edge,sizeof(int));
    }

    enqueue_M(rear,src2X,src2Y,src2X,src2Y);//將原點加入佇列
    mazeVisit[src2Y][src2X] = level;//固定該點層數
    temp2X = src2X;
    temp2Y = src2Y;
    int queue_length = queue_size(rear,front);
    while(is_empty(rear,front) == 0){ //若佇列不為空，跑整個佇列
        level++;//層數增加
        for(int i = 0 ; i < queue_length ; i++){//跑佇列節點數量的次數
            temp2X = front->prev->x_axis;//存目前值進入temp
            temp2Y = front->prev->y_axis;
            for(int j = 0 ; j < 4 ; j++){//跑四個方向
                int newX = temp2X+dx[j];
                int newY = temp2Y+dy[j];
                if(newY > 0 && newY < maze_edge-1 && newX > 0 && newX < maze_edge-1){//若點在地圖範圍內
                    if(mazeVisit[newY][newX] == 0 && maze[newY][newX] == 0){//若此點可以走訪
                        mazeVisit[newY][newX] = level;//新matrix的點設為層數
                        if((newX == dst1_x && newY == dst1_y)||(newX == dst2_x && newY == dst2_y)){//如果到達終點
                            if(newX == dst1_x && newY == dst1_y){//抵達終點1
                                path(mazeVisit,dst1_x,dst1_y,level);
                                dst1_x = 0;//碰到的終點消失
                                dst1_y = 0;
                            }
                            else{//抵達終點2
                                path(mazeVisit,dst2_x,dst2_y,level);
                                dst2_x = 0;
                                dst2_y = 0;
                            }
                            return;
                        }
                        else{//如果能走，但未達終點
                            enqueue_M(rear,newX,newY,newX,newY);
                            
                        }
                    }
                }
            }
            dequeue_M(front);
            
        }
        queue_length = queue_size(rear,front);//更新佇列長度
    }
}

void bfs_src1(M_queue *rear,M_queue *front,int **maze,int srcX,int srcY,int moveX,int moveY){//第一顆球的bfs
    while(rear->next != front){//清空佇列
        dequeue_M(front);
    }
    int **mazeVisit = (int**)calloc(maze_edge,sizeof(int*));//建一個新matrix，記層數及走訪
    for(int i = 0 ; i < maze_edge ; i++){
        mazeVisit[i] = (int*)calloc(maze_edge,sizeof(int));
    }

    enqueue_M(rear,srcX,srcY,moveX,moveY);//將原點加入佇列
    mazeVisit[srcY][srcX] = level;//固定該點層數
    tempX = srcX;
    tempY = srcY;
    int queue_length = queue_size(rear,front);
    while(is_empty(rear,front) == 0){//若佇列不為空，跑整個佇列
        level++;//層數增加
        for(int i = 0 ; i < queue_length ; i++){//跑佇列節點數量的次數
            tempX = front->prev->x_axis;//存目前值進入temp
            tempY = front->prev->y_axis;
            moveX = front->prev->moveX_axis;
            moveY = front->prev->moveY_axis;
            for(int j = 0 ; j < 4 ; j++){//跑四個方向
                int newX = tempX+dx[j];
                int newY = tempY+dy[j];
                temp_moveX = moveX+dx[j];
                temp_moveY = moveY+dy[j];
                int check_move = 0;//確認跟隨球是否移動
                if(newY > 0 && newY < maze_edge-1 && newX > 0 && newX < maze_edge-1){//若點在地圖範圍內
                    if(!mazeVisit[newY][newX] && maze[newY][newX] == 0){//若此點可以走訪
                        if(maze[temp_moveY][temp_moveX] == 0){//若跟隨球可以跟著移動
                            if((temp_moveX == dst1_x && temp_moveY == dst1_y)||(temp_moveX == dst2_x && temp_moveY == dst2_y)){//若跟隨球先抵達終點  
                                bfs_src1(rear,front,maze,src2_x, src2_y,src1_x,src1_y);//以跟隨球原位置為原點重新搜
                            } 
                            else{
                                check_move = 1;
                            }
                        }
                        if((newX == dst1_x && newY == dst1_y)||(newX == dst2_x && newY == dst2_y)){//如果到達終點
                            if(check_move == 1){//若移動則更新位置
                                moveX = temp_moveX;
                                moveY = temp_moveY; 
                            }
                            mazeVisit[newY][newX] = level;
                            if(newX == dst1_x && newY == dst1_y){//抵達終點1
                                path(mazeVisit,dst1_x,dst1_y,level);
                                dst1_x = 0;//碰到的終點消失
                                dst1_y = 0;
                                
                            }
                            else{//抵達終點2
                                path(mazeVisit,dst2_x,dst2_y,level);
                                dst2_x = 0;
                                dst2_y = 0;
                            }
                            bfs_src2(rear,front,maze,moveX,moveY);//bfs第二個球
                        }
                        else{//如果能走，但未達終點
                            if(check_move == 1){//若移動則傳入更新後的位置
                                enqueue_M(rear,newX,newY,temp_moveX,temp_moveY);
                            }
                            else{
                                enqueue_M(rear,newX,newY,moveX,moveY);
                            }
                            mazeVisit[newY][newX] = level;//新matrix的點設為層數
                        }
                    }
                }
            }
            dequeue_M(front);
        }
        queue_length = queue_size(rear,front);//更新佇列長度
    }
}

int main(){
    scanf("%d",&maze_edge);//迷宮長寬
    int **maze_arr = (int**)malloc(sizeof(int*)*maze_edge);
    for(int i = 0 ; i < maze_edge ; i++){
        maze_arr[i] = (int*)malloc(sizeof(int)*maze_edge);
    }
    for(int i = maze_edge-1 ; i >= 0 ; i--){//存入迷宮，y值隨陣列增加
        for(int j = 0 ; j < maze_edge ; j++){
            scanf("%d",&maze_arr[i][j]);
        }
    }
    
    //輸入兩個原點及兩個終點
    scanf("%d %d",&src1_x,&src1_y);
    scanf("%d %d",&src2_x,&src2_y);
    scanf("%d %d",&dst1_x,&dst1_y);
    scanf("%d %d",&dst2_x,&dst2_y);

    M_queue *rear = (M_queue *)malloc(sizeof(M_queue));//建立地圖上每一格的佇列
    M_queue *front = (M_queue *)malloc(sizeof(M_queue));
    rear->next = front;
    front->prev = rear;
    bfs_src1(rear,front,maze_arr,src1_x, src1_y,src2_x,src2_y);
    return 0 ;
}