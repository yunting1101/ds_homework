#include<stdio.h>
#include<stdlib.h>
int node_num=0,toltal_step=0;
int old_path[1000000]={0}, new_path[1000000]={0};

typedef struct node{
    int next_node;//下一個節點位置
    int length;//舊順序中的節點相差距離
    int check_update;//是否更新
}Node;
Node update_arr[1000000];

void print(int **table){//輸出步數及更新過程
    int check_delete = 0;
    for(int i = 0 ; i < node_num ; i++){//確認最後一次更新是否與new相同(沒有更新後不被用到的點)
        if(table[toltal_step-1][i] == new_path[i]){
            check_delete++;
        }
    }
    if(check_delete == node_num){//若每一個都與new相同，則總步數減一
        toltal_step--;
    }
    printf("%d\n",toltal_step+2);
    for(int i = 0 ; i < node_num ; i++){
        printf("%d ", old_path[i]);
    }
    printf("\n");
    for(int i = 0 ; i < toltal_step ; i++){
        for(int j = 0 ; j < node_num ; j++){
            printf("%d ",table[i][j]);
        }
        printf("\n");
    }
    for(int i = 0 ; i < node_num ; i++){
        printf("%d ", new_path[i]);
    }
}

void update(int **table, int length){
    int not_update = 0;//確認未更新的節點數
    while(not_update != 1){
        not_update = 0;
        int max = 0;//長度的最大值
        int now_len = 0;//現節點的長度
        int next_len = 0;//下一節點的長度
        int now = 0;//現節點位置
        int next = 0;//下一節點位置
        for(int i = 1 ; i < length ; i++){
            for(int j = 0 ; j < node_num ; j++){//跑每一個有長度的節點
                if(i == update_arr[j].length && update_arr[j].check_update == 0){//若與長度相符且未更新
                    now = j;
                    next = new_path[now];
                    while(update_arr[next].check_update != 0){//若已更新就跳過
                        next = new_path[next];
                    }
                    if(update_arr[next].length-update_arr[now].length > max){//更新兩長度的差為最大值
                        max = update_arr[next].length-update_arr[now].length;
                        now_len = update_arr[now].length;
                        next_len = update_arr[next].length;
                    }

                }
            }
        }
        for(int i=0 ; i < node_num ; i++){//將now設為與下一節點長度差為最大值的節點
            if(update_arr[i].length == now_len){
                now = i;
            }
        }
        update_arr[now].next_node = new_path[now];//更新該點
        update_arr[now].check_update = 1;
        for(int i = 0 ; i < node_num ; i++){//將新步驟輸入table
            table[toltal_step][i] = update_arr[i].next_node;
        }
        toltal_step++;//總步數增加
        
        if(next_len-now_len > 1){
            for(int i = 0 ; i < node_num ; i++){
                if(update_arr[i].length < next_len && update_arr[i].length > now_len && update_arr[i].check_update == 0){//若有節點的長度在更新節點及下一節點的長度之間則可同時更新
                    update_arr[i].next_node = new_path[i];
                    update_arr[i].check_update = 1;
                }
            }
            for(int i = 0 ; i < node_num ; i++){//將新步驟輸入table
                table[toltal_step][i] = update_arr[i].next_node;
            }
            toltal_step++;//總步數增加
        }
        for(int i = 0 ; i < node_num ; i++){//確認有多少點未更新
            if(update_arr[i].check_update == 0 && new_path[i] != -1){
                not_update++;
            }
        }
    }
    for(int i = 0 ; i < node_num ; i++){//更新最後一個點
        if(update_arr[i].check_update == 0 && new_path[i] != -1){
            update_arr[i].next_node = new_path[i];
        }
    }
    for(int i = 0 ; i < node_num ; i++){//將新步驟輸入table
        table[toltal_step][i] = update_arr[i].next_node;
    }
    toltal_step++;//總步數增加
    print(table);
}

int main(){
    scanf("%d",&node_num);
    for(int i = 0 ; i < node_num ; i++){
        scanf("%d",&old_path[i]);
    }
    for(int i = 0 ; i < node_num ; i++){
        scanf("%d",&new_path[i]);
    }

    int **table = (int**)calloc(node_num,sizeof(int*));//建立所有更新步驟的表
    for(int i = 0 ; i < node_num ; i++){
        table[i] = (int*)calloc(node_num,sizeof(int));
    }
    for(int i = 0 ; i < node_num ; i++){//若在舊的順序中為-1，優先更新成新的順序
        if(old_path[i] == -1){
            table[0][i] = new_path[i];
            update_arr[i].next_node = new_path[i];
            update_arr[i].check_update = 1;
        }
        else{
            table[0][i] = old_path[i];
        }
    }
    toltal_step++;//步驟總數加一
    int now = 0;
    int length = 1;
    while(now != node_num-1){//更新舊順序中的長度
        if(new_path[now] != -1){//重複節點才會更新
            update_arr[now].next_node = old_path[now];
            update_arr[now].length = length;
            length++;
        }
        now = old_path[now];
    }
    if(now == node_num-1){
        update_arr[now].length = length;
    }
    for(int i=0; i<node_num; i++){//舊的值放入struct
        if(update_arr[i].next_node == 0){
            update_arr[i].next_node = old_path[i];
        }
    }
    update(table, length);
}