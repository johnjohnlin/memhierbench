#!python
import numpy as np
from matplotlib import pyplot as plt
import sys
lines = [line.strip() for line in sys.stdin.readlines()]
lines = np.array([float(line) for line in lines if line])
xtick = ["1K","2K","4K","8K","16K","32K","64K","128K","256K","512K","1M","2M","4M","8M","16M","32M","64M",]
plt.figure(figsize=(9,4))
plt.axhline(lines[-4], label="AIDA64", color="k", linestyle="--")
for aida in lines[-3:]:
	plt.axhline(aida, color="k", linestyle="--")
plt.plot(range(len(xtick)), lines[:-4], ".-", label="Ours")
plt.ylim(ymax=128, ymin=0.5)
plt.ylabel("Latency (ns)")
plt.xticks(range(len(xtick)), xtick)
plt.yscale('log', basey=2)
plt.legend(framealpha=1, loc=2)
plt.tight_layout()
plt.savefig('out.png')
