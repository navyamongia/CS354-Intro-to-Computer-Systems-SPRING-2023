////////////////////////////////////////////////////////////////////////////////
// Main File:        csim.c
// This File:        csim.c
// Other Files:      Makefile, trace files
//
// Semester:         CS 354 Spring 2023
// Instructor:       Deppeler
// Author:           Navya Mongia
// GG:				 9
// Email:            mongia@wisc.edu
// CS Login:         navya
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   Fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          NONE
// Online sources:   NONE
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013,2019-2020
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission for Spring 2023
//
////////////////////////////////////////////////////////////////////////////////

/*
 * csim.c:  
 * A cache simulator that can replay traces (from Valgrind) and output
 * statistics for the number of hits, misses, and evictions.
 * The replacement policy is LRU.
 *
 * Implementation and assumptions:
 *  1. Each load/store can cause at most one cache miss plus a possible eviction.
 *  2. Instruction loads (I) are ignored.
 *  3. Data modify (M) is treated as a load followed by a store to the same
 *  address. Hence, an M operation can result in two cache hits, or a miss and a
 *  hit plus a possible eviction.
 */  

#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

/******************************************************************************/
/* DO NOT MODIFY THESE VARIABLES **********************************************/

//Globals set by command line args.
int b = 0; //number of block (b) bits
int s = 0; //number of set (s) bits
int E = 0; //number of lines per set

//Globals derived from command line args.
int B; //block size in bytes: B = 2^b
int S; //number of sets: S = 2^s

//Global counters to track cache statistics in access_data().
int hit_cnt = 0;
int miss_cnt = 0;
int evict_cnt = 0;

//Global to control trace output
int verbosity = 0; //print trace if set
/******************************************************************************/
  
  
//Type mem_addr_t: Use when dealing with addresses or address masks.
typedef unsigned long long int mem_addr_t;

//Type cache_line_t: Use when dealing with cache lines.

typedef struct cache_line {                    
    char valid;
    mem_addr_t tag;
    int countNum; //to keep track 
} cache_line_t;

//Type cache_set_t: Use when dealing with cache sets
//Note: Each set is a pointer to a heap array of one or more cache lines.
typedef cache_line_t* cache_set_t;

//Type cache_t: Use when dealing with the cache.
//Note: A cache is a pointer to a heap array of one or more sets.
typedef cache_set_t* cache_t;

// Create the cache we're simulating. 
//Note: A cache is a pointer to a heap array of one or more cache sets.
cache_t cache;  

/* 
 * init_cache:
 * Allocates the data structure for a cache with S sets and E lines per set.
 * Initializes all valid bits and tags with 0s.
 */                    
void init_cache() { 
	//calculating S and B 
    S = pow(2, s);
    
    B = pow(2, b);
    
	//allocating mem
	cache = malloc(sizeof(cache_set_t) * S);
    
	//checking return value of malloc
	if(cache == NULL) {
	printf("Could not allocate the requested sets!");
	exit(1);
    }
   
    for (int i = 0; i < S; i++){
		*(cache + i) = calloc(E, sizeof(cache_line_t));
		//checking return value of calloc
		if (*(cache + i) == NULL) {
		       printf("Could not allocate the requested sets!");
		       exit(1);
		}
	}
}
  

/* 
 * free_cache:
 * Frees all heap allocated memory used by the cache.
 */                    
void free_cache() { 
	//freeing the cache in order
	for(int x = 0; x < S; x++){
		free(*(cache+x));
		*(cache+x) = NULL;
	} 
	
	free(cache) ;  
	cache = NULL;       
}


/*  
 * access_data:
 * Simulates data access at given "addr" memory address in the cache.
 *
 * If already in cache, increment hit_cnt
 * If not in cache, cache it (set tag), increment miss_cnt
 * If a line is evicted, increment evict_cnt
 */                    
void access_data(mem_addr_t addr) { 
	
	int mask = pow(2,s) -1;
	//getting the set number and tag number by masking and shifts
	mem_addr_t set_num = (addr >> b) & mask;

	
	mem_addr_t tag_num = (addr >> (b + s));

    
	//CASE 1: CACHE HIT
	//if tag is present and v bit is 1 then increment hit counter and return
	for (int i = 0; i < E; i++){
		if (cache[set_num][i].tag == tag_num && cache[set_num][i].valid == 1){
		
				hit_cnt++;
		       
		        
		        int count_max = 0;
		        for (int j = 0; j < E; j++){
			       if (cache[set_num][j].countNum > count_max){
				       count_max = cache[set_num][j].countNum;
		       	       }	
		        }
			
				//increasing the lines
		        cache[set_num][i].countNum = count_max + 1;
		        return;
		}
	}	
	
	//if not there then it means its a miss
	
	miss_cnt++;
	
	//CASE 2: Invalid bit status 	
	for (int i = 0; i < E; i++){
                if (cache[set_num][i].valid == 0){
                       cache[set_num][i].tag = tag_num;
		       		   cache[set_num][i].valid = 1;
                       
						
                       int count_max = 0; //var for max count
                       for (int j = 0; j < E; j++){
                               if (cache[set_num][j].countNum > count_max){
                                       count_max = cache[set_num][j].countNum;
                               }
                       }
		      
                       cache[set_num][i].countNum = count_max + 1; //incr lines 
                       return;
		}
	}

	
	//if the pgm comes here then evict case
	evict_cnt++;

	//finding the minimum count
	int count_min = cache[set_num][0].countNum;
	for (int i = 1; i < E; i++){
                if (cache[set_num][i].countNum < count_min){
			count_min = cache[set_num][i].countNum;
		}
	}
	
	
   // find the most lru line and make it victim
	for (int i = 0; i < E; i++){
                if (cache[set_num][i].countNum == count_min){
						cache[set_num][i].tag = tag_num;
		
                        int count_max = 0;
                        for (int j = 0; j < E; j++){
                               if (cache[set_num][j].countNum > count_max){
                                       count_max = cache[set_num][j].countNum;
                               }
                       }
			
						
						//incrm lines
                       cache[set_num][i].countNum = count_max + 1;
                       return;
		}
	}


 
}
  
  
/* 
 * replay_trace:
 * Replays the given trace file against the cache.
 *
 * Reads the input trace file line by line.
 * Extracts the type of each memory access : L/S/M
 * TRANSLATE each "L" as a load i.e. 1 memory access
 * TRANSLATE each "S" as a store i.e. 1 memory access
 * TRANSLATE each "M" as a load followed by a store i.e. 2 memory accesses 
 */                    
void replay_trace(char* trace_fn) {           
    char buf[1000];  
    mem_addr_t addr = 0;
    unsigned int len = 0;
    FILE* trace_fp = fopen(trace_fn, "r"); 

    if (!trace_fp) { 
        fprintf(stderr, "%s: %s\n", trace_fn, strerror(errno));
        exit(1);   
    }

    while (fgets(buf, 1000, trace_fp) != NULL) {
        if (buf[1] == 'S' || buf[1] == 'L' || buf[1] == 'M') {
            sscanf(buf+3, "%llx,%u", &addr, &len);
      
            if (verbosity)
                printf("%c %llx,%u ", buf[1], addr, len);

            // 
            // GIVEN: 1. addr has the address to be accessed
            //        2. buf[1] has type of acccess(S/L/M)
            // call access_data function here depending on type of access
             
			if(buf[1] == 'S' || buf[1] == 'L'){ //only 1 access in L or S case
			 	access_data(addr);
			 }
			 
	   		 if(buf[1] == 'M') { //M means load and store both , therefore 2 accesses
				access_data(addr);
				access_data(addr);
	    		}

            if (verbosity)
                printf("\n");
        }
    }

    fclose(trace_fp);
}  
  
  
/*
 * print_usage:
 * Print information on how to use csim to standard output.
 */                    
void print_usage(char* argv[]) {                 
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]);
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of s bits for set index.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of b bits for block offsets.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\nExamples:\n");
    printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", argv[0]);
    printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", argv[0]);
    exit(0);
}  
  
  
/*
 * print_summary:
 * Prints a summary of the cache simulation statistics to a file.
 */                    
void print_summary(int hits, int misses, int evictions) {                
    printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
    FILE* output_fp = fopen(".csim_results", "w");
    assert(output_fp);
    fprintf(output_fp, "%d %d %d\n", hits, misses, evictions);
    fclose(output_fp);
}  
  
  
/*
 * main:
 * Main parses command line args, makes the cache, replays the memory accesses
 * free the cache and print the summary statistics.  
 */                    
int main(int argc, char* argv[]) {                      
    char* trace_file = NULL;
    char c;
    
    // Parse the command line arguments: -h, -v, -s, -E, -b, -t 
    while ((c = getopt(argc, argv, "s:E:b:t:vh")) != -1) {
        switch (c) {
            case 'b':
                b = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'h':
                print_usage(argv);
                exit(0);
            case 's':
                s = atoi(optarg);
                break;
            case 't':
                trace_file = optarg;
                break;
            case 'v':
                verbosity = 1;
                break;
            default:
                print_usage(argv);
                exit(1);
        }
    }

    //Make sure that all required command line args were specified.
    if (s == 0 || E == 0 || b == 0 || trace_file == NULL) {
        printf("%s: Missing required command line argument\n", argv[0]);
        print_usage(argv);
        exit(1);
    }

    //Initialize cache.
    init_cache();

    //Replay the memory access trace.
    replay_trace(trace_file);

    //Free memory allocated for cache.
    free_cache();

    //Print the statistics to a file.
    //DO NOT REMOVE: This function must be called for test_csim to work.
    print_summary(hit_cnt, miss_cnt, evict_cnt);
    return 0;   
}  

// Spring 202301
