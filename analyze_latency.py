import csv
import statistics
import matplotlib.pyplot as plt

def analyze_and_plot(csv_file):
    with open(csv_file, 'r') as f:
        reader = csv.reader(f)
        latencies = [int(row[0]) for row in reader]

    if not latencies:
        print("No data found.")
        return

    # Statistical Analysis
    latencies.sort()
    median = statistics.median(latencies)
    p99 = latencies[int(len(latencies) * 0.99)]
    
    print(f"Median: {median} cycles")
    print(f"P99:    {p99} cycles")

    # Plotting
    plt.hist(latencies, bins=50, color='skyblue', edgecolor='black')
    plt.title('HFT Engine Latency Distribution')
    plt.xlabel('Cycles')
    plt.ylabel('Frequency')
    plt.savefig('latency_histogram.png')
    print("Histogram saved as 'latency_histogram.png'")

if __name__ == "__main__":
    analyze_and_plot('latency_results.csv')