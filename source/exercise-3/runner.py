import subprocess
import re
import numpy as np
import matplotlib.pyplot as plt


def runAndGetResult(programName: str, threadCount: str):
    # Matches floats/integers located strictly at the end of each line
    pattern = r"\d+\.\d+"

    processResult = subprocess.run(
        [f"./{programName}"], capture_output=True, text=True, env={"OMP_NUM_THREADS": threadCount}
    )

    # Extract all floating-point numbers across lines
    matches = re.findall(pattern, processResult.stdout, flags=re.MULTILINE)

    # Convert extracted strings to floats inside a NumPy array
    return [float(match) for match in matches]


# Compile the program (just in case)
def compile(programName: str):
    subprocess.run(["g++", "-std=c++20", "-O3", "-fopenmp", f"{programName}.cpp", "-o", f"{programName}"])

program = "exercise3"

compile(program)

threadCounts = np.arange(2, 16 + 1, 1)

resultCase1 = []
resultCase2 = []
resultCase3 = []

for threadCount in threadCounts:
    resultCase1iter = []
    resultCase2iter = []
    resultCase3iter = []
    for i in range(5):
        results = runAndGetResult(program, str(threadCount))
        resultCase1iter.append(results[0])
        resultCase2iter.append(results[1])
        resultCase3iter.append(results[2])
    resultCase1.append(np.mean(resultCase1iter))
    resultCase2.append(np.mean(resultCase2iter))
    resultCase3.append(np.mean(resultCase3iter))


fig, ax = plt.subplots()
ax.plot(threadCounts, resultCase1, label="Outermost loop parallelized")
ax.plot(threadCounts, resultCase2, label="Outer two loops parallelized")
ax.plot(threadCounts, resultCase3, label="All loops parallelized")

box = ax.get_position()
ax.set_position([box.x0, box.y0 + box.height * 0.1,
                     box.width, box.height * 0.9])

ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.05),
              fancybox=True, shadow=True, ncol=5)
plt.savefig(f"exercise3.png", dpi=400, bbox_inches='tight')