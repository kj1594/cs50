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
    /* cycling through all acndidates assuming no two have the same name */
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            /* ranks[i] - voter's ith preference */
            /* ranks[rank] - voter's preference at rank (rank) is candidate index */
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    /* the ranks are for one voter */
    /* preferences[i][j] = number of people who prefer i over j */
    for (int i = 0; i < candidate_count; i++)
    {
        /* for each candidate loop through other candidates */
        for (int j = 0; j < candidate_count; j++)
        {
            /* if not comparing to himself */
            if (i != j)
            {
                /* ranks[i] would be higer rank than ranks[j] in this case */
                if (i < j)
                {
                    /* incrementing the votes for candidate present at ranks[i] over the one at ranks[j] */
                    preferences[ranks[i]][ranks[j]] += 1;
                }
                /* ranks[j] would be the higher rank than ranks[i] in this case */
                else
                {
                    /* else incrementing the votes for candidate present at ranks[j] over the one at ranks[i] */
                    preferences[ranks[j]][ranks[i]] += 1;
                }
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    /* looping over candidates again */
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i != j)
            {
                if (preferences[i][j] > preferences[j][i])
                {
                    pairs[pair_count].winner = i;
                    pairs[pair_count].loser = j;
                    pair_count += 1;
                }
                else if (preferences[i][j] < preferences[j][i])
                {
                    pairs[pair_count].winner = j;
                    pairs[pair_count].loser = i;
                    pair_count += 1;
                }
                /* excluding the case where they are equal */
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    /* variable for swapping */
    pair temp;
    /* iterating through pairs */
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = 0; j < pair_count; j++)
        {
            /* not wanting to sort the same pair :p */
            if (i != j)
            {
                /* if the place of first pair is lower */
                if (i > j)
                {
                    /* if they have more votes*/
                    if (preferences[pairs[i].winner][pairs[i].loser] > preferences[pairs[j].winner][pairs[j].loser])
                    {
                        /* swap them to a higher place */
                        temp = pairs[i];
                        pairs[i] = pairs[j];
                        pairs[j] = temp;
                    }
                }
                /* else if they are lower */
                else
                {
                    /* but have lesser votes */
                    if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[j].winner][pairs[j].loser])
                    {
                        /* swap them lower */
                        temp = pairs[j];
                        pairs[j] = pairs[i];
                        pairs[i] = temp;
                    }
                }
            }
        }
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

