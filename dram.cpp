#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <functional>
#include <ctime>
#include <cstdlib>

struct Event {
    double event_time;
    char event_type;
    int row, col;

    bool operator<(const Event& other) const {
        return event_time > other.event_time;
    }
};

class DRAMSimulator {
private:
    std::vector<std::vector<bool>> memory;
    std::priority_queue<Event> events;
    int rows, cols;
    int errors_encountered = 0;
    int errors_corrected = 0;
    double sim_time;
    double batch_duration;

    std::mt19937 rng;
    std::uniform_real_distribution<double> dist_time;
    std::uniform_int_distribution<int> dist_row, dist_col;

public:
    DRAMSimulator(int r, int c, double error_rate, long int num_accesses, double scrub_interval, double sim_t, int batches)
    : rows(r), cols(c), sim_time(sim_t), batch_duration(sim_t / batches), rng(std::random_device{}()), 
      dist_row(0, r - 1), dist_col(0, c - 1) {
        memory.resize(rows, std::vector<bool>(cols, false));
        double currentTime = 0;

        // Generate and process events in batches
        for (int batch = 0; batch < batches; ++batch) {
            generateEvents(currentTime, currentTime + batch_duration, error_rate, num_accesses / batches, scrub_interval);
            processEvents();
            currentTime += batch_duration;
        }
    }

    void generateEvents(double start_time, double end_time, double error_rate, long int num_accesses, double scrub_interval) {
        std::uniform_real_distribution<double> dist_batch_time(start_time, end_time);

        // Schedule errors
        int total_errors = static_cast<int>((error_rate * (end_time - start_time) / 3600));
        for (int i = 0; i < total_errors; ++i) {
            double time = dist_batch_time(rng);
            int row = dist_row(rng);
            int col = dist_col(rng);
            events.push(Event{time, 'e', row, col});
        }

        // Schedule accesses
        for (long int i = 0; i < num_accesses; ++i) {
            double time = dist_batch_time(rng);
            int row = dist_row(rng);
            int col = dist_col(rng);
            events.push(Event{time, 'a', row, col});
        }

        // Schedule scrubs
        for (int row = 0; row < rows; ++row) {
            for (double time = start_time; time < end_time; time += scrub_interval) {
                events.push(Event{time, 's', row, -1});
            }
        }
    }

    void processEvents() {
        while (!events.empty()) {
            Event event = events.top();
            events.pop();

            if (event.event_type == 'a') { 
                if (memory[event.row][event.col]) {
                    ++errors_encountered;
                    memory[event.row][event.col] = false;
                }
            } else if (event.event_type == 'e') { 
                memory[event.row][event.col] = true;
            } else if (event.event_type == 's') { 
                for (int col = 0; col < cols; ++col) {
                    if (memory[event.row][col]) {
                        ++errors_corrected;
                        memory[event.row][col] = false;
                    }
                }
            }
        }
    }

    void run_simulation() {
        std::cout << "Errors Encountered: " << errors_encountered << std::endl;
        std::cout << "Errors Corrected: " << errors_corrected << std::endl;
    }
};

int main(int argc, char* argv[]) {
    if (argc != 8) {
        std::cerr << "Usage: " << argv[0] << " rows cols error_rate num_accesses scrub_interval sim_time batches" << std::endl;
        return 1;
    }

    int rows = std::atoi(argv[1]);
    int cols = std::atoi(argv[2]);
    double error_rate = std::strtod(argv[3], nullptr);
    long int num_accesses = std::strtoll(argv[4], nullptr, 10);
    double scrub_interval = std::strtod(argv[5], nullptr);
    double sim_time = std::strtod(argv[6], nullptr);
    int batches = std::atoi(argv[7]);

    DRAMSimulator simulator(rows, cols, error_rate, num_accesses, scrub_interval, sim_time, batches);
    simulator.run_simulation();

    return 0;
}
