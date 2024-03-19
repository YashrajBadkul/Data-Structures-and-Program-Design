#include<stdio.h>
#include<string.h>
#include<math.h>
#include<limits.h>

#define SIZE 100
#define NUM_OF_DAYS_FOR_EACH_STOCK 5
#define FIRST_DATE 9
#define LAST_DATE 13

struct database{
    char name[50];
    int date;
    float open;
    float high;
    float low;
    float close;
    int vol;
};

struct stock_performance{
    char name[50];
    float data;
};


void percentage_change(struct database s[], int n);
void sort_given(struct database s[], int n);
void profit_loss(struct database s[], int n);
void average(struct database s[], int n);
void highest_performing(struct database s[], int n);
void lowest_performing(struct database s[], int n);
void overall_performance(struct database s[], int n);
void selection_sort(struct stock_performance s[], int n);

int main(){

    struct database stocks[SIZE];
    int n = 0, num;
    FILE *file = fopen("a.csv", "r");
    do{
        fscanf(file, "%49[^,], %d, %f, %f, %f, %f, %d \n", &stocks[n].name, &stocks[n].date, &stocks[n].open, &stocks[n].high, &stocks[n].low, &stocks[n].close, &stocks[n].vol);
        n++;
    }
    while(!feof(file));

    do{
        printf("\nEnter the serial no. of the operation that you want to perform or press 8 for exiting the program \n");
        scanf("%d", &num);
        
        if(num == 1){
            percentage_change(stocks, n);
        }
        else if(num == 2){
            sort_given(stocks, n);
        }
        else if(num == 3){
            profit_loss(stocks, n);
        }
        else if(num == 4){
            highest_performing(stocks, n);
        }
        else if(num == 5){
            lowest_performing(stocks, n);
        }
        else if(num == 6){
            overall_performance(stocks, n);
        }
        else if(num == 7){
            average(stocks, n);
        }
        else if(num == 8){
            printf("Exiting the program \n");
        }
        else{
            printf("Please enter a valid serial number \n");
        }
    }
    while(num >= 1 && num <= 7);

    return 0;
}

void percentage_change(struct database s[], int n){
    int start_date, end_date;
    char name[50];
    printf("Enter the start and the end date between %d and %d \n", FIRST_DATE, LAST_DATE);
    scanf("%d %d", &start_date, &end_date);

    if(start_date > end_date || start_date < FIRST_DATE || end_date > LAST_DATE){
        printf("Invalid dates entered, returning back with status 0 \n");
        return ;
    }
    printf("Enter the name of the stock \n");
    scanf("%s", &name);
    strupr(name);


    int flag_1 = 1, flag_2 = 1;
    float initial, final, change;

    
    for(int i=0; i<n && (flag_1 == 1 || flag_2 == 1); i++){
        if(strcmp(s[i].name, name) == 0 && s[i].date == start_date){
            initial = s[i].open;
            flag_1 = 0;
        }
        if(strcmp(s[i].name, name) == 0 && s[i].date == end_date){
            final = s[i].close;
            flag_2 = 0;
        }
    }
    if(flag_1 == 1 || flag_2 == 1){
        printf("No record found, returning back with status 0 \n");
        return ;
    }

    change = ((final - initial) / initial) * 100;
    if(final < initial){
        printf("The stock %s has a percentage change of %f in loss \n", name, fabs(change));
    }
    else{
        printf("The stock %s has a percentage change of %f in profit \n", name, fabs(change));
    }
    
}

void sort_given(struct database s[], int n){
    char name[50];
    struct database dummy[NUM_OF_DAYS_FOR_EACH_STOCK];
    //dummy is made so that the main array of stocks is not altered and multiple functions could run in a single program

    int flag = 1;
    int i = 0, index = 0;

    printf("Enter the name of the stock to be sorted \n");
    scanf("%s", &name);
    strupr(name);
    
    while(i < n && flag == 1){
        if(strcmp(s[i].name, name) == 0){
            flag = 0;
        }
        else{
            i = i + NUM_OF_DAYS_FOR_EACH_STOCK;
        }
    }
    if(flag == 1){
        printf("Record with given name not found, returning back to main with status 0 \n");
        return ;
    }

    for(int k=i; k<NUM_OF_DAYS_FOR_EACH_STOCK+i; k++){
        dummy[index] = s[k];
        index++;
    }
    for(int j=0; j<NUM_OF_DAYS_FOR_EACH_STOCK-1; j++){
        for(int k=0; k<NUM_OF_DAYS_FOR_EACH_STOCK - j - 1; k++){
            if(dummy[k].vol > dummy[k+1].vol){
                struct database temp = dummy[k];
                dummy[k] = dummy[k+1];
                dummy[k+1] = temp;
            }
            else if(dummy[k].vol == dummy[k+1].vol){
                if(dummy[k].close > dummy[k+1].close){
                    struct database temp = dummy[k];
                    dummy[k] = dummy[k+1];
                    dummy[k+1] = temp;
                }
            }
        }
    }
    for(int j=0; j<NUM_OF_DAYS_FOR_EACH_STOCK; j++){
        printf("%s  %d %f %f %f %f %d",dummy[j].name, dummy[j].date, dummy[j].open, dummy[j].high, dummy[j].low, dummy[j].close, dummy[j].vol);
        printf("\n");
    }
    printf("Returning back to main with status 1 \n");
}

void profit_loss(struct database s[], int n){
    int buy_date, sell_date;
    float buy_price, sell_price;
    float ans;
    char name[50];
    int num_stocks;
    printf("Enter the number of stocks for which you want to calculate profit or loss \n");
    scanf("%d", &num_stocks);
    printf("Enter the date on which stock is bought and the date on which it is sold \n");
    scanf("%d %d", &buy_date, &sell_date);
    if(buy_date < FIRST_DATE || sell_date > LAST_DATE){
        printf("Entered dates are out of the bound, returning back with status 0 \n");
    }

    else{
        while(num_stocks--){
            printf("Enter the name of stock that you want to check : ");
            scanf("%s", name);
            strupr(name);
            int i=0, flag = 1;
            while(i<n && flag == 1){
                if(strcmp(s[i].name, name) == 0){
                    flag = 0;
                }
                else{
                    i = i + NUM_OF_DAYS_FOR_EACH_STOCK;
                }
            }
            if(flag == 1){
                printf("Stocks with specified name not found in the record \n");
            }
            else{
                int flag_1 = 1, flag_2 = 1;
                for(int j=i; j<i+5 && (flag_1 == 1 || flag_2 == 1); i++){
                    if(s[i].date == buy_date){
                        buy_price = s[i].low;
                        flag_1 = 0;
                    }
                    if(s[i].date == sell_date){
                        sell_price = s[i].high;
                        flag_2 = 0;
                    }
                }
                ans = sell_price - buy_price;
                if(ans >= 0){
                    printf("The stock is in profit and profit is %f \n", ans);
                }
                else{
                    ans = ans * (-1);
                    printf("The stock is in loss ans loss is %f \n", ans);
                }    
            }        
        }
    }
}

void average(struct database s[], int n){
    int start_date, end_date, i=0, days;
    double sum = 0, avg, avg_sum = 0, avg_of_avg;
    printf("Enter the starting date and ending date for the calculation of average \n");
    scanf("%d %d", &start_date, &end_date);
    if(end_date < start_date || start_date < FIRST_DATE || end_date > LAST_DATE){
        printf("Entered dates are out of the bound, returning back with status 0 \n");
    }
    else{
        days = end_date - start_date + 1;
        while(i < n){
            sum = 0;
            for(int j=i; j<5+i; j++){
                if(s[j].date >= start_date && s[j].date <= end_date){
                    sum = sum + (s[j].open * s[j].vol);
                }
            }
            i = i + NUM_OF_DAYS_FOR_EACH_STOCK;
            avg = sum / days;
            avg_sum = avg_sum + avg;
        }
        avg_of_avg = avg_sum / 10;
        printf("The average of average of all stocks from %d to %d is %lf \n", start_date, end_date, avg_of_avg);
    }
}

void highest_performing(struct database s[], int n){
    float profit, max_profit_perc = INT_MIN, profit_perc, start_price, end_price;
    int start_date, end_date;
    printf("Enter the starting date and the ending date \n");
    scanf("%d %d", &start_date, &end_date);
    if(start_date < FIRST_DATE || end_date > LAST_DATE || end_date < start_date){
        printf("Entered dates are out of the bounds, returning back with status 0 \n");
    }
    else{
        int i=0, flag_1, flag_2;
        char name_of_highest_perf[50] = "";
        while(i < n){
            flag_1 = 1;
            flag_2 = 1;
            for(int j=i; j<NUM_OF_DAYS_FOR_EACH_STOCK+i && ((flag_1 == 1) || flag_2 == 1); j++){
                if(s[j].date == start_date){
                    start_price = s[j].low;
                    flag_1 = 0;
                }
                if(s[j].date == end_date){
                    end_price = s[j].high;
                    flag_2 = 0;
                }
            }
            profit = end_price - start_price;
            profit_perc = (profit / start_price) * 100;
            if(profit_perc > max_profit_perc){
                strcpy(name_of_highest_perf, s[i].name);
                max_profit_perc = profit_perc;
            }
            i = i + NUM_OF_DAYS_FOR_EACH_STOCK;
        }
        printf("The highest performing stock is %s with a profit percentage of %f \n", name_of_highest_perf, max_profit_perc);
    }
}

void lowest_performing(struct database s[], int n){
    float profit, profit_perc, min_profit_perc = INT_MAX;
    float start_price, end_price;
    int i=0, flag_1, flag_2, start_date, end_date;
    char name_of_lowest_perf[50];

    printf("Enter the start date and the ending date for which you want to check \n");
    scanf("%d %d", &start_date, &end_date);
    if(start_date < FIRST_DATE || end_date > LAST_DATE || end_date < start_date){
        printf("Entered dates are out of the bounds, returning back with status 0 \n");
    }
    else{
        while(i < n){
            flag_1 = 1;
            flag_2 = 1;
            for(int j=i; j<NUM_OF_DAYS_FOR_EACH_STOCK+i; j++){
                if(s[j].date == start_date){
                    flag_1 = 0;
                    start_price = s[j].low;
                }
                if(s[j].date == end_date){
                    flag_2 = 0;
                    end_price = s[j].high;
                }
            }
            profit = end_price - start_price;
            profit_perc = (profit / start_price) * 100;
            if(profit_perc < min_profit_perc){
                min_profit_perc = profit_perc;
                strcpy(name_of_lowest_perf, s[i].name);
            }
            i = i + NUM_OF_DAYS_FOR_EACH_STOCK;
        }
        printf("The stock with minimum performance is %s with a profit percentage of %f \n", name_of_lowest_perf, min_profit_perc);
    }
}

void overall_performance(struct database s[], int n){
    int start_date, end_date;
    struct stock_performance performance[10];
    float percentage, sum;
    int flag, i=0, k=0;
    //here dates are taken from 10 to 13 as the data for the day previous to 9 is not taken in the file
    printf("Enter the starting and ending date from %d to %d \n", FIRST_DATE+1, LAST_DATE);
    scanf("%d %d", &start_date, &end_date);
    if(start_date < FIRST_DATE+1 || end_date > LAST_DATE || end_date < start_date){
        printf("Entered dates are out of the bound, returning back with status 0 \n");
    }
    else{
        while(i < n){
            flag = 1;
            sum = 0;
            for(int j=i; j<NUM_OF_DAYS_FOR_EACH_STOCK+i && flag == 1; j++){
                if(s[j].date >= start_date && s[j].date <= end_date){
                    sum = sum + ((((s[j].open-s[j-1].close)/s[j-1].close)*100) * s[j].vol);
                }
                if(s[j].date == end_date){
                    flag = 0;
                }
            }
            strcpy(performance[k].name, s[i].name);
            performance[k].data = sum;
            k++;
            i = i + NUM_OF_DAYS_FOR_EACH_STOCK;
        }
        selection_sort(performance, 10);
        for(int i=0; i<10; i++){
            printf("%s     %f \n", performance[i].name, performance[i].data);
        }
    }
}

void selection_sort(struct stock_performance s[], int n){
    struct stock_performance temp;
    int j;
    for(int i=0; i<n-1; i++){
        temp = s[i];
        int min = i;
        for(j=i+1; j<n; j++){
            if(s[j].data < temp.data){
                temp = s[j];
                min = j;
            }
        }
        s[min] = s[i];
        s[i] = temp;
    }
}



