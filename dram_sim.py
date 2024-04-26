import heapq
import random

class Event:
    def __init__(self, time, type, location=None):
        self.time = time
        self.type = type
        self.location = location 
    
    def __lt__(self, other):
        return self.time < other.time

class DRAMSimulator:
    def __init__(self, rows, cols, errors, scrub_interval, sim_time, access_rate):
        self.dram = [[0 for _ in range(cols)] for _ in range(rows)]
        self.rows = rows
        self.cols = cols
        self.errors = errors
        self.scrub_interval = scrub_interval
        self.sim_time = sim_time
        self.access_rate = access_rate 
        self.events = []
        self.errors_encountered = 0
        self.errors_corrected = 0

    def introduce_error(self):
        row = random.randint(0, self.rows - 1)
        col = random.randint(0, self.cols - 1)
        self.dram[row][col] = 1 

    def memory_access(self, location):
        row, col = location
        if self.dram[row][col] == 1:
            self.errors_encountered += 1
            self.dram[row][col] = 0 

    def scrub(self):
        for row in range(self.rows):
            for col in range(self.cols):
                if self.dram[row][col] == 1:
                    self.dram[row][col] = 0
                    self.errors_corrected += 1

    def schedule_events(self):
        for _ in range(self.errors):
            time = random.uniform(0, self.sim_time)
            heapq.heappush(self.events, Event(time, 'error'))
        
        num_accesses = int(self.access_rate * self.sim_time)
        for _ in range(num_accesses):
            time = random.uniform(0, self.sim_time)
            row = random.randint(0, self.rows - 1)
            col = random.randint(0, self.cols - 1)
            heapq.heappush(self.events, Event(time, 'access', (row, col)))
        
        for time in range(0, self.sim_time, self.scrub_interval):
            heapq.heappush(self.events, Event(time, 'scrub'))

    def run_simulation(self):
        self.schedule_events()
        while self.events:
            event = heapq.heappop(self.events)
            self.current_time = event.time
            if event.type == 'error':
                self.introduce_error()
            elif event.type == 'access':
                self.memory_access(event.location)
            elif event.type == 'scrub':
                self.scrub()

        print(f'Errors Encountered: {self.errors_encountered}')
        print(f'Errors Corrected: {self.errors_corrected}')

# Parameters for the simulation
rows, cols = 1024, 1024
errors = 10
scrub_interval = 5
sim_time = 3600
access_rate = 100000

simulator = DRAMSimulator(rows, cols, errors, scrub_interval, sim_time, access_rate)
simulator2 = DRAMSimulator(rows, cols, errors, 10, sim_time, access_rate)
simulator3 = DRAMSimulator(rows, cols, errors, 20, sim_time, access_rate)
simulator4 = DRAMSimulator(rows, cols, errors, 40, sim_time, access_rate)
simulator4 = DRAMSimulator(rows, cols, errors, 60, sim_time, access_rate)
simulator5 = DRAMSimulator(rows, cols, errors, 120, sim_time, access_rate)
simulator6 = DRAMSimulator(rows, cols, errors, 300, sim_time, access_rate)

simulator.run_simulation()
simulator2.run_simulation()
simulator3.run_simulation()
simulator4.run_simulation()
simulator5.run_simulation()
simulator6.run_simulation()
