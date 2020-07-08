# mhierbench
**Mhierbench** (memory hierarchy benchmark) aims to reproduce the cache/DDR bandwidth and latency metrics of [AIDA64 Cache and memory benchmark](https://www.aida64.co.uk/user-manual/cache-and-memory-benchmark).
I try to implement this program using only C++ code and as less compiler/OS dependent features as possible, and the main goal is to reproduce a similar results as AIDA64 under most Linux systems.

# Methdology
## Cache/DDR Latency
Use the link list traversing techniques reported in lmbench.

## DDR bandwidth
sysbench can report a similar number as AIDA64; will investigate more on this.

## Cache bandwidth
Will investigate pmbw for more information.

# Results vs AIDA64
I validate the accuracy of mhierbench on as many machines as possible (2400G/3700X/8700/4700U/9300H),
and the results are compared with aida64\_reference/, which I collect from the Internet.
I am sorry that I forget to track the sources of the figures, please let me know if I use your figures improperly.

## Cache/DDR Latency
These figures show that mhierbench can accurately reflect the latency reported by AIDA64. The dotted lines are the L1/L2/L3 cache lines reported by AIDA64 while the blue lines are the numbers from against different memory footprint size.

**3700X**

**8700**

**2400G**

**4700U**

**9300H**

I also think that reporting the memory latency is not that meaningful due to the variance of cache sizes.
For example, while AIDA64 reports a similar 10.2-11.8 L3 latency for the five CPUs I've test,
the latency values at 16MB memory size varies from 18.4-108.3.
Instead of reporting the independent cache latency, I report the latency against different memory footprint size which is more normalized, but you can still estimate the independent memory latency from the results.

## DDR bandwidth
TODO (sysbench)

## Cache bandwidth
TODO
