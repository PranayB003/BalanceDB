# BalanceDB
A simple key-value store built on top of a Chord implementation that is retrofitted with 
a Runtime Verification (RV) based module that provides better storage load balancing. The "power of
2 choices" scheme has been used to facilitate better load balancing, and the system allows the selection 
of any number of choices in general, not just two.

## Build
Clone the project and run `make` to build the program.
```bash
git clone https://github.com/PranayB003/BalanceDB.git
cd BalanceDB
make # 'make debug' to compile with debug info for GDB
```

## Usage
The program provides a CLI for interacting with the chord node. Run `help` in the CLI to list available commands.
In addition, the following options may be passed while invoking the program:
- `mode=create | join,IP,PORT`: Creates a new Chord ring, or joins an existing ring in which the node IP:PORT is a member.
- `port=PORT`: The port used by this node to communicate with other nodes in the ring.
- `http=PORT`: Start a HTTP server on the given port. Useful for connecting clients to run automated tests and benchmarks.
- `choices=NUM`: The number of hash functions used by the protocol to map a key to possible destination nodes. The least loaded
node is chosen.

Example:
```bash
./build/prog mode=create port=10000 http=8080 choices=2
```

The `samples/` directory contains a few useful scripts for testing and evaluation. The bulk of the evaluation of this project
was done using the [Yahoo! Cloud Serving Benchmark (YCSB)](https://github.com/brianfrankcooper/YCSB). The YCSB client bindings and usage 
instructions for BalanceDB can be found [here](https://github.com/PranayB003/YCSB/tree/master/balancedb).

Example:
```bash
cd ./samples

# Insert the key-value pairs in input.txt
./insert.sh 50-input.txt http://localhost:8080 

# Get the number of keys stored at each node and clear their state
./num-keys.sh localhost:8080 localhost:8081 
```
