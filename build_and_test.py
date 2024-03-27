import subprocess
import os
import argparse

def run_command(command):
    subprocess.run(command, shell=True, check=True)

def main():

    core_count_default = 8
    parser = argparse.ArgumentParser(description="Development cycle automation script.")


    parser.add_argument("-j", "--cores", type=int, default=core_count_default, help=f"Number of cores for cmake build, default = {core_count_default} ")
    args = parser.parse_args()

    # Step 1: Create build files
    run_command("cmake -S . -B build")

    # Step 2: Build the project
    run_command(f"cmake --build build -j{args.cores}")

    # Step 3: Run the main executable
    # os.chdir("build")
    # run_command("./sammine")
    # os.chdir("..")

    # Step 4: Run tests
    os.chdir("build")
    run_command("ctest --output-on-failure")
    os.chdir("..")

if __name__ == "__main__":
    main()
