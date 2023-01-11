# GPP Research topic
## A chess AI using minmax

## MinMax algorithm

the min max algorithm will go through every possibility in a certain set depth, and will return the best possibility for the set depth.

it does this by comparing which possibilities are more advantageous than others, it gives a value to it, to compare these and will do this for every depth (depth as in step after next step) and will then give the possibility that will lead to the path with the most gain/least loss.

## Alpha Beta pruning

Min max checks every possibility until the set depth and this can have some performace issues and also can mean that it checks an irrelevant possibility, alpha pruning makes it so that if a possibility leads to for example a max loss of 8 in the second step, then if another possibility that from step one step to leads to a loss of 10, it will not check all the other options for that posibility

