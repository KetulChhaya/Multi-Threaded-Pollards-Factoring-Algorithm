import csv
import subprocess

C_PROGRAM_PATH = "./pollards_rho"
CSV_FILE = "all-moduli.csv"

def read_csv(file_path):
    moduli_list = []
    with open(file_path, "r") as file:
        reader = csv.DictReader(file, delimiter=",")    
        for i, row in enumerate(reader):
            if i < 31:  # Skip the rows for custom output
                continue
            if row["ModulusName"] and row["Modulus"]:
                moduli_list.append((row["ModulusName"], row["Modulus"]))
    return moduli_list

# Function to execute Pollards Methods
def run_c_program(modulus_name, modulus_value):
    try:
        print(f"Running for {modulus_name} with modulus {modulus_value}")
        subprocess.run([C_PROGRAM_PATH, modulus_value], check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error processing {modulus_name}: {e}")

if __name__ == "__main__":
    moduli = read_csv(CSV_FILE)
    for name, value in moduli:
        # print(name, value)
        run_c_program(name, value)
