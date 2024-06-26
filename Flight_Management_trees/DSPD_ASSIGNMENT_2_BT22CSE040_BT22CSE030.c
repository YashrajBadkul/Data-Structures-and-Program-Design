#include<stdio.h>
#include<stdlib.h>

static int id = 1;

typedef enum{FALSE, TRUE} bool;

typedef struct time_tag{
    int hr;
    int min;
}TIME;

typedef struct flight_plan_tag{
    int flight_ID;
    TIME* arr_time;
    TIME* dept_time;
    struct flight_plan_tag* left;
    struct flight_plan_tag* right;
    int height;
}flight_plan;

typedef struct bucket_tag{
    int bucketID;
    TIME* start_ETA;
    TIME* end_ETA;
    flight_plan* flightList;
    struct bucket_tag* left;
    struct bucket_tag* right;
    int height;
}bucket;

int maxTime(TIME* a, TIME* b){
    int retVal = 0;
    if(a -> hr > b -> hr)
        retVal = -1;
    else if(b -> hr > a -> hr)
        retVal = 1;
    else if(a -> min > b -> min)
        retVal = -1;
    else if(b -> min > a -> min)
        retVal = 1;

    return retVal;
}

int time_diff(TIME* a, TIME* b){
    //returns a - b
    int diff = 0;
    diff = a -> min - b -> min;
    diff += (a -> hr - b -> hr) * 60;

    return diff;
}

int max(int a, int b){
    int retVal = b;
    if(a > b){
        retVal = a;
    }

    return retVal;
}

int height_bucket(bucket* root){
    if(root == NULL){
        return 0;
    }
    return root -> height;
}

int height_flight_plan(flight_plan* root){
    if(root == NULL){
        return 0;
    }
    return root -> height;
}

bucket* rightRotateBucket(bucket* root){
    bucket* leftNode = root -> left;
    root -> left = leftNode -> right;
    leftNode -> right = root;

    root -> height = max(height_bucket(root -> left), height_bucket(root -> right)) + 1;
    leftNode -> height = max(height_bucket(leftNode -> left), height_bucket(leftNode -> right)) + 1;

    return leftNode;
}

bucket* leftRotateBucket(bucket* root){
    bucket* rightNode = root -> right;
    root -> right = rightNode -> left;
    rightNode -> left = root;

    root -> height = max(height_bucket(root -> left), height_bucket(root -> right)) + 1;
    rightNode -> height = max(height_bucket(rightNode -> left), height_bucket(rightNode -> right)) + 1;
    return rightNode;
}

flight_plan* leftRotate_flight_plan(flight_plan* root){
    flight_plan* rightNode = root -> right;
    root -> right = rightNode -> left;
    rightNode -> left = root;

    root -> height = max(height_flight_plan(root -> left), height_flight_plan(root -> right)) + 1;
    rightNode -> height = max(height_flight_plan(rightNode -> left), height_flight_plan(rightNode -> right)) + 1;
    return rightNode;
}

flight_plan* rightRotate_flight_plan(flight_plan* root){
    flight_plan* leftNode = root -> left;
    root -> left = leftNode -> right;
    leftNode -> right = root;

    root -> height = max(height_flight_plan(root -> left), height_flight_plan(root -> right)) + 1;
    leftNode -> height = max(height_flight_plan(leftNode -> left), height_flight_plan(leftNode -> right)) + 1;

    return leftNode;
}

int getBalanceBucket(bucket* root){
    if(root == NULL)    
        return 0;

    return (height_bucket(root -> right) - height_bucket(root -> left));
}

int getBalance_flight(flight_plan* root){
    if(root == NULL){
        return 0;
    }
    return (height_flight_plan(root -> right) - height_flight_plan(root -> left));
}

bucket* createBucket(int start_hr, int end_hr, int start_min, int end_min){
    bucket* newNode = (bucket*)malloc(sizeof(bucket));
    newNode -> bucketID = id++;
    newNode -> start_ETA = (TIME*)malloc(sizeof(TIME));
    newNode -> end_ETA = (TIME*)malloc(sizeof(TIME));
    newNode -> start_ETA -> hr = start_hr;
    newNode -> start_ETA -> min = start_min;
    newNode -> end_ETA -> hr = end_hr;
    newNode -> end_ETA -> min = end_min;

    newNode -> flightList = NULL;
    newNode -> height = 1;
    newNode -> left = NULL;
    newNode -> right = NULL;

    return newNode;
}

flight_plan* createFlightPlan(int f_id, int arr_time_hr, int arr_time_min, int dept_time_hr, int dept_time_min){
    flight_plan* newFlight = (flight_plan*)malloc(sizeof(flight_plan));
    newFlight -> arr_time = (TIME*)malloc(sizeof(TIME));
    newFlight -> dept_time = (TIME*)malloc(sizeof(TIME));

    newFlight -> flight_ID = f_id;
    newFlight -> arr_time -> hr = arr_time_hr;
    newFlight -> arr_time -> min = arr_time_min;
    newFlight -> dept_time -> hr = dept_time_hr;
    newFlight -> dept_time -> min = dept_time_min;

    newFlight -> height = 1;
    newFlight -> left = NULL;
    newFlight -> right = NULL;

    return newFlight;
}

flight_plan* insertFlight_flight_plan(flight_plan* root, flight_plan* flight){
    if(root == NULL){
        root = flight;
        return root;
    }
    else if(maxTime(root -> dept_time, flight -> dept_time) == 1){
        root -> right = insertFlight_flight_plan(root -> right, flight);
    }
    else if(maxTime(root -> dept_time, flight -> dept_time) == -1){
        root -> left = insertFlight_flight_plan(root -> left, flight);
    }
    else{
        printf("Duplicate Encountered \n");
        return root;
    }

    //updating the height once we return back
    root -> height = max(height_flight_plan(root -> left), height_flight_plan(root -> right)) + 1;

    //finding the balance factor
    int bf = getBalance_flight(root);

    //checking for the conditions of rotation
    if(bf > 1 && maxTime(root -> right -> dept_time, flight -> dept_time) == 1){
        //RR case
        return leftRotate_flight_plan(root);
    }
    else if(bf > 1 && maxTime(root -> right -> dept_time, flight -> dept_time) == -1){
        //RL case
        root -> right = rightRotate_flight_plan(root -> right);
        return leftRotate_flight_plan(root);
    }
    else if(bf < -1 && maxTime(root -> left -> dept_time, flight -> dept_time) == -1){
        //LL case
        return rightRotate_flight_plan(root);
    }
    else if(bf < -1 && maxTime(root -> left -> dept_time, flight -> dept_time) == 1){
        //LR case
        root -> left = leftRotate_flight_plan(root -> left);
        return rightRotate_flight_plan(root);
    }

    return root;
}

bucket* insertFlight_bucket(bucket* root, flight_plan* flight){
    if(root == NULL || (maxTime(root -> start_ETA, flight -> arr_time) != -1 && maxTime(root -> end_ETA, flight -> arr_time) == -1)){
        //make a new root or insert in existing
        if(root == NULL){
            printf("Created and ");
            root = createBucket(flight -> arr_time -> hr, (flight -> arr_time -> hr + 1)%24, 0, 0);
        }

        printf("Inserted in the bucket with start and end ETA %d:%d  %d:%d \n", root -> start_ETA -> hr, root -> start_ETA -> min, root -> end_ETA -> hr, root -> end_ETA -> min);
        root -> flightList = insertFlight_flight_plan(root -> flightList, flight);
    }
    else if(root -> end_ETA -> hr == 0 && flight -> arr_time -> hr == 23){
        root -> flightList = insertFlight_flight_plan(root -> flightList, flight);
    }
    else if(maxTime(root -> end_ETA, flight -> arr_time) != -1){
        root -> right = insertFlight_bucket(root -> right, flight);
    }
    else if(maxTime(root -> start_ETA, flight -> arr_time) == -1){
        root -> left = insertFlight_bucket(root -> left, flight);
    }
    else{
        printf("Condition Error Encountered \n");
        return root;
    }

    //after we return again to this root
    root -> height = max(height_bucket(root -> left), height_bucket(root -> right)) + 1;

    //getting the balance factor of the root to check for the rotation
    int bf = getBalanceBucket(root);

    //checking for all the different cases possible for rotation
    if(bf > 1 && maxTime(root -> right -> end_ETA, flight -> arr_time) != -1){
        //RR case
        return leftRotateBucket(root);
    }
    else if(bf > 1 && maxTime(root -> right -> start_ETA, flight -> arr_time) == -1){
        //RL case
        root -> right = rightRotateBucket(root -> right);
        return leftRotateBucket(root);
    }
    else if(bf < -1 && maxTime(root -> left -> start_ETA, flight -> arr_time) == -1){
        //LL case
        return rightRotateBucket(root);
    }
    else if(bf < -1 && maxTime(root -> left -> end_ETA, flight -> arr_time) != -1){
        //LR case
        root -> left = leftRotateBucket(root -> left);
        return rightRotateBucket(root);
    }

    return root;
}

void printDetails(flight_plan* root){
    printf("The flight ID is %d, the arrival time is %d:%d and the departure time is %d:%d \n", root -> flight_ID, root -> arr_time -> hr, root -> arr_time -> min, root -> dept_time -> hr, root -> dept_time -> min);
    return ;
}

void search_flight_plan(flight_plan* root, int flight_id, bool* found){
    if(root == NULL){
        return ;
    }

    if(root -> flight_ID == flight_id){
        printDetails(root);
        *found = TRUE;
        return ;
    }

    if(*found == FALSE){
        search_flight_plan(root -> left, flight_id, found);
    }
    if(*found == FALSE){
        search_flight_plan(root -> right, flight_id, found);
    }
}

void show_status_bucket(bucket* root, int flight_id, bool* found){
    if(root == NULL || *found == TRUE){
        return ;
    }

    show_status_bucket(root -> left, flight_id, found);
    if(*found == FALSE){
        search_flight_plan(root -> flightList, flight_id, found);
    }
    if(*found == FALSE){
        show_status_bucket(root -> right, flight_id, found);
    }
}

void range_search_flight(flight_plan* root, TIME* start_time, TIME* end_time, FILE* ptr){
    //we have to traverse it completely as the sorting is done on departure time and we have to look for arrival time
    if(root == NULL){
        return ;
    }

    range_search_flight(root -> left, start_time, end_time, ptr);
    if(maxTime(root -> arr_time, start_time) != 1 && maxTime(root -> arr_time, end_time) != -1){
        fprintf(ptr, "%d   %d:%d    %d:%d \n", root -> flight_ID, root -> arr_time -> hr, root -> arr_time -> min, root -> dept_time -> hr, root -> dept_time -> min);
    }
    range_search_flight(root -> right, start_time, end_time, ptr);
}

void range_search_bucket(bucket* root, TIME* start_time, TIME* end_time, FILE* ptr){
    if(root == NULL){
        return ;
    }

    if(maxTime(root -> start_ETA, end_time) == -1){
        //in the case where the root, start_ETA is greater than end time of the input
        //We need to search in the left of the list
        range_search_bucket(root -> left, start_time, end_time, ptr);
    }
    else if(root -> end_ETA -> hr == 0 && root -> end_ETA -> min == 0){
        //in the case when maxTime fails i.e. 12 midnight
        range_search_bucket(root -> left, start_time, end_time, ptr);
        range_search_flight(root -> flightList, start_time, end_time, ptr);
        range_search_bucket(root -> right, start_time, end_time, ptr);
    }
    else if(maxTime(root -> end_ETA, start_time) != -1){
        //in the case where root, end_ETA is smaller than the start time of the input.
        //We need to search in the right of the list
        range_search_bucket(root -> right, start_time, end_time, ptr);
    }
    else{
        //when the time lies in the given time frame 
        //Wen need to search left, then the current flight ist and also the right of the flight list
        range_search_bucket(root -> left, start_time, end_time, ptr);
        range_search_flight(root -> flightList, start_time, end_time, ptr);
        range_search_bucket(root -> right, start_time, end_time, ptr);        
    }
}

bucket* getMinValue_bucket(bucket* root){
    bucket* temp = root;
    while(temp -> left){
        temp = temp -> left;
    }

    return temp;
}

flight_plan* getMinValue_flight_plan(flight_plan* root){
    flight_plan* temp = root;

    while(temp -> left)
        temp = temp -> left;

    return temp;
}

flight_plan* delete_flight_plan(flight_plan* root, TIME* dept_t){
    if(root == NULL){
        return root;
    }
    else if(maxTime(root -> dept_time, dept_t) == -1){
        root -> left = delete_flight_plan(root -> left, dept_t);
    }
    else if(maxTime(root -> dept_time, dept_t) == 1){
        root -> right = delete_flight_plan(root -> right, dept_t);
    }
    else{
        //found the flight plan to be deleted
        if(root -> left == NULL || root -> right == NULL){
            flight_plan* temp = root -> left ? root -> left : root -> right;

            if(temp == NULL){
                //no child
                temp = root;
                root = NULL;
            }
            else{
                root -> arr_time -> hr = temp -> arr_time -> hr;
                root -> arr_time -> min = temp -> arr_time -> min;
                root -> dept_time -> hr = temp -> dept_time -> hr;
                root -> dept_time -> min = temp -> dept_time -> min;
                root -> flight_ID = temp -> flight_ID;
                root -> left = temp -> left;
                root -> right = temp -> right;

                //*root = *temp
            }

            free(temp -> arr_time);
            free(temp -> dept_time);
            free(temp);
        }
        else{
            //both children are present
            flight_plan* temp = getMinValue_flight_plan(root -> right);

            root -> flight_ID = temp -> flight_ID;
            root -> arr_time -> hr = temp -> arr_time -> hr;
            root -> arr_time -> min = temp -> arr_time -> min;
            root -> dept_time -> hr = temp -> dept_time -> hr;
            root -> dept_time -> min = temp -> dept_time -> min;

            root -> right = delete_flight_plan(root -> right, root -> dept_time);
        }
    }

    if(root == NULL){
        return root;
    }

    root -> height = max(height_flight_plan(root -> left), height_flight_plan(root -> right)) + 1;
    int balance = getBalance_flight(root);

    if(balance > 1 && maxTime(root -> right -> dept_time, dept_t) == 1){
        return leftRotate_flight_plan(root);
    }
    else if(balance > 1 && maxTime(root -> right -> dept_time, dept_t) == -1){
        root -> right = rightRotate_flight_plan(root -> right);
        return leftRotate_flight_plan(root);
    }
    else if(balance < -1 && maxTime(root -> left -> dept_time, dept_t) == -1){
        return rightRotate_flight_plan(root);
    }
    else if(balance < -1 && maxTime(root -> left -> dept_time, dept_t) == 1){
        root -> left = leftRotate_flight_plan(root -> left);
        return rightRotate_flight_plan(root);
    }

    return root;
}

flight_plan* delete_plan_flight_plan(flight_plan* root, flight_plan* flight){
    if(root == NULL){
        return root;
    }
    else if(maxTime(root -> dept_time, flight -> dept_time) == -1){
        root -> left = delete_plan_flight_plan(root -> left, flight);
    }
    else if(maxTime(root -> dept_time, flight -> dept_time) == 1){
        root -> right = delete_plan_flight_plan(root -> right, flight);
    }
    else if(maxTime(root -> dept_time, flight -> dept_time) == 0 && root -> flight_ID == flight -> flight_ID){
        //found the flight plan
        //checking the children of the root
        if(root -> left == NULL || root -> right == NULL){
            //one or less child
            flight_plan* temp = root -> left ? root -> left : root -> right;

            if(temp == NULL){
                //no child
                temp = root;
                root = NULL;
            }
            else{
                //one child 
                root -> arr_time -> hr= temp -> arr_time -> hr;
                root -> arr_time -> min = temp -> arr_time -> min;
                root -> dept_time -> hr = temp -> dept_time -> hr;
                root -> dept_time -> min = temp -> dept_time -> min;
                root -> left = temp -> left;
                root -> right = temp -> right;
                root -> flight_ID = temp -> flight_ID;                
                //*root = *temp;
            }

            free(temp -> arr_time);
            free(temp -> dept_time);
            free(temp);
        }
        else{
            //both children exists
            flight_plan* temp = getMinValue_flight_plan(root -> right);

            root -> arr_time -> hr = temp -> arr_time -> hr;
            root -> arr_time -> min = temp -> arr_time -> min;
            root -> dept_time -> hr = temp -> dept_time -> hr;
            root -> dept_time -> min = temp -> dept_time -> min;
            root -> flight_ID = temp -> flight_ID;

            root -> right = delete_flight_plan(root -> right, root -> dept_time);
        }
    }

    if(root == NULL){
        return root;
    }

    root -> height = max(height_flight_plan(root -> left), height_flight_plan(root -> right)) + 1;

    int balance = getBalance_flight(root);

    if(balance > 1 && maxTime(root -> right -> dept_time, flight -> dept_time) == 1){
        return leftRotate_flight_plan(root);
    }
    else if(balance > 1 && maxTime(root -> right -> dept_time, flight -> dept_time) == -1){
        root -> right = rightRotate_flight_plan(root -> right);
        return leftRotate_flight_plan(root);
    }
    else if(balance < -1 && maxTime(root -> left -> dept_time, flight -> dept_time) == -1){
        return rightRotate_flight_plan(root);
    }
    else if(balance < -1 && maxTime(root -> left -> dept_time, flight -> dept_time) == 1){
        root -> left  =leftRotate_flight_plan(root -> left);
        return rightRotate_flight_plan(root);
    }

    return root;
}

bucket* delete_bucket(bucket* root, TIME* start_time){
    if(root == NULL){
        return root;
    }
    else if(maxTime(root -> start_ETA, start_time) == -1){
        root -> left = delete_bucket(root -> left, start_time);
    }
    else if(maxTime(root -> start_ETA, start_time) == 1){
        root -> right = delete_bucket(root -> right, start_time);
    }
    else{
        //one or no child case
        if(root -> left == NULL || root -> right == NULL){
            bucket* temp = root -> left ? root -> left : root -> right;

            if(temp == NULL){
                //no child
                temp = root;
                root = NULL;
            }
            else{
                *root = *temp;
            }

            free(temp -> start_ETA);
            free(temp -> end_ETA);
            free(temp);
        }
        else{
            bucket* temp = getMinValue_bucket(root -> right);

            root -> start_ETA -> hr = temp -> start_ETA -> hr;
            root -> start_ETA -> min = temp -> start_ETA -> min;
            root -> end_ETA -> hr = temp -> end_ETA -> hr;
            root -> end_ETA -> min = temp -> end_ETA -> min;
            root -> flightList = temp -> flightList;

            root -> right = delete_bucket(root -> right, root -> start_ETA);
        }
    }

    if(root == NULL){
        return root;
    }

    root -> height = max(height_bucket(root -> left), height_bucket(root -> right)) + 1;

    int balance = getBalanceBucket(root);
    if(balance > 1 && maxTime(root -> right -> start_ETA, start_time) == 1){
        return leftRotateBucket(root);
    }
    else if(balance > 1 && maxTime(root -> right -> start_ETA, start_time) == -1){
        root -> right = rightRotateBucket(root -> right);
        return leftRotateBucket(root);
    }
    else if(balance < -1 && maxTime(root -> left -> start_ETA, start_time) == -1){
        return rightRotateBucket(root);
    }
    else if(balance < -1 && maxTime(root -> left -> start_ETA, start_time) == 1){
        root -> left = leftRotateBucket(root -> left);
        return rightRotateBucket(root);
    }

    return root;
}

bucket* delete_plan_bucket(bucket* root, flight_plan* flight){
    if(root == NULL){
        return root;
    }
    else if(maxTime(root -> start_ETA, flight -> arr_time) == -1){
        root -> left = delete_plan_bucket(root -> left, flight);
    }
    else if(root -> end_ETA -> hr == 0 && flight -> arr_time -> hr == 23){
        root -> flightList = delete_plan_flight_plan(root -> flightList, flight);

        if(root -> flightList == NULL){
            printf("Deleting the bucket \n");
            //deleting the bucket condition
            if(root -> left == NULL || root -> right == NULL){
                bucket* temp = root -> left ? root -> left : root -> right;

                if(temp == NULL){
                    //No child case
                    temp = root;
                    root = NULL;
                }
                else{
                    //single child case
                    *root = *temp;
                }

                free(temp -> start_ETA);
                free(temp -> end_ETA);
                free(temp);
            }
            else{
                bucket* temp = getMinValue_bucket(root -> right);

                //copying the data values
                root -> start_ETA -> hr = temp -> start_ETA -> hr;
                root -> start_ETA -> min = temp -> start_ETA -> min;
                root -> end_ETA -> hr = temp -> end_ETA -> hr;
                root -> end_ETA -> min = temp -> end_ETA -> min;
                root -> flightList = temp -> flightList;

                root -> right = delete_bucket(root -> right, root -> start_ETA);
            }
        }
    }
    else if(maxTime(root -> end_ETA, flight -> arr_time) != -1){
        root -> right = delete_plan_bucket(root -> right, flight);
    }
    else{
        root -> flightList = delete_plan_flight_plan(root -> flightList, flight);

        if(root -> flightList == NULL){
            printf("Deleting the bucket \n");
            //deleting the bucket condition
            if(root -> left == NULL || root -> right == NULL){
                bucket* temp = root -> left ? root -> left : root -> right;

                if(temp == NULL){
                    //No child case
                    temp = root;
                    root = NULL;
                }
                else{
                    //single child case
                    *root = *temp;
                }

                free(temp -> start_ETA);
                free(temp -> end_ETA);
                free(temp);
            }
            else{
                bucket* temp = getMinValue_bucket(root -> right);

                //copying the data values
                root -> start_ETA -> hr = temp -> start_ETA -> hr;
                root -> start_ETA -> min = temp -> start_ETA -> min;
                root -> end_ETA -> hr = temp -> end_ETA -> hr;
                root -> end_ETA -> min = temp -> end_ETA -> min;
                root -> flightList = temp -> flightList;

                root -> right = delete_bucket(root -> right, root -> start_ETA);
            }
        }
    }

    if(root == NULL){
        return root;
    }

    root -> height = max(height_bucket(root -> right), height_bucket(root -> left)) + 1;

    int balance = getBalanceBucket(root);

    if(balance > 1 && maxTime(root -> right -> end_ETA, flight -> arr_time) != -1 && maxTime(root -> right -> start_ETA, flight -> arr_time) == -1){
        return leftRotateBucket(root);
    }
    else if(balance > 1 && maxTime(root -> right -> start_ETA, flight -> arr_time) == -1){
        root -> right = rightRotateBucket(root -> right);
        return leftRotateBucket(root);
    }
    else if(balance < -1 && maxTime(root -> left -> start_ETA, flight -> arr_time) == -1){
        return rightRotateBucket(root);
    }
    else if(balance < -1 && maxTime(root -> left -> end_ETA, flight -> arr_time) != -1){
        root -> left = leftRotateBucket(root -> left);
        return rightRotateBucket(root);
    }

    return root;
}

void outputData_flight_plan(flight_plan* root, FILE* ptr){
    if(root == NULL){
        return ;
    }

    outputData_flight_plan(root -> left, ptr);
    fprintf(ptr, "%d %d:%d %d:%d \n", root -> flight_ID, root -> arr_time -> hr, root -> arr_time -> min, root -> dept_time -> hr, root -> dept_time -> min);
    outputData_flight_plan(root -> right, ptr);
}

void outputData_bucket(bucket* root, FILE* ptr){
    if(root == NULL){
        return ;
    }

    outputData_bucket(root -> left, ptr);
    fprintf(ptr, "In the bucket with start and end ETA as %d:%d %d:%d \n", root -> start_ETA -> hr, root -> start_ETA -> min, root -> end_ETA -> hr, root -> end_ETA -> min);
    outputData_flight_plan(root -> flightList, ptr);
    fprintf(ptr, "\n\n\n");
    outputData_bucket(root -> right, ptr);
}

void inorder_flight(flight_plan* root){
    if(!root){
        return ;
    }
    inorder_flight(root -> left);
    printf("%d   ", getBalance_flight(root));

    inorder_flight(root -> right);
}

void inorder_bucket(bucket* root){
    if(root == NULL){
        return ;
    }

    inorder_bucket(root -> left);
    printf("%d   ", getBalanceBucket(root));
    inorder_flight(root -> flightList);
    inorder_bucket(root -> right);
}


int main(){
    bucket* root = NULL;
    int data, op, hr, min, id, arr_time_hr, arr_time_min, dep_time_hr, dep_time_min;

    FILE* ptr = fopen("a.csv", "r");

    while(!feof(ptr)){
        fscanf(ptr, "%d,%d,%d,%d,%d \n", &id, &arr_time_hr, &arr_time_min, &dep_time_hr, &dep_time_min);

        flight_plan* flight = createFlightPlan(id, arr_time_hr, arr_time_min, dep_time_hr, dep_time_min);
        root = insertFlight_bucket(root, flight);
    }

    printf("Press 1. Show Status, 2. Inserting a new flight plan 3. Deleting a flight plan, 4. Range Search 5. Exit \n");
    scanf("%d", &op);

    if(op >=1 && op <= 5){
        if(op == 1){
            bool found = FALSE;
            printf("Enter the flight id you want to search for \n");
            scanf("%d", &id);

            show_status_bucket(root, id, &found);
            if(found == FALSE){
                printf("No flight id exists with given flight id \n");
            }
        }
        else if(op == 2){
            FILE* ptr = fopen("inserted.txt", "w");
            printf("Enter the flight id, arrival time hr, arrival time min, departure time hr, departure time min in correct order \n");
            scanf("%d %d %d %d %d", &id, &arr_time_hr, &arr_time_min, &dep_time_hr, &dep_time_min);

            flight_plan* flight = (flight_plan*)malloc(sizeof(flight_plan));
            flight -> arr_time = (TIME*)malloc(sizeof(TIME));
            flight -> dept_time = (TIME*)malloc(sizeof(TIME));
            flight -> flight_ID = id;
            flight -> arr_time -> hr = arr_time_hr;
            flight -> arr_time -> min = arr_time_min;
            flight -> dept_time -> hr = dep_time_hr;
            flight -> dept_time -> min = dep_time_min;
            flight -> left = NULL;
            flight -> right = NULL;

            root = insertFlight_bucket(root, flight);
            inorder_bucket(root);
            outputData_bucket(root, ptr);
        }
        else if(op == 3){
            FILE* ptr = fopen("deleted.txt", "w");
            printf("Enter the flight id, arrival time hr, arrival time min, departure time hr, departure time min in correct order \n");
            scanf("%d %d %d %d %d", &id, &arr_time_hr, &arr_time_min, &dep_time_hr, &dep_time_min);

            flight_plan* flight = (flight_plan*)malloc(sizeof(flight_plan));
            flight -> arr_time = (TIME*)malloc(sizeof(TIME));
            flight -> dept_time = (TIME*)malloc(sizeof(TIME));
            flight -> flight_ID = id;
            flight -> arr_time -> hr = arr_time_hr;
            flight -> arr_time -> min = arr_time_min;
            flight -> dept_time -> hr = dep_time_hr;
            flight -> dept_time -> min = dep_time_min;
            flight -> left = NULL;
            flight -> right = NULL;

            root = delete_plan_bucket(root, flight);
            outputData_bucket(root, ptr);
            inorder_bucket(root);
        }
        else if(op == 4){
            printf("Enter the starting and ending time for range search \n");
            scanf("%d %d %d %d", &arr_time_hr, &arr_time_min, &dep_time_hr, &dep_time_min);

            TIME* start_time = (TIME*)malloc(sizeof(TIME));
            TIME* end_time = (TIME*)malloc(sizeof(TIME));
            start_time -> hr = arr_time_hr;
            start_time -> min = arr_time_min;
            end_time -> hr = dep_time_hr;
            end_time -> min = dep_time_min;

            if(maxTime(start_time, end_time) == -1){
                printf("Enter the start time and end time correctly \n");
            }
            else{
                FILE* ptr = fopen("range_search.txt", "w");
                range_search_bucket(root, start_time, end_time, ptr);
            }
        }
        else{
            printf("Exiting \n");
        }
    }
    else{
        printf("Enter a valid operation \n");
    }

    return 0;
}