# [CFR](https://www.pbinfo.ro/probleme/4145/cfr)

This is the first problem that I created for a programming competition (and one of the two created for this particular competition, specifically RAU-Coder).

## Problem statement 

As an input, you are given a **weigthed tree** (having positive edge weights which are less than or qual to $10$<sup>$9$</sup>) with $N$ nodes and $Q$ events. For each event $i$ you are given $2$ positive integer parameters, $M$<sub>$i$</sub> (which is also less than or qual to $10$<sup>$9$</sup>) and $K$<sub>$i$</sub> (which is less than or qual to $N$), which mean that you have at your disposal a tree that only contains the edges from the initial one that a have weight which is less than or equal to $M$<sub>$i$</sub>. From now on, we will call a connected component of this new structure a **city**. 

A kid is playing with this newly created structure for each day and he can choose from each city only its **longest simple path** (the length of a path is the **number of edges it has** and **it has nothing to do with the weights of edges**). If there are more of them, the kid can only choose one and it does not matter how he does that. This kid also owns $K$<sub>$i$</sub> additional edges. His purpose is to create a path each day that is **as long as possible** by merging the paths extracted from cities with his additional $K$<sub>$i$</sub> (he does not need to use all of his edges, but the unused ones will not carry out for other days).

The problem asks for the **length of the longest path** attainable for each day - first task - and also for the **number of different ways** you can get that modulo $10$<sup>$9$</sup>$+7$ (two ways are different if the two **subsets of cities** from where the paths were selected are different) - second task.

The expected time complexity of a solution that can solve this problem for a full score is $O((N + Q) * log(N))$ with a **linear** memory complexity.

## Solution
