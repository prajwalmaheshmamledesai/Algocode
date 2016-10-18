package leeteCode;

import java.io.*;
import java.util.*;
import java.text.*;
import java.math.*;
import java.util.regex.*;

public class Alicecandies {
    private static final int INCREASING = 0;
    private static final int DECREASING = 1;
    private static final int EQUAL = 2;

    public static void main(String[] args) {
        /* Enter your code here. Read input from STDIN. Print output to STDOUT. Your class should be named Solution. */
        Scanner scanner = new Scanner(System.in);
        int n = scanner.nextInt();
        final int[] ratings = new int[n];
        
        for (int i = 0; i < n; i++) {
            ratings[i] = scanner.nextInt();

        }
        
        int state = EQUAL;
        int[] candies = new int[n];
        candies[0] = 1;
        for (int i = 1; i < n; i++) {
            if (ratings[i] > ratings[i-1]) {
                if (state == DECREASING) goback(ratings, candies, i-1);
                state = INCREASING;
                candies[i] = candies[i-1] + 1;
            } else if (ratings[i] < ratings[i-1]) {
                state = DECREASING;
                candies[i] = candies[i-1] - 1;
            } else {
                if (state == DECREASING) goback(ratings, candies, i-1);
                state = EQUAL;
                candies[i] = 1;
            }
            
            if (i == ratings.length - 1 && state == DECREASING) 
            	goback(ratings, candies, i); 
        }
        
        int count = 0;
        for (int c : candies) {
            count += c;
            System.out.println(c);
        }
        

        
        System.out.println(count);
    }
    
    private static void goback(int[] ratings, int candies[], int s) {
        int subtract_value = candies[s] - 1;
        int i = s;
        while (i > 0 && ratings[i-1] > ratings[i]) {
            candies[i] -= subtract_value;  
            i--;
        }
        if (i == 0) {
            candies[i] -= subtract_value;
        } else {
            candies[i] = Math.max(candies[i], candies[i+1]+1);
        }
    }
}