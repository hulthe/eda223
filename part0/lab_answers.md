## Problem 3.a
 Consider that alternating ’1’s and ’0’s are written to the DAC
so that a tone of frequency 200 Hz is produced. The period of the tone-
generating task is equal to one of the following. Tick the correct one. [Hint:
1 sec = 10^3 ms =10^6 μs]

**Answer: 2500µs**


## Problem 3.b
Consider that the DAC is written with alternating ’1’s and
’0’s so that a tone of frequency 1 kHz is produced. Find the period of the
tone-generating task.

**Answer: 500µs**


## Problem 4.a
Consider that the DAC is written with alternating ’1’s and
’0’s so that a tone with base frequency p(0) is produced. Find the period of
the tone-generating task. p(0) = 440 Hz

**Answer: 1136µs**


## Problem 4.b
Given that p(0) = 440 Hz, the frequency p(k) for index k
can be computed using Equation (1). Complete the following expression for
frequency p(k) as a function of frequency index k:

**Answer: p(k) = 440 x 2^(k/12)**


## Problem 4.c
 As mentioned before the period of the tone-generating task
must be 1 sec in order to produce a tone of frequency f Hz. What should
the period 2f of the tone-generating task be when (a) the frequency index is −7,
and (b) the frequency index is 9?


**Answer: (see table at the 5.b)**


## Problem 4.d
Add an integer array of length 32 to your application code
and copy into it the 32 frequency indices for the Brother John tune, in the
order given by Table 1.

0 2 4 0 0 2 4 0 4 5 7 4 5 7 7 9 7 5 4 0 7 9 7 5 4 0 0 -5 0 0 -5 0

## Problem 4.e
Inspect the 32 frequency indices given in Table 1 and find the
maximum and minimum frequency indices.

**Answer: min -5 max 9**

## Problem 5.a
What are the first 10 frequency indices of the Brother John
tune for key = −5?

```
[-5, -3, -1, -5, -5, -3, -1, -5, -1, 0, 2, -1, 0, 2, 2, 4, 2, 0, -1, -5, 2, 4, 2, 0, -1, -5, -5, -10, -5, -5, -10, -5]
```

## Problem 5.b
Inspect the 32 frequency indices given for key=0.  If the key can  be  within  the  range  of[−5...5],  what  is  the  maximum  and  minimum frequency index of the Brother John tune for any given key?

**Answer: max 14 min -10**

## Problem 5.b
| index | freq | period (s) | period (µs) |
|-------|------|------------|-------------|
|   -10 | 246,9416506 | 0,00202477  | 2024,769814 |
|    -9 | 261,6255653 | 0,001911128 | 1911,128216 |
|    -8 | 277,182631  | 0,001803865 | 1803,864832 |
|    -7 | 293,6647679 | 0,001702622 | 1702,621678 |
|    -6 | 311,1269837 | 0,001607061 | 1607,060866 |
|    -5 | 329,6275569 | 0,001516863 | 1516,863471 |
|    -4 | 349,2282314 | 0,001431728 | 1431,728466 |
|    -3 | 369,9944227 | 0,001351372 | 1351,371722 |
|    -2 | 391,995436  | 0,001275525 | 1275,525055 |
|    -1 | 415,3046976 | 0,001203935 | 1203,935334 |
|     0 | 440         | 0,001136364 | 1136,363636 |
|     1 | 466,1637615 | 0,001072584 | 1072,584446 |
|     2 | 493,8833013 | 0,001012385 | 1012,384907 |
|     3 | 523,2511306 | 0,000955564 | 955,5641082 |
|     4 | 554,365262  | 0,000901932 | 901,9324159 |
|     5 | 587,3295358 | 0,000851311 | 851,3108391 |
|     6 | 622,2539674 | 0,00080353  | 803,5304332 |
|     7 | 659,2551138 | 0,000758432 | 758,4317353 |
|     8 | 698,4564629 | 0,000715864 | 715,8642329 |
|     9 | 739,9888454 | 0,000675686 | 675,6858608 |
|    10 | 783,990872  | 0,000637763 | 637,7625274 |
|    11 | 830,6093952 | 0,000601968 | 601,9676672 |
|    12 | 880         | 0,000568182 | 568,1818182 |
|    13 | 932,327523  | 0,000536292 | 536,2922231 |
|    14 | 987,7666025 | 0,000506192 | 506,1924535 |



## Problem 5.d
In the C programming language, array indices cannot be
negative. Consequently, the indices for the new array will not correspond
directly to the frequency indices, but will have to be translated. To that end,
when the frequency index is k (where min index ≤ k ≤ max index), the period
array element period[x] needs to be accessed (where x ≥ 0).

**Answer: x(k) = k+10**