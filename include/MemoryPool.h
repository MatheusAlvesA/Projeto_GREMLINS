#ifndef _MEMORYPOOL_
#define _MEMORYPOOL_

class StoragePool {

 public:

  virtual ~StoragePool();
  virtual void *Allocate(size_t) = 0;
  virtual void Free(void *) = 0;
};

class SLPool : public StoragePool {

public:
struct Header {
 unsigned int mui_Length;
 Header () : mui_Length(0u) { /* Empty */ };
};

struct Block : public Header {

enum { BlockSize = 16 }; // Each block has 16 bytes.

union {
 Block *mp_Next;
 char mc_RawArea [BlockSize - sizeof(Header)];
};

Block() : Header (), mp_Next (nullptr) { /* Empty */ };

};

private:
unsigned int mui_NumberOfBlocks;

Block *mp_Pool;
// ! < Head of list .
Block *mr_Sentinel;
// ! < End of the list .
public:
explicit SLPool (size_t) {/*implementar*/};
~SLPool() {/*implementar*/};
void *Allocate (size_t) {/*implementar*/ return nullptr;};
void Free (void *) {/*implementar*/};

};

#endif