#include <cs50.h>
#include <stdio.h>
#include <string.h>


// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

/* dictionary like data structure */
typedef struct
{
    int lead;
    int index;
}
pair_dictionary;


// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

/* functions i've added for merge sort */
void mergesort(pair_dictionary array[], int length);
void merge(pair_dictionary left[], pair_dictionary right[], int lenght_left, int length_right, pair_dictionary merged[]);

/* functions for locking pairs */
bool loopcheck();

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    /* check the name first */
    for (int i = 0; i < candidate_count; i++)
    {
        /* if the name matches */
        if (strcmp(name, candidates[i]) == 0)
        {
            /* the rank of i-th candidate is rank (j) */
            ranks[i] = rank;
            return true;
        }
        /* else keep looking until the end of the list */
    }
    /* if the candidte as such does not exist it must be an inalid vote */
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    /* looping throught the prefrences */
    for (int i = 0; i < candidate_count; i++)
    {
        /* j = i  because if we start with j = 0 we also cover cases which are already compared - giving rise to false votes*/
        for (int j = i + 1; j < candidate_count; j++)
        {
            /* if rank of candidate i is higher than canddidate j for this voter */
            if (ranks[i] > ranks[j])
            {
                preferences[i][j] += 1;
            }
            /* if it is vice versa */
            else if (ranks[j] > ranks[i])
            {
                preferences[j][i] += 1;
            }
            /*explicitly ignoring the case where they are equal - that will not happen */
        }
    }
    /* the work is done */
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    /* starting with prefrences array */
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            /* if more people prefer candidate i over candidate j */
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count += 1;
            }
            /* if more people prefer canddidate j over canidate i */
            else if (preferences[j][i] > preferences[i][j])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count += 1;
            }
            /* else this is case of tie - ignore it */
        }
    }
    /* updating actual number of pairs - because we started it with zero */
    pair_count += 1;
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    /**
     * According to the given definition, sorting is to be done according to
     * the number of voters who prefer one candidate over the other
    */
    pair_dictionary unsorted[pair_count];
    /* inititalize the pair dictionary */
    for (int i = 0; i < pair_count; i++)
    {
        unsorted[i].lead = preferences[pairs[i].winner][pairs[i].loser];
        unsorted[i].index = i;
    }
    /* sort the dictionary */
    mergesort(unsorted, pair_count);
    /* sorted dictionary is in ascending order - reversing it */
    pair_dictionary temp;
    int mid = pair_count / 2;
    if (pair_count % 2 != 0)
    {
        mid = (pair_count - 1) / 2;
    }
    for (int i = 0; i < mid; i++)
    {
        temp = unsorted[i];
        unsorted[i] = unsorted[(pair_count - 1) - i];
        unsorted[(pair_count - 1) - i] = temp;
    }
    /* finally sort the pairs array with obtained sorted index sort */
    pair temp2;
    for (int i = 0; i < pair_count; i++)
    {
        temp2 = pairs[i];
        pairs[i] = pairs[unsorted[i].index];
        pairs[unsorted[i].index] = temp2;
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    return;
}

/* merge sort implementation */
void mergesort(pair_dictionary array[], int length)
{
    /* if there is only one element there is nothing to do */
    if (length == 1)
    {
        return;
    }
    else
    {
        /* slicing the array into two parts */
        int length_left, length_right;
        /* if the given array is even in length it may have two equal slices */
        if (length % 2 == 0)
        {
            length_left = length_right = length / 2;
        }
        /* otherwise the one on the left has one less element than the one on the right */
        else
        {
            length_left = (length - 1) / 2;
            length_right = length - length_left; 
        }
        /* creating two place holders for these arrays */
        pair_dictionary left[length_left], right[length_right];

        /* filling up the arrays */
        for (int i = 0; i < length_left; i++)
        {
            left[i] = array[i];
        }
        
        /* filling up the right hand side part */
        for (int i = 0; i < length_right; i++)
        {
            right[i] = array[i + length_left];
        }

        /* recursively call mergesort on both parts */
        mergesort(left, length_left);
        mergesort(right, length_right);

        /* merge the sorted arrays into one */
        merge(left, right, length_left, length_right, array);
    }
}


/* the merge function */
void merge(pair_dictionary left[], pair_dictionary right[], int length_left, int length_right, pair_dictionary merged[])
{
    /* length  of merged array */
    int length_merged = length_left + length_right;
    /* indexes for the two arrays maintained separately */
    int i, j;
    /* i - index for left array and j - index for right array */
    i = j = 0;

    /**
     * here we need to cover these cases
     * 
     * 1. i < length_left
     *      - 1a. j < length_right
     *              - 1a.a left[i] < right[i]
     *                  - add from left
     *              - 1a.b left[i] > right[j]
     *                  - add from right
     *      - 1b. j >= length_right
     *              - add from left
     * 
     * 2. i >= length_left
     *      - 2a. j < length_right
     *              - add from right
     *      - 2b. j >= length_right
     *              - do nothing
    */

   /* run until new array reaches end */
    for (int k = 0; k < length_merged; k++)
    {
        /* case - 1*/
        if (i < length_left)
        {
            /* case 1a */
            if (j < length_right)
            {
                /* case 1a.a */
                if (left[i].lead < right[j].lead)
                {
                    merged[k] = left[i];
                    i += 1;
                }
                /* case 1a.b */
                else
                {
                    merged[k] = right[j];
                    j += 1;
                }
                
            }
            /* case 1b */
            else
            {
                merged[k] = left[i];
                i += 1;
            }
        }
        /* case 2 */
        else
        {
            /* case 2a */
            if (j < length_right)
            {
                merged[k] = right[j];
                j += 1;
            }
            /* else case 2b - do nothing*/
        }
    }
    return;
}
