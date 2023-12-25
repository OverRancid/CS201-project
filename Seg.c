#include <stdio.h>
#include <stdlib.h>

int N;  //number of elements in Segment Tree
int *segTree;   //Segment Tree array
int type;   //defines the query type of segment tree

int __gcd(int a, int b){
    //Euclidean GCD algorithm
    return b==0? a: __gcd(b, a%b);
}

int max(int a, int b){
    //boilerplate functions
    return a>b? a: b;
}

int min(int a, int b){
    //boilerplate function
    return a>b? b: a;
}

int type_updater(int val1, int val2){
    //boilerplate function
    switch(type){
    case -1:
        return max(val1, val2);
    case 0:
        return val1+val2;
    case 1:
        return __gcd(max(val1, val2), min(val1, val2));
    case 2:
        return val1 ^ val2;
    case 3:
        return val1 | val2;
    case 4:
        return val1 * val2;
    case 5:
        return val1 & val2;
    case 6:
        return min(val1, val2);
    default:
        return -1;
    }
}

int type_initializer(){
    //boilerplate function
    switch(type){
    case -1:
        return __INT16_MAX__*-1; //range max
    case 0:             //range sum
    case 1:             //range gcd
    case 2:             //range XOR
    case 3:             //range OR
        return 0;
    case 4:             //range product
        return 1;
    case 5:             //range AND
    case 6:             //range min
        return __INT16_MAX__; 
    default:            //error catching
        return -1;
    }
}

//Function to initialize segment tree
void build(int arr[], int n){
    N = __builtin_popcount(n)==1?n:1<<(32-__builtin_clz(n));
    //N = closest power of 2 greater than or equal to 'n'.
    //size of segTree[] is 2N.

    int i;


    segTree = (int*)malloc((N<<1)*sizeof(int));
    
    //Initializing the data based on input array
    segTree[0] = 0;
    for(i=0; i<N; i++){
        segTree[i+N] = i<n?arr[i]:type_initializer();
    }
    for(i=N-1; i>0; i--){
        segTree[i] = type_updater(segTree[i<<1], segTree[(i<<1)|1]);
    }
} 

//function to update a node.
void update(int idx, int val){
    idx += N;
    //information of arr[i] is stored at segTree[i+N]
    segTree[idx] += val;
    for(idx>>=1; idx; idx>>=1){
        //further that information is stored in all its parents.
        segTree[idx] = type_updater(segTree[idx<<1], segTree[(idx<<1)|1]);
    }
    //As idx is halved at every step this alorithm takes logarithmic time.
}

//Function which answers range query.
int query(int l, int r){
    int sum = type_initializer();

    //Using bottom-up DP type algorithm to calculate for query in logarithmic time.
    l+=N; r+=N;
    while(l<=r){
        if(l&1)
            sum = type_updater(sum, segTree[l++]);
        if(!(r&1))
            sum = type_updater(sum, segTree[r--]);
        l>>=1; r>>=1;
    }
    return sum;
}

void display(){
    for(int i=1; i<2*N; i++){
        printf("%d ", segTree[i]);
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
    
    printf("\nEnter the type of Segment Tree:\n-1: Range Max Query\n0: Range Sum\n1: Range GCD\n2: Range XOR\n3: Range OR\n4: Range Product\n5: Range AND\n6: RMQ\n");
    scanf("%d", &type);

    if(type_initializer()==-1){
        printf("Invalid input.\n");
        exit(0);
    }
    
    build(arr, n);

    int q;
    printf("\nEnter the number of queries (number of updates/range queries) on Segment Tree: ");
    scanf("%d", &q);

    printf("\nEnter %d queries with the following format:\n1. Point Update:'+ index value'\n2. Range Query:'? l r'\n\nNote: indexing is 0 based and ranges are closed on both ends.\n", q);

    fflush(stdout);
    while(q--){
        char t;
        scanf(" %c ", &t);
        if(t=='+'){
            int idx, val;
            scanf("%d %d", &idx, &val);
            update(idx, val);
        }
        else if(t=='?'){
            int l, r;
            scanf("%d %d", &l, &r);
            printf("%d\n", query(l, r));
        }
    }

    printf("Execution Complete.\nFinal Segment Tree after all queries:\n");
    display();

    return 0;
}