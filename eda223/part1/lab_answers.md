## problem 2.a
With the tone generator producing a 1 kHz tone increase the
value of variable background loop range from 1000 to 8000. What happens
with the generated tone as you increase the background load?

**Answer: It becomes distorted**

## problem 2.b
Repeat the procedure in Problem 2.a, but let the tone gener-
ator task produce a 769 Hz tone. Is the distortion different now? If so, why?
What happens with the generated tone as you increase the background load?

**Answer: With a period of 650µs, the tone fades very fast when increasing background load**

## problem 2.C
Repeat the procedure in Problem 2.a, but let the tone generator task produce a 537 Hz tone.
 Is the distortion different now?  If so, why? What happens with the generated tone as
 you increase the background load?

**Answer: witha period of 931, the tone sounds like its pulsating**

## problem 3.b
With deadlines enabled for the two tasks, and the tone generator producing a 1 kHz tone, 
use the load control keys to increase the valueof variablebackgroundlooprangefrom 1000 to 8000.  
What happens withthe generated tone as you change the background load?

**Answer: The tone remains unchanged**

## problem 3.c
With deadlines enabled for the two tasks, and the tone generator producing a 1 kHz tone, 
increase the background load beyond the range used in Problem 3.b until you hear a pitch drop 
effect where the tone generator starts producing a tone with a lower frequency.  What is the 
value of variable background_loop_range when the pitch drop effect begins to be noticeable?

**Answer: 13500**

## problem 4.a
Measure  the  WCET  of  the  background  task  by  running  itin  isolation,  that  is,  
without running the tone generator task.   The  variable background_loop_range should have the 
value that you identified in Problem 3.c for the pitch drop effect.  Perform the measurement 
by running the taska  large  number  of  times,  as  described  above.   What  are  the  
values  for  themaximum WCET and average WCET, respectively?

**Answer: WCET Maximum: 1290µs Average: 1289µs**

## problem 4.b
Measure  the  WCET  of  the  background  task  by  running  itin  isolation,  that  is,  
without running the tone generator task.   The variable background_loop_range should have a value of 1000.  
Perform the measure-ment by running the task a large number of times, as described above.  
What are the values for the maximum WCET and average WCET, respectively?

**Answer: WCET Maximum: 100µs Average: 100µs**

## problem 4.c
Measure the WCET of the tone generator task by running it in isolation, that is, 
without running the background task.  Perform the measurement by running the task a large number of times,  
as described above. What are the values for the maximum WCET and average WCET, respectively?

**Answer: WCET Maximum: 200ns Average: 195ns**
