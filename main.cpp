#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <sstream>
#include <iomanip>

// A simple implementation of a "Block" in a Blockchain (Bitcoin's core concept)
// This uses a Linked List structure where each block points to the previous block's hash.

class Block {
public:
    int index;
    long long timestamp;
    std::string data;
    std::string previousHash;
    std::string hash;
    int nonce;

    Block(int idx, const std::string& d, const std::string& prevHash) 
        : index(idx), data(d), previousHash(prevHash), nonce(0) {
        timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        hash = calculateHash();
    }

    // A mock SHA-256 hash function for demonstration (actual SHA-256 would need a library)
    std::string calculateHash() const {
        std::stringstream ss;
        ss << index << timestamp << data << previousHash << nonce;
        std::string input = ss.str();
        
        // Simple hashing logic for demonstration
        unsigned long hashVal = 5381;
        for (char c : input) {
            hashVal = ((hashVal << 5) + hashVal) + c;
        }
        
        std::stringstream res;
        res << std::hex << std::setw(16) << std::setfill('0') << hashVal;
        return res.str();
    }

    // "Mining" simulation: increment nonce until hash starts with zeros
    void mineBlock(int difficulty) {
        std::string target(difficulty, '0');
        while (hash.substr(0, difficulty) != target) {
            nonce++;
            hash = calculateHash();
        }
        std::cout << "Block Mined! Hash: " << hash << std::endl;
    }
};

class Blockchain {
private:
    std::vector<Block> chain;
    int difficulty;

public:
    Blockchain() {
        difficulty = 2; // Simple difficulty for demo
        // Genesis Block
        chain.emplace_back(0, "Genesis Block (Bitcoin Start)", "0");
    }

    void addBlock(const std::string& data) {
        std::cout << "Mining new block for: " << data << "..." << std::endl;
        Block newBlock(chain.size(), data, chain.back().hash);
        newBlock.mineBlock(difficulty);
        chain.push_back(newBlock);
    }

    void displayChain() const {
        std::cout << "\n--- Bitcoin Blockchain Visualization ---\n";
        for (const auto& block : chain) {
            std::cout << "Index: " << block.index << "\n"
                      << "Data: " << block.data << "\n"
                      << "Hash: " << block.hash << "\n"
                      << "Prev: " << block.previousHash << "\n"
                      << "Nonce: " << block.nonce << "\n"
                      << "----------------------------------------\n";
        }
    }

    bool isChainValid() const {
        for (size_t i = 1; i < chain.size(); ++i) {
            const Block& currentBlock = chain[i];
            const Block& prevBlock = chain[i - 1];

            if (currentBlock.hash != currentBlock.calculateHash()) return false;
            if (currentBlock.previousHash != prevBlock.hash) return false;
        }
        return true;
    }
};

int main() {
    Blockchain bitcoinDemo;

    std::cout << "Starting Bitcoin-themed DSA Project...\n";
    
    bitcoinDemo.addBlock("Transaction: Alice -> Bob 0.5 BTC");
    bitcoinDemo.addBlock("Transaction: Bob -> Charlie 0.1 BTC");

    bitcoinDemo.displayChain();

    std::cout << "Is Blockchain Valid? " << (bitcoinDemo.isChainValid() ? "Yes" : "No") << "\n";

    return 0;
}
