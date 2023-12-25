#include <stdio.h>    
#include <stdlib.h>

int N;  //number of elements in Segment Tree
int *segTree;
int *lazy;

void build(int arr[], int n){
    N = __builtin_popcount(n)==1?n:1<<(32-__builtin_clz(n));
    int i;
    segTree = (int*)malloc((N<<1)*sizeof(int));
    lazy = (int*)malloc((N<<1)*sizeof(int));
    segTree[0] = 0;
    for(i=0; i<N; i++){
        segTree[i+N] = i<n?arr[i]:0;
        lazy[i] = lazy[i+N] = 0;
    }
    for(i=N-1; i>0; i--){
        segTree[i] = segTree[i<<1] + segTree[(i<<1)^1];
    }
} 

void update(int l, int r, int value, int first, int last, int idx){
    if(lazy[idx]){
        segTree[idx] += (last-first+1)*lazy[idx];
        if(last!=first){
            lazy[idx<<1] += value;
            lazy[(idx<<1)|1] += value;
        }
        lazy[idx] = 0;
    }
    if(last<l || r<first || last<first){
        return;
    }
 
    if(l<=first && last<=r){
        segTree[idx] += (last-first+1)*value;
        if(last!=first){
            lazy[idx<<1] += value;
            lazy[(idx<<1)|1] += value;
        }
        return;
    }
 
    int mid = (first+last)/2;
    update(l, r, value, first, mid, idx<<1);
    update(l, r , value, mid+1, last, (idx<<1)|1);
    segTree[idx] = segTree[idx<<1] + segTree[(idx<<1)|1]; 
}

void lazyUpdate(int l, int r, int value){
    update(l, r, value, 0, N-1, 1);
}

int query(int l, int r, int first, int last, int idx){
    if(lazy[idx]){
        segTree[idx] += (last-first+1)*lazy[idx];
        if(last!=first){
            lazy[idx<<1] += lazy[idx];
            lazy[(idx<<1)|1] += lazy[idx];
        }
        lazy[idx] = 0;
    }
    if(last<l || r<first || last<first){
        return 0;
    }
    if(l<=first && last<=r){
        return segTree[idx];
    }
    int mid = (first+last)/2;
    return query(l, r, first, mid, idx<<1) + query(l, r, mid+1, last, (idx<<1)|1);
}

int lazyQuery(int l, int r){
    return query(l, r, 0, N-1, 1);
}

void display(){
    printf("Segment Tree: ");
    for(int i=1; i<(N<<1); i++){
        printf("%d  ", segTree[i]);
    }
    printf("\n");
    printf("Lazy Tree: ");
    for(int i=1; i<(N<<1); i++){
        printf("%d  ", lazy[i]);
    }
    printf("\n");
}

int main(){
    int *arr, n, i;
    printf("Enter number of elements in array: ");
    scanf("%d", &n);
    printf("\n");

    arr = (int*)malloc(n*sizeof(int));
    printf("Enter %d elements to initialze array\n", n);
    for(i=0; i<n; i++){
        scanf("%d", &arr[i]);
    }
    
    build(arr, n);

    int q;
    printf("\nEnter the number of queries (number of updates/range queries) on Lazy Segment Tree: ");
    scanf("%d", &q);

    printf("\nEnter %d queries with the following format:\n1. Range Update:'+ l r value'\n2. Range Query:'? l r'\n\nNote: indexing is 0 based and ranges are closed on both ends.\n", q);

    fflush(stdout);
    while(q--){
        char t;
        scanf("%c ", &t);
        if(t=='+'){
            int l, r, val;
            scanf("%d%d%d", &l, &r, &val);
            lazyUpdate(l, r, val);
        }
        else if(t=='?'){
            int l, r;
            scanf("%d %d", &l, &r);
            printf("%d\n", lazyQuery(l, r));
        }
    }

    printf("Execution Complete.\nFinal Segment Tree after all queries:\n");
    display();    
    return 0;
}