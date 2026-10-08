import subprocess
import re
import numpy as np
import matplotlib.pyplot as plt


def runAndGetResult(programName: str, schedule: str):
    # Matches floats/integers located strictly at the end of each line
    pattern = r"\d+\.\d+"

    processResult = subprocess.run(
        [f"./{programName}"], capture_output=True, text=True, env={"OMP_SCHEDULE": schedule}
    )

    # Extract all floating-point numbers across lines
    matches = re.findall(pattern, processResult.stdout, flags=re.MULTILINE)

    # Convert extracted strings to floats inside a NumPy array
    return float(matches[0])


# Compile the program (just in case)
def compile(programName: str):
    subprocess.run(["g++", "-std=c++20", "-O3", "-fopenmp", f"{programName}.cpp", "-o", f"{programName}"])


programNames = ["exercise4Column", "exercise4Row"]
schedules = ["static", "dynamic", "guided"]

results = []


for program in programNames:
    compile(program)
    print(program)
    print("------------------------------------")
    for schedule in schedules:
        runs = []
        for i in range(1):
            runs.append(runAndGetResult(program, schedule))
        average = np.mean(runs)
        print(f"Schedule: {schedule} duration: {average.round(3)}")
    print("\n\n")
