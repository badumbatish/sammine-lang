import subprocess
import os

def run_command(command):
    subprocess.run(command, shell=True, check=True)

# Step 1: Create build files
run_command("cmake -S . -B build")

# Step 2: Build the project
run_command("cmake --build build -j10")

# Step 3: Run the main executable
os.chdir("build")
run_command("./sammine")
os.chdir("..")

# Step 4: Run tests
os.chdir("build")
run_command("ctest")
os.chdir("..")
