import time
import sys

def main():
    print("Dummy App started. Generating logs...")
    counter = 0
    while True:
        counter += 1
        if counter % 3 == 0:
            # Purposefully generate an error message for testing
            print("ERROR: Database Connection Error - Unable to connect to host 'db:5432'", file=sys.stderr)
        else:
            print(f"INFO: Application is running smoothly. Iteration {counter}")
        
        sys.stdout.flush()
        sys.stderr.flush()
        time.sleep(10)

if __name__ == "__main__":
    main()
