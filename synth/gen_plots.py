import numpy as np
import matplotlib.pyplot as plt

with open('comparison_cvc4_pebbles.txt') as f:
    lines = f.readlines()
    indices = range(len(lines))
    cvc4_times = [float(line.split()[0]) for line in lines]
    pebbles_times = [float(line.split()[1]) for line in lines]

with open('multithread.txt') as f:
    lines = f.readlines()
    pebbles_times_mt = [float(line) for line in lines]

with open('gpu_version.txt') as f:
    lines = f.readlines()
    pebbles_times_gpu = [float(line) for line in lines if not line.startswith("Synthesizer")]

with open('found.txt') as f:
    lines = f.readlines()
    has_solution = ['T' in line for line in lines]

# Instead of directly sorting, get an array of what the indices should be to sort cvc4_times
# Then we sort both cvc4_times and pebbbles_times according to that order
sort_order = np.argsort(cvc4_times)
cvc4_times = np.array(cvc4_times)[sort_order]
pebbles_times = np.array(pebbles_times)[sort_order]
pebbles_times_mt = np.array(pebbles_times_mt)[sort_order]
pebbles_times_gpu = np.array(pebbles_times_gpu)[sort_order]

cvc4_times_solved = cvc4_times[has_solution]
pebbles_times_solved = pebbles_times[has_solution]

cvc4_times_no_solution = cvc4_times[np.invert(has_solution)]
pebbles_times_no_solution = pebbles_times[np.invert(has_solution)]

plt.yscale('log')
plt.plot(cvc4_times_solved,'ro--',linewidth=0.5, markersize=1.5, label="cvc4")
plt.plot(pebbles_times_solved,'go--',linewidth=0.5, markersize=1.5, label="pebbles")
plt.ylabel("Execution time in seconds")
plt.xlabel("SyGuS CrCi problems with solutions")
plt.title("Log-scale comparison of CVC4 1.8 and Pebbles")
plt.xticks([], []) # turn off x-ticks
leg = plt.legend(loc='upper left')
plt.savefig('cvc4_versus_pebbles_solved.png')
plt.show()

plt.yscale('log')
plt.plot(cvc4_times_no_solution,'ro--',linewidth=0.5, markersize=1.5, label="cvc4")
plt.plot(pebbles_times_no_solution,'go--',linewidth=0.5, markersize=1.5, label="pebbles")
plt.ylabel("Execution time in seconds")
plt.xlabel("SyGuS CrCi problems with no solution")
plt.title("Log-scale comparison of CVC4 1.8 and Pebbles")
plt.xticks([], []) # turn off x-ticks
leg = plt.legend(loc='upper left')
plt.savefig('cvc4_versus_pebbles_unsolved.png')
plt.show()

plt.yscale('log')
plt.plot(pebbles_times_mt,'ro--',linewidth=0.5, markersize=1.5, label="multithread")
plt.plot(pebbles_times,'go--',linewidth=0.5, markersize=1.5, label="singlethread")
plt.plot(pebbles_times_gpu,'bo--',linewidth=0.5, markersize=1.5, label="gpu")
plt.ylabel("Execution time in seconds")
plt.xlabel("SyGuS CrCi problems with solutions")
plt.title("Comparison of Pebbles with single thread versus multithread")
plt.xticks([], []) # turn off x-ticks
leg = plt.legend(loc='upper left')
plt.savefig('single_versus_multi.png')
plt.show()
