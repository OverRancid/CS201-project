#include <stdio.h>
#include <stdlib.h>

int N;  //number of elements in Segment Tree
unsigned long long *segTree;    //Segment Tree array
unsigned long long *lazy;       //lazy array for Lazy Implimentation

//function that initialized segTree[] and lazy[]
void build(unsigned long long arr[], int n){
    N = __builtin_popcount(n)==1?n:1<<(32-__builtin_clz(n));
    //N = closest power of 2 greater than or equal to 'n'.
    //size of segTree[] and lazy[] is 2N.
    
    int i;

    segTree = (unsigned long long*)malloc((N<<1)*sizeof(unsigned long long));
    lazy = (unsigned long long*)malloc((N<<1)*sizeof(unsigned long long));
    if(segTree==NULL||lazy==NULL){
        printf("malloc failed\n");
        exit(0);
    }
    
    //Initializing the data based on input array
    segTree[0] = 0;
    for(i=0; i<N; i++){
        segTree[i+N] = i<n?arr[i]:0;
        lazy[i] = lazy[i+N] = 0;
    }
    for(i=N-1; i>0; i--){
        segTree[i] = segTree[i<<1] | segTree[(i<<1)^1];
    }
} 

//function to update the value in range of nodes using lazy implimentation.
/*
    when the function is called,
    data of segTree[idx] = data of (arr[first] + arr[first+1] + ... + arr[last])
*/
void update(int l, int r, unsigned long long value, int first, int last, int idx){
    //Propogating lazy knowledge as we come accross it
    //This is an O(1) operation which happens armotized O(logN) times.
    if(lazy[idx]){
        segTree[idx] |= lazy[idx];
        if(last!=first){
            lazy[idx<<1] |= value;
            lazy[(idx<<1)|1] |= value;
        }
        lazy[idx] = 0;
    }

    //If [l, r] and [first, last] are disjoint, no operations need to be performed.
    if(last<l || r<first || last<first){
        return;
    }
 
    //If [l, r] is contains [first, last] , entire segTree[idx] must be updated. 
    if(l<=first && last<=r){
        segTree[idx] |= value;
        if(last!=first){
            lazy[idx<<1] |= value;
            lazy[(idx<<1)|1] |= value;
        }
        return;
    }
 
    //Traversing the tree in a Top-down fashion and only updating those values which are needed.
    int mid = (first+last)/2;
    update(l, r, value, first, mid, idx<<1);
    update(l, r , value, mid+1, last, (idx<<1)|1);

    //Updating node value after all updates on children.
    segTree[idx] = segTree[idx<<1] | segTree[(idx<<1)|1]; 
}

//Helper function which calls the actual update function.
void lazyUpdate(int l, int r, unsigned long long value){
    update(l, r, value, 0, N-1, 1);
}

//Function which answers range query.
unsigned long long query(int l, int r, int first, int last, int idx){
    //Propogating lazy knowledge as we come accross it
    if(lazy[idx]){
        segTree[idx] |= lazy[idx];
        if(last!=first){
            lazy[idx<<1] |= lazy[idx];
            lazy[(idx<<1)|1] |= lazy[idx];
        }
        lazy[idx] = 0;
    }

    //If [l, r] and [first, last] are disjoint, no operations need to be performed.
    if(last<l || r<first || last<first){
        return 0;
    }

    //If [l, r] is contains [first, last] , entire segTree[idx] information is of value. 
    if(l<=first && last<=r){
        return segTree[idx];
    }

    //Traversing the tree in a Top-down fashion and accumulating all data.
    int mid = (first+last)/2;
    return query(l, r, first, mid, idx<<1) | query(l, r, mid+1, last, (idx<<1)|1);
}

//Helper function that calls query function.
unsigned long long lazyQuery(int l, int r){
    return query(l, r, 0, N-1, 1);
}

//Function that solves the geometry task given.
void countIntersectingLines(){
    int q, T;
    printf("Enter number of line segments: ");
    scanf("%d", &T);
    printf("Enter %d line segments [l, r] such that 0<=l<=r<=100\n", T);
    for(int i=0; i<T; i++){
        int l, r;
        scanf("%d %d", &l, &r);
        lazyUpdate(l, r, 1<<i);
        //Each binary digit keeps information about one line.
    }
    printf("Enter number of query line segments: ");
    scanf("%d", &q);
    printf("Enter %d line segments [l, r] such that 0<=l<=r<=100\n", T);
    for(int i=0; i<q; i++){
        int l, r;
        scanf("%d %d", &l, &r);
        printf("Number of lines passing through given line is: %d\n",__builtin_popcountll(lazyQuery(l, r)));
    }
}
/*
Possible Optimizations: 
This can be implemented using array of integers to increase the number of fixed line segments from 64 to ~1e5
The implementation of this is not very tough but is out of scope of this project.

Even just using this implementation. If 2 lines are disjoint, they can be represented by a common binary digit.
This optimization, in the best case can handle about 5e4 line segments.
This can be implemented in the following way:
    After taking the line segment, treat it as a query line segment.
    Obtain a bit which is off throughout [l, r] - this is done in O(logN)
    Assign this bit it represent the line segment. 
*/

int main(){
    int n = 100;
    unsigned long long arr[n];
    for(int i=0; i<n; i++){
        arr[i] = 0LL;
    }
    build(arr, n);

    countIntersectingLines();

    return 0;
}