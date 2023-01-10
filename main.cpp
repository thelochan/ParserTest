#include <fstream>
#include <iostream>
#include <vector>
#include <bitcoin/bitcoin.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

int main()
{
    // Read the block file
    std::ifstream file("path/to/blockfile", std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file" << std::endl;
        return 1;
    }

    // Read the block header
    bc::block_header header;
    file >> header;

    // Read the transactions
    std::vector<bc::transaction> transactions;
    bc::transaction tx;
    while (file >> tx) {
        transactions.push_back(tx);
    }

    // Create a vector to hold the transaction hashes
    std::vector<bc::hash_digest> tx_hashes;
    for (const bc::transaction& tx : transactions) {
        tx_hashes.push_back(tx.hash());
    }

    // Calculate the Merkle root
    bc::hash_digest merkle_root = bc::bitcoin_merkle(tx_hashes);

    // Check the Merkle root against the one recorded in the block header
    if (header.merkle != merkle_root) {
        std::cout << "Warning: Merkle root in block header does not match calculated root!" << std::endl;
    }

    // Verify the proof-of-work on the block
    bc::hash_digest hash = header.hash();
    if (!bc::check_proof_of_work(hash, header.bits)) {
        std::cout << "Error: Block failed proof-of-work check!" << std::endl;
        return 1;
    }

    // Calculate the total amount of bitcoins in the block
    uint64_t total_btc = 0;
    for (const bc::transaction& tx : transactions) {
        for (const bc::transaction_input& input : tx.inputs) {
            total_btc -= input.value();
        }
        for (const bc::transaction_output& output : tx.outputs) {
            total_btc += output.value();
        }
    }
    double total_btc_in_btc = total_btc / bc::satoshi_per_bitcoin;

    // Prepare JSON output
    json block_json;
    block_json["hash"] = bc::encode_hex(hash);
    block_json["merkle_root"] = bc::encode_hex(merkle_root);
    block_json["total_btc"] = total_btc_in_btc;
    block_json["transactions"] = json::array();
    for (const bc::transaction& tx : transactions) {
        json tx_json;
        tx_json["hash"] = bc::encode_hex(tx.hash());
        block_json["transactions"].push_back(tx_json);
    }
    std::cout << block
    
    // Prepare CSV output
    std::stringstream csv;
    csv << "hash,total_btc,transactions" << std::endl;
    csv << bc::encode_hex(hash) << "," << total_btc_in_btc << ",";
    for (const bc::transaction& tx : transactions) {
        csv << bc::encode_hex(tx.hash()) << ",";
    }
    csv << std::endl;
    std::cout << csv.str();
