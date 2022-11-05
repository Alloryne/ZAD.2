#include "hash.h"

#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <cstdio>
#include <vector>

namespace jnp1 {

namespace {
#ifdef NDEBUG
  const bool debug = false;
#else
  const bool debug = true;
#endif
  using string = std::string;

    struct HashingObject {
    public:
    hash_function_t function;
    HashingObject(hash_function_t function) {
        this->function = function;
    }
    uint64_t operator()( std::vector<uint64_t> const seq1)
                        const {
        return function(seq1.data(), seq1.size());
    }
    };


using hash_array = std::unordered_set<
                    std::vector<uint64_t>,
                    HashingObject>;
using hash_array_map = std::unordered_map<unsigned long, hash_array *>;

//unsigned long next_usable_identifier = 0;

hash_array_map &all_arrays() {
    static hash_array_map all_arrays;
    return all_arrays;
}
//hash_array_map all_arrays;

/*
string arg_id_to_string(const unsigned long id){
  return std::to_string(id);
}
string arg_size_to_string(size_t size){
  return std::to_string(size);
}*/

string arg_seq_to_string(uint64_t const * seq, size_t size){

  if(seq == NULL){
    string arg = "NULL";
    return arg;
  }

  else{
    string arg = "\"";
    if(size > 0){
      string tmp = std::to_string(*(seq));
      arg += tmp;
    }
    for(size_t i = 1; i < size; i++){
      string tmp = std::to_string(*(seq+i));
      arg = arg + " " + tmp;
    }
    arg += "\"";
    return arg;
  }
}

void functionInfo(const string funcName, hash_function_t hash_function){
  if(debug){
    std::cerr << funcName << "(" << &hash_function << ")" << "\n";
  }
}

void functionInfo(const string funcName, const unsigned long id){
  if(debug){
    //const string args = arg_id_to_string(id);
    std::cerr << funcName << "(" << id << ")" << "\n";
  }
}

void functionInfo(const string funcName, unsigned long id, uint64_t const * seq, size_t size){
  if(debug){
    //const string arg1 = arg_id_to_string(id);
    const string arg2 = arg_seq_to_string(seq, size);
    //const string arg3 = arg_size_to_string(size);
    std::cerr << funcName << "(" << id << ", " << arg2 << ", " << size << ")" << "\n";
  }
}

void hash_create_log(unsigned long id){
  if(debug)
    std::cerr << "hash_create: hash table #" << id << " created\n";
}

void hash_delete_log(unsigned long id){
  if(debug){
    if(all_arrays().count(id) == 0){
      std::cerr << "hash_delete: hash table #" << id << " does not exist\n";
    }
    else{
      std::cerr << "hash_delete: hash table #" << id << " deleted\n";
    }
  }
}

void hash_size_log(unsigned long id, size_t x){
  if(debug){
    if(all_arrays().count(id) == 0){
      std::cerr << "hash_size: hash table #" << id << " does not exist\n";
    }
    else{
      std::cerr << "hash_size: hash table #" << id << " contains " << x << " element(s)\n";
    }
  }
}

void hash_insert_log(unsigned long id, uint64_t const * seq, size_t size, bool is_inserted){
  if(debug){
    if(seq == NULL){
      std::cerr << "hash_insert: invalid pointer (NULL)\n";
    }
    if(size == 0){
      std::cerr << "hash_insert: invalid size (0)\n";
    }
    if(seq == NULL || size == 0)
      return;
    if(all_arrays().count(id) == 0){
      std::cerr << "hash_insert: hash table #" << id << " does not exist\n";
    }
    else{

      string args = arg_seq_to_string(seq, size);
      if(is_inserted){
        std::cerr << "hash_insert: hash table #" << id << ", sequence " << args <<" inserted\n";
      }
      else{
        std::cerr << "hash_insert: hash table #" << id << ", sequence " << args <<" was present\n";
      }
    }

  }
}

void hash_remove_log(unsigned long id, uint64_t const * seq, size_t size, bool is_removed){
  if(debug){
    if(seq == NULL){
      std::cerr << "hash_remove: invalid pointer (NULL)\n";
    }
    if(size == 0){
      std::cerr << "hash_remove: invalid size (0)\n";
    }
    if(seq == NULL || size == 0)
      return;
    if(all_arrays().count(id) == 0){
      std::cerr << "hash_remove: hash table #" << id << " does not exist\n";
    }
    else{


      string args = arg_seq_to_string(seq, size);
      if(is_removed){
        std::cerr << "hash_remove: hash table #" << id << ", sequence " << args <<" removed\n";
      }
      else{
        std::cerr << "hash_remove: hash table #" << id << ", sequence " << args <<" was not present\n";
      }
    }

  }
}

void hash_clear_log(unsigned long id, bool was_empty){
  if(debug){
    if(all_arrays().count(id) == 0){
      std::cerr << "hash_clear: hash table #" << id << " does not exist\n";
    }
    else{
        if(was_empty){
          std::cerr << "hash_clear: hash table #" << id << " was empty\n";
        }
        else{
          std::cerr << "hash_clear: hash table #" << id << " cleared\n";
        }
    }
  }
}

void hash_test_log(unsigned long id, uint64_t const * seq, size_t size, bool is_present){
  if(debug){
    if(seq == NULL){
      std::cerr << "hash_test: invalid pointer (NULL)\n";
    }
    if(size == 0){
      std::cerr << "hash_test: invalid size (0)\n";
    }
    if(seq == NULL || size == 0)
      return;
    if(all_arrays().count(id) == 0){
      std::cerr << "hash_test: hash table #" << id << " does not exist\n";
    }
    else{
      string args = arg_seq_to_string(seq, size);
      if(is_present){
        std::cerr << "hash_test: hash table #" << id << ", sequence " << args << " is present\n";
      }
      else{
        std::cerr << "hash_test: hash table #" << id << ", sequence " << args << " is not present\n";
      }
    }

  }
}

}
// Tworzy tablicę haszującą i zwraca jej identyfikator. Parametr
// hash_function jest wskaźnikiem na funkcję haszującą, która daje w wyniku
// liczbę uint64_t i ma kolejno parametry uint64_t const * oraz size_t.

unsigned long hash_create(hash_function_t hash_function) {
    const string funcName = "hash_create";
    functionInfo(funcName, hash_function);
    static uint64_t next_usable_identifier = 0;
    uint64_t usable_identifier = next_usable_identifier;
    next_usable_identifier++;
    HashingObject Hashing = HashingObject(hash_function);
    hash_array *array = new hash_array(0, Hashing);
    all_arrays().insert({usable_identifier, array});
    hash_create_log(usable_identifier);
    return usable_identifier;
}

// Usuwa tablicę haszującą o identyfikatorze id, o ile ona istnieje.
// W przeciwnym przypadku nic nie robi.

void hash_delete(unsigned long id) {
    const string funcName = "hash_delete";
    functionInfo(funcName, id);
    hash_delete_log(id);
    all_arrays().erase(id);
}

// Daje liczbę ciągów przechowywanych w tablicy haszującej
// o identyfikatorze id lub 0, jeśli taka tablica nie istnieje.

size_t hash_size(unsigned long id) {
    const string funcName = "hash_size";
    functionInfo(funcName, id);
    size_t x;
    try {
        x = ((*(all_arrays().at(id)))).size();
    } catch (std::out_of_range&) {
        x = 0;
    }
    hash_size_log(id, x);
    return x;
}

// Wstawia do tablicy haszującej o identyfikatorze id ciąg liczb
// całkowitych seq o długości size. Wynikiem jest informacja, czy operacja
// się powiodła. Operacja się nie powiedzie, jeśli nie ma takiej tablicy
// haszującej, jeśli tablica haszująca zawiera już taki ciąg, jeśli
// parametr seq ma wartość NULL lub parametr size ma wartość 0.

bool hash_insert(unsigned long id, uint64_t const * seq, size_t size) {
    const string funcName = "hash_insert";
    functionInfo(funcName, id, seq, size);
    if (seq == NULL || size == 0) {
        hash_insert_log(id, seq, size, false);
        return false;
    }
    bool is_inserted;
    try {
        hash_array *to_insert_to = all_arrays().at(id);
        std::vector<uint64_t> seqcopy;
        for (size_t it = 0; it < size; it++) {
            seqcopy.push_back(seq[it]);
        }
        is_inserted = ((*to_insert_to).insert(seqcopy)).second;
    } catch (std::out_of_range&) {
        is_inserted = false;
    }
    hash_insert_log(id, seq, size, is_inserted);
    return is_inserted;
}

// Usuwa z tablicy haszującej o identyfikatorze id ciąg liczb całkowitych
// seq o długości size. Wynikiem jest informacja, czy operacja się
// powiodła. Operacja się nie powiedzie, jeśli nie ma takiej tablicy
// haszującej, jeśli tablica haszująca nie zawiera takiego ciągu,
// jeśli parametr seq ma wartość NULL lub parametr size ma wartość 0.

bool hash_remove(unsigned long id, uint64_t const * seq, size_t size) {
    const string funcName = "hash_remove";
    functionInfo(funcName, id, seq, size);
    if (seq == NULL || size == 0) {
        hash_remove_log(id, seq, size, false);
        return false;
    }
    bool is_removed;
    try {
        hash_array *to_insert_to = all_arrays().at(id);
        std::vector<uint64_t> seqcopy;
        for (size_t it = 0; it < size; it++) {
            seqcopy.push_back(seq[it]);
        }
        is_removed = ((*to_insert_to).erase(seqcopy) == 1);
    } catch (std::out_of_range&) {
        is_removed = false;
    }
    hash_remove_log(id, seq, size, is_removed);
    return is_removed;
}

// Jeśli tablica haszująca o identyfikatorze id istnieje i nie jest pusta,
// to usuwa z niej wszystkie elementy. W przeciwnym przypadku nic nie robi.

void hash_clear(unsigned long id) {
  const string funcName = "hash_clear";
    functionInfo(funcName, id);
    bool was_empty = false;
    try {
        hash_array *to_insert_to = all_arrays().at(id);
        if((*to_insert_to).empty())
          was_empty = true;
        (*to_insert_to).clear();
    } catch (std::out_of_range&) {}
    hash_clear_log(id, was_empty);
}

// Daje wynik true, jeśli istnieje tablica haszująca o identyfikatorze id
// i zawiera ona ciąg liczb całkowitych seq o długości size. Daje wynik
// false w przeciwnym przypadku oraz gdy parametr seq ma wartość NULL lub
// parametr size ma wartość 0.

bool hash_test(unsigned long id, uint64_t const * seq, size_t size) {
    const string funcName = "hash_test";
    functionInfo(funcName, id, seq, size);
    if (seq == NULL || size == 0) {
        hash_test_log(id, seq, size, false);
        return false;
    }
    bool is_present;
    try {
        hash_array *to_insert_to = all_arrays().at(id);
        std::vector<uint64_t> seqcopy;
        for (size_t it = 0; it < size; it++) {
            seqcopy.push_back(seq[it]);
        }
        is_present = (
                (*to_insert_to).find(seqcopy) !=
                (*to_insert_to).end()
            );
    } catch (std::out_of_range&) {
        is_present = false;
    }
    hash_test_log(id, seq, size, is_present);
    return is_present;
}
}
